#include <a.out.h>
#include <stab.h>
#include <stdio.h>

int valcmp(struct nlist *s1, struct nlist *s2)
{
  if ((s1->n_type > 0x7) ^ (s2->n_type > 0x7))
    return (s1->n_type > 0x7 ? 1 : -1);

  if (s1->n_value > s2->n_value)
    return 1;
  else if (s1->n_value < s2->n_value)
    return -1;
  else
    return 0;
}

void main(int argc, char **argv)
{
  FILE *fp;
  struct exec ex;
  int symoff, stroff, i, sz, nsyms, eofln, addr;
  struct nlist *syms;
  char *strs, *ptr;

  if (argc < 3)
    {
      printf("usage: nmmap <prog-name> <hex-addr>\n");
      return;
    }

  fp = fopen(argv[1], "r");
  if (!fp)
    {
      perror(argv[1]);
      return;
    }

  fread(&ex, sizeof(struct exec), 1, fp);
  if (N_BADMAG(ex))
    {
      printf("%s: bad magic number\n", argv[1]);
      return;
    }

  symoff = N_SYMOFF(ex);
  stroff = N_STROFF(ex);
  sz = ex.a_syms;

  if (!sz)
    {
      printf("%s: no name list\n", argv[1]);
      return;
    }

  /* read symbol table */
  syms = (struct nlist *) malloc(sz);
  nsyms = sz / sizeof(struct nlist);

  fseek(fp, symoff, 0);
  fread(syms, sizeof(struct nlist), nsyms, fp);

  /* read string table */
  fseek(fp, 0, 2);
  eofln = ftell(fp);
  fseek(fp, stroff, 0);

  sz = eofln - stroff;
  strs = (char *) malloc(sz);
  fread(strs, sz, 1, fp);

  fclose(fp);

  addr = strtol(argv[2], &ptr, 0);
  if (*ptr != '\0')
    {
      printf("nmmap: invalid parameter\n");
      return;
    }

  /* sort the table by address */
  qsort(syms, nsyms, sizeof(struct nlist), valcmp);

  /* scan symbol table */
  for (i = 0; i < nsyms; i++)
    if (syms[i].n_type & N_TEXT)
      if (syms[i].n_value <= addr &&
	  (i == nsyms - 1 || syms[i+1].n_value > addr))
	break;

  if (i < nsyms)
    printf("%s: %08x %s\n", argv[1], syms[i].n_value,
	   strs + syms[i].n_un.n_strx);
  else
    printf("%s: %08x <not found>\n", argv[1], addr);
}
