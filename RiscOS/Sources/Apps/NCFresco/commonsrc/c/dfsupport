/* -*-c-*- */

/* dfsupport.c */

/* Drawfile support bits */

/* Export an HTML document as a Draw file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "drawftypes.h"

#include "wimp.h"
#include "swis.h"
#include "bbc.h"
#include "sprite.h"

#include "version.h"
#include "dfsupport.h"

extern os_error *df_write_data(int fh, int at, void *base, int size)
{
    os_gbpbstr gps;

    gps.action = 1;
    gps.file_handle = fh;
    gps.data_addr = base;
    gps.number = size;
    gps.seq_point = at;

    return os_gbpb(&gps);
}

extern void df_write_fileheader(int fh, int width, int dheight, int *writepoint, char *program_name)
{
    draw_fileheader dfh;
    int len;

    memcpy(dfh.title, "Draw", 4);
    dfh.majorstamp = 201;
    dfh.minorstamp = 0;

    len = strlen(program_name);
    if (len > 12)
        len = 12;
    memset(dfh.progident, ' ', 12);
    memcpy(dfh.progident, program_name, len);
    dfh.bbox.x0 = 0;
    dfh.bbox.y0 = 0;
    dfh.bbox.x1 = width << 8;
    dfh.bbox.y1 = dheight << 8;

    df_write_data(fh, *writepoint, &dfh, sizeof(dfh));
    *writepoint += sizeof(dfh);
}

extern int df_write_box(int fh, char *title, int *writepoint)
{
    draw_groustr grp;
    int len;
    int owp = *writepoint;

    len = strlen(title);

    grp.tag = draw_OBJGROUP;
    if (len < 12)
	memcpy(grp.name.ch, "            ", 12);
    else
	len = 12;
    memcpy(grp.name.ch, title, len);

    df_write_data(fh, *writepoint, &grp, sizeof(grp));
    *writepoint += sizeof(grp);

    return owp;
}

extern void df_write_box_fix(int fh, int boxbase, wimp_box *bb, int *writepoint)
{
    draw_objhdr oh;

    oh.tag = draw_OBJGROUP;
    oh.size = *writepoint - boxbase;
    oh.bbox.x0 = bb->x0 << 8;
    oh.bbox.y0 = bb->y0 << 8;
    oh.bbox.x1 = bb->x1 << 8;
    oh.bbox.y1 = bb->y1 << 8;

    df_write_data(fh, boxbase, &oh, sizeof(oh));
}

extern void df_write_filled_rect(int fh, wimp_box *bb,
			  wimp_paletteword col, wimp_paletteword fill, int *writepoint)
{
    struct {
	draw_pathstrhdr hdr;
	Path_movestr move;
	Path_linestr line1;
	Path_linestr line2;
	Path_linestr line3;
	Path_linestr line4;
	Path_closestr close;
	Path_termstr term;
    } robj;
    draw_bboxtyp lb;
    draw_pathstyle ps = {0x42, 0, 0, 0};

    lb = *((draw_bboxtyp *)bb);

    lb.x0 <<= 8;
    lb.y0 <<= 8;
    lb.x1 <<= 8;
    lb.y1 <<= 8;

    robj.hdr.tag = draw_OBJPATH;
    robj.hdr.size = sizeof(robj);
    robj.hdr.bbox = lb;
    robj.hdr.fillcolour = (int) fill.word;
    robj.hdr.pathcolour = (int) col.word;
    robj.hdr.pathwidth = 0;
    robj.hdr.pathstyle = ps;

    robj.move.tag = draw_PathMOVE;
    robj.move.x = lb.x0;
    robj.move.y = lb.y0;

    robj.line1.tag = draw_PathLINE;
    robj.line1.x = lb.x1;
    robj.line1.y = lb.y0;

    robj.line2.tag = draw_PathLINE;
    robj.line2.x = lb.x1;
    robj.line2.y = lb.y1;

    robj.line3.tag = draw_PathLINE;
    robj.line3.x = lb.x0;
    robj.line3.y = lb.y1;

    robj.line4.tag = draw_PathLINE;
    robj.line4.x = lb.x0;
    robj.line4.y = lb.y0;

    robj.close.tag = draw_PathCLOSE;

    robj.term.tag = draw_PathTERM;

    df_write_data(fh, *writepoint, &robj, sizeof(robj));
    *writepoint += sizeof(robj);
}

extern void df_stretch_bb(wimp_box *bb, wimp_box *ob)
{
    if (bb->x0 > ob->x0)
	bb->x0 = ob->x0;
    if (bb->y0 > ob->y0)
	bb->y0 = ob->y0;
    
    if (bb->x1 < ob->x1)
	bb->x1 = ob->x1;
    if (bb->y1 < ob->y1)
	bb->y1 = ob->y1;
}

extern void df_write_border(int fh, wimp_box *bb, wimp_paletteword col, int width, int *writepoint)
{
    struct {
	draw_pathstrhdr hdr;
	Path_movestr move;
	Path_linestr line1;
	Path_linestr line2;
	Path_linestr line3;
	Path_linestr line4;
	Path_closestr close;
	Path_termstr term;
    } robj;
    draw_bboxtyp lb;
    draw_pathstyle ps = {0x68, 0, 0, 0};

    lb = *((draw_bboxtyp *)bb);

    lb.x0 <<= 8;
    lb.y0 <<= 8;
    lb.x1 <<= 8;
    lb.y1 <<= 8;

    robj.hdr.tag = draw_OBJPATH;
    robj.hdr.size = sizeof(robj);
    robj.hdr.bbox = lb;
    robj.hdr.fillcolour = ~0;
    robj.hdr.pathcolour = (int) col.word;
    robj.hdr.pathwidth = width << 7;
    robj.hdr.pathstyle = ps;

    lb.x0 += width << 7;
    lb.y0 += width << 7;
    lb.x1 -= width << 7;
    lb.y1 -= width << 7;

    robj.move.tag = draw_PathMOVE;
    robj.move.x = lb.x0;
    robj.move.y = lb.y0;

    robj.line1.tag = draw_PathLINE;
    robj.line1.x = lb.x1;
    robj.line1.y = lb.y0;

    robj.line2.tag = draw_PathLINE;
    robj.line2.x = lb.x1;
    robj.line2.y = lb.y1;

    robj.line3.tag = draw_PathLINE;
    robj.line3.x = lb.x0;
    robj.line3.y = lb.y1;

    robj.line4.tag = draw_PathLINE;
    robj.line4.x = lb.x0;
    robj.line4.y = lb.y0;

    robj.close.tag = draw_PathCLOSE;

    robj.term.tag = draw_PathTERM;

    df_write_data(fh, *writepoint, &robj, sizeof(robj));
    *writepoint += sizeof(robj);
}

extern void df_write_plinth(int fh, wimp_box *bb,
		     wimp_paletteword tlcol, wimp_paletteword brcol,
		     int *writepoint)
{
    struct {
	draw_pathstrhdr hdr;
	Path_movestr move;
	Path_linestr line1;
	Path_linestr line2;
	Path_linestr line3;
	Path_linestr line4;
	Path_linestr line5;
	Path_linestr line6;
	Path_closestr close;
	Path_termstr term;
    } robj;
    draw_bboxtyp lb;
    draw_pathstyle ps = {0x42, 0, 0, 0};
    int plw = (4 << 8);

    lb = *((draw_bboxtyp *)bb);

    lb.x0 <<= 8;
    lb.y0 <<= 8;
    lb.x1 <<= 8;
    lb.y1 <<= 8;

    robj.hdr.tag = draw_OBJPATH;
    robj.hdr.size = sizeof(robj);
    robj.hdr.bbox = lb;
    robj.hdr.fillcolour = tlcol.word;
    robj.hdr.pathcolour = ~0;
    robj.hdr.pathwidth = 0;
    robj.hdr.pathstyle = ps;

    robj.move.tag = draw_PathMOVE;
    robj.move.x = lb.x0;
    robj.move.y = lb.y0;

    robj.line1.tag = draw_PathLINE;
    robj.line1.x = lb.x0 + plw;
    robj.line1.y = lb.y0 + plw;

    robj.line2.tag = draw_PathLINE;
    robj.line2.x = lb.x0 + plw;
    robj.line2.y = lb.y1 - plw;

    robj.line3.tag = draw_PathLINE;
    robj.line3.x = lb.x1 - plw;
    robj.line3.y = lb.y1 - plw;

    robj.line4.tag = draw_PathLINE;
    robj.line4.x = lb.x1;
    robj.line4.y = lb.y1;

    robj.line5.tag = draw_PathLINE;
    robj.line5.x = lb.x0;
    robj.line5.y = lb.y1;

    robj.line6.tag = draw_PathLINE;
    robj.line6.x = lb.x0;
    robj.line6.y = lb.y0;

    robj.close.tag = draw_PathCLOSE;

    robj.term.tag = draw_PathTERM;

    df_write_data(fh, *writepoint, &robj, sizeof(robj));
    *writepoint += sizeof(robj);

    robj.hdr.fillcolour = brcol.word;

    robj.line2.tag = draw_PathLINE;
    robj.line2.x = lb.x1 - plw;
    robj.line2.y = lb.y0 + plw;

    robj.line5.tag = draw_PathLINE;
    robj.line5.x = lb.x1;
    robj.line5.y = lb.y0;

    df_write_data(fh, *writepoint, &robj, sizeof(robj));
    *writepoint += sizeof(robj);
}

extern void df_write_sprite(int fh, char *sprite, int x, int y, int *fileoff)
{
    draw_objhdr obj;
    sprite_header *sph;
    sprite_area *area;
    sprite_id id;
    os_regset r;
    os_error *ep;
    int ex, ey, l2bpp, lbit, width, height;

    ep = os_swix(Wimp_BaseOfSprites, &r);
    
    if (ep)
	return;

    area = (sprite_area *) (long) r.r[1];

    id.tag = sprite_id_name;
    id.s.name = sprite;

    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

    if (ep)
    {
	area = (sprite_area *) (long) r.r[0];
	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
    }
	
    if (ep)
    {
	usrtrc( "Error writing sprite: %s\n", ep->errmess);
	return;
    }

    ex = bbc_modevar(sph->mode, bbc_XEigFactor);
    ey = bbc_modevar(sph->mode, bbc_YEigFactor);
    l2bpp = bbc_modevar(sph->mode, bbc_Log2BPP);

    lbit = (sph->mode > 255) ? 0 : sph->lbit;
	    
    width = ((((sph->width+1) << 5) - lbit - (31 - sph->rbit)) >> l2bpp ) << ex;

    height = (sph->height+1) << ey;

    obj.tag = draw_OBJSPRITE;
    obj.size = sizeof(obj) + sph->next;
    obj.bbox.x0 = x << 8;
    obj.bbox.y0 = y << 8;
    obj.bbox.x1 = (x+width) << 8;
    obj.bbox.y1 = (y+height) << 8;

    df_write_data(fh, *fileoff, &obj, sizeof(obj));
    *fileoff += sizeof(obj);
    df_write_data(fh, *fileoff, sph, sph->next);
    *fileoff += sph->next;
}

extern void df_write_text(int fh, char *s, int font, int x, int y, int *fileoff)
{
}
