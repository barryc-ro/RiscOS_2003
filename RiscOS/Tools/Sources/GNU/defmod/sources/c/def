/*def.c - support routines*/

/*OSLib---efficient, type-safe, transparent, extensible,\n"
   register-safe A P I coverage of RISC O S*/
/*Copyright © 1994 Jonathan Coxhead*/

/*
      OSLib is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

      OSLib is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
   along with this programme; if not, write to the Free Software
   Foundation, Inc, 675 Mass Ave, Cambridge, MA 02139, U S A.
*/

/*From CLib*/
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*From Support*/
#include "lookup.h"

/*Local*/
#include "def.h"

#define ISLOWER(s, i, len)        \
(                                 \
   (i) < (len) &&                 \
   (  isdigit ((s) [i])?          \
         islower ((s) [(i) - 1]): \
         islower ((s) [i])        \
   )                              \
)

#define ISUPPER(s, i, len)        \
(                                 \
   (i) < (len) &&                 \
   (  isdigit ((s) [i])?          \
         isupper ((s) [(i) - 1]): \
         isupper ((s) [i])        \
   )                              \
)

/*A string type on the stack, for type encodings.*/
typedef struct String {char s [def_ID_LIMIT + 1];} String;

void def_as_extern
(
   char *s0,
   const char *s1
)
{
   enum {MODULE, NAME}  state = MODULE;
   char                *cc0 = s0, *last = cc0;
   int                  i, len = strlen (s1);

   if (strcmp (s1, "Hourglass_LEDs") == 0)
      strcpy (s0, "hourglass_leds");
   else if (strcmp (s1, "OS_CallAVector") == 0)
      strcpy (s0, "os_call_a_vector");
   else
   {
      for (i = 0; i < len; i++)
      {
         switch (state)
         {
            case MODULE:
               *cc0++ = tolower (s1 [i]);
               last = cc0;
               if (s1 [i] == '_')
                  state = NAME;
            break;

            case NAME:
               *cc0++ = tolower (s1 [i]);
               last = cc0;
               if
               /* isupper [i + 1] &&
                  (islower [i] || islower [i + 2]) &&
                  [i + 2] isn't the null at the end &&
                  this isn't a one-letter word
               */
               (
                  ISUPPER (s1, i + 1, len) &&
                  (ISLOWER (s1, i, len) || ISLOWER (s1, i + 2, len)) &&
                  i + 2 != len &&
                  cc0 [-2] != '_'
               )
                  *cc0++ = '_';
            break;
         }
      }
      *last = '\0';
   }
}

void def_as_macro
(
   char *s0,
   const char *s1
)
{
   enum {MODULE, NAME} state = MODULE;
   char *cc0 = s0, *last = cc0;
   int i, len = strlen (s1);

   if (strcmp (s1, "Hourglass_LEDs") == 0)
      strcpy (s0, "hourglass_LEDS");
   else if (strcmp (s1, "OS_CallAVector") == 0)
      strcpy (s0, "os_CALL_A_VECTOR");
   else
   {
      for (i = 0; i < len; i++)
      {
         switch (state)
         {
            case MODULE:
               *cc0++ = tolower (s1 [i]);
               last = cc0;
               if (s1 [i] == '_') state = NAME;
            break;

            case NAME:
               *cc0++ = toupper (s1 [i]);
               last = cc0;
               if
               (
                  ISUPPER (s1, i + 1, len) &&
                  (ISLOWER (s1, i, len) || ISLOWER (s1, i + 2, len)) &&
                  i + 2 != len &&
                  cc0 [-2] != '_'
               )
                  *cc0++ = '_';
            break;
         }
      }
      *last = '\0';
   }
}

void def_as_prefix
(
   char *s0,
   char *s1
)
{
   char *cc0 = s0, *last = cc0;
   int i, len = strlen (s1);

   for (i = 0; i < len; i++)
   {
      *cc0++ = tolower (s1 [i]);
      last = cc0;
      if (s1 [i] == '_') break;
   }
   *last = '\0';
}

static String Name_Code
(
   def_t t
)
{
   String result;

   switch (t->tag)
   {
      case def_TYPE_INT:
         strcpy (result.s, "i");
      break;

      case def_TYPE_SHORT:
         strcpy (result.s, "s");
      break;

      case def_TYPE_BYTE:
         strcpy (result.s, "Uc");
      break;

      case def_TYPE_CHAR:
         strcpy (result.s, "c");
      break;

      case def_TYPE_BITS:
         strcpy (result.s, "Ui");
      break;

      case def_TYPE_BYTES:
         strcpy (result.s, "Ui");
      break;

      case def_TYPE_BOOL:
         strcpy (result.s, "i");
      break;

      case def_TYPE_REF:
      {
         String n = Name_Code (t->data AS ref);

         sprintf (result.s, "P%s", n.s);
      }
      break;

      case def_TYPE_STRING:
         strcpy (result.s, "c");
      break;

      case def_TYPE_ASM:
         strcpy (result.s, "v");
      break;

      case def_TYPE_DATA:
         strcpy (result.s, "v");
      break;

      case def_TYPE_ROW:
      {
         String n = Name_Code (t->data AS row.base);

         sprintf (result.s, "A%d_%s", t->data AS row.count, n.s);
      }
      break;

      case def_TYPE_VOID:
         strcpy (result.s, "v");
      break;

      case def_TYPE_ID:
      {
         /*We should look up an id in the types table until we find a struct
            or union definition, becaue |typedef| does not introduce a new
            type, but only a new name for an old one. This fails horribly if
            there isn't a definition for the type (because it's defined in
            some other module, typically), so we should give an error
            then.*/
         char c_name [def_ID_LIMIT + 1];

         def_as_extern (c_name, t->data AS id);

         sprintf (result.s, "%d%s", strlen (c_name), c_name);
      }
      break;
   }

   return result;
}

void def_as_c_plus_plus
(
   char  *c_plus_plus_name,
   char  *swi_name,
   def_s  s
)
{
   osbool    first = TRUE;
   int       i, argc = 0;
   String    arg;
   lookup_t  argt;
   char     *ptr;
   os_error *error;

   /*A source of unique addresses.*/
   static char Cookies [] =
   {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
      10, 11, 12, 13, 14, 15, 16, 17, 18, 19
   };

   assert (lookup_new (&argt, 1) == NULL);

   def_as_extern (c_plus_plus_name, swi_name);
   strcat (c_plus_plus_name, "__F");

   if (!def_using_block (s))
   {
      for (i = 0; i < 10; i++)
         if ((s->i & 1 << i) != NONE)
         {
            first = FALSE;

            if ((s->ri & 1 << i) != NONE)
            {
               String n = Name_Code (s->inputs [i]);

               strcpy (arg.s, "P");
               strcat (arg.s, n.s);
            }
            else
               arg = Name_Code (s->inputs [i]);

            /*Check whether this argument has been used before.*/
            assert
            (
               (error = lookup (argt, arg.s, (void **) &ptr)) == NULL ||
               error->errnum == os_GLOBAL_NO_ANY
            );
            if (error == NULL)
               /*Yes it has - we want to use a reference to this.*/
               /* *** THIS IS AMBIGUOUS IF |*ptr >= 9| *** */
               sprintf (arg.s, "T%d", *ptr + 1);
            else
               /*Put the argument coding into the table.*/
               assert (lookup_insert (argt, arg.s, &Cookies [argc]) ==
                     NULL);

            argc++;

            /*Append this argument to the total.*/
            strcat (c_plus_plus_name, arg.s);
         }

      for (i = 0; i < 10; i++)
         if ((s->o & 1 << i) != NONE)
         {
            String n = Name_Code (s->outputs [i]);

            first = FALSE;
            if ((s->ro & 1 << i) != NONE)
               strcpy (arg.s, "PP");
            else
               strcpy (arg.s, "P");
            strcat (arg.s, n.s);

            /*Check whether this argument has been used before.*/
            assert
            (
               (error = lookup (argt, arg.s, (void **) &ptr)) == NULL ||
               error->errnum == os_GLOBAL_NO_ANY
            );
            if (error == NULL)
               /*Yes it has - we want to use a reference to this.*/
               /* *** THIS IS AMBIGUOUS IF |*ptr >= 9| *** */
               sprintf (arg.s, "T%d", *ptr + 1);
            else
               /*Put the argument coding into the table.*/
               assert (lookup_insert (argt, arg.s, &Cookies [argc]) ==
                     NULL);

            argc++;

            /*Append this argument to the total.*/
            strcat (c_plus_plus_name, arg.s);
         }

      if (s->f_out)
      {
         first = FALSE;
         strcpy (arg.s, "PUi");

         /*Check whether this argument has been used before.*/
         assert
         (
            (error = lookup (argt, arg.s, (void **) &ptr)) == NULL ||
            error->errnum == os_GLOBAL_NO_ANY
         );
         if (error == NULL)
            /*Yes it has - we want to use a reference to this.*/
            /* *** THIS IS AMBIGUOUS IF |*ptr >= 9| *** */
            sprintf (arg.s, "T%d", *ptr + 1);
         else
            /*Put the argument coding into the table.*/
            assert (lookup_insert (argt, arg.s, &Cookies [argc]) == NULL);

         argc++;

         /*Append this argument to the total.*/
         strcat (c_plus_plus_name, arg.s);
      }
   }
   else
   {
      /*First locate the register pointing to the block.*/
      for (i = 0; i < 10; i++)
         if ((s->i & 1 << i) != 0)
         {
            int cpt;

            for (cpt = 0; cpt < s->inputs [i]->data AS list.count; cpt++)
            {
               first = FALSE;

               arg = Name_Code (s->inputs [i]->data AS list.members [cpt]);

               /*Check whether this argument has been used before.*/
               assert
               (
                  (error = lookup (argt, arg.s, (void **) &ptr)) == NULL ||
                  error->errnum == os_GLOBAL_NO_ANY
               );
               if (error == NULL)
                  /*Yes it has - we want to use a reference to this.*/
                  /* *** THIS IS AMBIGUOUS IF |*ptr >= 9| *** */
                  sprintf (arg.s, "T%d", *ptr + 1);
               else
                  /*Put the argument coding into the table.*/
                  assert (lookup_insert (argt, arg.s, &Cookies [argc]) ==
                        NULL);

               argc++;

               /*Append this argument to the total.*/
               strcat (c_plus_plus_name, arg.s);
            }

            break;
         }
   }

   if (first)
      strcat (c_plus_plus_name, "v");

   assert (lookup_delete (argt) == NULL);
}

/* Whether the given SWI should be defined using _BLOCK:

   no output arguments
   only one input argument, which is

         Rx -> .Struct (<something with no ...>)
*/

osbool def_using_block
(
   def_s s
)
{
   int i;
   osbool got_one = FALSE;

   if (s->o != 0) return FALSE;

   for (i = 0; i < 10; i++)
      if ((s->i & 1 << i) != 0)
      {
         if (!got_one &&
               (s->ri & 1 << i) != 0 &&
               s->inputs [i]->tag == def_TYPE_STRUCT &&
               !s->inputs [i]->data AS list.ellipsis)
            got_one = TRUE;
         else
            return FALSE;
      }

   return got_one;
}
/*------------------------------------------------------------------------*/

/*Returns the index of the |arg|'th set bit of |i|.*/

int def_bit_index
(
   bits i,
   int arg
)
{
   int b = 0, n;

   for (n = 0; n < 16; n++)
      if ((i & 1 << n) != 0 && b++ == arg)
         return n;

   return -1;
}
/*-----------------------------------------------------------------------*/

/*Whether a SWI could be rendered inline by a SWI instruction.*/

osbool def_inlinable
(
   def_s s
)
{
   if (s->absent) return FALSE;

   /*Input registers must be a leading subsequence of (R0, R1, R2, R3).*/
   if (!(s->i == NONE || s->i == 1u || s->i == 3u || s->i == 7u ||
         s->i == 0xFu))
      return FALSE;

   /*Output registers must be R0 or nothing.*/
   if (!(s->o == NONE || s->o == 1u)) return FALSE;

   /*Corrupted registers must be among {R0, R1, R2, R3}.*/
   if ((s->c & ~0xFu) != NONE) return FALSE;

   /*No constants can be provided.*/
   if (s->k != NONE) return FALSE;

   /*No flags can be provided on input.*/
   if (s->f_in) return FALSE;

   /*Or returned on output.*/
   if (s->f_out) return FALSE;

   /*The returned register must be R0 or nothing.*/
   if (!(s->value == NONE || s->value == 1u)) return FALSE;

   return TRUE;
}
