#ifndef territory_H
#define territory_H

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

#include <swis.h>

#ifndef types_H
   #include "types.h"
#endif

typedef int territory_t;

#define territory_CURRENT ((territory_t) -1)

#define territory_IGNORE_CASE    (1U << 0)
#define territory_IGNORE_ACCENTS (1U << 1)

#define territory_SYMBOL_DECIMAL_POINT 0

#ifdef EXECUTE_ON_UNIX

/* No UNIX definitions required for this component */

#else /* EXECUTE_ON_UNIX */

#define xterritory_number() \
   _swix (Territory_Number, )

#define xterritory_register() \
   _swix (Territory_Register, )

#define xterritory_deregister() \
   _swix (Territory_Deregister, )

#define xterritory_number_to_name() \
   _swix (Territory_NumberToName, )

#define xterritory_exists() \
   _swix (Territory_Exists, )

#define xterritory_alphabet_number_to_name() \
   _swix (Territory_AlphabetNumberToName, )

#define xterritory_select_alphabet() \
   _swix (Territory_SelectAlphabet, )

#define xterritory_set_time() \
   _swix (Territory_SetTime, )

#define xterritory_read_current_timezone() \
   _swix (Territory_ReadCurrentTimeZone, )

#define xterritory_convert_time_to_utc_ordinals() \
   _swix (Territory_ConvertTimeToUTCOrdinals, )

#define xterritory_read_timezones() \
   _swix (Territory_ReadTimeZones, )

#define xterritory_convert_date_and_time() \
   _swix (Territory_ConvertDateAndTime, )

#define xterritory_convert_standard_date_and_time() \
   _swix (Territory_ConvertStandardDateAndTime, )

#define xterritory_convert_standard_date() \
   _swix (Territory_ConvertStandardDate, )

#define xterritory_convert_standard_time() \
   _swix (Territory_ConvertStandardTime, )

#define xterritory_convert_time_to_ordinals() \
   _swix (Territory_ConvertTimeToOrdinals, )

#define xterritory_convert_time_string_to_ordinals() \
   _swix (Territory_ConvertTimeStringToOrdinals, )

#define xterritory_convert_ordinals_to_time() \
   _swix (Territory_ConvertOrdinalsToTime, )

#define xterritory_alphabet() \
   _swix (Territory_Alphabet, )

#define xterritory_alphabet_identifier() \
   _swix (Territory_AlphabetIdentifier, )

#define xterritory_select_keyboard_handler() \
   _swix (Territory_SelectKeyboardHandler, )

#define xterritory_write_direction() \
   _swix (Territory_WriteDirection, )

#define xterritory_character_property_table() \
   _swix (Territory_CharacterPropertyTable, )

#define xterritory_lower_case_table(terr, table) \
   _swix (Territory_LowerCaseTable, _IN (0) | _OUT (0), \
         (int) (terr), (char **) (table))

#define xterritory_upper_case_table(terr, table) \
   _swix (Territory_UpperCaseTable, _IN (0) | _OUT (0), \
         (int) (terr), (char **) (table))

#define xterritory_control_table() \
   _swix (Territory_ControlTable, )

#define xterritory_plain_table() \
   _swix (Territory_PlainTable, )

#define xterritory_value_table() \
   _swix (Territory_ValueTable, )

#define xterritory_representation_table() \
   _swix (Territory_RepresentationTable, )

#define xterritory_collate(terr, s1, s2, flags, collate) \
   _swix (Territory_Collate, \
         _IN (0) | _IN (1) | _IN (2) | _IN (3) | _OUT (0), \
         (territory_t) (terr), (char *) (s1), (char *) (s2), \
         (int) (flags), (int *) (collate))

#define xterritory_read_string_symbols(terr, sym, val) \
   _swix (Territory_ReadSymbols, \
         _IN (0) | _IN (1) | _OUT (0), \
         (territory_t) (terr), (int) (sym), (char **) (val))

#define xterritory_read_calendar_information() \
   _swix (Territory_ReadCalendarInformation, )

#define xterritory_name_to_number() \
   _swix (Territory_NameToNumber, )

#define xterritory_convert_text_to_string() \
   _swix (Territory_ConvertTextToString, )

#endif /* EXECUTE_ON_UNIX */

#endif
