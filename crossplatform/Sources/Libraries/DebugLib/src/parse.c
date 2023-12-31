/*****************************************************************************/
/* File:    parse.c                                                          */
/* Purpose: Level string parser                                              */
/*                                                                           */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved     */
/*                                                                           */
/* The copyright in this material is owned by Pace Micro Technology PLC      */
/* ("Pace").  This material is regarded as a highly confidential trade       */
/* secret of Pace.  It may not be reproduced, used, sold or in any           */
/* other way exploited or transferred to any third party without the         */
/* prior written permission of Pace.                                         */
/*****************************************************************************/

#include "include.h"
#include "parse.h"

#ifdef STDC_HEADERS
#  include <ctype.h>
#else
#  error "This file requires ctype.h"
#endif

static char *debug_skip_ws (char *c)
{
  while (*c && isspace(*(unsigned char *)c))
  {
    ++c;
  }
  return c;
}

static unsigned int debug_parse_bit_spec(char *spec, int ex, unsigned int bits)
{
  int prefix;
  char *next;

  prefix = strspn(spec, "0123456789-, ");
  if (spec[prefix])
  {
    /* Faulty character */
#ifdef TEST
    fprintf(stderr, "Invalid character in bit spec: `%s'\n", spec);
#endif
    return bits;
  }

  for (; *spec; spec=next)
  {
    int min,max, cm1, cm2, cm3;
    prefix = strcspn(spec, ",");
    next = spec + prefix;
    if (*next)
    {
      *next++ = '\0';
    }
    prefix = sscanf(spec, "%d%n-%n%d%n", &min, &cm1, &cm2, &max, &cm3);
    if (prefix <= 0)
    {
#ifdef TEST
      fprintf(stderr, "Unable to parse bit spec: `%s'\n", spec);
#endif
      continue; /* error */
    }
    if (prefix == 1)
    {
      /* Fake a range */
      max = min;
      cm3 = cm1;
    }
    if (min > max)
    {
      int tmp = max;
      max = min;
      min = tmp;
    }
    if (min < 0 || max > 9)
    {
#ifdef TEST
      fprintf(stderr, "Out of range (%d, %d)\n", min, max);
#endif
      continue;
    }
    if (ex)
    {
      bits |= _LEVELS_EXCLUDED(min, max);
    }
    else
    {
      bits |= _LEVELS_INCLUDED(min, max);
    }
  }

  return bits;
}

void debug_parse_levels(debug_level_setter callback, char *c)
{
  char *area, *next, z;
  int prefix;
  unsigned int bits;

  for (; c && *c; c = next)
  {
    next = NULL;
    bits = 0;
    area = debug_skip_ws(c);
    prefix = strcspn(area, ",(");
    z = area[prefix];
    if (z == ',')
    {
      next = area + prefix + 1;
    }
    c = area + prefix;
    *c++ = '\0'; /* Terminate the area name */
    if (z != '(')
    {
      (*callback)(area, _LEVELS_INCLUDED(0,9));
      continue;
    }
    prefix = strcspn(c, ")");
    if (!c[prefix])
    {
      /* No matching ) - panic */
#ifdef TEST
      printf("panic - no matching )\n");
#endif
      return;
    }
    next = c + prefix;
    *next++ = '\0';
    next = debug_skip_ws(next);
    bits = debug_parse_bit_spec(c, 0, bits);
    if (*next == '(')
    {
      ++next;
      prefix = strcspn(next, ")");
      if (!next[prefix])
      {
        return;
      }
      next[prefix] = '\0';
      bits = debug_parse_bit_spec(next, 1, bits);
      next += prefix + 1;
    }

    (*callback)(area, bits);

    next = debug_skip_ws(next);
    if (*next == ',')
    {
      next = debug_skip_ws(next + 1);
    }
    else
    {
      if (*next) return;
    }
  }
}


#ifdef TEST
static void level_dumper(const char *area, unsigned int level)
{
  unsigned int i;
  printf("Area: >>%s<<\n", area);
  printf("  Included: ");
  for (i=0; i<10; ++i)
  {
    if (_LEVEL_INCLUDED(i) & level)
      putchar(i + '0');
    else
      putchar(' ');
  }
  printf("   Excluded: ");
  for (i=0; i<10; ++i)
  {
    if (_LEVEL_EXCLUDED(i) & level)
      putchar(i + '0');
    else
      putchar(' ');
  }
  putchar('\n');
}

static const char *tests[] =
{
  "UILib()(0-9)",
  "AreaA,AreaB",
  "AreaC(0-4)",
  "AreaD(1-9)(0-4)",
  "UILib()(),Spong()(1),Bar(6,7,4-8,1-2,9-9)",
  "ErrorBig(10)",
  "ErrorSym(-1)",
  "",
  "(4)",
};

int main(void)
{
  char test[BUFSIZ + 1];
  int i;

  test[BUFSIZ] = '\0';
  for (i=0; i<(sizeof(tests)/sizeof(*tests)); ++i)
  {
    strncpy(test, tests[i], BUFSIZ);
    printf("********************\nTest %d: >>>%s<<<\n", i, test);
    debug_parse_levels(level_dumper, test);
  }

  return 0;
}
#endif
