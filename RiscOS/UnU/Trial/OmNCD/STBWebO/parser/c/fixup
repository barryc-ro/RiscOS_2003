/* fixup.c - finish pointer entries for parser information */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

/* included by attrgen.py into htmldefs.c automatically */
/* When created, most "pointers" are actually the index into */
/* the relevant table. Converts these index values to pointers. */

extern void sgml_do_parser_fixups(void)
{
        static long done = 0;

        STRING *string;
        ATTRIBUTE *attrib, **attribp;
        ELEMENT *element;
        long ix, i;

        if (done)
                return;
        else
                done = 1;

        for (string = &templates_table[0], ix = 0; ix < sizeof(templates_table); ix += sizeof(STRING), string++)
        {
                i = (long) (string->ptr);
                string->ptr = i == -1 ? NULL : string_table[i];
        }

        for (attrib = &attributes_list[0], ix = 0; ix < sizeof(attributes_list); ix += sizeof(ATTRIBUTE), attrib++)
        {
                i = (long) attrib->name.ptr;
                attrib->name.ptr = i == -1 ? NULL : string_table[i];
                i = (long) attrib->templates;
                attrib->templates = i == -1 ? NULL : &templates_table[i];
        }

        for (attribp = &attributes_table[0], ix = 0; ix < sizeof(attributes_table); attribp++, ix += sizeof(ATTRIBUTE *))
        {
                i = (long) *attribp;
                *attribp = i == -1 ? NULL : &attributes_list[i];
        }

        for (element = &elements[0], ix = 0; ix < sizeof(elements); ix += sizeof(ELEMENT), element++)
        {
                i = (long) element->name.ptr;
                element->name.ptr = i == -1 ? NULL : string_table[i];
                i = (long) element->attributes;
                element->attributes = &attributes_table[i];
                i = (long) element->requirements;
                element->requirements = &requirements_table[i];
        }
}

/* end fixup.c */
