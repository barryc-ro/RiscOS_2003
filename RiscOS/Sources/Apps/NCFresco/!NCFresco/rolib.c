/* rolib.c

 * Functions missing from the ROM version of RiscOSLib

 */

#include "font.h"
#include "sprite.h"
#include "wimp.h"
#include "coords.h"
#include "bbc.h"

#define ReadPixTrans        (0x000400c0+45)
#define OS_SpriteOp         0x0000002E

os_error *wimp_readpixtrans(sprite_area *area, sprite_id *id,
                         sprite_factors *factors, sprite_pixtrans *pixtrans)
{
   os_regset r;
   os_error *e;

   if ((area == (sprite_area *)0) || (area == (sprite_area *)1))
   {
     r.r[0] = 0x0000;
     r.r[2] = (int) id->s.name;
   }
   else if ((id->tag) == sprite_id_name)
   {
     r.r[0] = 0x0100;
     r.r[2] = (int) id->s.name;
   }
   else if ((id->tag) == sprite_id_addr)
   {
     r.r[0] = 0x0200;
     r.r[2] = (int) id->s.addr;
   }

   r.r[1] = (int) area;
   r.r[6] = (int) factors;
   r.r[7] = (int) pixtrans;

   e = os_swix(ReadPixTrans, &r);
   return(e);
}

#define Converttopoints      0x40089

os_error *font_converttopoints(int x_os, int y_os, int *x_inch, int *y_inch)
{
   os_regset r;
   os_error *e;

   r.r[1] = x_os;
   r.r[2] = y_os;

   e = os_swix(Converttopoints, &r);

   *x_inch = r.r[1];
   *y_inch = r.r[2];

   return e;
}

/* ----------------------------------------------------------------------------- */
/* sprite functions not in the ROM plus their support routines */

static os_error * sprite__op(os_regset *r)
{
  return os_swix(OS_SpriteOp, r);
}

/* Modify op if using sprite address is address, not name */
/* But only if using own sprite area */
static void setfromtag(int op, sprite_area *area, sprite_id *spr, os_regset *r)
{
  if (area == sprite_mainarea)
  {
    r->r[0] = op;
 /* r->r[1] unused */
  }
  else
  {
    r->r[1] = (int) area;
    if ((spr->tag) == sprite_id_addr)
    {
      r->r[0] = 512 + op;
      r->r[2] = (int) (spr->s.addr);
    }
    else
    {
      r->r[0] = 256 + op;
      r->r[2] = (int) (spr->s.name);
    }
  }
}

os_error *sprite_restorestate(sprite_state state)
{
  os_regset r;
  os_error *result;

  r.r[0] = state.r[0];
  r.r[1] = state.r[1];
  r.r[2] = state.r[2];
  r.r[3] = state.r[3];

  result = sprite__op(&r);
  return result;
}

os_error * sprite_put_given(sprite_area *area, sprite_id *spr, int gcol_action,
                            int x, int y)
{
  os_regset r;
  os_error *result;
  setfromtag(34, area, spr, &r);
  r.r[3] = x;
  r.r[4] = y;
  r.r[5] = gcol_action;
  result = sprite__op(&r);
  return result;
}

os_error *sprite_outputtosprite(sprite_area *area, sprite_id *id,
                                int *save_area, sprite_state *state)
{
  os_regset r;
  os_error *result;

  setfromtag(0x3c, area, id, &r);
  r.r[3] = (int) save_area;

  result = sprite__op(&r);
  if (result == NULL)
  {
    state->r[0] = r.r[0];
    state->r[1] = r.r[1];
    state->r[2] = r.r[2];
    state->r[3] = r.r[3];
  }
  return result;
}

os_error *sprite_outputtomask(sprite_area *area, sprite_id *id,
                              int *save_area, sprite_state *state)
{
  os_regset r;
  os_error *result;

  setfromtag(0x3d, area, id, &r);
  r.r[3] = (int) save_area;

  result = sprite__op(&r);
  if (result == NULL)
  {
    state->r[0] = r.r[0];
    state->r[1] = r.r[1];
    state->r[2] = r.r[2];
    state->r[3] = r.r[3];
  }
  return result;
}

os_error * sprite_put_mask_scaled(sprite_area *area, sprite_id *spr,
                                  int x, int y,
                                  sprite_factors *factors)
{
  os_regset r;
  os_error *result;
  setfromtag(50, area, spr, &r);
  r.r[3] = x;
  r.r[4] = y;
  r.r[6] = (int) factors;
  result = sprite__op(&r);
  return result;
}

os_error *sprite_sizeof_spritecontext(sprite_area *area, sprite_id *id,
                                      int *size)
{
  os_regset r;
  os_error *result;

  setfromtag(0x3e, area, id, &r);

  result = sprite__op(&r);

  if (result == NULL)
    *size = r.r[3];

  return result;
}

/************************************************************************/
/* � Acorn Computers Ltd, 1992.                                         */
/*                                                                      */
/* This file forms part of an unsupported source release of RISC_OSLib. */
/*                                                                      */
/* It may be freely used to create executable images for saleable       */
/* products but cannot be sold in source form or as an object library   */
/* without the prior written consent of Acorn Computers Ltd.            */
/*                                                                      */
/* If this file is re-distributed (even if modified) it should retain   */
/* this copyright notice.                                               */
/*                                                                      */
/************************************************************************/

/*
 * Title: coords.c
 * Purpose: mapping of screen/window coords
 * History: IDJ: 05-Feb-92: prepared for source release
 *
*/


/*--------------------------- Coordinate conversion -----------------------*/
/* Each of these takes a pointer to a block containing a work area box and */
/* the scroll position. Since the type of these can vary (they can appear  */
/* in different types of wimp block), we use our own type. Macros are      */
/* defined in the header file for common casts to this type.               */

int coords_x_toscreen(int x, coords_cvtstr *cvt)
{ return (x - cvt->scx + cvt->box.x0); }

int coords_y_toscreen(int y, coords_cvtstr *cvt)
{ return (y - cvt->scy + cvt->box.y1); }


int coords_x_toworkarea(int x, coords_cvtstr *cvt)
{ return (x + cvt->scx - cvt->box.x0); }

int coords_y_toworkarea(int y, coords_cvtstr *cvt)
{ return (y + cvt->scy - cvt->box.y1); }


void coords_box_toscreen(wimp_box *b, coords_cvtstr *cvt)
{
  b->x0 = coords_x_toscreen(b->x0, cvt);
  b->y0 = coords_y_toscreen(b->y0, cvt);
  b->x1 = coords_x_toscreen(b->x1, cvt);
  b->y1 = coords_y_toscreen(b->y1, cvt);
}


void coords_box_toworkarea(wimp_box *b, coords_cvtstr *cvt)
{
  b->x0 = coords_x_toworkarea(b->x0, cvt);
  b->y0 = coords_y_toworkarea(b->y0, cvt);
  b->x1 = coords_x_toworkarea(b->x1, cvt);
  b->y1 = coords_y_toworkarea(b->y1, cvt);
}

void coords_point_toscreen(coords_pointstr *point, coords_cvtstr *cvt)
{
  point->x = coords_x_toscreen(point->x, cvt);
  point->y = coords_y_toscreen(point->y, cvt);
}


void coords_point_toworkarea(coords_pointstr *point, coords_cvtstr *cvt)
{
  point->x = coords_x_toworkarea(point->x, cvt);
  point->y = coords_y_toworkarea(point->y, cvt);
}


/*
 Function    : within_box
 Purpose     : determine whether a point lies within a box
 Parameters  : pointer to point
               box
 Returns     : TRUE or FALSE
 Notes       : the box vertices need not be ordered
*/

BOOL coords_withinbox(coords_pointstr *point, wimp_box *box)
{ int dx = box->x1 - box->x0;
  int dy = box->y1 - box->y0;

  return (((dx > 0) ? (box->x1 >= point->x && point->x >= box->x0) :
                      (box->x0 >= point->x && point->x >= box->x1))
          &&
          ((dy > 0) ? (box->y1 >= point->y && point->y >= box->y0) :
                      (box->y0 >= point->y && point->y >= box->y1)));
}

/*
 Function    : offset_box
 Purpose     : offset a box by a given amount
 Parameters  : pointer to box to shift
               x, y shifts
               pointer to box for result (may be same as source)
 Returns     : void
 Description :
*/

void coords_offsetbox(wimp_box *source, int byx, int byy, wimp_box *result)
{
  result->x0 = source->x0 + byx;
  result->y0 = source->y0 + byy;
  result->x1 = source->x1 + byx;
  result->y1 = source->y1 + byy;
}

/*
 Function    : intersects
 Purpose     : determine if a line intersects a rectangle
 Parameters  : box defining end points of line
               box defining rectangle
               widening
 Returns     : TRUE or FALSE
 Description : sorts the bbox of the line, and then does an approximate
               check, by seeing if the lower limit in each direction is less
               than the upper limit of the rectangle, and similarly for the
               upper limit.
               The box around the line is widened by the specified amount in
               doing this.
*/

BOOL coords_intersects(wimp_box *line, wimp_box *rect, int widen)
{
  int x0, y0, x1, y1; /* Line points after sorting */

  if (line->x0 < line->x1)  { x0 = line->x0; x1 = line->x1; }
  else  { x1 = line->x0; x0 = line->x1; }

  if (line->y0 < line->y1)  { y0 = line->y0; y1 = line->y1; }
  else  { y1 = line->y0; y0 = line->y1; }

  x0 -= widen;
  x1 += widen;
  y0 -= widen;
  y1 += widen;
  return (x0 <= rect->x1 && y0 <= rect->y1
          && x1 >= rect->x0 && y1 >= rect->y0);
}

/*
 Function    : boxes_overlap
 Purpose     : see if two boxes overlap
 Parameters  : pointers to boxes
 Returns     : TRUE or FALSE
 Notes       : box vertices must be in correct order
*/

BOOL coords_boxesoverlap(wimp_box *box1, wimp_box *box2)
{
  return (box1->x0 <= box2->x1 && box1->y0 <= box2->y1
          && box1->x1 >= box2->x0 && box1->y1 >= box2->y0);
}


/* Draw a circle outline at absolute coordinates: x, y, radius. */
os_error *bbc_circle(int x, int y, int r)
{
   os_error *e = bbc_move(x, y);
   if (!e) e = bbc_plot(bbc_Circle + bbc_DrawAbsFore, x + r, y);
   return(e);
}

os_error *bbc_moveby(int x, int y)
{
   return(bbc_plot(bbc_SolidBoth, x, y));
}


#define ColourTrans_ReturnGCOL                      0x00040742

os_error *colourtran_returnGCOL (wimp_paletteword entry, int *gcol)
{
  os_regset r;
  os_error *e;

  r.r[0] = entry.word;

  e = os_swix(ColourTrans_ReturnGCOL, &r);

  if (e == 0)
    *gcol = r.r[0];

  return(e);
}

/* eof rolib.c */

