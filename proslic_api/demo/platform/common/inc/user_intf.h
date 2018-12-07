/*
** Copyright (c) 2015-2016 by Silicon Laboratories
**
** $Id: user_intf.h 7065 2018-04-12 20:24:50Z nizajerk $
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
** basic user interface header file
**
**
*/

#ifndef __PROSLIC_UI_HDR__
#define __PROSLIC_UI_HDR__

#include <stdio.h>
#include "si_voice_datatypes.h"

#define ASCII_ESC 27
#define PROSLIC_EOL       '\n'
#define PROSLIC_CLS printf("%c[2J", ASCII_ESC)
#define PROSLIC_MAX_COL 80
#define PROSLIC_PROMPT "--> "
#define display_menu_prompt(X) printf("Select Menu item for chan: %u or 'q' to quit or 'r' to redisplay menu %s ", (X), PROSLIC_PROMPT)
#define REDISPLAY_MENU 0xFE
#define QUIT_MENU 0xFF
#define DEFAULT_VMB2_BITMASK 0x00FE
#define DEFAULT_RSPI_BITMASK 0x00FE
#define DEFAULT_VMB1_BITMASK 0x0011
#define DEFAULT_SPI_DEV_BITMASK 0x0083
#define DEFAULT_FIRMWARE_VER 0xFFFF
#define BOARD_MODEL 0x0003
#define IS_VMB1     1
#define IS_VMB2     2
#define IS_SPI_DEV  3
#define ISI_CAPABLE 0x0004
#define FIRMWARE_ISI 0x0103
#define MODIFY_SCLK 0x0008
#define MODIFY_PCM_SOURCE 0x0010
#define MODIFY_FSYNC 0x0020
#define MODIFY_LOOPBACK 0x0040
#define FIRMWARE_LOOPBACK 0x0107
#define MODIFY_SPI_CLOCK 0x0080

#ifdef SILABS_USE_SETVBUF
#define SETUP_STDOUT_FLUSH setvbuf(stdout, NULL, _IONBF, 0); setvbuf(stderr, NULL, _IONBF, 0);
#define FLUSH_STDOUT printf("\n");fflush(stdout);
#else
#define SETUP_STDOUT_FLUSH
#define FLUSH_STDOUT
#endif

void print_banner(const char *banner);
int display_menu(const char *menu_header, const char **menu_items);

int get_menu_selection(int max_count, int current_channel);
int get_int(int min_value, int max_max);
int get_prompted_int(int min_value, int max_max, const char *prompt);
uInt32 get_hex(uInt32 min_value, uInt32 max_value);
int get_char(char *legal_chars, char *user_input);
int get_char_range(char start_range, char end_range);
int get_fn(char *prompt, char *buffer);
#ifdef __GNUC__
int kbhit(void);
#endif

#endif /* __PROSLIC_UI_HDR__ */
