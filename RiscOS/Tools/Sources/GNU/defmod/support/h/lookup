#ifndef lookup_H
#define lookup_H

/*lookup.h - simple lookup facilities*/

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

#ifndef os_H
   #include "os.h"
#endif

/* Implement ation of an abstract lookup table consisting of a token
**  and a pointer to an associated data item
*/


typedef struct lookup_t *lookup_t;

/* create a new abstract lookup table */
extern os_error *lookup_new( lookup_t *,  /* updated with pointer to new table */
                             int          /* initial size of table */
                           );

/* delete the entire table */
extern os_error *lookup_delete (lookup_t);

/* search the table for a specific item */
extern os_error *lookup(lookup_t, /* pointer to table */
                        char *,   /* token to match */
                        void **   /* pointer to destination for result:
                                        NULL if entry not found
                                        else pointer to data item
                                  */
                       );

/* add an entry to the table */
extern os_error *lookup_insert( lookup_t, /* table */
                                char *,   /* pointer to token */
                                void *    /* pointer to data item */
                              );

/* sequentially retrieve all tokens in the table */
extern os_error *lookup_enumerate(  lookup_t, /* table */
                                    char **,  /* destination for pointer to token */
                                    void **,  /* destination for pointer to data item */
                                    void **   /* context pointer; start with 0;
                                              ** updated to for each iteration
                                              ** 0 if no more found
                                              */
                                 );

#endif
