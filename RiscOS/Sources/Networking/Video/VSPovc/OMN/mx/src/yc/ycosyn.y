%{
/* Copyright (c) 1995-96 by Oracle Corporation.  All Rights Reserved.
 *
 * ycosyn.y - Oracle Media Net CORBA IDL Syntax Definition
 *
 * This is an input file to the parser generator slax (sx). It's an
 * internal tool that was never meant to escape from its cage, so we have
 * no documentation on it. Here is the little bit of usage information
 * I managed to pry out of it:
 *
 * Usage: sx [-v] [-l] [-c] [-d] [-p] [-s startprod] [-x lexername] fn1 fn2...
 * -v : generate slax.actions, which lists all states
 * -l : turn off line numbering in all generated files
 * -c : stop after pass1, only generating .pdl files
 * -d : generate slax.h, listing #defines for tokens
 * -p : #define PXDEBUG in main parser
 * -s startprod: specify starting symbol, where 
 *     startprod is [gramname.]nonterminal
 * -x lexername: specify lexer from command line
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YC_ORACLE
#include <yc.h>
#endif

externref ysmtagDecl(YCONODE_TAG);

#define CREATE(type) \
  type *tt = (type *) ysmGlbAlloc(sizeof(type), YCONODE_TAG); \
  tt->line = (ycln *) SOURCE_NAME;
#define NEW_LIST(target, elm) \
  target = ysLstCreate(); DISCARD ysLstEnq(target, (dvoid *) elm);
#define APPEND_LIST(target, list, elm) \
  target = list; DISCARD ysLstEnq(target, (dvoid *) elm);
#define MAKE_ONEOF(ty, tv, arm, val) \
  CREATE(ty); tt->tag = tv; tt->data.arm = val; pxtmpv.node = (dvoid *) tt;
%}

%union {
  dvoid *node;
  yslst *list;
  char  *literal;
  ub4    val;
}

%token	yct_identifier yct_numeric_literal yct_character_literal yct_string

%token  YCTANY YCTATTRIBUTE YCTBOOLEAN YCTCASE YCTCHAR YCTCONST YCTCONTEXT
%token  YCTDEFAULT YCTDOUBLE YCTENUM YCTEXCEPTION YCTFALSE YCTFLOAT YCTIN
%token  YCTINOUT YCTINTERFACE YCTLONG YCTMODULE YCTOCTET YCTONEWAY
%token  YCTOUT YCTRAISES YCTREADONLY YCTSEQUENCE YCTSHORT YCTSTRING YCTSTRUCT
%token  YCTSWITCH YCTTRUE YCTTYPEDEF YCTUNION YCTUNSIGNED YCTVOID
%token  YCTTYPECODE YCTPRAGMAID YCTPRAGMAPREFIX YCTPRAGMAVERSION
%token  YCTPRAGMADBATTR YCTPRAGMADBCREAT YCTPRAGMADBLST
%token  YCTNEWLINE    /* for pragma termination */

%token  YCTDCOLON  YCTLS      YCTRS                              /* :: << >> */

%type<literal>	yct_identifier yct_numeric_literal yct_character_literal
%type<literal>	yct_string

%type<node>	definition module interface interface_dcl forward_dcl
%type<node>	interface_header export inheritance_spec scoped_name
%type<node>	const_dcl const_type const_exp or_expr xor_expr and_expr
%type<node>	shift_expr add_expr mult_expr unary_expr primary_expr
%type<node>	literal positive_int_const type_dcl type_declarator
%type<node>	type_spec simple_type_spec base_type_spec template_type_spec
%type<node>	constr_type_spec declarator complex_declarator integer_type
%type<node>	signed_int unsigned_int char_type boolean_type octet_type
%type<node>	any_type struct_type member union_type switch_type_spec
%type<node>	case case_label element_spec enum_type sequence_type
%type<node>	string_type array_declarator fixed_array_size attr_dcl
%type<node>	except_dcl op_dcl op_type_spec parameter_dcls param_dcl
%type<node>	raises_expr context_expr param_type_spec inheritance_spec_opt
%type<node>	raises_expr_opt context_expr_opt enumerator
%type<node>	pragma_dir pragma_id pragma_prefix pragma_version
%type<node>	pragma_dbattr pragma_dbcreat pragma_dblst prg_par_key
%type<node>	type_code_type

%type<list>	specification interface_body declarators member_list
%type<list>	switch_body definition_list export_list scoped_name_list
%type<list>	case_label_list enumerator_list fixed_array_size_list
%type<list>	simple_declarator_list param_dcl_list string_literal_list
%type<list>	string_literal prg_par_key_list

%type<val>	unary_operator boolean_literal floating_pt_type op_attribute
%type<val>	param_attribute readonly_opt op_attribute_opt

%type<literal>  simple_declarator

%start	specification
%lexical ycprLex

%%

specification	: definition
		{ NEW_LIST($$, $1); ((ycctx *) cs)->root = $$; }
		| specification definition
		{ APPEND_LIST($$, $1, $2); }
		;

definition	: type_dcl ';'
		{ MAKE_ONEOF(yctDefinition, ycttTypeDcl, typedcl, $1); }
		| const_dcl ';'
		{ MAKE_ONEOF(yctDefinition, ycttConstDcl, constdcl, $1); }
		| except_dcl ';'
		{ MAKE_ONEOF(yctDefinition, ycttExceptDcl, exceptdcl, $1); }
		| interface ';'
		{ MAKE_ONEOF(yctDefinition, ycttInterface, interface, $1); }
		| module ';'
		{ MAKE_ONEOF(yctDefinition, ycttModule, module, $1); }
		| pragma_dir YCTNEWLINE
		{ MAKE_ONEOF(yctDefinition, ycttPragma, pragmadir, $1); }
		;

module		: YCTMODULE yct_identifier '{' definition_list '}'
		{
		  CREATE(yctModule);
		  tt->id = $2;
		  tt->defs = $4;
		  $$ = tt;
		}
		;

interface	: interface_dcl
		{
		  MAKE_ONEOF(yctInterface, ycttInterfaceDcl, interfacedcl, $1);
		}
		| forward_dcl
		{ MAKE_ONEOF(yctInterface, ycttForwardDcl, forwarddcl, $1); }
		;

interface_dcl	: interface_header '{' '}'
		{
		  CREATE(yctInterfaceDcl);
		  tt->header = $1;
		  tt->body = (yslst*) 0;
		  $$ = tt;
		}
		| interface_header '{' interface_body '}'
		{
		  CREATE(yctInterfaceDcl);
		  tt->header = $1;
		  tt->body = $3;
		  $$ = tt;
		}
		;

forward_dcl	: YCTINTERFACE yct_identifier
		{
		  CREATE(yctForwardDcl);
		  tt->id = $2;
		  $$ = tt;
		}
		;

interface_header: YCTINTERFACE yct_identifier inheritance_spec_opt
		{
		  CREATE(yctInterfaceHeader);
		  tt->id = $2;
		  tt->inheritancespec = $3;
		  $$ = tt;
		}
		;

interface_body	: export_list
		{ $$ = $1; }
		;

export		: type_dcl ';'
		{ MAKE_ONEOF(yctExport, ycttTypeDcl, typedcl, $1); }
		| const_dcl ';'
		{ MAKE_ONEOF(yctExport, ycttConstDcl, constdcl, $1); }
		| except_dcl ';'
		{ MAKE_ONEOF(yctExport, ycttExceptDcl, exceptdcl, $1); }
		| attr_dcl ';'
		{ MAKE_ONEOF(yctExport, ycttAttrDcl, attrdcl, $1); }
		| op_dcl ';'
		{ MAKE_ONEOF(yctExport, ycttOpDcl, opdcl, $1); }
		| pragma_dir YCTNEWLINE
		{ MAKE_ONEOF(yctExport, ycttPragma, pragmadir, $1); }
		;

inheritance_spec: ':' scoped_name_list
		{
		  CREATE(yctInheritanceSpec);
		  tt->names = $2;
		  $$ = tt;
		}
		;

scoped_name	: yct_identifier
		{
		  CREATE(yctScopedName);
		  tt->name = (dvoid *) 0;
		  tt->leadcolon = FALSE;
		  tt->id = $1;
		  $$ = tt;
		}
		| YCTDCOLON yct_identifier
		{
		  CREATE(yctScopedName);
		  tt->name = (dvoid *) 0;
		  tt->leadcolon = TRUE;
		  tt->id = $2;
		  $$ = tt;
		}
		| scoped_name YCTDCOLON yct_identifier
		{
		  CREATE(yctScopedName);
		  tt->name = $1;
		  tt->leadcolon = TRUE;
		  tt->id = $3;
		  $$ = tt;
		}
		;

const_dcl	: YCTCONST const_type yct_identifier '=' const_exp
		{
		  CREATE(yctConstDcl);
		  tt->type = $2;
		  tt->id = $3;
		  tt->exp = $5;
		  $$ = tt;
		}
		;

const_type	: integer_type
		{ MAKE_ONEOF(yctConstType, ycttIntegerType, inttype, $1); }
		| char_type
		{ MAKE_ONEOF(yctConstType, ycttCharType, chartype, $1); }
		| boolean_type
		{ MAKE_ONEOF(yctConstType, ycttBooleanType, booleantype, $1); }
		| floating_pt_type
		{ MAKE_ONEOF(yctConstType, ycttFloatType, floattype, $1); }
		| string_type
		{ MAKE_ONEOF(yctConstType, ycttStringType, stringtype, $1); }
		| scoped_name
		{ MAKE_ONEOF(yctConstType, ycttScopedName, name, $1); }
		;

const_exp	: or_expr
		{ $$ = $1; }
		;

or_expr		: xor_expr
		{
		  CREATE(yctOrExpr);
		  tt->orexpr = (yctOrExpr *) 0;
		  tt->xorexpr = $1;
		  $$ = tt;
		}
		| or_expr '|' xor_expr
		{
		  CREATE(yctOrExpr);
		  tt->orexpr = $1;
		  tt->xorexpr = $3;
		  $$ = tt;
		}
		;

xor_expr	: and_expr
		{
		  CREATE(yctXorExpr);
		  tt->xorexpr = (yctXorExpr *) 0;
		  tt->andexpr = $1;
		  $$ = tt;
		}
		| xor_expr '^' and_expr
		{
		  CREATE(yctXorExpr);
		  tt->xorexpr = $1;
		  tt->andexpr = $3;
		  $$ = tt;
		}
		;

and_expr	: shift_expr
		{
		  CREATE(yctAndExpr);
		  tt->andexpr = (yctAndExpr *) 0;
		  tt->shiftexpr = $1;
		  $$ = tt;
		}
		| and_expr '&' shift_expr
		{
		  CREATE(yctAndExpr);
		  tt->andexpr = $1;
		  tt->shiftexpr = $3;
		  $$ = tt;
		}
		;

shift_expr	: add_expr
		{
		  CREATE(yctShiftExpr);
		  tt->shiftexpr = (yctShiftExpr *) 0;
		  tt->addexpr = $1;
		  $$ = tt;
		}
		| shift_expr YCTRS add_expr
		{
		  CREATE(yctShiftExpr);
		  tt->shiftexpr = $1;
		  tt->addexpr = $3;
		  tt->operid = YCTRS;
		  $$ = tt;
		}
		| shift_expr YCTLS add_expr
		{
		  CREATE(yctShiftExpr);
		  tt->shiftexpr = $1;
		  tt->addexpr = $3;
		  tt->operid = YCTLS;
		  $$ = tt;
		}
		;

add_expr	: mult_expr
		{
		  CREATE(yctAddExpr);
		  tt->addexpr = (yctAddExpr *) 0;
		  tt->multexpr = $1;
		  $$ = tt;
		}
		| add_expr '+' mult_expr
		{
		  CREATE(yctAddExpr);
		  tt->addexpr = $1;
		  tt->multexpr = $3;
		  tt->operid = '+';
		  $$ = tt;
		}
		| add_expr '-' mult_expr
		{
		  CREATE(yctAddExpr);
		  tt->addexpr = $1;
		  tt->multexpr = $3;
		  tt->operid = '-';
		  $$ = tt;
		}
		;

mult_expr	: unary_expr
		{
		  CREATE(yctMultExpr);
		  tt->multexpr = (yctMultExpr *) 0;
		  tt->unaryexpr = $1;
		  $$ = tt;
		}
		| mult_expr '*' unary_expr
		{
		  CREATE(yctMultExpr);
		  tt->multexpr = $1;
		  tt->unaryexpr = $3;
		  tt->operid = '*';
		  $$ = tt;
		}
		| mult_expr '/' unary_expr
		{
		  CREATE(yctMultExpr);
		  tt->multexpr = $1;
		  tt->unaryexpr = $3;
		  tt->operid = '/';
		  $$ = tt;
		}
		| mult_expr '%' unary_expr
		{
		  CREATE(yctMultExpr);
		  tt->multexpr = $1;
		  tt->unaryexpr = $3;
		  tt->operid = '%';
		  $$ = tt;
		}
		;

unary_expr	: unary_operator primary_expr
		{
		  CREATE(yctUnaryExpr);
		  tt->unaryop = $1;
		  tt->primexpr = $2;
		  $$ = tt;
		}
		| primary_expr
		{
		  CREATE(yctUnaryExpr);
		  tt->unaryop = 0;
		  tt->primexpr = $1;
		  $$ = tt;
		}
		;

unary_operator	: '-'	{ $$ = '-'; }
		| '+'	{ $$ = '+'; }
		| '~'   { $$ = '~'; }
		;

primary_expr	: scoped_name
		{ MAKE_ONEOF(yctPrimaryExpr, ycttScopedName, name, $1); }
		| literal
		{ MAKE_ONEOF(yctPrimaryExpr, ycttLiteral, literal, $1); }
		| '(' const_exp ')'
		{ MAKE_ONEOF(yctPrimaryExpr, ycttConstExp, constexp, $2); }
		;

literal		: yct_numeric_literal
		{ MAKE_ONEOF(yctLiteral, ycttNumLit, numlit, $1); }
		| string_literal
		{ MAKE_ONEOF(yctLiteral, ycttStrLit, strlit, $1); }
		| yct_character_literal
		{ MAKE_ONEOF(yctLiteral, ycttCharLit, charlit, $1); }
		| boolean_literal
		{ MAKE_ONEOF(yctLiteral, ycttBoolLit, boollit, $1); }
		;

boolean_literal	: YCTTRUE  { $$ = 1; }
		| YCTFALSE { $$ = 0; }
		;

positive_int_const
		: const_exp
		{ $$ = $1; }
		;

type_dcl	: YCTTYPEDEF type_declarator
		{ MAKE_ONEOF(yctTypeDcl, ycttTypeDeclarator, typedecl, $2); }
		| struct_type
		{ MAKE_ONEOF(yctTypeDcl, ycttStructType, structtype, $1); }
		| union_type
		{ MAKE_ONEOF(yctTypeDcl, ycttUnionType, uniontype, $1); }
		| enum_type
		{ MAKE_ONEOF(yctTypeDcl, ycttEnumType, enumtype, $1); }
		;

type_declarator	: type_spec declarators
		{
		  CREATE(yctTypeDeclarator);
		  tt->typespec = $1;
		  tt->declarators = $2;
		  $$ = tt;
		}
		;

type_spec	: simple_type_spec
		{ MAKE_ONEOF(yctTypeSpec, ycttSimple, simple, $1); }
		| constr_type_spec
		{ MAKE_ONEOF(yctTypeSpec, ycttConstr, constr, $1); }
		;

simple_type_spec: base_type_spec
		{ MAKE_ONEOF(yctSimpleTypeSpec, ycttBase, base, $1); }
		| template_type_spec
		{ MAKE_ONEOF(yctSimpleTypeSpec, ycttTemplate, template, $1); }
		| scoped_name
		{ MAKE_ONEOF(yctSimpleTypeSpec, ycttScopedName, name, $1); }
		;

base_type_spec	: floating_pt_type
		{ MAKE_ONEOF(yctBaseTypeSpec, ycttFloatType, floattype, $1); }
		| integer_type
		{ MAKE_ONEOF(yctBaseTypeSpec, ycttIntegerType, inttype, $1); }
		| char_type
		{ MAKE_ONEOF(yctBaseTypeSpec, ycttCharType, chartype, $1); }
		| boolean_type
		{
		  MAKE_ONEOF(yctBaseTypeSpec, ycttBooleanType, booleantype,
			     $1);
		}
		| octet_type
		{ MAKE_ONEOF(yctBaseTypeSpec, ycttOctetType, octettype, $1); }
		| any_type
		{ MAKE_ONEOF(yctBaseTypeSpec, ycttAnyType, anytype, $1); }
		| type_code_type
		{ 
		  MAKE_ONEOF(yctBaseTypeSpec, ycttTypeCodeType, typecodetype, 
		             $1); 
		}
		;

template_type_spec
		: sequence_type
		{ MAKE_ONEOF(yctTemplateTypeSpec, ycttSeqType, seqtype, $1); }
		| string_type
		{ MAKE_ONEOF(yctTemplateTypeSpec, ycttStrType, strtype, $1); }
		;

constr_type_spec: struct_type
		{ MAKE_ONEOF(yctConstrTypeSpec, ycttStructType, structtype,
			     $1);
		}
		| union_type
		{ MAKE_ONEOF(yctConstrTypeSpec, ycttUnionType, uniontype,
			     $1);
		}
		| enum_type
		{ MAKE_ONEOF(yctConstrTypeSpec, ycttEnumType, enumtype, $1); }
		;

declarators	: declarator
		{ NEW_LIST($$, $1); }
		| declarators ',' declarator
		{ APPEND_LIST($$, $1, $3); }
		;

declarator	: simple_declarator
		{ MAKE_ONEOF(yctDeclarator, ycttSimple, simple, $1); }
		| complex_declarator
		{ MAKE_ONEOF(yctDeclarator, ycttComplex, complex, $1); }
		;

simple_declarator
		: yct_identifier { $$ = $1; }
		;

complex_declarator
		: array_declarator { $$ = $1; }
		;

type_code_type	: YCTTYPECODE { $$ = (dvoid*)0; }
		;

floating_pt_type: YCTFLOAT   { $$ = YCTFLOAT; }
		| YCTDOUBLE  { $$ = YCTDOUBLE; }
		;

integer_type	: signed_int   { $$ = $1; }
		| unsigned_int { $$ = $1; }
		;

signed_int	: signed_long_int
		{
		  CREATE(yctIntegerType);
		  tt->signed_t = TRUE;
		  tt->long_t = 1;
		  $$ = tt;
		}
		| signed_short_int
		{
		  CREATE(yctIntegerType);
		  tt->signed_t = TRUE;
		  tt->long_t = 0;
		  $$ = tt;
		}
		| signed_long_long_int
		{
		  CREATE(yctIntegerType);
		  tt->signed_t = TRUE;
		  tt->long_t = 2;
		  $$ = tt;
		}
		;

signed_long_int	: YCTLONG
		;

signed_short_int: YCTSHORT
		;

signed_long_long_int
		: YCTLONG YCTLONG
		;

unsigned_int	: unsigned_long_int
		{
		  CREATE(yctIntegerType);
		  tt->signed_t = FALSE;
		  tt->long_t = 1;
		  $$ = tt;
		}
		| unsigned_short_int
		{
		  CREATE(yctIntegerType);
		  tt->signed_t = FALSE;
		  tt->long_t = 0;
		  $$ = tt;
		}
		;

unsigned_long_int
		: YCTUNSIGNED YCTLONG
		;

unsigned_short_int
		: YCTUNSIGNED YCTSHORT
		;

char_type	: YCTCHAR 	{ $$ = (dvoid *) 0; }
		;

boolean_type	: YCTBOOLEAN	{ $$ = (dvoid *) 0; }
		;

octet_type	: YCTOCTET	{ $$ = (dvoid *) 0; }
		;

any_type	: YCTANY	{ $$ = (dvoid *) 0; }
		;

struct_type	: YCTSTRUCT yct_identifier '{' member_list '}'
		{
		  CREATE(yctStructType);
		  tt->id = $2;
		  tt->members = $4;
		  $$ = tt;
		}
		;

member_list	: member
		{ NEW_LIST($$, $1); }
		| member_list member
		{ APPEND_LIST($$, $1, $2); }
		;

member		: type_spec declarators ';'
		{
		  CREATE(yctMember);
		  tt->typespec = $1;
		  tt->declarators = $2;
		  $$ = tt;
		}
		;

union_type	: YCTUNION yct_identifier YCTSWITCH '(' switch_type_spec ')'
			'{' switch_body '}'
		{
		  CREATE(yctUnionType);
		  tt->id = $2;
		  tt->typespec = $5;
		  tt->cases = $8;
		  $$ = tt;
		}
		;

switch_type_spec: integer_type
		{
		  MAKE_ONEOF(yctSwitchTypeSpec, ycttIntegerType, inttype,
			     $1);
		}
		| char_type
		{ MAKE_ONEOF(yctSwitchTypeSpec, ycttCharType, chartype, $1); }
		| boolean_type
		{
		  MAKE_ONEOF(yctSwitchTypeSpec, ycttBooleanType, booltype,
			     $1);
		}
		| enum_type
		{ MAKE_ONEOF(yctSwitchTypeSpec, ycttEnumType, enumtype, $1); }
		| scoped_name
		{ MAKE_ONEOF(yctSwitchTypeSpec, ycttScopedName, name, $1); }
		;

switch_body	: case
		{ NEW_LIST($$, $1); }
		| switch_body case
		{ APPEND_LIST($$, $1, $2); }
		;

case		: case_label_list element_spec ';'
		{
		  CREATE(yctCase);
		  tt->caselabels = $1;
		  tt->elemspec = $2;
		  $$ = tt;
		}
		;

case_label	: YCTCASE const_exp ':'
		{
		  CREATE(yctCaseLabel);
		  tt->constexp = $2;
		  $$ = tt;
		}
		| YCTDEFAULT ':'
		{
		  CREATE(yctCaseLabel);
		  tt->constexp = (yctConstExp *) 0;
		  $$ = tt;
		}
		;

element_spec	: type_spec declarator
		{
		  CREATE(yctElementSpec);
		  tt->typespec = $1;
		  tt->declarator = $2;
		  $$ = tt;
		}
		;

enum_type	: YCTENUM yct_identifier '{' enumerator_list '}'
		{
		  CREATE(yctEnumType);
		  tt->id = $2;
		  tt->enumerators = $4;
		  $$ = tt;
		}
		;

enumerator	: yct_identifier
		{
		  CREATE(yctEnumerator);
		  tt->id = $1;
		  $$ = tt;
		}
		;

sequence_type	: YCTSEQUENCE '<' simple_type_spec ',' positive_int_const '>'
		{
		  CREATE(yctSequenceType);
		  tt->simple = $3;
		  tt->intconst = $5;
		  $$ = tt;
		}
		| YCTSEQUENCE '<' simple_type_spec '>'
		{
		  CREATE(yctSequenceType);
		  tt->simple = $3;
		  tt->intconst = (yctPositiveIntConst *) 0;
		  $$ = tt;
		}
		;

string_type	: YCTSTRING '<' positive_int_const '>'
		{
		  CREATE(yctStringType);
		  tt->intconst = $3;
		  $$ = tt;
		}
		| YCTSTRING
		{
		  CREATE(yctStringType);
		  tt->intconst = (yctPositiveIntConst *) 0;
		  $$ = tt;
		}
		;

array_declarator: yct_identifier fixed_array_size_list
		{
		  CREATE(yctArrayDeclarator);
		  tt->id = $1;
		  tt->arraysize = $2;
		  $$ = tt;
		}
		;

fixed_array_size: '[' positive_int_const ']'
		{
		  CREATE(yctFixedArraySize);
		  tt->intconst = $2;
		  $$ = tt;
		}
		;

attr_dcl	: readonly_opt YCTATTRIBUTE param_type_spec
			simple_declarator_list
		{
		  CREATE(yctAttrDcl);
		  tt->readon = $1;
		  tt->paramtype = $3;
		  tt->simples = $4;
		  $$ = tt;
		}
		;

except_dcl	: YCTEXCEPTION yct_identifier '{' '}'
		{
		  CREATE(yctExceptDcl);
		  tt->id = $2;
		  tt->members = (yslst *) 0;
		  $$ = tt;
		}
		| YCTEXCEPTION yct_identifier '{' member_list '}'
		{
		  CREATE(yctExceptDcl);
		  tt->id = $2;
		  tt->members = $4;
		  $$ = tt;
		}
		;

op_dcl		: op_attribute_opt op_type_spec yct_identifier parameter_dcls
			raises_expr_opt context_expr_opt
		{
		  CREATE(yctOpDcl);
		  tt->opattribute = $1;
		  tt->optype = $2;
		  tt->id = $3;
		  tt->paramdcls = $4;
		  tt->raisesexpr = $5;
		  tt->contextexpr = $6;
		  $$ = tt;
		}
		;

op_attribute	: YCTONEWAY  { $$ = YCTONEWAY; }
		;

op_type_spec	: param_type_spec
		{
		  CREATE(yctOpTypeSpec);
		  tt->paramtype = $1;
		  $$ = tt;
		}
		| YCTVOID
		{
		  CREATE(yctOpTypeSpec);
		  tt->paramtype = (yctParamTypeSpec *) 0;
		  $$ = tt;
		}
		;

parameter_dcls	: '(' param_dcl_list ')'
		{
		  CREATE(yctParameterDcls);
		  tt->paramdcls = $2;
		  $$ = tt;
		}
		| '(' ')'
		{
		  CREATE(yctParameterDcls);
		  tt->paramdcls = (yslst *) 0;
		  $$ = tt;
		}
		;

param_dcl	: param_attribute param_type_spec simple_declarator
		{
		  CREATE(yctParamDcl);
		  tt->paramattr = $1;
		  tt->typespec = $2;
		  tt->simple = $3;
		  $$ = tt;
		}
		;

param_attribute	: YCTIN		{ $$ = YCTIN; }
		| YCTOUT	{ $$ = YCTOUT; }
		| YCTINOUT	{ $$ = YCTINOUT; }
		;

raises_expr	: YCTRAISES '(' scoped_name_list ')'
		{
		  CREATE(yctRaisesExpr);
		  tt->names = $3;
		  $$ = tt;
		}
		;

context_expr	: YCTCONTEXT '(' string_literal_list ')'
		{
		  CREATE(yctContextExpr);
		  tt->ids = $3;
		  $$ = tt;
		}
		;

param_type_spec	: base_type_spec
		{ MAKE_ONEOF(yctParamTypeSpec, ycttBase, base, $1); }
		| string_type
		{
		  MAKE_ONEOF(yctParamTypeSpec, ycttStringType, stringtype,
			     $1);
		}
		| scoped_name
		{ MAKE_ONEOF(yctParamTypeSpec, ycttScopedName, name, $1); }
		;

inheritance_spec_opt
		: inheritance_spec	{ $$ = $1; }
		| /* empty */		{ $$ = (dvoid *) 0; }
		;

definition_list	: definition
		{ NEW_LIST($$, $1); }
		| definition_list definition
		{ APPEND_LIST($$, $1, $2); }
		;

export_list	: export
		{ NEW_LIST($$, $1); }
		| export_list export
		{ APPEND_LIST($$, $1, $2); }
		;

scoped_name_list: scoped_name
		{ NEW_LIST($$, $1); }
		| scoped_name_list ',' scoped_name
		{ APPEND_LIST($$, $1, $3); }
		;

case_label_list	: case_label
		{ NEW_LIST($$, $1); }
		| case_label_list case_label
		{ APPEND_LIST($$, $1, $2); }
		;

enumerator_list	: enumerator
		{ NEW_LIST($$, $1); }
		| enumerator_list ',' enumerator
		{ APPEND_LIST($$, $1, $3); }
		;

fixed_array_size_list
		: fixed_array_size
		{ NEW_LIST($$, $1); }
		| fixed_array_size_list fixed_array_size
		{ APPEND_LIST($$, $1, $2); }
		;

readonly_opt	: YCTREADONLY  { $$ = 1; }
		| /* empty */  { $$ = 0; }
		;

simple_declarator_list
		: simple_declarator
		{ NEW_LIST($$, $1); }
		| simple_declarator_list ',' simple_declarator
		{ APPEND_LIST($$, $1, $3); }
		;

op_attribute_opt: op_attribute  { $$ = $1; }
		| /* empty */   { $$ = 0; }
		;

raises_expr_opt	: raises_expr   { $$ = $1; }
		| /* empty */   { $$ = (dvoid *) 0; }
		;

context_expr_opt: context_expr  { $$ = $1; }
		| /* empty */   { $$ = (dvoid *) 0; }
		;

param_dcl_list	: param_dcl
		{ NEW_LIST($$, $1); }
		| param_dcl_list ',' param_dcl
		{ APPEND_LIST($$, $1, $3); }
		;

string_literal_list
		: string_literal
		{ NEW_LIST($$, $1); }
		| string_literal_list ',' string_literal
		{ APPEND_LIST($$, $1, $3); }
		;

string_literal	: yct_string
		{ NEW_LIST($$, $1); }
		| string_literal yct_string
		{ APPEND_LIST($$, $1, $2); }
		;

pragma_dir	: pragma_id
		{ MAKE_ONEOF(yctPragma, ycttPragmaId, prgid, $1); }
		| pragma_prefix
		{ MAKE_ONEOF(yctPragma, ycttPragmaPrefix, prgprefix, $1); }
		| pragma_version
		{ MAKE_ONEOF(yctPragma, ycttPragmaVersion, prgversion, $1); }
		| pragma_dbattr
		{ MAKE_ONEOF(yctPragma, ycttPragmaDbAttr, prgdbattr, $1); }
		| pragma_dbcreat
		{ MAKE_ONEOF(yctPragma, ycttPragmaDbCreat, prgdbcreate, $1); }
		| pragma_dblst
		{ MAKE_ONEOF(yctPragma, ycttPragmaDbLst, prgdblst, $1); }
		;

pragma_id	: YCTPRAGMAID scoped_name yct_string
		{ 
		  CREATE(yctPragmaId);
		  tt->name = $2;
		  tt->id = $3;
		  $$ = tt;
		}
		;

pragma_prefix	: YCTPRAGMAPREFIX yct_string
		{ 
		  CREATE(yctPragmaPrefix);
		  tt->prefix = $2;
		  $$ = tt;
		}
		;

pragma_version	: YCTPRAGMAVERSION scoped_name yct_numeric_literal
		{ 
		  CREATE(yctPragmaVersion);
		  tt->name = $2;
		  tt->vers = $3;
		  $$ = tt;
		}
		;

pragma_dbattr	: YCTPRAGMADBATTR scoped_name string_literal 
		{
		  CREATE(yctPragmaDbAttr);
		  tt->name = $2;
		  tt->keystr = $3;
                  $$ = tt;
		}
		;

prg_par_key	: yct_identifier string_literal 
		{
		  CREATE(yctPrgParKey);
		  tt->paramnm = $1;
		  tt->keystr = $2;
                  $$ = tt;
		}
		;

prg_par_key_list
		: prg_par_key
		{ NEW_LIST($$, $1); }
		| prg_par_key_list ',' prg_par_key
		{ APPEND_LIST($$, $1, $3); }

pragma_dbcreat	: YCTPRAGMADBCREAT scoped_name prg_par_key_list
		{
		  CREATE(yctPragmaDbCreat);
		  tt->name = $2;
		  tt->mapping = $3;
		  $$ = tt;
		}
		;

pragma_dblst	: YCTPRAGMADBLST scoped_name
		{
		  CREATE(yctPragmaDbLst);
		  tt->name = $2;
		  $$ = tt;
		}
		;
