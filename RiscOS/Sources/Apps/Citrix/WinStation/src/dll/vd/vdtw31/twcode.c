/* > twcode.c

 * RISC OS graphics code building code.

 */

#include "windows.h"
#include "fileio.h"
#include "vdu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swis.h"

#include "../../../app/version.h"
#include "../../../app/utils.h"

#include "../../../inc/client.h"
#include "../../../inc/debug.h"

#include "../../../inc/clib.h"

#include "wfglobal.h"

#include "twro.h"

/* ---------------------------------------------------------------------------------------------------- */

/* rop3 defines */

#define EPS_OFF		3	/* 0000000000000011	Offset within parse string */
#define EPS_INDEX	0x1C	/* 0000000000011100	Parse string index */
#define LogPar		0x20	/* 0000000000100000	(1 indicates implied NOT as Logop6) */
/*#define LogOp1		   0000000011000000	Logical Operation #1 */
/*#define LogOp2		   0000001100000000	Logical Operation #2 */
/*#define LogOp3		   0000110000000000	Logical Operation #3 */
/*#define LogOp4		   0011000000000000	Logical Operation #4 */
/*#define LogOp5		   1100000000000000	Logical Operation #5 */
#define EPS_INDEX_SHIFT	2

#define LogOpShift	6
#define LogOpCount	5

#define OpSpec	0		  /* Special Operand as noted below */
#define OpSrc 	1		  /* Operand is source field */
#define OpDest	2		  /* Operand is destination field */
#define OpPat 	3		  /* Operand is pattern field */

#define OpRes	4
#define OpOut   5
#define OpDone	6		/* No more operands */

#define OpMask	3

#define LogNOT	0			/* NOT result */
#define LogXOR	1			/* XOR result with next operand */
#define LogOR	2			/* OR  result with next operand */
#define LogAND	3			/* AND result with next operand */

#define LogMask	3

#define LogBIC	4			/* not used in protocol but can be used in code */
#define LogMOV	5			/* not used in protocol but can be used in code */

/* register definitions */

/* used in rop3_function */
#define rn_DEST		0
#define rn_OUTPUT	0
#define rn_SRC		1
#define rn_PAT		2
#define rn_RESULT	3
#define rn_SPECIAL	12	// where the pushed 'special' operand goes

/* */
#define rn_PIXEL	4
#define rn_SOURCE	5
#define rn_SOURCE_PTR	6
#define rn_SOURCE_PHASE	7
#define rn_SOURCE_MASK	8
#define rn_SOURCE_BPP	9
#define rn_COLTRANS	10

#define rn_LR		14
#define rn_PC		15

/* arm assembler defines */

#define code_ALWAYS	0xE0000000

#define code_AND	(0 << 21)
#define code_EOR	(1 << 21)
#define code_ORR	(0xC << 21)
#define code_MOV	(0xD << 21)
#define code_BIC	(0xE << 21)
#define code_MVN	(0xF << 21)

#define OP1(a)	((a) << 16)
#define DST(a)	((a) << 12)
#define OP2(a)	(a)

/* some prebuilt instructions */

#define code_MVN_output_result		(code_ALWAYS | code_MVN | DST(rn_OUTPUT) | OP2(rn_RESULT))
#define code_MOV_output_result		(code_ALWAYS | code_MOV | DST(rn_OUTPUT) | OP2(rn_RESULT))
#define code_MVN_result_result		(code_ALWAYS | code_MVN | DST(rn_RESULT) | OP2(rn_RESULT))
#define code_MOV_special_result		(code_ALWAYS | code_MOV | DST(rn_SPECIAL) | OP2(rn_RESULT))
#define code_MOV_pc_lr			(code_ALWAYS | code_MOV | DST(rn_PC) | OP2(rn_LR))

/* ---------------------------------------------------------------------------------------------------- */

static int ParseStrings[] =
{
    0x07AAA,		/* src,pat,dest,dest,dest,dest,dest,dest */
    0x079E7,		/* src,pat,dest,src,pat,dest,src,pat */
    0x06DB6,		/* src,dest,pat,src,dest,pat,src,dest */
    0x0AAAA,		/* dest,dest,dest,dest,dest,dest,dest,dest */
    0x0AAAA,		/* dest,dest,dest,dest,dest,dest,dest,dest */
    0x04725,		/* src,spec,src,pat,spec,dest,src,src */
    0x04739,		/* src,spec,src,pat,spec,pat,dest,src */
    0x04639		/* src,spec,src,dest,spec,pat,dest,src */
};

/* ---------------------------------------------------------------------------------------------------- */

#ifdef DEBUG

static char *operand_name[] =
{
    "SPE",
    "SRC",
    "DST",
    "PAT",
    "RES",
    "OUT",
    "OVR"
};

static char *logop_name[] =
{
    "NOT",
    "XOR",
    "OR",
    "AND",
    "BIC",
    "MOV"
};
#endif

/* ---------------------------------------------------------------------------------------------------- */
/*
 * dest and operand2 are always the result register
 *
 * LogXOR EOR result, op1, result
 * LogOR  ORR result, op1, result
 * LogAND AND result, op1, result
 * LogBIC BIC result, op1, result
 */

static unsigned construct_op(int logop, int op1, int op2)
{
    static unsigned logop_codes[] = { code_MVN, code_EOR, code_ORR, code_AND, code_BIC, code_MOV };
    static unsigned operand_codes[] = { rn_SPECIAL, rn_SRC, rn_DEST, rn_PAT, rn_RESULT, rn_OUTPUT };
    unsigned code;

    if (logop == LogMOV || logop == LogNOT)
	code = code_ALWAYS | logop_codes[logop] | DST(operand_codes[op1]) | OP2(operand_codes[op2]);
    else
	code = code_ALWAYS | logop_codes[logop] | DST(rn_RESULT) | OP1(operand_codes[op1]) | OP2(operand_codes[op2]);

    return code;
}

/* get next operand from the parse string and decrement the shift
 * factor.
 * Check for underflow.
 */

static int get_operand(int *parse_string)
{
    int operand = (*parse_string >>= 2) & OpMask;

    TRACE((TC_TW, TT_TW_res4, "get_operand: %s (%d)", operand_name[operand], operand));

    return operand;
}

/*
 * This function constructs an ARM assembler function to implement the given ROP.
 * The routine is pretty well optimal for a single pixel function.
 * It will also work as well for a batch of pixels packed into a word.

 * IN
 *   rn_SRC     = source bitmap pixel
 *   rn_PAT     = pattern pixel
 *   rn_DEST    = destination bitmap pixel
 * OUT
 *   rn_OUTPUT  = combined pixel
 * TRASHES
 *   rn_RESULT  = temporary stash
 *   rn_SPECIAL = pushed result
 */

static int rop3_function(unsigned *code, int rop3)
{
    int parse_string_index = (rop3 & EPS_INDEX) >> EPS_INDEX_SHIFT;
    int parse_string = ParseStrings[parse_string_index];
    BOOL invert = FALSE;
    BOOL pushed_special = FALSE;
    unsigned *code_start = code;

    int logop_shift;
    int current;
    int n_operands;
    int parse_shift;

    /* calculate number of operands that will be used.
     * It's 1 to start
     * plus 1 for each non-invert operator
     * plus an extra 2 if the parse_string has SPECIALS in it (strings 5-7)
     */
    n_operands = parse_string_index >= 5 ? 3 : 1;
    for (logop_shift = LogOpShift; logop_shift < 16; logop_shift += 2)
    {
	int logop = (rop3 >> logop_shift) & LogMask;
	if (logop != LogNOT)
	    n_operands++;
    }

    /* initialise the parse_string and current operand */
    parse_shift = (8 - (rop3 & EPS_OFF) - n_operands)*2;
    parse_string >>= parse_shift;

    current = parse_string & OpMask;
    TRACE((TC_TW, TT_TW_res4, "get_operand: %s (%d)", operand_name[current], current));

    for (logop_shift = LogOpShift; logop_shift < 16; logop_shift += 2)
    {
	int logop = (rop3 >> logop_shift) & LogMask;

	TRACE((TC_TW, TT_TW_res4, "logop: %s (%d)", logop_name[logop], logop));

	// if it is an invert then delay invert and just keep count for now
	if (logop == LogNOT)
	{
	    invert = !invert;
	}
	else
	{
	    // get next operand
	    int operand = get_operand(&parse_string);

	    // if it is a special operand then either push or use special register
	    if (operand == OpSpec)
	    {
		if (!pushed_special)
		{
		    *code++ = construct_op(LogMOV, OpSpec, current);

		    TRACE((TC_TW, TT_TW_res4, "PUSH SPECIAL"));

		    operand = get_operand(&parse_string);
		}
		else
		{
		    TRACE((TC_TW, TT_TW_res4, "PULL SPECIAL"));
		}
		    
		pushed_special = !pushed_special;
	    }

	    // attempt to use too many operands
	    if (operand == OpDone)
	    {
		TRACE((TC_TW, TT_TW_res4, "not enough operands"));
		ASSERT(0, 0);
		break;
	    }
	    
	    // if invert then do explicit invert or use BIC if we can
	    if (invert)
	    {
		TRACE((TC_TW, TT_TW_res4, "WRITE CODE INVERT"));
		
		if (logop == LogAND)
		    *code++ = construct_op(LogBIC, operand, current);
		else
		{
		    *code++ = construct_op(LogNOT, OpRes, current);
		    *code++ = construct_op(logop, operand, OpRes);
		}
		
		invert = FALSE;
	    }
	    else
	    {    
		TRACE((TC_TW, TT_TW_res4, "WRITE CODE"));
		*code++ = construct_op(logop, operand, current);
	    }

	    current = OpRes;
	}
    }

    // take into account parity bit
    if (rop3 & LogPar)
    {
	TRACE((TC_TW, TT_TW_res4, "logop: NOT"));
	invert = !invert;
    }

    // copy to output with possible inversion
    *code++ = construct_op(invert ? LogNOT : LogMOV, OpOut, current);
  
    return code - code_start;
}

pp_function make_function(int rop3)
{
    unsigned *code = code_array;

    code += rop3_function(code, rop3);

    *code = code_MOV_pc_lr;	// note, no increment so SWI gets inclusive size

    LOGERR(_swix(OS_SynchroniseCodeAreas, _INR(0,2), 1, code_array, code));

    dump_code(code_array);
    
    return (pp_function)code_array;
}

/* ---------------------------------------------------------------------------------------------------- */

#if 0

#define CONST(n)
#define PREINC(n)
#define SHIFT(reg, shift)

static void construct_pixel(int *code)
{
    *code++ = construct_arith	(code_ALWAYS,	rn_PIXEL,		code_AND, rn_SOURCE,		rn_SOURCE_MASK);
    *code++ = construct_arith	(code_ALWAYS,	rn_SOURCE_PHASE,	code_ADD, rn_SOURCE_PHASE,	rn_SOURCE_BPP);
    *code++ = construct_cmp	(code_ALWAYS,				code_CMP, rn_SOURCE_PHASE,	CONST(32));

    *code++ = construct_load	(code_EQ,	rn_SOURCE,		rn_SOURCE_PTR,			PREINC(4));
    *code++ = construct_arith	(code_EQ,	rn_SOURCE_PHASE,	code_MOV, 0,			CONST(0));

    *code++ = construct_arith	(code_NE,	rn_SOURCE,		code_MOV, 0,			REGSHIFT(rn_SOURCE, rn_SOURCE_BPP));

    if (line->coltrans)
	*code++ = construct_load(code_ALWAYS,	rn_PIXEL,		rn_COLTRANS,			REGINDEX(rn_PIXEL));
}
#endif

/* ---------------------------------------------------------------------------------------------------- */

/* eof twcode.c */
