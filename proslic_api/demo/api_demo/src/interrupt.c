/*
** Copyright (c) 2013-2018 by Silicon Laboratories, Inc.
**
** $Id: interrupt.c 7054 2018-04-06 20:57:58Z nizajerk $
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
**
** This file contains example implementation and use of ProSLIC API
** interrupt generating resources
**
*/

#include "api_demo.h"
#include "macro.h"
#include "user_intf.h"
#include "proslic.h"

#ifndef DISABLE_HOOKCHANGE
/* Pulse Dial/Hookflash Config */
static hookChange_Cfg hook_change_cfg = { 20,    /* Min Break Time */
                                          80,    /* Max Break Time */
                                          20,    /* Min Make Time */
                                          80,    /* Max Make Time */
                                          90,    /* Min Inter-digit Delay */
                                          100,   /* Min hookflash */
                                          800,   /* Max hookflash */
                                          850
                                        }; /* Min Idle Time */
#endif

#define MAX_INT_STRINGS 38
char *intMapStrings[] =
{
  "IRQ_OSC1_T1",
  "IRQ_OSC1_T2",
  "IRQ_OSC2_T1",
  "IRQ_OSC2_T2",
  "IRQ_RING_T1",
  "IRQ_RING_T2",
  "IRQ_PM_T1",
  "IRQ_PM_T2",
  "IRQ_FSKBUF_AVAIL", /**< FSK FIFO depth reached */
  "IRQ_VBAT",
  "IRQ_RING_TRIP", /**< Ring Trip detected */
  "IRQ_LOOP_STATUS",  /**< Loop Current changed */
  "IRQ_LONG_STAT",
  "IRQ_VOC_TRACK",
  "IRQ_DTMF",         /**< DTMF Detected - call @ref ProSLIC_DTMFReadDigit to decode the value */
  "IRQ_INDIRECT",     /**< Indirect/RAM access completed */
  "IRQ_TXMDM",
  "IRQ_RXMDM",
  "IRQ_PQ1",          /**< Power alarm 1 */
  "IRQ_PQ2",          /**< Power alarm 2 */
  "IRQ_PQ3",          /**< Power alarm 3 */
  "IRQ_PQ4",          /**< Power alarm 4 */
  "IRQ_PQ5",          /**< Power alarm 5 */
  "IRQ_PQ6",          /**< Power alarm 6 */
  "IRQ_RING_FAIL",
  "IRQ_CM_BAL",
  "IRQ_USER_0",
  "IRQ_USER_1",
  "IRQ_USER_2",
  "IRQ_USER_3",
  "IRQ_USER_4",
  "IRQ_USER_5",
  "IRQ_USER_6",
  "IRQ_USER_7",
  "IRQ_DSP",
  "IRQ_MADC_FS",
  "IRQ_P_HVIC",
  "IRQ_P_THERM", /**< Thermal alarm */
  "IRQ_P_OFFLD"
};

extern const char *linefeed_states;

/*****************************************************************************************************/
static void irq_demo_handle_interrupt(demo_state_t *pState,
                                      ProslicInt interrupt, uInt8 *hook_det)
{

  switch(interrupt)
  {
    case IRQ_LOOP_STATUS:
      ProSLIC_ReadHookStatus(pState->currentChanPtr,hook_det);
      if(*hook_det == PROSLIC_OFFHOOK)
      {
        printf("OFFHOOK\n");
      }
      else
      {
        printf("ONHOOK\n");
      }
      FLUSH_STDOUT;
      break;

    case IRQ_DTMF:
      {
        uInt8 digit;
        char digit_char;
        ProSLIC_DTMFReadDigit(pState->currentChanPtr, &digit);
        if( (digit >=1) && (digit <= 9 ) )
        {
          digit_char = digit + '0';
        }
        else
        {
          if(digit == 0)
          {
            digit_char = 'D';
          }
          else
          {
            char digit_decode[] = "0*#ABC";
            digit_char = digit_decode[digit - 10];
          }
        }
        printf("detected dtmf-%c\n", digit_char);
        FLUSH_STDOUT;
      }
      break;

    default:
      break;
  }
}

/*****************************************************************************************************/
int irq_demo_check_interrupts(demo_state_t *pState, uInt8 *hook_det)
{
  proslicIntType irqs;
  ProslicInt arrayIrqs[MAX_PROSLIC_IRQS];

  irqs.irqs = arrayIrqs;

  if (ProSLIC_GetInterrupts(pState->currentChanPtr, &irqs) != 0)
  {
    unsigned int i;
    /* Iterate through the interrupts and handle */
    for(i=0 ; i<irqs.number;  i++)
    {
      if(irqs.irqs[i] < MAX_INT_STRINGS)
      {
        printf("detected: %s\n", intMapStrings[irqs.irqs[i]]);
        FLUSH_STDOUT;
      }
      irq_demo_handle_interrupt(pState,irqs.irqs[i], hook_det);
    }
  }

  if(irqs.number)
  {
    printf("\n");
  }

  return irqs.number;
}

/*****************************************************************************************************/
static int get_interrupt_state(demo_state_t  *pState)
{
  int i;
  int irqEn = 0;
  for(i = PROSLIC_REG_IRQEN1; i < PROSLIC_REG_IRQEN4; i++)
  {
    irqEn += SiVoice_ReadReg(pState->currentChanPtr, i);
  }

  return( (irqEn != 0) );
}

/*****************************************************************************************************/
static void print_irqen(demo_state_t *pState)
{
  int i;
  for(i = PROSLIC_REG_IRQEN1; i <= PROSLIC_REG_IRQEN4; i++)
  {
    printf("IRQEN%d\t= 0x%02X \n",
           (i-PROSLIC_REG_IRQEN1+1), SiVoice_ReadReg(pState->currentChanPtr, i));
  }
}

/*****************************************************************************************************/
void interruptMenu(demo_state_t *pState)
{
  uInt32 regValue;
  uInt8 done = 0;
  int poll_interval = 1;   /* ms */
  int onhook_state = LF_FWD_ACTIVE;
  int user_selection;
  int digit = 0;
  int int_enabled = 0;
  int i;
  uInt8 hook_changed=0;

  const char *menu_items[] =
  {
    "Read IRQ Regs",
    "Read IRQEN Regs",
    "Change IRQ Enable values",
    "Toggle interrupt enable (constants file)",
    "Toggle interrupt enable (manually set)",
    "Wait for interrupt(single)",
    "Wait for interrupt (continuous)",
    "Adjust interrupt poll interval",
    "Set default onhook state",
    "Pulse digit decode/hook flash demo",
    NULL
  };

  do
  {
    int_enabled = get_interrupt_state(pState);

    digit =  display_menu("Interrupt Menu", menu_items);
    printf("Poll Interval: %d ms Interrupts: %s\n", poll_interval,
           GET_ENABLED_STATE(int_enabled));
    printf("Default onhook state = %d\n\n", onhook_state);
    user_selection = get_menu_selection( digit, pState->currentChannel);

    switch(user_selection)
    {
      case 0:
        print_banner("Interrupt Status");
        printf("IRQ\t= 0x%02X \n",
               SiVoice_ReadReg(pState->currentChanPtr,PROSLIC_REG_IRQ));
        for(i = PROSLIC_REG_IRQ0; i <= PROSLIC_REG_IRQ4; i++)
        {
          printf("IRQ%d\t= 0x%02X \n", (i-PROSLIC_REG_IRQ0),
                 SiVoice_ReadReg(pState->currentChanPtr,i));
        }
        break;

      case 1:
        print_banner("Interrupt Enable status");
        print_irqen(pState);
        break;

      case 2:
        for(i = PROSLIC_REG_IRQEN1; i <= PROSLIC_REG_IRQEN4; i++)
        {
          do
          {
            printf("Enter IRQEN%d value (hex) %s", (i-PROSLIC_REG_IRQEN1)+1, PROSLIC_PROMPT);
            regValue = get_hex(0,0xFF);
          }
          while(regValue > 0xFF);
          SiVoice_WriteReg(pState->currentChanPtr, i, (uInt8)(regValue & 0xFF));
        }

        print_irqen(pState);
        break;

      case 3:
        if(int_enabled == 0)
        {
          print_banner("Interrupts Enabled - using constants file settings");
          ProSLIC_EnableInterrupts(pState->currentChanPtr);
          print_irqen(pState);
        }
        else
        {
          ProSLIC_DisableInterrupts(pState->currentChanPtr);
          print_banner("Interrupts Disabled");
        }
        break;

     case 4:
        if(int_enabled == 0)
        {
          print_banner("Interrupts Enabled - using manual settings");
          demo_restore_slic_irqens(pState);
          print_irqen(pState);
        }
        else
        {
          print_banner("Interrupts Disabled - caching settings");
          demo_save_slic_irqens(pState);
          ProSLIC_DisableInterrupts(pState->currentChanPtr);
        }
        break;

      case 5:
        done = 0;
        printf("\n\nWaiting for Interrupt - press enter to abort\n\n");
        while((!kbhit())&&(!done))
        {
          Delay(pProTimer,poll_interval);
          done = (irq_demo_check_interrupts(pState, &hook_changed) != 0);
        }
        (void)getchar();
        break;

      case 6:
        printf("\n\nContinuously Monitoring IRQs - press enter to abort\n\n");
        while(!kbhit())
        {
          Delay(pProTimer,poll_interval);
          irq_demo_check_interrupts(pState, &hook_changed);
        }
        (void)getchar();
        break;

      case 7:
        do
        {
          printf("Enter IRQ Poll Interval (ms) %s", PROSLIC_PROMPT);
          poll_interval =  get_int(1,0xFFFF);
        }
        while (poll_interval > 0xFFFF);
        break;

      case 8:
        {
          const uInt8 lf_mode[] = {LF_FWD_ACTIVE, LF_FWD_OHT, LF_REV_ACTIVE, LF_REV_OHT };
          const char *lf_strings[] = {"FWD ACTIVE", "FWD OHT", "REV ACTIVE", "REV OHT", NULL };
          i = get_menu_selection( display_menu("Onhook State", lf_strings),
                                  pState->currentChannel);
          if(i != QUIT_MENU)
          {
            ProSLIC_ReadHookStatus(pState->currentChanPtr, &hook_changed);
            if(hook_changed == PROSLIC_ONHOOK)
            {
              onhook_state = lf_mode[i];
              ProSLIC_SetLinefeedStatus(pState->currentChanPtr, onhook_state);
            }
          }
        }
        break;

      case 9:
#ifndef DISABLE_HOOKCHANGE
        {
          timeStamp timers;
          int result_value;
          int looking_for_pulse = FALSE;
          int old_hook_detect = 0;
          hookChangeType hookdetection;

          GetTime(NULL, &timers);
          ProSLIC_InitializeHookChangeDetect(&hookdetection, &timers);

          ProSLIC_SetLinefeedStatus(pState->currentChanPtr, onhook_state);

          printf("Press a key to stop demo\n");
          do
          {
            Delay(pProTimer,poll_interval);
            irq_demo_check_interrupts(pState, &hook_changed);

            if(hook_changed != old_hook_detect)
            {
              old_hook_detect = hook_changed;
              looking_for_pulse = TRUE;
              result_value = ProSLIC_HookChangeDetect(pState->currentChanPtr,
                                                      &hook_change_cfg, &hookdetection);
              if(result_value == SI_HC_HOOKFLASH)
              {
                printf("--> Hook flash\n");
                looking_for_pulse = FALSE;
              }
            }

            if(looking_for_pulse == TRUE)
            {
              result_value = ProSLIC_HookChangeDetect(pState->currentChanPtr,
                                                      &hook_change_cfg, &hookdetection);

              if(result_value != SI_HC_NEED_MORE_POLLS)
              {
                /*  printf("--> %s %d return code: %d\n", __FUNCTION__, __LINE__, result_value); */

                looking_for_pulse = FALSE;
                if( SI_HC_DIGIT_DONE(result_value) )
                {
                  printf("Pulses/Digit detected: %d\n", result_value);
                }
                else
                {
                  switch(result_value)
                  {
                    case SI_HC_ONHOOK_TIMEOUT:
                      printf("Onhook qualified\n");
                      break;

                    case SI_HC_OFFHOOK_TIMEOUT:
                      printf("Offhook qualified\n");
                      break;

                    case SI_HC_HOOKFLASH:
                      printf("Hook flash qualified\n");
                      break;

                    default:
                      break;
                  }
                }
              }
            }
          }
          while(!kbhit());
          (void)getchar();
        }
#else
        printf("%sFeature disabled\n", LOGPRINT_PREFIX);
#endif
        break;

      default:
        break;
    }
  }
  while(user_selection != QUIT_MENU);
}

