/*---------------------------------- mkd.c ----------------------------------*/
/* mkd.c - Media Kernel Data                                                 */
/*                                                                           */
/* This file is intended to hold certain constant data structures that are   */
/* required on the client and server side of the calling  interface.         */
/*                                                                           */
/* Modification History:                                                     */
/*   dweaver    12/05/96        Add mkdUnformed                              */
/*   dpawson    06/06/96        Creation                                     */
/*---------------------------------------------------------------------------*/
/* Oracle Corporation                                                        */
/* Oracle Media Server (TM)                                                  */
/* All Rights Reserved                                                       */
/* Copyright (C) 1993-1996                                                   */
/*---------------------------------------------------------------------------*/

/*--------------------------------- Includes --------------------------------*/

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif /* !SYSX_ORACLE */

#ifndef MKD_ORACLE
#include <mkd.h>
#endif /* !MKD_ORACLE*/

/*------------------------------ Constant Data ------------------------------*/

externdef CONST_DATA mkd_pos mkdBeginning = 
{
  mkd_posTypeBeginning,
  {(ub4)0, (ub1)0, (ub1)0, (ub1)0}
};

externdef CONST_DATA mkd_pos mkdEnd = 
{
  mkd_posTypeEnd,
  {(ub4)0, (ub1)0, (ub1)0, (ub1)0}
};

externdef CONST_DATA mkd_pos mkdCurrent = 
{
  mkd_posTypeCurrent,
  {(ub4)0, (ub1)0, (ub1)0, (ub1)0}
};

externdef CONST_DATA mkd_pos mkdUnformed = 
{
  mkd_posTypeUnformed,
  {(ub4)0, (ub1)0, (ub1)0, (ub1)0}
};
