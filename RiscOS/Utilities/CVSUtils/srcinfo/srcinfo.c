/************************************************************************/
/*                  Copyright 1996 Acorn Computers Ltd                  */
/*                                                                      */
/*  This material is the confidential trade secret and proprietary      */
/*  information of Acorn Computers. It may not be reproduced, used      */
/*  sold, or transferred to any third party without the prior written   */
/*  consent of Acorn Computers. All rights reserved.                    */
/*                                                                      */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

char *progname;
int little_endian;

#define SPRITE_NAME_LEN    12
#define DIGEST_SIZE	   16
#define NUM_FILES 	   2
#define TEMPLATE_NAME_LEN  12

#define ERR_GOTO(x) printf(x);goto exit_gracefully;

typedef enum {sprite,template} t_processing;

typedef struct {
  char          name[SPRITE_NAME_LEN+1];
  unsigned char digest[DIGEST_SIZE];
  int 		found;
} t_sprite_entry;

typedef struct {
  char          name[TEMPLATE_NAME_LEN+1];
  unsigned char digest[DIGEST_SIZE];
  int 		found;
} t_template_entry;

/* 
 * read string
 */
void
read_string(unsigned char *data, char *str, int limit, int pos)
{
  int i;
  for (i=0; i<limit; i++)
    str[i] = (char)((data[pos+i]>=32)?data[pos+i]:0);

  str[i] = 0;
}

/* 
 * read an integer in from the current file
 */
int
read_integer(unsigned char *data, int pos)
{
  int i,n=0;
  for (i=0; i<4; i++)
    n|=data[pos+i]<<(i*8);

  return (n);
}

/* 
 * display the message digest
 */
void
display_digest(unsigned char *buffer, int length)
{
  int i;
  for (i=0; i<length; i++)
    printf("%02x",buffer[i]);
  printf("\n");
}

/* 
 * compare digests
 */
int 
cmp_digest(unsigned char *buf1, unsigned char *buf2, int length)
{
  int i,d;

  for (i=0; i<length; i++)
  {
    d = buf1[i] - buf2[i];
    if (d)
      return d;
  }
  return (0);
}

/* 
 * process the sprite file
 */
int
sprite_handler(unsigned char *data, int size, t_sprite_entry **entries)
{
  int i,num_sprites,sprite_size;
  int pos=0;
  struct MD5Context ctx;
  t_sprite_entry *sprites;
  
  num_sprites = read_integer(data,pos);
  pos+=4;

  if ((sprites=malloc(num_sprites*sizeof(t_sprite_entry)))==NULL)
  {
    fprintf(stderr,"%s: malloc failed\n",progname);
    return(-1);
  }
  
  pos = read_integer(data,pos)-4;
  
  for (i=0; i<num_sprites; i++)
  {
    sprite_size = read_integer(data,pos);

    read_string(data,sprites[i].name,SPRITE_NAME_LEN,pos+4);
    MD5Init(&ctx);
    MD5Update(&ctx,(md5byte*)(data+pos+8),sprite_size-8);
    MD5Final(sprites[i].digest, &ctx);
    pos += sprite_size;
  }
  *entries = sprites;
  return (num_sprites);
}

/* 
 * process the template file
 */
int
template_handler(unsigned char *data, int size, t_template_entry **entries)
{
  int i,num_windows=0;
  int pos=0;
  t_template_entry *template;
  int offset,window_size;
  struct MD5Context ctx;

  /* 
   * count the windows
   */
  pos = 0x10;
  while (read_integer(data,pos)!=0)
  {
    num_windows++;
    pos+=0x18;
  }

  if ((template=malloc(num_windows*sizeof(t_template_entry)))==NULL)
  {
    fprintf(stderr,"%s: malloc failed\n",progname);
    return(-1);
  }
  
  pos = 0x10;
  for (i=0; i<num_windows; i++)
  {
    offset      = read_integer(data,pos+0);
    window_size = read_integer(data,pos+4);
    read_string(data,template[i].name,TEMPLATE_NAME_LEN,pos+0xC);
    
    MD5Init(&ctx);
    MD5Update(&ctx,(md5byte*)(data+offset),window_size-8);
    MD5Final(template[i].digest, &ctx);
    pos+=0x18;
  }
  *entries = template;
  return(num_windows);
}

/* 
 * load file into memory
 */
int
load_file(char *filename, unsigned char **data)
{
  FILE *fh;
  int size;
  
  if ((fh=fopen(filename,"rb"))==NULL)
  {
    fprintf(stderr,"%s: cannot read file %s\n",progname,filename);
    return (-1);
  }
  
  fseek(fh,0,SEEK_END);
  size = (int)ftell(fh);
  fseek(fh,0,SEEK_SET);
  
  if ((*data = malloc(size))==NULL)
  {
    fprintf(stderr,"%s: malloc failed\n",progname);
    return(-1);
  }
  if (fread(*data,1,size,fh)!=size)
  {
    fprintf(stderr,"%s: error reading from file\n",progname);
    return(-1);
  }
  
  fclose(fh);
  
  return (size);
}

int
process_template(char *filename0, char *filename1)
{
  char *filename[NUM_FILES];
  unsigned char *data=NULL;
  int i,j,file,file_size;
  t_template_entry *entries[NUM_FILES];
  int num_windows[NUM_FILES];

  filename[0] = filename0;
  filename[1] = filename1;

  for (file=0; file<NUM_FILES; file++)
  {
    if (data)
      free(data);
      
    if ((file_size = load_file(filename[file],&data))<=0)
      goto exit_gracefully;
      
    if ((num_windows[file]=template_handler(data,file_size,&entries[file]))<=0)
      goto exit_gracefully;
      
/*     for (i=0; i<num_windows[file]; i++) */
/*     { */
/*       printf("%-12s : ",entries[file][i].name); */
/*       display_digest(entries[file][i].digest, DIGEST_SIZE); */
/*     } */
  }

  for (i=0; i<num_windows[0]; i++)
    entries[0][i].found=0;
  for (j=0; j<num_windows[1]; j++)
    entries[1][j].found=0;

  for (i=0; i<num_windows[0]; i++)
  {
    for (j=0; j<num_windows[1]; j++)
    {
      if (strcmp(entries[0][i].name,entries[1][j].name)==0)
      {
        
        if (cmp_digest(entries[0][i].digest,entries[1][j].digest,DIGEST_SIZE))
          printf("  %-12s : different\n",entries[0][i].name);

        entries[0][i].found=1;
        entries[1][j].found=1;
        break;
      }
    }
    if (!(entries[0][i].found))
      printf("  %-12s : missing from %s\n",entries[0][i].name,filename[1]);
  }

  for (j=0; j<num_windows[1]; j++)
    if (!(entries[1][j].found))
      printf("  %-12s : missing from %s\n",entries[1][j].name,filename[0]);

exit_gracefully:
  if (data)
    free(data);

  for (file=0; file<NUM_FILES; file++)
    if (entries[file])
      free(entries[file]);
      
  return (0);
}

int
process_sprite(char *filename0, char *filename1)
{
  int i,j;
  t_sprite_entry *entries[NUM_FILES];
  int num_sprites[NUM_FILES];
  char *filename[NUM_FILES];
  unsigned char *data=NULL;
  int file_size;
  int file;

  filename[0] = filename0;
  filename[1] = filename1;

  for (file=0; file<NUM_FILES; file++)
  {
    if (data)
      free(data);
      
    if ((file_size = load_file(filename[file],&data))<=0)
      goto exit_gracefully;
      
    if ((num_sprites[file] = sprite_handler(data,file_size,&entries[file]))<=0)
      goto exit_gracefully;
      
/*     for (i=0; i<num_sprites[file]; i++) */
/*     { */
/*       printf("%-12s : ",entries[file][i].name); */
/*       display_digest(entries[file][i].digest, DIGEST_SIZE); */
/*     } */
  }

  /* 
   * clear all found markers
   */
  for (i=0; i<num_sprites[0]; i++)
    entries[0][i].found=0;
  for (j=0; j<num_sprites[1]; j++)
    entries[1][j].found=0;


  for (i=0; i<num_sprites[0]; i++)
  {
    for (j=0; j<num_sprites[1]; j++)
    {
      if (strcmp(entries[0][i].name,entries[1][j].name)==0)
      {
        
        if (cmp_digest(entries[0][i].digest,entries[1][j].digest,DIGEST_SIZE))
          printf("  %-12s : different\n",entries[0][i].name);

        entries[0][i].found=1;
        entries[1][j].found=1;
        break;
      }
    }
    if (!(entries[0][i].found))
      printf("  %-12s : missing from %s\n",entries[0][i].name,filename[1]);
  }

  for (j=0; j<num_sprites[1]; j++)
    if (!(entries[1][j].found))
      printf("  %-12s : missing from %s\n",entries[1][j].name,filename[0]);

exit_gracefully:
  if (data)
    free(data);

  for (file=0; file<NUM_FILES; file++)
    if (entries[file])
      free(entries[file]);
      
  return (0);    
}

int
main(int argc, char *argv[])
{
  int i;
  t_processing processing;
  
  progname = argv[0];
  if (argc<3 || argc>4)
  {
    fprintf(stderr,"Usage: %s [-sprite|-template] <filename1> <filename2>\n",progname);
    exit(1);
  }
  
  little_endian = ((*(char*)&i)==1)?0:1;	/* find endianess */
  
  i=1;
  while (argv[i][0] == '-')
  {
    if (strcmp(argv[i],"-sprite")==0)
      processing = sprite;
    else if (strcmp(argv[i],"-template")==0)
      processing = template;
    i++;
  }

  switch (processing)
  {
    case sprite :
      process_sprite(argv[i],argv[i+1]);
      break;
    case template :
      process_template(argv[i],argv[i+1]);
      break;
    default :
      printf("%s unsupported format\n",progname);
  }
  
  return (0);
}
