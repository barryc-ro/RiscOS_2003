/*******************************************************************************
********************************************************************************
*
*      File        :   $RCSfile$
*
*      Description :   BLADE Audio DSP firmware
*
*      Author      :   $Author$
*
*      Status      :   $State$
*
*      History     :   $Log$
*      History     :   Revision 1.10  2001/08/31 11:10:55Z  corby_s
*      History     :   mod to frame patch to fix infinate audio clip wrapping problem
*      History     :   disable interrupts around buffer mods in post-processing
*      History     :   Revision 1.9  2001/07/27 14:08:03Z  corby_s
*      History     :   sample rate changes handled in firmware
*      History     :   always enable host interrupts on a decode command
*      History     :   SPDIF channel stat info from operation register
*      History     :   Revision 1.8  2001/07/19 13:50:08Z  corby_s
*      History     :   allow host to specify underflow watermark
*      History     :   Revision 1.7  2001/07/12 11:09:49Z  corby_s
*      History     :   CFG functionality embedded in operation register to remove the need for CFG on channel change causing phase bug on DAC
*      History     :   Revision 1.6  2001/06/21 15:15:36Z  corby_s
*      History     :   fixed bug in tone generation scaling
*      History     :   Revision 1.5  2001/06/11 16:51:14Z  corby_s
*      History     :   report repeat status in host read
*      History     :   errors reported only in PP IRQ (underflow may not be valid in consecutive IRQ)
*      History     :   Revision 1.4  2001/05/31 14:01:29Z  corby_s
*      History     :   force read of 4 words from FIFO on startup to ensure no odd byte left behind
*      History     :   as this can lead to white noise being generated when playing PCM
*      History     :   Revision 1.3  2001/05/25 13:52:35Z  corby_s
*      History     :   fix problems with glitching on startup by halving time in parallel host ISR
*      History     :   Revision 1.2  2001/05/24 14:33:59Z  corby_s
*      History     :   fix for clipping signals on loud streams
*      History     :   always trigger IRQ to execute host ISR
*      History     :   Revision 1.1 2001/05/15 11:51:35
*      History     :   Initial revision
*
*      Copyright   :   Pace Micro Technology 2000 (c)
*
*      The copyright in this material is owned by Pace Microtechnology PLC
*      ("Pace"). This material is regarded as a highly confidential trade
*      secret of Pace. It may not be reproduced, used, sold or in any other
*      way exploited or transferred to any third party without the prior
*      written permission of Pace.
*
********************************************************************************
********************************************************************************/

//*****************************************************************************************
//
//      Pacemain.asm
//      Shell for Pace BSkyB STB.
//
//
//      6-1-00  Started KHH, this version uses Zoran API with PARAM extensions.
//
//
//      10-19-00 Version 0.95,  Almost done with alpha development.
//                  includes error/sample rate interrupts to host.
//
//      10-19-00 Version 0.96,  Now takes sample rate info for G.729 and tones
//                  from PLLCFG command, not bitstream.
//
//      10-19-00 Version 0.97,  Fixed bug causing Lo tone generation to run on 
//                  after being diabled.
//
//      01-25-01 Version 0.9C,  Improvements in tolerance of req_cntr going negative
//                  and PTS skipping.
//      01-27-01 Version 0.9D,  Finalized PTS tolerance patch, Fixed L/R channel
//                  swap on SPDIF Tx PCM output.
//
//      02-10-01 Version 0.9G,  Reconstructed 0.9F from 0.9D.
//
//      02-13-01 Version 0.9H,  Patched the MPEG lo-bit rate lip-sync problem.
//
//      02-23-01 Version 0.9J,  Fixed 'stuttering' associated with PTS tolerance/lip-sync
//                  patches.  Added compile time option for new error reporting.
//
//      02-27-01 Version 0.9K,  Finalized error reporting changes, fixed sampling frequency
//                  from being zeroed in timer ISR.
//
//      02-28-01 Version 0.9L,  Fixed several 'interrupt off' situations, added PCM fixes for
//                  channel swapping, g729 mixing, added test of GPIO0 before
//                  changing requests in timer interupt.
//
//      03-09-01 Version 0.9M, * Change the loop count for layer 1 in SoftMute_proc to 4 
//                  instead of 12 for layer 2.
//                  * if layer 1, force the data request amount to be six time the
//                  current frame size.
//                  * Move 'G729_input_flag' from 'InitAll' to Start
//                  * Turn on AV sync on the fly
//
//      03-14-01 Version 0.9N, Incorporated and optimized breakpoint patches for low bit rates
//                  and AV sync in Layer I streams.
//      03-16-01 Version 0.9O, changed AVS_mask to avs_flags
//      03-22-01 Version 0.9P, add extra nops in timer int to improve handling of 
//                  Woolworth clip
//
//      18-04-01 Version Eval0002
//               timer interrupt removed.  Requests to host are made from post processing
//               PHI interrupt parses temporary buffer until and makes further requests 
//               until no more data can be sent from the host
//               low bitrate fix implemented to fool sync function
//
//      04-19-01 Version Eval0003
//               Optimized program and fixed SPDIF L/R sync for MPEG&AC3 and PCM.
//               optimised request mechanism to eliminate redundant request for data
//               (ie host sends 0 to hregin when buffer empty instead of DSP detecting
//               an empty burst)
//
//      05-09-01 Version Eval0004
//               req_cntr increased to accomodate new IBUF_SIZE which has been corrected
//               for problems associated with very high bit rate MPEG streams
//               G729 code removed for optimisation
//               USER_exec (PCM) re-written to take advantage of new request mechanism
//               bip variable added to protect post-processing if previous req in progress
//               pace.inc and 38601a2.inc integrated into pacemain.asm file
//
//      05-15-01 Version Eval0005
//               residual bytes in FIFO now picked up at beginning of PHIsr.  This fixes
//               an unrecoverable intermitant problem with buffer overflow
//
//      05-24-01 Version Eval0006
//               DSP generates an IRQ always during PP regardless of whether necassary
//               this is to act as a catalyst for the host ISR and may be removed later
//               fix for clipping on output when gain used on FS stream but attenuated by
//               global PCM output to DAC
//
//      05-25-01 Version Eval0007
//               MAX_REQ_SIZE halved to 256 bytes to reduce time spent parsing PES header
//               and reduce effect on interrupted thread.  Fixes glitch on playback start
//
//      05-32-01 Version Eval0008
//               force clear of 4 words from FIFO in init to fix odd byte residue on PCM
//
//      06-11-01 Version Eval0009
//               report repeat flag in host read
//               errors reported only once in PP IRQ (underflow not valid in consec. IRQs)
//
//      21-06-01 Version 1.0
//               fix bug in tone generation where samples not shifted correctly
//
//      12-07-01 Version 1.1
//               PES enable and SPDIF enable moved to op register to remove the need for
//               a seperate CFG command on channel change which results in phase error on
//               analogue outputs.
//
//      18-07-01 Version 1.2
//               bespoke underflow check implemented.  Allows host to modify low watermark
//               on MPEG stream as ROM implementation can cause problems with some streams
//               the new underflow check replaces the repeat bit in the host read register
//
//      25-07-01 Version 1.3
//               implement PLL sample rate changes to move responsibility from the host in
//               order to optimise the time taken to change to a new frequency.  MPEG allow
//               variable sample rates (32,44.1,48KHz), AC3 and PCM are always 48KHz
//               IRQs always enabled on a "play" command (MPG,AC3 or USR) to get data ASAP
//               setup SPDIF channel status flag based on PCM/compressed audio bit op reg
//
//
//      31-08-01 Version 1.4
//               modification to frame patch to fix ignored underflow condition in Sync
//               disable interrupts around critical buffer pointer mods in post processing
//
//*****************************************************************************************

#include "macros.inc"

// code revision
#define VERSION_ADDRESS         0x600
#define	VERSION                 {0x7061,0x6365,0x7631,0x2e34 } // pacev1.4

// ROM code subroutines and return addresses
#define exe_kernel_entry        0xE0042
#define DISP_NOP                0xE16FB
#define parse_pes_header        0xE19D1
#define save_dreq_data          0xE1B5D
#define AC3_exec                0xE1D02
#define trans_samp_time         0xE1C87
#define MPEG_exec               0xE2A42
#define MPEG_check_dmph         0xE2B89
#define Sync_wait_out_init      0xE2E59
#define Sync_exit2              0xE2E60
#define SoftMute_proc_ROM       0xE2E89
#define SoftMute_loop_ROM       0xE2E8C
#define SPDIF_Syncronization    0xE31ED

// ROM code variables
#define	Temp_out                0x0400
#define LAYER                   0x05C6
#define	Temp_out_ptr            0x05ED
#define ibfull_cntr             0x05F1
#define frame_patch             0x07D1
#define mpeg_bass_list          0x07F4
#define off_intb                0x2715
#define bkp_entry               0x271B
#define phi_entry               0x271D
#define off_inta                0x271E
#define dfifo_entry             0x271F
#define user_buf_ptr            0x2720
#define avs_flags               0x2759
#define scr_ms                  0x2763
#define last_frame_size         0x276B
#define curr_frame_size         0x276C
#define STATUS_STAT             0x277C
#define postproc_ptr            0x2784
#define initproc_ptr            0x2786
#define AVS_mask                0x2789
#define req_cntr                0x278A
#define spdif_init_flag         0x278F
#define rd_config               0x27C5
#define d3_reg                  0x27C7
#define d4_reg                  0x27C8
#define sh_dbx                  0x27CB
#define STREAM_sr               0x27EB
#define nwa_input_flag          0x27EF
#define USER_HANDLER            0x27F0
#define AC3_HANDLER             0x27F5
#define MPEG_HANDLER            0x27F7
#define process_status          0x27FA

#define INSTRUCTION_CACHE       0xD07F8
#define BIT_BUCKET              0xF0000

#define BLK_SIZE                128

// Configuration
#define OP_OUT_ENABLE           0
#define OP_PCM_OUT              1
#define PACKETISED              2
#define AV_SYNC_EN              3
#define TONE_HILO               4
#define TONE_ENABLE             5
#define IRQ_ENABLE              7
// ~ +/- 50 msec, in units of 128 sample blocks
#define DEFAULT_PTS_TOL         0x01212

// MPEG flags
#define LAYER_I                 3
#define AVS_ACT                 6

// output buffer pointers and offsets
#define LEFT                    0
#define RGHT                    2
#define BASS_IN_OPTR            5
#define BASS_IN_OOFF            6
#define BASS_IN_OMOD            7

// input buffer sizes
// FIFO_SIZE is the lenth in words of our temporary input buffer
#define FIFO_SIZE               256
// MAX_REQ_SIZE is the number of words requested of the host - it
// must not exceed FIFO_SIZE or overflow will occur
//#define MAX_REQ_SIZE            FIFO_SIZE
#define MAX_REQ_SIZE            128
// TRGT_SIZE is the size of the entire input buffer - 4 words to
// accomodate residue left in the DFIFO between requests, and - 1
// to eliminate potential wrapping condition.  MPEG only.
#define IBUF_SIZE               1862
#define TRGT_SIZE               IBUF_SIZE - 5
// PCM_BUF_SIZE is the size of the output buffer.  Target is - 5
// words for the same reason as TRGT_SIZE
#define PCM_BUF_SIZE            1536
#define PCM_TGT_SIZE            (PCM_BUF_SIZE - 5)
#define PCM_BUF_BASE            0x800
// MAX_REQ caps the number of back to back requests to the host.
// it is included for completeness and set to an unrealistic level
// so that the host<>DSP comms never stall
#define MAX_REQ                 1000

// MPEG patches
#define LAYER_I_SOFTMUTE_CNTR   4           // 2.65ms skip for layer I
#define LAYER_II_SOFTMUTE_CNTR  12          // 8.00ms skip for layer II

// misc flags
#define PLAY                    0
#define INIT                    1
#define PES_bit                 2
#define FRM_RPT                 11          // frame repeat flag in avs_flags 

// parallel read register
#define FIFO_REQ_BIT            0
#define SR_MASK                 0x0000C
#define SR_SHIFT                2
#define STATUS_MASK             0x00070
#define STATUS_SHIFT            4
#define UNDERFLOW_BIT           7
// trigger IRQ if repeat OR data request
#define HREG_FLAG_MASK          0x00081

// sample rates from STREAM_sr (MPEG only)
#define FS_32                   2
#define FS_44                   1
#define FS_48                   0
#define FS_UNKNOWN              0x0c
#define FS_32_DIV               0x0465
#define FS_32_MUL               0x0200
#define FS_44_DIV               0x0271
#define FS_44_MUL               0x0188
#define FS_48_DIV               0x0177
#define FS_48_MUL               0x0100
#define RST_AUDPLL_BIT          1
// SPDIF channel status bits sample rates
#define SPDIF_48KHZ             0x20000
#define SPDIF_44KHZ             0x00000
#define SPDIF_32KHZ             0x30000

FORWARD D3_TO_SRG;
FORWARD INCA7_INCA7;

ORG VERSION_ADDRESS;
DATA    version VERSION;

ORG BIT_BUCKET;
DATA    null;                               // The bit bucket;

ORG INSTRUCTION_CACHE;
DATA    no_operation    {0x00000000};
DATA    d3_to_srg       {0x0003C0E3};       // for SPDIF PCM operation
DATA    ie_to_srg       {0x0003CE23};       // for SPDIF mute operation
DATA    inca7_inca7     {0x00001E38};

ORG;
DATA    Operation_Reg   {0};                // Operation register
DATA    Tone_Volume     {0};                // Linear volume for tones.
DATA    PCM_Volume  2   {0};                // Linear volume for left PCM output.
DATA    Dgain_Reg       {0};                // Decoder gain register.
DATA    Dgain_Upd       {1};                // Decoder gain update flag.
DATA    PTStol_reg      {DEFAULT_PTS_TOL};  // PTS tolerance defaults.
DATA    Last_Tone_Mode  {0};                // Mode change register.
DATA    PCM_target      {0};                // Trigger address for new block.
DATA    PCM_ptrs 6      {#null};            // PCM post-processing pointers.
DATA    PCM_offs 6      {2};                // PCM post_processing offsets.
DATA    PCM_mods 6      {PCM_BUF_SIZE};     // PCM post_processing modulii.
DATA    Decode_Mult     {0.999999};         // Linear multiplier for decoder gain.
DATA    Decode_Shift    {0};                // Linear shift for decoder gain.
DATA    hregout_stat    {0};                // Hregout data fields.
DATA    requestedWords  {0};                // req_cntr back to host.
DATA    bip             {0};                // mutual exclusion of request/send data
DATA    in_buf  (FIFO_SIZE+4) {0};          // Temporary input buffer. (+8 bytes for any FIFO residue)
DATA    in_buf_ptr      {#in_buf};          // Input buffer pointer.
DATA    myFrameSize     {0};                // store real MPEG frame size here
DATA    d2_bkp          {0};                // register protection for interrupts
DATA    d3_bkp          {0};                //                  "
DATA    d4_bkp          {0};                //                  "
DATA    d3_phi_protect  {0};                //                  "
DATA    d4_phi_protect  {0};                //                  "
DATA    d5_phi_protect  {0};                //                  "
DATA    a0_phi_protect  {0};                //                  "
DATA    i0_phi_protect  {0};                //                  "
DATA    m0_phi_protect  {0};                //                  "
DATA    predictedWrPntr {0};                //
DATA    watermark       {500};              // MPEG low water mark level default 500 words
DATA    underflow       {0};                // bespoke underflow flag 
DATA    last_rate       {0};                // current sample rate

DATA Osc_Cfs {              // Ftone = 206 Hz
         -0.999636,         // a1/2, Fs=48 kHz
         -0.999569,         // a1/2, Fs=44.1 kHz
         -0.999182,         // a1/2, Fs=32 kHz
                            // Ftone = 824 Hz
         -0.994189,         // a1/2, Fs=48 kHz
         -0.993117,         // a1/2, Fs=44.1 kHz
         -0.986940};        // a1/2, Fs=32 kHz

DATA Osc_State  2       {0x70000};          // Oscillator state.

DATA    Param_List
        { 8,                                // Number of extensions supported.
          #Operation_Reg,                   // Pointer to the extension 0 buffer.
          3,                                // Number of !bytes! in the buffer.
          #null,                            // Update flag for operation register.
          #watermark,                       // pointer to low water mark level
          3,                                // number of bytes...
          #null,                            // update flag for low water mark level
          #Tone_Volume,                     // Pointer to extension 2 buffer.
          3,                                // Number of !bytes! in the buffer.
          #null,                            // Update flag for Tvol register.
          #PCM_Volume,                      // Pointer to extension 3 buffer.
          6,                                // Number of !bytes! in the buffer.
          #null,                            // Update flag for Pvol register.
          #null,                            // this extension has been abandoned
          15,                               // maintain buffer for backward compatibility
          #null,                            // this extension has been abandoned
          #null,                            // this extension has been abandoned
          6,                                // maintain buffer for backward compatibility
          #null,                            // this extension has been abandoned
          #PTStol_reg,                      // PTS tolerance register.
          3,                                // Number of !bytes! in the buffer.
          #null,                            // PTS tolerance update flag
          #Dgain_Reg,                       // Pointer to decoder volume register.
          3,                                // Number of !bytes! in the buffer.
          #Dgain_Upd};                      // Decoder volume update flag.

SUBROUTINE Restart;
SUBROUTINE MPEG_exec_dummy;
SUBROUTINE AC3_exec_dummy;
SUBROUTINE USER_exec;
SUBROUTINE PCMinject;
SUBROUTINE Init_all;
SUBROUTINE Intb_ISR;
SUBROUTINE Dfifo_ISR;
SUBROUTINE Phi_ISR;
SUBROUTINE MPG_Bkp_ISR;
SUBROUTINE AC3_Bkp_ISR;
SUBROUTINE Post_Shell;
SUBROUTINE myFramePatch;
SUBROUTINE endOfCode;

ORG;
SUBROUTINE Restart {
    setb    #0,gpio;                            // Clear the IRQ pin.
    setb    #0,gpioc;                           // It is an output.

    move_m( #Param_List,(user_buf_ptr));        // install paramList ptr for storing input para
    move_m( #MPEG_exec_dummy,(MPEG_HANDLER));   // set patch to MPEG_HANDLER
    move_m( #AC3_exec_dummy,(AC3_HANDLER));     // set patch to AC3_HANDLER
    jmp_2(  (exe_kernel_entry));                // Jump to Loopm.
    move_m( #USER_exec,(USER_HANDLER));         // set patch to PROL_HANDLER
}

SUBROUTINE MPEG_exec_dummy {
    move    (process_status),d0;                // test init flag
    cmpi    #INIT,d0;                           // is it INIT ?
    dbeq    init;                               // yes, initialize all
// Set up breakpoint patches
     move_m (#MPG_Bkp_ISR,(bkp_entry));         // Set up 1st set of breakpoint handlers
    move_m  (#Sync_wait_out_init,bkp1);         // break address in Sync
    move_m  (#MPEG_check_dmph,bkp2);            // break after Sync
    move    ie,bct1;                            // bct1=bct2=1 (stop every break)
    move    ie,bct2;
    setb    #0,bcr;                             // Enable breakpoint 1
    setb    #1,bcr;                             // Enable breakpoint 2
    jsrq    #MPEG_exec;                         // go decode MPEG....
    rpt     #4;                                 // Interrupts off.
    move    #0,ie;                              //
    move    a0,d5;
    // use 'predictedWrPntr' as we may still be rx'ing data from last frame
    move    (predictedWrPntr),d2;               // get last "safe" write pointer
    sub     d5,d2,d5;                           // take difference
    jge_1   (noadd_mod);                        // are we wrapping
     move   #IBUF_SIZE,d2;                      // compensate for
    add     d2,d5;                              // wrapping buffer
noadd_mod:
    move    #TRGT_SIZE,d2;                      // target buffer level
    move    (watermark),d3;                     // get user watermark level
    move    ie,d4;                              // use d4 for underflow bit
    cmp     d3,d5;                              // check if we are starved
    dbgt    no_uflow;                           //
     sub     d5,d2,d5;                          // subtract occupied words
     move    d5,(req_cntr);                     // how many bytes are required
    setb    #UNDERFLOW_BIT,d4;                  // set underflow bit
no_uflow:
    move    d4,(underflow);                     //
    move    (STREAM_sr),d2;                     // Get sample rate
    move    (last_rate),d5;                     // get the previous sample rate
    move    d2,(last_rate);                     // always store last rate
    cmp     d2,d5;                              // has anything changed
    dbeq    no_fs_change;                       // not then quit out
     move   #SPDIF_32KHZ,d5;                    //
     cmpi   #FS_32,d2;                          // are we now 32 KHz
    dbeq    setauddaudm;                        // load PLL settings for 32 KHz
     move   #FS_32_DIV,d3;                      // and load them
     move   #FS_32_MUL,d4;                      //
    move    #SPDIF_44KHZ,d5;                    //
    cmpi    #FS_44,d2;                          // same for 44KHz
    dbeq    setauddaudm;                        //
     move   #FS_44_DIV,d3;                      //
     move   #FS_44_MUL,d4;                      //
    move    #SPDIF_48KHZ,d5;                    //
    move    #FS_48_DIV,d3;                      // must be 48KHz
    move    #FS_48_MUL,d4;                      //
setauddaudm:
    clr     d2;
    move    d2,clkmode;                         // clear all flags
    move    d3,audd;
    move    d4,audm;
    setb    #1,clkmode;
    move    spfchs,d3;                          // setup SPDIF CS for 48KHz
    andi    #0xcffff,d3;                        // clear the current SR field
    or      d5,d3;                              // SPDIF channel stat SR in d5
    move    d3,spfchs;
no_fs_change:
    rts_1;                                      // return in 2 instructions
     move   #1,ie;                              // re-enable interrupts first
init:
    jsrq    #MPEG_exec;                         // Initialize MPEG decoder.
    move_m  (#myFramePatch,(frame_patch));      // install frame patch
    move    #1,d1;                              // Set d1 for 6ch initialization.
    db      Init_all;                           // Initialize timer, fifo, etc.
     rpt    #4;                                 // Interrupts off.
     move   #0,ie;                              //
}


SUBROUTINE AC3_exec_dummy {
    move    (process_status),d0;                // test init flag
    cmpi    #INIT,d0;                           // is it INIT ?
    dbne    #AC3_exec;                          // no, decode next block of samples
     move_m  (#DISP_NOP,(initproc_ptr));        // no special initialisation
// Initialization:
    jsrq    #AC3_exec;                          // Initialize AC3 decoder.  
    rpt     #4;                                 // Interrupts off.
    move    #0,ie;                              //
    move    ie,clkmode;                         // clear all flags
    move    #FS_48_DIV,d3;                      // must be 48KHz
    move    d3,audd;
    move    #FS_48_MUL,d3;
    move    d3,audm;
    move    spfchs,d3;                          // setup SPDIF CS for 48KHz
    andi    #0xcffff,d3;                        // clear the current SR field
    ori     #SPDIF_48KHZ,d3;
    move    d3,spfchs;
    db      Init_all;                           // Initialize timer, fifo, etc.
     setb   #1,clkmode;                         // setup PLL for 48Khz operation
     move   #1,d1;                              // Set d1 for 6ch initialization.
}

// PCM bypass executive
//
//                        +---+---+---+---+---+---+
//                        | 1 | 2 | 3 | 4 | 5 | 6 |
//                        +---+---+---+---+---+---+
//
//  io buffer is 1536 words long (room for six blocks of 128 stereo PCM samples)
//  the PCM bypass routine enables output interrupts at initialisation so that
//  the buffer is always output regardless of availability of input data
//
//  A record is kept of 256 word boundaries (PCM_target).  Upon a7 reaching the
//  next boundary (eg 1|2) then PCM_target is incremented and wrapped to the next
//  boundary (2|3).  At the same time the next block (4) is post-processed, which 
//  also has the effect of requesting another 512 bytes of data from the host.
//  Additionally the input pointer (a6) is always reset to point to the next block
//  (5) to ensure that we always have the sequence of events (req/pp/oput).
SUBROUTINE USER_exec {
    move    (process_status),d0;                // test init flag
    cmpi    #INIT,d0;                           // is it INIT ?
    dbeq    PCM_init;                           // Initialize.
     move    a7,d3;
     move    a7,d2;
    cmp     d3,d2;
waitb:
    dbeq    waitb;
     move   a7,d2;
     cmp    d3,d2;
    move    (PCM_target),d3;                    // next 128 sample boundary
    cmp     d2,d3;                              //
    dbne    not_next_block;
     move   d3,a0;                              // set up circ buffer in a0
     move   #2*BLK_SIZE,i0;                     // and index
    move    #PCM_BUF_SIZE,m0;                   // modulo
    move_m  (#2*BLK_SIZE,(req_cntr));           // setup request count for PP
    move    (a0)+i;                             // increment to next target
    move    a0,(PCM_target);                    //
    move    a0,(PCM_ptrs+LEFT);                 // Save it as left pp pointer
    move    (a0)+;                              // Advance to right
    move    a0,(PCM_ptrs+RGHT);                 // Save it as right pp pointer
    move    (a0)-;                              // backtrack to original
    db      Post_Shell;                         // Do post processing
     move   (a0)+i;                             // and increment to next input
     move   a0,a6;                              // reset input pointer
not_next_block:
    rts;

PCM_init:
    rpt     #4;                                 // Interrupts off.
    move    #0,ie;                              //
    move    ie,clkmode;                         // clear all flags
    move    #FS_48_DIV,d3;                      // must be 48KHz
    move    d3,audd;
    move    #FS_48_MUL,d3;
    move    d3,audm;
    move    spfchs,d3;                          // setup SPDIF CS for 48KHz
    andi    #0xcffff,d3;                        // clear the current SR field
    ori     #SPDIF_48KHZ,d3;
    move    d3,spfchs;
    setb    #1,clkmode;                         // setup PLL for 48Khz operation
    clrb    #19,spmode;                         // reset SPDIF TX
    clr     d1;                                 // 0 in d1 for 2 channel operation
    jsrq    Init_all;                           // Initialize timer, fifo, etc. for 2 channels (d1=0)
    move_m  (PCMinject,(off_inta));             // Replace PES parser with PCM handler
    move    #PCM_BUF_BASE,a7;                   // initialise pointers
    move    a7,(PCM_ptrs+LEFT);                 // for post-processing
    move    (a7)+;                              //
    move    a7,(PCM_ptrs+RGHT);                 //
    move_m  (#PCM_ptrs,(mpeg_bass_list+BASS_IN_OPTR));  // Init the post-processing parameter list.
    move_m  (#PCM_offs,(mpeg_bass_list+BASS_IN_OOFF));  //
    move_m  (#PCM_mods,(mpeg_bass_list+BASS_IN_OMOD));  //
    move    #PCM_BUF_BASE,a7;                   // initialise output
    move    #PCM_BUF_SIZE,m7;                   // pointers to input
    move    #PCM_BUF_BASE,a6;                   // buffer pointers exactly
    move    #PCM_BUF_SIZE,m6;                   // the same....
    move_m  (#PCM_BUF_BASE+BLK_SIZE,(PCM_target));  // next block start
    setb    #16,mode;                           // set BM field (B irqs on)
//  Synchronize output pointer a7 and SPDIF TX with L/R clock 
    jsrq    #SPDIF_Syncronization;              // L/R channel sync for SDPIF TX
    clrb    #0,a7;                              // sync a7 with L channel (even)
    rts_2;                                      // Return in two.
     move_m( #PLAY,(process_status));           // Set process status to play.
}


SUBROUTINE PCMinject
{
    move    (req_cntr),d4;                      // get req count
    dec     d4 d3,(a6)+;                        // transfer data
    pop     mode;                               // and dec req_count
    rti_1;
     move    d4,(req_cntr);                     // save new req_count
}


SUBROUTINE Init_all
{
    move    ie,(bip);                           // clear burst in progress variable
    move    a6,(predictedWrPntr);               // initialise predicted input write pointer
// Initialize data requests:
    setb    #0,gpio;                            // Clear any unserviced data requests.
    move_m( #0,hregout);                        // Clear the flag bits.
// Modify code:
    move    (Operation_Reg),d2;                 // Get the operation register.
    setb    #IRQ_ENABLE,d2;                     // enable IRQs
    move    d2,(Operation_Reg);                 // and save it
    tstb    #OP_OUT_ENABLE,d2;                  // optical output enabled
    dbeq    end_spdif_init;
     move   (ie_to_srg),d0;                     // copy 
     tstb   #OP_PCM_OUT,d2;                     // Check for PCM output on SPDIF.
    dbeq    end_spdif_init;                     // If bit 1 is zero, send pcm
     move   (d3_to_srg),d0;                     // Modify code for PCM out on SPDIf.
     clrb   #1,spfchs;                          // set SPDIF channel status dig audio
    move    (no_operation),d0;                  //
    setb    #1,spfchs;                          // set SPDIF channel status non audio data
end_spdif_init:
    move    d0,(D3_TO_SRG);                     // Modify code for compressed data on SPDIF.
    cmpz    d1 #0,d3;                           // Test for 2 or 6 ch operation.
    dbeq    intb_isr_2ch;                       // If zero, set for 2 ch.
     move   (no_operation),d1;                  // Get instr. for 2ch operation in Intb_ISR.
     move   d3,(spdif_init_flag);               // VF9 enable spdif transmitter L/R resync
    move    (inca7_inca7),d1;                   // Get instr. for 6ch operation in Intb_ISR.
intb_isr_2ch:                                   // Return instruction in d1.    
    move    d1m,(INCA7_INCA7);                  // Modify code for 6ch/2ch buffer in Intb_ISR.
// Clear the fifo:
    move_m  (#in_buf,(in_buf_ptr));             // Initialize the temp input buffer pointer.
    rpt     #4;                                 // Clear the fifo.
    move    dfifo,d3;
// Initialize vectors:
    move    (AVS_mask),d4;
    tstb    #PACKETISED,d2;
    jeq_2   (no_pes);
     move   #save_dreq_data,d3;
     clrb   #PES_bit,d4;
    move    #parse_pes_header,d3;
    setb    #PES_bit,d4;
no_pes;
    move    d4,(AVS_mask);
    move    d3,(off_inta);
    move_m  (#Intb_ISR,(off_intb));             // Load pointer to serial b service routine.
    move_m  (#Dfifo_ISR,(dfifo_entry));         // Load pointer to fifo service routine.
    move_m  (#Phi_ISR,(phi_entry));             // Load pointer to parallel host service routine.
    move    #1,ie;                              // Interrupts back on.
    rts_2;
     move_m( #Post_Shell,(postproc_ptr));       // Enable post-processing.
}


// Intb_ISR is our output interrupt service routine
SUBROUTINE Intb_ISR
{
    push    mode;                               //
    move    #0,ds;                              // data shifter to 0
    clrb    #0,mode;                            // no barrel shift
    setb    #3,mode;                            // Set the multiplication mode.
    tstb    #8,status;                          // Check for left or right.
    dbeq    left_channel;                       // 
     move   (PCM_Volume),d4;                    // Get PCM volume setting for left.
     move   (a7),d3;                            // Get data for volume scaling.
    move    (PCM_Volume+1),d4;                  // Replace volume setting if a right interrupt.

left_channel:
    push    d3;                                 // save a copy of output sample and shift back into
    ashi    #4,d3;                              // MSBits for SPDIF (may clip when gain is in use)
EXPORT D3_TO_SRG:                               // smc (disable next instruction with NOP)
    move    d3,srg_spf;                         // Send PCM to SPDIF hardware or disabled
    pop     d3;                                 // get original sample back
    mul     d4,d3;                              // attenuate by PCM volume control first
    ashi    #4,d3;                              // and shift back into MSBits to avoid clipping
    move    d3,srb;                             // output PCM sample to serial port hardware
    move    (sp)+,mode;                         // Send srb data
    move    ie,(a7)+;                           // clear output buffer
EXPORT INCA7_INCA7:                             // self modifying code
    move    (a7)+   (a7)+;                      // Skip unused channels for MPEG and AC3
    move    (d4_reg),d4;                        // Restore d4
    rti_1;                                      // Return
    move    (d3_reg),d3;                        // Restore d3
}


SUBROUTINE Dfifo_ISR
{
    push    mode;                               // Save mode register.
    push    a0;                                 // Save a0.
    push    m0;                                 // Save m0.
    clrb    #0,mode;                            // Set shift direction.
    move    #0,ds;                              // Defeat auto shift modes.
    move    dffcnt,d3;                          // Get the fifo count.
    lshi    #-17,d3;                            // Shift count down to LSB position.
    dbeq    exit;                               // If empty, exit.
     move   (in_buf_ptr),a0;                    // Get input buffer pointer.
     dec    d3  #0,m0;                          //
data_loop:
    dbne    data_loop;                          //
     dec    d3  dfifo,d4;                       //
     cmpz   d3  d4,(a0)+;                       //
    move    a0,(in_buf_ptr);                    // Save the pointer.
exit:
    pop     m0;                                 // Restore registers.
    pop     a0;                                 //
    pop     mode;                               //
    move    (d3_reg),d3;                        //
    move    (d4_reg),d4;                        //
    rti_1;                                      //
     clrb   #12,imr;                            // Re-enable dfifo interrupts. 
}


SUBROUTINE Phi_ISR
{
    move    d3,(d3_phi_protect);
    move    d4,(d4_phi_protect);
    move    d5,(d5_phi_protect);
    move    a0,(a0_phi_protect);
    move    i0,(i0_phi_protect);
    move    m0,(m0_phi_protect);
    tstb    #10,status;                         // Test the HWR bit.
    dbeq    exit_phi;                           // If it's a read, skip ahead.
     move   (requestedWords),d2;                // copy requested words for next read
     move   d2,hregout;                         // if there is one before next post-proc
    setb    #0,gpio;                            // always clear the interrupt If it's a write
    push    mode;                               // now go check for residual bytes in the FIFO
    clrb    #0,mode;                            // Set shift direction.
    move    #0,ds;                              // Defeat auto shift modes.
    move    dffcnt,d3;                          // Get the fifo count.
    lshi    #-17,d3;                            // Shift count down to LSB position.
    dbeq    exit;                               // If empty, exit.
     move   (in_buf_ptr),a0;                    // Get input buffer pointer.
     dec    d3  #0,m0;                          //
data_loop:
    dbne    data_loop;                          //
     dec    d3  dfifo,d4;                       //
     cmpz   d3  d4,(a0)+;                       //
    move    a0,(in_buf_ptr);                    // Save the pointer.
exit:
    pop     mode;
    move    (in_buf_ptr),d5;                    // Get the temp buffer pointer.
    move    #in_buf,d4;                         // Get base of buffer.
    sub     d4,d5   d4,a0;                      // Figure the number of words in the buffer.
    move    a0,(in_buf_ptr);                    // Reset the buffer pointer.
    cmpz    d5  #0,m0;                          // Compare to zero.
deformat_lp:
    dbeq    deformat_end;                       // If zero, break out of loop.
    move    (nwa_input_flag),d4;                // Get 'not-word-aligned' flag in d4.
    move    (a0),d3;                            // Get data in d3;
    rpt     #4;
    move    #0,ie;                              // turn interrupts off.
    andi    #0xFFFF0,d3;                        // Mask off relevant stuff.
    move    d5,(a0)+;                           // Clear the temp buffer.
    move    #rti_address,i0;                    // Prep i0 register for simulated interrupt.
    push    i0;                                 // Push return address.
    push    status;                             // Push status register.
    push    mode;                               // Push mode register.
    db      (off_inta);                         // Load the program counter.
     move   #0,ds;                              // clear data register shift mode
     clrb   #0,mode;                            // define positive shift direction
rti_address:
    db      deformat_lp;                        // Send the next word.
    nop;
    dec     d5;                                 // Decrement the count.
deformat_end:
    move    (bip),d3;                           // MAX bursts to go in d3
    dec     d3;                                 // -1
    dble    exit_phi;                           // if 0 quit
     move    d3,(bip);                          // update bip count
     move    hregin,d3;                         // what was written by host
    rpt     #4;
    move    #0,ie;                              // turn interrupts off.
    move    a6,(predictedWrPntr);               // store "real" write pointer
    cmpz    d3;                                 // if host wrote 0
    dbeq    clear_bip;                          // then host has no more data to send
     move   (hregout_stat),d5;                  // get sample rate and status 
     move   (req_cntr),d4;                      // Get the request count.
    cmpz    d4;                                 // greater than 0
    dble    clear_bip;                          // no .. then quit ISR
     move   #MAX_REQ_SIZE-1,d2;                 // max is 512 bytes
     dec    d4;                                 // compute words to request
    movemin d2,d4;                              //
    move    d4,(requestedWords);                // store for subsequent read...
    move    a6,a0;                              // get current write pointer
    move    m6,m0;                              // get modulo for input buffer
    inc     d4;                                 // d4 is actual requested words
    move    d4,i0;                              // as index
    setb    #0,d5;                              // Set flag for PES interrupt.
    move    (a0)+i;                             // go update it...
    move    a0,(predictedWrPntr);               // store next safe write pointer
    db      exit_phi;
     move   d5,hregout;                         // Set flags for host.
     clrb   #0,gpio;                            // Send interrupt, active low.
clear_bip:
    move    #0,d3;
    move    d3,(bip);
exit_phi:
    move    (a0_phi_protect),a0;
    move    (i0_phi_protect),i0;
    move    (m0_phi_protect),m0;
    move    (d3_phi_protect),d3;
    move    (d4_phi_protect),d4;
    move    (d5_phi_protect),d5;
    pop     d2;
    rti_1;
     move    (sh_dbx),dbx;
}

SUBROUTINE MPG_Bkp_ISR
{
    move    d3,(d3_bkp);
    move    (sp)+,z;                            // z = status
    move    (sp)+,d3;                           // d3 = return address
    cmpi    #Sync_wait_out_init+1,d3;
    dbeq    Patch1;                             // Patch1 stops Sync subroutine from waiting
    move    d2,(d2_bkp);                        // for output buffer to cycle to its origin
    cmpi    #MPEG_check_dmph+1,d3;              // before proceeding (Keeps STC from drifting).
    dbeq    Patch2;                             // Patch2 returns to ROM code after Patch1 
    move    #trans_samp_time,d2;                // completes and switches the breakpoints to  
    cmpi    #trans_samp_time+1,d3;              // second set, for Patch3 and Patch4.
    dbeq    Patch3;                             // Patch3 allows changing of the default
    move    (PTStol_reg),d2;                    // PTS tolerances.
    cmpi    #SoftMute_proc_ROM+1,d3;            // 
    dbeq    Patch4;                             // Patch4 corrects Softmute subroutine
    move    (LAYER),d2;                         // to output 1/3 as many samples (128) for
Exit:
    tstb    #13,status;                         // Layer 1 streams as for Layer 2 streams.
Nested_bkp_exit:
    dbeq    _no_ie;                             // SIE bit tested above.
    push    d3;                                 // Push nested interrupt's return address.
    move    z,status;                           // Restore status.
    move    #1,ie;                              // Enable interrupts.
_no_ie:
    setb	#0,bct1;                            // VF4 Reset breakpoint1 counter.
    setb	#0,bct2;                            // VF4 Reset breakpoint2 counter.
    rts_2;                                      // Common exit point.
     move    (d2_bkp),d2;                       // Restore registers.
     move    (d3_bkp),d3;

Patch1:
// The ROM implementation, in subroutine Sync, waits for the output pointer to
// arrive at the base of the output buffer before proceeding.  This results in a
// variable amount of time elapsed depending on the current positions of the pointers.
// The patch skips this code (it is unnecessary) so that the STC counter will not drift.
    jmp_2(  Exit);                              // Normal exit.
    clrb    #0,bsr;                             // Clear breakpoint1 status.
    move    #Sync_exit2,d3;                     // Skip around offending code in ROM.

Patch2:
// The patch replaces the breakpoint registers with new addresses to extend the
// number of break addresses available, enabling Patch3 and Patch4.  This could 
// not be incorporated in Patch1 because the breakpoint for Patch1 is not achieved
// on each call to sync, but the breakpoints need updating in all cases.
    move    d2,bkp1;                            // for Patch3
    move    (myFrameSize),d2;
    move    d2,(last_frame_size);
    move    #SoftMute_proc_ROM,d2;              // and Patch4.
    jmp_2(  Exit);
    clrb    #1,bsr;                             // Clear breakpoint2 status.
    move    d2,bkp2;

Patch3:
// Breakpoint1 is set to the call to trans_samp_time, which calculates the 
// PTS tolerances in terms of 90 KHz clocks, from units of 128 sample blocks. 
// PTS tolerance values are in registers d5 (positive tolerance) and d6 (negative
// tolerance).  If the PTStol_reg is zero, the default ROM code tolerances are
// left in d5 and d6, otherwise the values in PTStol_reg are used.  They are packed 
// with the positive tolerance in bits 7->0 and the negative tolerance in bits 15->8.
    move    ie,(ibfull_cntr);                   // req_cntr+ibfull_cntr wraps ibuffer
    cmpz    d2;                                 // If it's zero, 
    jeq_2(  Exit);                              // use ROM code tolerances.
     clrb    #0,bsr;                            // Clear breakpoint1 status.
     setb    #0,bct1;                           // Reset breakpoint1 counter.
    move    d2,d5;                              // Copy new tolerance to positive tolerance register,
    move    d2,d6;                              // and to negative tolerance register.
    andi    #0x000FF,d5;                        // Mask off the positive tolerance.
    jmp_2(  Exit);                              // Normal return.
     andi   #0x0FF00,d6;                        // Mask off the negative tolerance.
     lshi   #-8,d6;                             // Shift it into position.

Patch4:
// In the event of an error, subroutine Softmute is called in ROM to copy zeros to the
// output buffer.  It copies 384 samples per call, which is correct for Layer II streams,
// however, Layer I frames are 1/3 the size.  This causes the decoder to not be able to skip
// frames and achieve AV sync.  The patch alters the loop count register appropriately for
// Layer I streams (128 samples per call) and Layer II streams (384 samples per call).
    move    #SoftMute_loop_ROM,d3;              // Skip around the offending code in ROM.
    cmpi    #LAYER_I,d2;                        // Is it Layer I?
    jeq_2   (Set_Loop_Count);                   // If yes, mute for 128 samples.
    move    #Temp_out,a2;                       // Set pointer for Softmute.
    move    #LAYER_I_SOFTMUTE_CNTR,lc;
    move    #LAYER_II_SOFTMUTE_CNTR,lc;         // Layer II - mute for 384 samples.
Set_Loop_Count:
    jmp_2(  Exit);                              // Normal return.
    clrb    #1,bsr;                             // Clear breakpoint2 status.
    move    a2,(Temp_out_ptr);                  // Save pointer for Softmute.
}


SUBROUTINE Post_Shell {
    push    mode;                               // Save mode register.
    clrb    #0,mode;                            // Set shift direction.
    move    #0,ds;                              // Defeat auto shift modes.
    move    (mpeg_bass_list+BASS_IN_OPTR),a0;   // Get pointer to output pointers.
    move    (mpeg_bass_list+BASS_IN_OOFF),a1;   // Get pointer to output offsets.
    move    (mpeg_bass_list+BASS_IN_OMOD),a2;   // Get pointer to output moduli.
    move    (a0)+,a3;                           // Get left pointer.
    move    (a1)+,i3;                           // Get left offset.
    move    (a2)+,m3;                           // Get left modulo.
    move    (a0)+   (a1)+;                      // Advance to right.
    move    (a2)+;                              // Advance to right.
    move    (a0),a4;                            // Get right pointer.
    move    (a1),i4;                            // Get right offset.
    move    (a2),m4;                            // Get right modulo.
    rpt     #4;
    move    #0,ie;                              // Interrupts off to protect Dgain_Upd
    move    (Dgain_Upd),d2;     
    cmpz    d2;                                 // Flag is active low.
    jne_2   (End_Dgain_Update);                 //
     move   #1,d2;                              // Reset the flag.
     move   d2,(Dgain_Upd);                     //
    move    (Dgain_Reg),d2;                     // Get the new gain code.
    move    #0x3,d3;                            // Get mask for the shift bits.
    and     d2,d3   #0x3,d4;                    // Get the shift amount.
    move    d3,(Decode_Shift);                  // Save it.
    or      d4,d2;                              // Set the LSBs of the multiplier.
    move    d2,(Decode_Mult);                   // 
End_Dgain_Update:
    move    #1,ie;                              // re-enable interrupts
    move    (Decode_Mult),d2;                   // Get the multiplier.
    move    (Decode_Shift),d3;                  // Get the shift.
    push    a3;                                 // Save L pointer.
    push    a4;                                 // Save R pointer.
    do_1(   #BLK_SIZE,upscale_lp);              // Scale the block up.
    move    (a3),d0     (a4),d1;                // Get the first left and right samples.
    ashi    #-4,d0;                             //
    mul     d2,d0;                              // Scale L down 2 dB.
    ash     d3,d0;                              // Scale up 12 for a total of 10;
    ashi    #-4,d1;                             //
    mul     d2,d1   d0,(a3)+i;                  // Scale R down 2 dB, save L.
    ash     d3,d1   (a3),d0;                    // Scale up 12 for a total of 10, get next L.
    move    d1,(a4)+i   (a4),d1;                // Save R, get next R.
upscale_lp:
    pop     a4;                                 // Restore R pointer.
    pop     a3;                                 // Restore L pointer.
no_upscale:
    move    (Operation_Reg),d2;                 // Get the operation register.
    tstb    #AV_SYNC_EN,d2;                     // Test for av sync enabled.
    jeq_2   (no_av_sync);                       // If not enabled,
     move   (avs_flags),d3;                     // Get the AVS flags
     clrb   #AVS_ACT,d3;                        // and clear the AVS active bit.
    setb    #AVS_ACT,d3;                        // Otherwise, set the AVS active bit.
no_av_sync:
    move    d3,(avs_flags);                     // Save the AVS flags.
    move    (scr_ms),d3;                        // Prevent using the 33rd bit of SCR
    andi    #0x00FFF,d3;
    move    d3,(scr_ms);
    move    (Last_Tone_Mode),d3;                // Get last tone mode.
    andi    #0x30,d2;                           // Mask off salient bits.
    xor     d2,d3;                              // Test for change in mode.
    dbne    Tone_Init;                          // If changed, initialize.
     move   d2,(Last_Tone_Mode);                //
     tstb   #TONE_ENABLE,d2;                    // Test for tones enabled.
    dbeq    End_Tone_Insertion;                 // Flag is active high.
     move   (Tone_Volume),d7;                   // Get the tone volume.
     tstb   #TONE_HILO,d2;                      // Test for High or low tone.
    dbeq    Low_Tone_Selected;                  // Zero for low, One for high.
     move   (rd_config),d3;                     // Get configuration data.
     move   #Osc_Cfs,a0;                        // Point to low tone coefficients.
    move    #Osc_Cfs+3,a0;                      // Otherwise, point to High tone coeficients.
Low_Tone_Selected:
    andi    #0xC000,d3;                         // Mask off sample rate.
    lshi    #-14,d3;                            // Shift it down.
    move    d3,i0;                              // Make sample rate offset.
    move    #Osc_State,a2;                      // Point to the oscillator state.   
    move    (a0+i),d2;                          // Get oscillator coeff.
    do_1    (#BLK_SIZE,Osc_Loop);               // Generate 128 samples.
    move    (a2)+,d1;                           // Get Yn-1.
    mneg    d2,d1,d0    (a2),d3;                // d0=-Yn-1*a1/2, get Yn-2.
    msub    d2,d1,d0    d1,(a2)-;               // d0=-Yn-1*a1, save new Yn-2.
    sub     d3,d0,d1    (a3),d4 (a4),d5;        // d0-=Yn-2, get output samples.
    mul     d7,d1,d3    d1,(a2)+;
    ashi    #-4,d3;
    add     d3,d4;                              // d4=L sample + scaled tone, save new Yn-1.
    add     d3,d5       d4,(a3)+i;
    move    d5,(a4)+i;
Osc_Loop:
    jmp     End_Tone_Insertion;                 //
Tone_Init:
    move    #0x70000,d3;                        // Initialize the oscillator state bits.
    move    d3,(Osc_State);                     //
    move    d3,(Osc_State+1);                   //
End_Tone_Insertion:
    move    #0,m3;                              // Clean up
    move    #0,m4;                              //
    rpt     #4;                                 // disable interrupts
    move    #0,ie;                              // 
// Error and sample rate flags -> start building host read register here <-
    move    (bip),d2;                           // get burst in progress count
    cmpz    d2;                                 // should be no bursts in progress .. ?
    dbgt    no_IRQ;                             // yes, quit out .. we are still talking to the host
     move   (STREAM_sr),d2;                     // Get sample rate
     lshi   #SR_SHIFT,d2;                       // Shift into upper nibble.
    andi    #SR_MASK,d2;                        // Isolate the error bit
    move    d2,(hregout_stat);                  // Save hregout data for PHISR
    move    (underflow),d5;                     // Get the underflow bit
    or      d5,d2;                              // report back to host
    move    (STATUS_STAT),d5;                   // Get current status
    lshi    #STATUS_SHIFT,d5;                   // Shift sample rate code into position.
    andi    #STATUS_MASK,d5;                    // mask off sample rate
    or      d2,d5;                              // Add sample rate code to the hregout data.
    move    (Operation_Reg),d3;                 // its safe to request more data then
    tstb    #IRQ_ENABLE,d3;                     // is the FIFO enabled

    dbeq    no_IRQ;                             // no .. skip FIFO request and clear bip count
     move   (req_cntr),d4;                      // Get the request count.
     cmpz   d4;                                 // greater than 0
    dble    skipDataReq;                        // no .. skip FIFO request
     dec    d4;                                 // compute number of words to request
     move   #MAX_REQ_SIZE-1,d2;                 // max is 512 bytes
    movemin d2,d4;                              //
    move    d4,(requestedWords);                // and store for PHISR
// increment write pointer here by req_cntr to ensure next req_cntr is calculated
// correctly if input data is being recieved straddling frame boundary...
    move    a6,a0;                              // get current write pointer
    move    m6,m0;                              // get modulo for input buffer
    inc     d4;                                 // d4 is actual requested words
    move    d4,i0;                              // as index
    setb    #FIFO_REQ_BIT,d5;                   // Set flag for PES interrupt.
    move    (a0)+i;                             // go update it...
    move    a0,(predictedWrPntr);               // store next write pointer
skipDataReq:
    move    #HREG_FLAG_MASK,d4;                 // dont trigger IRQ on every PP .. are we requesting
    and     d5,d4;                              // more data or notifying the host of a repeated frame
    dbeq    no_IRQ;                             // if not then quit out without triggering an IRQ
     move   #MAX_REQ,d4;                        // number of bursts to go in d4
     move   d5,hregout;                         // Set flags for host regardless (ignored if no ISR)
    move    d4,(bip);
    clrb    #0,gpio;                            // Send interrupt, active low.
no_IRQ:
    move    #1,ie;                              // re enable interrupts
    pop     mode;
    rts;

}

SUBROUTINE myFramePatch {
    move    #TRGT_SIZE,d4;
    move    (curr_frame_size),d2;
    move    d2,(myFrameSize);
    rts_2;
     sub    d2,d4;
     move   d4,(curr_frame_size);
}

SUBROUTINE endOfCode {
    nop;
}
