/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: user_intf.c 6663 2017-08-12 23:46:05Z nizajerk $
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** basic user interface code...
**
**
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <string.h>
#include "user_intf.h"

#define CHAR_BS  8
#define CHAR_DEL 127
#define CHAR_NEWLINE 0xA

/*****************************************************************************************************/
int proslic_get_char_fflush()
{
  int user_input;

  FLUSH_STDOUT;

  /* fflush(stdin may not always work.. do this instead */
  do
  {
    user_input = getchar();
  }
  while(iscntrl(user_input));

  return user_input;
}

/*****************************************************************************************************/

/* print a char X times... */
void print_chars(unsigned int len, char print_char)
{
  unsigned int i;
  for(i = 0; i < len; i++)
  {
    putchar(print_char);
  }
}

/*****************************************************************************************************/

/* Simple banner creator */
void print_banner(const char *banner)
{
  unsigned int len = strlen(banner);
  unsigned int banner_len = ((PROSLIC_MAX_COL-len-2) >> 1) -1;

  putchar(PROSLIC_EOL);
  print_chars( PROSLIC_MAX_COL, '*');
  putchar(PROSLIC_EOL);

  print_chars( banner_len, '*');
  printf( "  %s  ", banner);

  /* For odd number of chars for a banner, add 1 extra asterisks */
  if(len &1)
  {
    putchar('*');
  }
  print_chars( banner_len, '*');
  putchar(PROSLIC_EOL);

  print_chars( PROSLIC_MAX_COL, '*');
  putchar(PROSLIC_EOL);
  FLUSH_STDOUT;
}

/*****************************************************************************************************/

int display_menu(const char *menu_header, const char **menu_items)
{
  int i;

  printf("\n");
  print_banner( menu_header );

  for(i = 0; menu_items[i]; i++)
  {
    printf( "%02d) %s%c", i, menu_items[i], PROSLIC_EOL);
  }

  print_chars(PROSLIC_MAX_COL, '*');
  putchar(PROSLIC_EOL);
  putchar(PROSLIC_EOL);
  FLUSH_STDOUT;

  return i;
}

/*****************************************************************************************************/
/* Returns back a positive number if did get a valid input (0-N), or N+1 if failed */
int get_menu_selection(int max_count, int current_channel)
{
  char buf[80];
  char input;
  int i = 0;
  int selection;

  do
  {
    display_menu_prompt(current_channel);
    FLUSH_STDOUT;

    *buf = 'r'; /* initialize value to something safe */

    do
    {
      input = getchar();
      if( (input == CHAR_BS) || (input == CHAR_DEL) )
      {
        i--;
      }else if (!iscntrl((int)input))
      {
        buf[i++] = input;
      }
    }
    while((input != CHAR_NEWLINE) && (i < 80));

    buf[i] = 0;

    if ((*buf == 'q') || (*buf == 'Q'))
    {
      return QUIT_MENU;
    }

    if ((*buf == 'r') || (*buf == 'R'))
    {
      return REDISPLAY_MENU;
    }

    selection = (int)strtol(buf, NULL, 10);
    i = 0;
  }
  while(iscntrl((int)*buf) || (selection > max_count));

  return selection;
}

/*****************************************************************************************************/

int get_int(int min_value, int max_value)
{
  int user_input;

  FLUSH_STDOUT;

  do
  {
    fflush(stdin);
  }
  while(scanf("%d", &user_input) != 1);

  (void)getchar(); /* Eat the \n */

  if( ( user_input < min_value) || (user_input > max_value) )
  {
    return (max_value+1);
  }

  return user_input;

}
/*****************************************************************************************************/
uInt32 get_hex(uInt32 min_value, uInt32 max_value)
{
  unsigned long user_input;

  FLUSH_STDOUT;

  do
  {
    fflush(stdin);
  }
  while(scanf("%lx", &user_input) != 1);

  (void)getchar(); /* Eat the \n */

  if( ( user_input < min_value) || (user_input > max_value) )
  {
    return (max_value+1);
  }

  return user_input;
}

/*****************************************************************************************************/

/* returns 0 if the character is not found... */

int get_char(char *legal_options, char *user_input)
{
  *user_input = proslic_get_char_fflush();
  return (strchr(legal_options, *user_input) != 0);
}

/*****************************************************************************************************/
/* Look for a particular character range + 'q' for quit */

int get_char_range(char start_range, char end_range)
{
  int user_input;

  user_input = proslic_get_char_fflush();

  if( (user_input < start_range) || ((user_input >= (end_range))
                                     && !(user_input == 'q') ) )
  {
    return -1;
  }

  return user_input;
}

/*****************************************************************************************************/
/* BUFFER is assumed to be of at least BUFSIZ in allocated space */
int get_fn(char *prompt, char *buffer)
{
  printf("%s %s", prompt, PROSLIC_PROMPT);
  FLUSH_STDOUT;
  fflush(stdin);
  scanf("%s", buffer);
  (void)getchar(); /* Eat the \n */
  buffer[strlen(buffer)] = 0;
  return 0;
}

/*****************************************************************************************************/
int get_prompted_int(int min_value, int max_value, const char *prompt)
{
  int user_input;
  do
  {
    printf("%s %s", prompt, PROSLIC_PROMPT);
    user_input = get_int(min_value, max_value);
  }
  while(user_input > max_value);
  return user_input;
}

/*****************************************************************************************************/
#ifdef __GNUC__
#ifndef __MINGW32__
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
#else
/* Uses MSFT implementation */
#endif /* __MINGW32 __ */
#endif /* __GNUC__ */

