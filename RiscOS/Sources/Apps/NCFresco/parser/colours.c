/* > colours.c

 *

 */

#include <stdlib.h>

#include "sgmlparser.h"
#include "util.h"

typedef struct
{
	char *name;
	int rgb_word;
} colour_info;

static colour_info colour_array[] =
{
	{ "aliceblue", 0xF0F8FF },
	{ "antiquewhite", 0xFAEBD7 },
	{ "aqua", 0x00FFFF },
	{ "aquamarine", 0x7FFFD4 },
	{ "azure", 0xF0FFFF },
	{ "beige", 0xF5F5DC },
	{ "bisque", 0xFFE4C4 },
	{ "black", 0x000000 },
	{ "blanchedalmond", 0xFFEBCD },
	{ "blue", 0x0000FF },
	{ "blueviolet", 0x8A2BE2 },
	{ "brown", 0xA52A2A },
	{ "burlywood", 0xDEB887 },
	{ "cadetblue", 0x5F9EA0 },
	{ "chartreuse", 0x7FFF00 },
	{ "chocolate", 0xD2691E },
	{ "coral", 0xFF7F50 },
	{ "cornflowerblue", 0x6495ED },
	{ "cornsilk", 0xFFF8DC },
	{ "crimson", 0xDC143C },
	{ "cyan", 0x00FFFF },
	{ "darkblue", 0x00008B },
	{ "darkcyan", 0x008B8B },
	{ "darkgoldenrod", 0xB8860B },
	{ "darkgray", 0xA9A9A9 },
	{ "darkgreen", 0x006400 },
	{ "darkgrey", 0xA9A9A9 },
	{ "darkkhaki", 0xBDB76B },
	{ "darkmagenta", 0x8B008B },
	{ "darkolivegreen", 0x556B2F },
	{ "darkorange", 0xFF8C00 },
	{ "darkorchid", 0x9932CC },
	{ "darkred", 0x8B0000 },
	{ "darksalmon", 0xE9967A },
	{ "darkseagreen", 0x8FBC8F },
	{ "darkslateblue", 0x483D8B },
	{ "darkslategray", 0x2F4F4F },
	{ "darkslategrey", 0x2F4F4F },
	{ "darkturquoise", 0x00CED1 },
	{ "darkviolet", 0x9400D3 },
	{ "deeppink", 0xFF1493 },
	{ "deepskyblue", 0x00BFFF },
	{ "dimgray", 0x696969 },
	{ "dimgrey", 0x696969 },
	{ "dodgerblue", 0x1E90FF },
	{ "firebrick", 0xB22222 },
	{ "floralwhite", 0xFFFAF0 },
	{ "forestgreen", 0x228B22 },
	{ "fuchsia", 0xFF00FF },
	{ "gainsboro", 0xDCDCDC },
	{ "ghostwhite", 0xF8F8FF },
	{ "gold", 0xFFD700 },
	{ "goldenrod", 0xDAA520 },
/* 	{ "gray", 0x808080 }, */
/* 	{ "grey", 0x808080 }, */
	{ "green", 0x008000 },
	{ "greenyellow", 0xADFF2F },
	{ "honeydew", 0xF0FFF0 },
	{ "hotpink", 0xFF69B4 },
	{ "indianred", 0xCD5C5C },
	{ "indigo", 0x4B0082 },
	{ "ivory", 0xFFFFF0 },
	{ "khaki", 0xF0E68C },
	{ "lavender", 0xE6E6FA },
	{ "lavenderblush", 0xFFF0F5 },
	{ "lawngreen", 0x7CFC00 },
	{ "lemonchiffon", 0xFFFACD },
	{ "lightblue", 0xADD8E6 },
	{ "lightcoral", 0xF08080 },
	{ "lightcyan", 0xE0FFFF },
	{ "lightgoldenrodyellow", 0xFAFAD2 },
	{ "lightgreen", 0x90EE90 },
	{ "lightgrey", 0xD3D3D3 },
	{ "lightpink", 0xFFB6C1 },
	{ "lightsalmon", 0xFFA07A },
	{ "lightseagreen", 0x20B2AA },
	{ "lightskyblue", 0x87CEFA },
	{ "lightslategray", 0x778899 },
	{ "lightslategrey", 0x778899 },
	{ "lightsteelblue", 0xB0C4DE },
	{ "lightyellow", 0xFFFFE0 },
	{ "lime", 0x00FF00 },
	{ "limegreen", 0x32CD32 },
	{ "linen", 0xFAF0E6 },
	{ "magenta", 0xFF00FF },
	{ "maroon", 0x800000 },
	{ "mediumaquamarine", 0x66CDAA },
	{ "mediumblue", 0x0000CD },
	{ "mediumorchid", 0xBA55D3 },
	{ "mediumpurple", 0x9370DB },
	{ "mediumseagreen", 0x3CB371 },
	{ "mediumslateblue", 0x7B68EE },
	{ "mediumspringgreen", 0x00FA9A },
	{ "mediumturquoise", 0x48D1CC },
	{ "mediumvioletred", 0xC71585 },
	{ "midnightblue", 0x191970 },
	{ "mintcream", 0xF5FFFA },
	{ "mistyrose", 0xFFE4E1 },
	{ "moccasin", 0xFFE4B5 },
	{ "navajowhite", 0xFFDEAD },
	{ "navy", 0x000080 },
	{ "oldlace", 0xFDF5E6 },
	{ "olive", 0x808000 },
	{ "olivedrab", 0x6B8E23 },
	{ "orange", 0xFFA500 },
	{ "orangered", 0xFF4500 },
	{ "orchid", 0xDA70D6 },
	{ "palegoldenrod", 0xEEE8AA },
	{ "palegreen", 0x98FB98 },
	{ "paleturquoise", 0xAFEEEE },
	{ "palevioletred", 0xDB7093 },
	{ "papayawhip", 0xFFEFD5 },
	{ "peachpuff", 0xFFDAB9 },
	{ "peru", 0xCD853F },
	{ "pink", 0xFFC0CB },
	{ "plum", 0xDDA0DD },
	{ "powderblue", 0xB0E0E6 },
	{ "purple", 0x800080 },
	{ "red", 0xFF0000 },
	{ "rosybrown", 0xBC8F8F },
	{ "royalblue", 0x4169E1 },
	{ "saddlebrown", 0x8B4513 },
	{ "salmon", 0xFA8072 },
	{ "sandybrown", 0xF4A460 },
	{ "seagreen", 0x2E8B57 },
	{ "seashell", 0xFFF5EE },
	{ "sienna", 0xA0522D },
	{ "silver", 0xC0C0C0 },
	{ "skyblue", 0x87CEEB },
	{ "slateblue", 0x6A5ACD },
	{ "slategray", 0x708090 },
	{ "slategrey", 0x708090 },
	{ "snow", 0xFFFAFA },
	{ "springgreen", 0x00FF7F },
	{ "steelblue", 0x4682B4 },
	{ "tan", 0xD2B48C },
	{ "teal", 0x008080 },
	{ "thistle", 0xD8BFD8 },
	{ "tomato", 0xFF6347 },
	{ "turquoise", 0x40E0D0 },
	{ "violet", 0xEE82EE },
	{ "wheat", 0xF5DEB3 },
	{ "white", 0xFFFFFF },
	{ "whitesmoke", 0xF5F5F5 },
	{ "yellow", 0xFFFF00 },
	{ "yellowgreen", 0x9ACD32 }
};


static int colour_compare_function(const void *ain, const void *bin)
{
    colour_info *ap = (colour_info *) ain, *bp = (colour_info *)bin;

    return strcasecomp(ap->name, bp->name);
}

VALUE colour_lookup(STRING name)
{
    colour_info test, *match;
    VALUE v;

    PRSDBGN(("colour_lookup: %.*s\n", name.bytes, name.ptr));
    
    /* special processing for grey */
    if (name.bytes >= 4 &&
	(strncasecomp(name.ptr, "grey", 4) == 0 ||
	 strncasecomp(name.ptr, "gray", 4) == 0))
    {
	v.type = value_tuple;
	
	if (name.bytes > 4 && isdigit(name.ptr[4]))
	{
	    int n;
	    n = name.ptr[4] - '0';
	    if (name.bytes > 5 && isdigit(name.ptr[5]))
		n = n*10 + (name.ptr[5] - '0');
	    n = (n*255 + 50)/99;
	    v.u.b = (n<<16) | (n<<8) | n;
	}
	else
	{
	    v.u.b = 0x888888;
	}
	
	PRSDBGN(("colour_lookup: grey %08x\n", v.u.b));

	return v;
    }
    
    /* else look it up in the array */
    test.name = stringdup(name);
    match = bsearch(&test, colour_array, sizeof(colour_array) / sizeof(colour_array[0]), sizeof(colour_array[0]), colour_compare_function);
    mm_free(test.name);

    if (match == NULL)
    {
	v.type = value_none;
	PRSDBGN(("colour_lookup: no match\n"));
    }
    else
    {
	v.type = value_tuple;
	v.u.b = match->rgb_word;
	PRSDBGN(("colour_lookup: tuple %08x\n", v.u.b));
    }
    
    return v;
}

/* eof colours.c */
