/************************************************************************/
/*                  Copyright 1996 Acorn Computers Ltd                  */
/*                                                                      */
/*  This material is the confidential trade secret and proprietary      */
/*  information of Acorn Computers. It may not be reproduced, used      */
/*  sold, or transferred to any third party without the prior written   */
/*  consent of Acorn Computers. All rights reserved.                    */
/*                                                                      */
/************************************************************************/

/*
 * ==========================================================================
 *
 * This code has been developed as part of the NCOS1.15 development strategy
 * for the HTTP transport section of the CSFS filing system. It is supplied
 * as is and no guarentee can be given as to its "fitness for function". It
 * is provided as a reference/starting point for the NetChannel server
 * developers. Some functions for query string parsing were nicked from the
 * web and these are identified as such.
 *
 * This code has been developed under Solaris-X86 and NetBSD and run with
 * the SERN and Apache web servers. Debugging information is enabled by
 * default and will write its output to /tmp/smurfs-debug. Testing can be
 * performed using a browser with URL along the lines of ...
 *
 * http://bungle/cgi-bin/smurfs?path=/&username=rich&password=coool
 *
 * ==========================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#define CHK_STR(x) (x==NULL?"(null)":x)

#define DEBUG_FILE     "/tmp/smurfs_debug"
#define ROOT_DIRECTORY "/export/home/ncd"
#define MAX_FILENAME   256
#define MAX_ENTRIES    10
#define MAX_ENTRY_SIZE 256

FILE *debug = NULL;

typedef struct {
    char name[MAX_ENTRY_SIZE];
    char val[MAX_ENTRY_SIZE];
} entry;

/*
 * --------------------------------------------------------------------------
 *
 * The following few routines were nicked from the web
 */
void
getword(char *word, char *line, char stop)
{
  int x = 0,y;

  for(x=0;((line[x]) && (line[x] != stop));x++)
    word[x] = line[x];

  word[x] = '\0';
  if(line[x])
    ++x;
  y=0;

  while((line[y++] = line[x++])) ;
}

char
x2c(char *what)
{
  register char digit;

  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));

  return(digit);
}

void
unescape_url(char *url)
{
  register int x,y;

  for(x=0,y=0;url[y];++x,++y)
  {
    if((url[x] = url[y]) == '%')
    {
      url[x] = x2c(&url[y+1]);
      y+=2;
    }
  }
  url[x] = '\0';
}

void
plustospace(char *str)
{
  register int x;

  for(x=0;str[x];x++)
    if(str[x] == '+')
      str[x] = ' ';
}
int
ind(char *s, char c)
{
  register int x;

  for(x=0;s[x];x++)
    if(s[x] == c) return x;

  return -1;
}

void
escape_shell_cmd(char *cmd)
{
  register int x,y,l;

  l=strlen(cmd);
  for(x=0;cmd[x];x++)
  {
    if(ind("&;`'\"|*?~<>^()[]{}$\\",cmd[x]) != -1)
    {
      for(y=l+1;y>x;y--)
        cmd[y] = cmd[y-1];
      l++; /* length has been increased */
      cmd[x] = '\\';
      x++; /* skip the character */
    }
  }
}

/*
 * --------------------------------------------------------------------------
 */

 /*
 * dump info to the debug file
 */
void
db_printf(char *format, ...)
{
  va_list list;
  char debug_line[256];

  if (debug)
  {
    va_start (list, format);
    vsprintf(debug_line,format, list);
    va_end(list);

    fprintf(debug, debug_line);
  }
}

/*
 * convert unix 4 byte time to 5 byte riscos time - number of cs since 1st Jan
 * 1900. Multiply by 100 breaks down into 3 shifts checking for carry. Then
 * add difference between 00:00:00 1st Jan 1900 and 00:00:00 1st Jan 1970
 */
void
to_riscos_time(time_t in, unsigned int *timelo, unsigned char *timehi)
{
  unsigned int  outlo,sum;
  unsigned char outhi;

  outlo  = in<<6;          /* *64 */
  outhi  = in>>(32-6);     /* remainder */

  sum = outlo+(in<<5);     /* *32 */
  if (sum<outlo) ++outhi;
  outhi += in>>(32-5);     /* remainder */
  outlo = sum;

  sum = outlo+(in<<2);     /* *4 */
  if (sum<outlo) ++outhi;
  outhi += in>>(32-2);     /* remainder */
  outlo = sum;

  sum = outlo+0x6e93ebc0U; /* add number of cs between Jan 1900 and Jan 1970 */
  if (sum<outlo) ++outhi;  /* check for carry */
  outhi += 0x33;
  outlo = sum;

  *timelo = outlo;
  *timehi = outhi;
}

/*
 * for the given file, dump out the stats in the correct format for
 * smurfs
 */
void
dump_dir_listing(struct dirent *dp, char *filename)
{
  unsigned int load_addr;
  unsigned int exec_addr;
  unsigned char timehi;
  char filescan[MAX_FILENAME];
  int file_attrib;
  int file_type;
  struct stat sb;
  int i,j,k;

  /*
   * ignore current and parent directory identifier
   */
  if ((strcmp(dp->d_name,".")==0) || (strcmp(dp->d_name,"..")==0))
    return;

  /*
   * accomodate missing /
   */
  if (filescan[strlen(filescan)-1]=='/')
    sprintf(filescan,"%s%s",filename,dp->d_name);
  else
    sprintf(filescan,"%s/%s",filename,dp->d_name);


  /*
   * get info on the file
   */
  stat(filescan,&sb);
  file_type = (sb.st_mode&S_IFDIR)?2:1;

  /*
   * should be safe to trash filescan
   */
  strcpy(filescan,dp->d_name);
  i=0;
  while (filescan[i]!=0)  /* change '.' to '/' required for RiscOS */
  {
    if (filescan[i]=='.')
      filescan[i] = '/';
    i++;
  }

  load_addr=(unsigned int)0xfff<<20 | 0xfff<<8;   /* 0xffftttdd */
  if (file_type==1) /* files */
  {
    i=strlen(filescan)-1;             /* point to end of filename */
    while (i>=0 && filescan[i]!=',')  /*  find the comma */
      i--;

    /* valid extension ? */
    if (i>0 && filescan[i+1]!=0 && filescan[i+2]!=0 && filescan[i+3]!=0)
    {
      load_addr &= (unsigned int)0xfff<<20; /* typed file */
      for (k=0; k<3; k++)
      {
        filescan[i+k+1] = toupper(filescan[i+k+1]);
        if (filescan[i+k+1]>='0'&&filescan[i+k+1]<='9')
          j=filescan[i+k+1]-'0';
        else if (filescan[i+k+1]>='A'&&filescan[i+k+1]<='F')
          j=filescan[i+k+1]-'A'+10;

        load_addr |= (j&0xf)<<(((2-k)*4)+8);
      }
      filescan[i] = '\0'; /* terminate string */
    }
  }
  /*
   * file permissions
   *
   * bit 0 owner read access
   *     1 owner write access
   *     3 owner locked
   *     4 other read access
   *     5 other write access
   *     7 other locked
   */
  file_attrib=0;
  if (sb.st_mode&S_IRUSR) file_attrib|=1<<0;
  if (sb.st_mode&S_IWUSR) file_attrib|=1<<1;
  if (sb.st_mode&S_IROTH) file_attrib|=1<<4;
  if (sb.st_mode&S_IWOTH) file_attrib|=1<<5;

  /*
   * time stamp conversions
   */
  to_riscos_time(mktime(localtime(&(sb.st_mtime))),&exec_addr,&timehi);
  load_addr &= ~0xff;  /* clear lower time byte 0xffftttdd */
  load_addr |= timehi; /* add in tme high byte */

  /*
   * output the data in the correct format
   */
  printf("%s\t0x%x\t",filescan,(int)sb.st_size);
  printf("0x%x\t0x%x\t0x%08x\t0x%08x\n",file_attrib,file_type,load_addr,exec_addr);
}

/*
 * just send a file
 */
void
dump_file(char *filename)
{
  char ch;
  FILE *fetch;

  if ((fetch = fopen(filename,"rb"))!=NULL)
  {
    printf("Content-type: application/octet-stream\n\n");
    while (!feof(fetch))
    {
      ch = fgetc(fetch);
      if (!feof(fetch))   /* don't want eof character */
        printf("%c",ch);
    }
    fclose(fetch);
  }
  else
  {
    db_printf("Error: %s (%s)\n",filename,strerror(errno));
    printf("Content-type: application/csfs-status\n\n");
    printf("%s (%d)\n",strerror(errno),errno);
  }
}

/*
 * return files or directory listings in the correct format for the NC
 * HTTP based filing system
 */
int
main (void)
{
  DIR *dirp;
  struct dirent *dp;
  char *path     = NULL;
  char *username = NULL;
  char *password = NULL;
  char filename[MAX_FILENAME];
  entry entries[MAX_ENTRIES];
  struct stat sb;
  int num_entries=0;
  char *cl;
  int i;

  /*
   * sort out debug bits
   */
  debug = fopen(DEBUG_FILE,"a");

  /*
   * sort out input args
   */
  if((cl = getenv("QUERY_STRING"))!=NULL)
  {
    db_printf("QUERY_STRING '%s'\n",cl);
    for(i=0; cl[0] != '\0';i++)
    {
      num_entries=i;
      getword(entries[i].val,cl,'&');
      plustospace(entries[i].val);
      unescape_url(entries[i].val);
      getword(entries[i].name,entries[i].val,'=');
    }
    for(i=0; i <= num_entries; i++)
    {
      if (strcmp(entries[i].name,"path")==0)     path     = entries[i].val;
      if (strcmp(entries[i].name,"username")==0) username = entries[i].val;
      if (strcmp(entries[i].name,"password")==0) password = entries[i].val;
    }
    db_printf("Path = %s\n",CHK_STR(path));
    db_printf("User = %s, Pass = %s\n",CHK_STR(username),CHK_STR(password));
  }

  /*
   * where are we looking, construct the path from the given root
   */
  if (path)
  {
    if (*path=='/')
    {
      if (strcmp(ROOT_DIRECTORY,"/")==0)
        strcpy(filename,path);
      else
        sprintf(filename,"%s%s",ROOT_DIRECTORY,path);
    }
    else
    {
      if (strcmp(ROOT_DIRECTORY,"/")==0)
        sprintf(filename,"/%s",path);
      else
        sprintf(filename,"%s/%s",ROOT_DIRECTORY,path);
    }
  }
  else
  {
    strcpy(filename,ROOT_DIRECTORY);
  }

  /*
   * now take a peek at the requested file
   */
  if (stat(filename,&sb)!=0)
  {
    db_printf("Error: %s (%s)\n",filename,strerror(errno));
    printf("Content-type: application/csfs-status\n\n");
    printf("%s (%d)\n",strerror(errno),errno);
    goto exit_gracefully;
  }

  /*
   * its a directory, return directory listing
   */
  if (sb.st_mode&S_IFDIR)
  {
    if ((dirp=opendir(filename))==NULL)
    {
      db_printf("Error: %s (%s)\n",filename,strerror(errno));
      printf("Content-type: application/csfs-status\n\n");
      printf("%s (%d)\n",strerror(errno),errno);
      goto exit_gracefully;
    }

    db_printf("dir listing for %s\n",filename);
    printf("Content-type: text/plain\n\n");
    printf("# Version 1\n");

    while((dp=readdir(dirp))!=NULL)
      dump_dir_listing(dp,filename);

    db_printf("\n");
    closedir(dirp);
  }
  else
    dump_file(filename);

exit_gracefully:

  if (debug)
    fclose(debug);

  return 0;
}
