/************************************************************************
 *
 *  quant.c, part of tmn (TMN encoder)
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


#include"sim.h"

/**********************************************************************
 *
 *	Name:        Quant
 *	Description:	quantizer for SIM3
 *	
 *	Input:        pointers to coeff and qcoeff
 *        
 *	Returns:	
 *	Side effects:	
 *
 *	Date: 940111	Author:	Karl.Lillevold@nta.no
 *
 ***********************************************************************/


void Quant(int *coeff, int *qcoeff, int QP, int Mode)
{
  int i;
  int level;
  
  if (QP) {
    if (Mode == MODE_INTRA || Mode == MODE_INTRA_Q) { /* Intra */
      qcoeff[0] = mmax(1,mmin(254,coeff[0]/8));

      for (i = 1; i < 64; i++) {
        level = (abs(coeff[i])) / (2*QP);
        qcoeff[i] =  mmin(127,mmax(-127,sign(coeff[i]) * level));
      }
    }
    else { /* non Intra */
      for (i = 0; i < 64; i++) {
        level = (abs(coeff[i])-QP/2)  / (2*QP);
        qcoeff[i] = mmin(127,mmax(-127,sign(coeff[i]) * level));
      }
    }
  }
  else {
    /* No quantizing.
       Used only for testing. Bitstream will not be decodable 
       whether clipping is performed or not */
    for (i = 0; i < 64; i++) {
      qcoeff[i] = coeff[i];
    }
  }
  return;
}

/**********************************************************************
 *
 *	Name:        Dequant
 *	Description:	dequantizer for SIM3
 *	
 *	Input:        pointers to coeff and qcoeff
 *        
 *	Returns:	
 *	Side effects:	
 *
 *	Date: 940111	Author:	Karl.Lillevold@nta.no
 *
 ***********************************************************************/


void Dequant(int *qcoeff, int *rcoeff, int QP, int Mode)
{
  int i;
  
  if (QP) {
    for (i = 0; i < 64; i++) {
      if (qcoeff[i]) {
        if ((QP % 2) == 1)
          rcoeff[i] = QP * (2*abs(qcoeff[i]) + 1);
        else
          rcoeff[i] = QP * (2*abs(qcoeff[i]) + 1) - 1;
        rcoeff[i] = sign(qcoeff[i]) * rcoeff[i];
      }
      else
        rcoeff[i] = 0;
    }
    if (Mode == MODE_INTRA || Mode == MODE_INTRA_Q) { /* Intra */
      rcoeff[0] = qcoeff[0]*8;
    }
  }
  else {
    /* No quantizing at all */
    for (i = 0; i < 64; i++) {
      rcoeff[i] = qcoeff[i];
    }
  }
  return;
}



