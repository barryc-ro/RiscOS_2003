/************************************************************************
 *
 *  config.h, part of tmn (TMN encoder)
 *  Copyright (C) 1995, 1996  Telenor R&D, Norway
 *        Karl Olav Lillevold <Karl.Lillevold@nta.no>
 *  
 *  Contacts: 
 *  Karl Olav Lillevold               <Karl.Lillevold@nta.no>, or
 *  Robert Danielsen                  <Robert.Danielsen@nta.no>
 *
 *  Telenor Research and Development  http://www.nta.no/brukere/DVC/
 *  P.O.Box 83                        tel.:   +47 63 84 84 00
 *  N-2007 Kjeller, Norway            fax.:   +47 63 81 00 76
 *  
 ************************************************************************/

/*
 * Disclaimer of Warranty
 *
 * These software programs are available to the user without any
 * license fee or royalty on an "as is" basis.  Telenor Research and
 * Development disclaims any and all warranties, whether express,
 * implied, or statuary, including any implied warranties or
 * merchantability or of fitness for a particular purpose.  In no
 * event shall the copyright-holder be liable for any incidental,
 * punitive, or consequential damages of any kind whatsoever arising
 * from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs
 * and user's customers, employees, agents, transferees, successors,
 * and assigns.
 *
 * Telenor Research and Development does not represent or warrant that
 * the programs furnished hereunder are free of infringement of any
 * third-party patents.
 *
 * Commercial implementations of H.263, including shareware, are
 * subject to royalty fees to patent holders.  Many of these patents
 * are general enough such that they are unavoidable regardless of
 * implementation design.
 * */


#define YES 1
#define NO 0

/*************************************************************************/

/* Default modes */
/* see http://www.nta.no/brukere/DVC/h263_options.html */

/* use Unrestricted Motion Vector mode as default (also option "-D") */
#define DEF_UMV_MODE   NO

/* use Syntax-based Arithmetic Coding mode as default (also option "-E") */
#define DEF_SAC_MODE   NO

/* use Advanced Prediction mode as default (also option "-F") */
#define DEF_ADV_MODE   NO

/* use PB-frames mode as default (also option "-G") */
#define DEF_PBF_MODE   NO

/*************************************************************************/

/* default coding format
   choose one of SF_SQCIF, SF_QCIF, SF_CIF, SF_4CIF, SF_16CIF */
#define DEF_CODING_FORMAT   SF_QCIF

/*************************************************************************/


/* Default filenames */

/* default output bitstream filename (also option "-B <filename>") */
#define DEF_STREAMNAME   "./stream.263"

/* default filename for reconstructed sequence 
   (also option "-o <filename>") */
#define DEF_OUTFILENAME   "./out.raw"

/* write difference image to file (also option "-w") */
#define DEF_WRITE_DIFF   NO

/* default filename for difference images if "-w" is used */
#define DEF_DIFFILENAME   "./diff.raw"

/*************************************************************************/


/* Frame rate parameters */

/* default reference frame rate, 25 or 30 Hz 
 * (also option "-Z <n>") */
#define DEF_REF_FRAME_RATE   30.0

/* default number of skipped frames in original sequence compared to */
/* the reference picture rate ( also option "-O <n>" ) */
/* 3 means that the original sequence is grabbed at 6.25/7.5 Hz */
/* 0 means that the original sequence is grabbed at 25.0/30.0 Hz */
#define DEF_ORIG_SKIP      0

/* default skipped frames between encoded frames (P or B) */
/* reference is original sequence */
/* 2 means 8.33/10.0 fps encoded frame rate with 25.0/30.0 fps original */
/* 0 means 8.33/10.0 fps encoded frame rate with 8.33/10.0 fps original */
#define DEF_FRAMESKIP      2   

/*************************************************************************/

/* Search windows */


/* default integer pel search seek distance ( also option "-s <n> " ) */
#define DEF_SEEK_DIST   15   

/* default integer search window for 8x8 search centered 
   around 16x16 vector. When it is zero only half pel estimation
   around the integer 16x16 vector will be performed */
/* for best performance, keep this small, preferably zero,
   but do your own simulations if you want to try something else */
#define DEF_8X8_WIN     0

/* default search window for PB delta vectors */
/* keep this small also */
#define DEF_PBDELTA_WIN   2

/*************************************************************************/


/* Frame numbers to start and stop encoding at */

/* default frame number to start at (also option "-a <n>") */
#define DEF_START_FRAME   0

/* default frame number to stop at (also option "-b <n>") */
#define DEF_STOP_FRAME    0

/*************************************************************************/


/* Quantization parameters */

/* default inter quantization parameter (also option "-q <n>") */
#define DEF_INTER_QUANT   10

/* default intra quantization parameter (also option "-I <n>") */
#define DEF_INTRA_QUANT   10

/* BQUANT parameter for PB-frame coding 
 *   (n * QP / 4 ) 
 *
 *  BQUANT  n 
 *   0      5 
 *   1      6 
 *   2      7 
 *   3      8 
 * ( also option "-Q <BQUANT>" ) */
#define DEF_BQUANT   2


/*************************************************************************/

/* Miscellaneous */

/* write repeated reconstructed frames to disk (useful for variable
 * framerate, since sequence will be saved at 25 Hz) 
 * Can be changed at run-time with option "-m" */
#define DEF_WRITE_REPEATED   NO

/* write bitstream trace to files trace.intra / trace 
 * (also option "-t") */
#define DEF_WRITE_TRACE   NO

#ifdef OFFLINE_RATE_CONTROL
/* start rate control after DEF_START_RATE_CONTROL % of sequence
 * has been encoded. Can be changed at run-time with option "-R <n>" */
#define DEF_START_RATE_CONTROL   0
#else
/* default target frame rate when rate control is used */
#define DEF_TARGET_FRAME_RATE 10.0
#endif

/* headerlength on concatenated 4:1:1 YUV input file 
 * Can be changed at run-time with option -e <headerlength> */
#define DEF_HEADERLENGTH   0

/* insert sync after each DEF_INSERT_SYNC for increased error robustness
 * 0 means do not insert extra syncs */
#define DEF_INSERT_SYNC   0

/*************************************************************************/
