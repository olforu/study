/*
** Copyright (c) 2017 by Silicon Laboratories
**
** $Id: si3218x.h 5419 2016-01-13 00:40:56Z nizajerk $
**
**
** Distributed by:
** Silicon Laboratories, Inc
**
** This file contains proprietary information.
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
*/

#ifndef SI3219XH_H
#define SI3219XH_H

#include "proslic.h"

#define SI3219X_CHAN_PER_DEVICE             1

/*
** Calibration Constants
*/

#define SI3219X_CAL_STD_CALR1       0xC0
#define SI3219X_CAL_STD_CALR2       0x18

/* Timeouts in 10s of ms */
#define SI3219X_TIMEOUT_DCDC_UP     200
#define SI3219X_TIMEOUT_DCDC_DOWN   200

/*
** SI3219X DataTypes/Function Definitions
*/


typedef ProSLIC_DCfeed_Cfg Si3219x_DCfeed_Cfg;

/*
** Si3219x General Parameter Struct
*/
typedef struct
{
  /* Flags */
  uInt8               device_key;
  bomOptionsType      bom_option;
  vdcRangeType        vdc_range;
  autoZcalType        zcal_en;
  pmBomType           pm_bom;
  /* Raw Parameters */
  ramData         i_oithresh_lo;
  ramData         i_oithresh_hi;
  ramData         v_ovthresh;
  ramData         v_uvthresh;
  ramData         v_uvhyst;
  /* RAM Updates */
  ramData         dcdc_fsw_vthlo;
  ramData         dcdc_fsw_vhyst;
  ramData         p_th_hvic;
  ramData         coef_p_hvic;
  ramData         bat_hyst;
  ramData
  vbath_expect;         /* default - this is overwritten by dc feed preset */
  ramData
  vbatr_expect;         /* default - this is overwritten by ring preset */
  ramData         pwrsave_timer;
  ramData         pwrsave_ofhk_thresh;
  ramData         vbat_track_min;       /* Same as DCDC_VREF_MIN */
  ramData         vbat_track_min_rng;   /* Same as DCDC_VREF_MIN_RNG */
  ramData         dcdc_ana_scale;
  ramData         therm_dbi;
  ramData         vov_dcdc_slope;
  ramData         vov_dcdc_os;
  ramData         vov_ring_bat_dcdc;
  ramData         vov_ring_bat_max;
  ramData         dcdc_verr;
  ramData         dcdc_verr_hyst;
  ramData         pd_uvlo;
  ramData         pd_ovlo;
  ramData         pd_oclo;
  ramData         pd_swdrv;
  ramData         dcdc_uvpol;
  ramData         dcdc_rngtype;
  ramData         dcdc_ana_toff;
  ramData         dcdc_ana_tonmin;
  ramData         dcdc_ana_tonmax;
  uInt8           irqen1;
  uInt8           irqen2;
  uInt8           irqen3;
  uInt8           irqen4;
  uInt8           enhance;
  uInt8            auto_reg;
} Si3219x_General_Cfg;

/*
** Defines structure for configuring pcm
*/
typedef struct
{
  uInt8 pcmFormat;
  uInt8 widebandEn;
  uInt8 pcm_tri;
  uInt8 tx_edge;
  uInt8 alaw_inv;
} Si3219x_PCM_Cfg;

/*
** Defines structure for configuring pulse metering
*/
typedef struct
{
  ramData pm_amp_thresh;
  uInt8 pmFreq;
  uInt8 pmAuto;
  ramData pmActive;
  ramData pmInactive;
} Si3219x_PulseMeter_Cfg;
/*
** Defines structure for configuring FSK generation
*/
typedef ProSLIC_FSK_Cfg Si3219x_FSK_Cfg;


/*
** Defines structure for configuring impedance synthesis
*/
typedef struct
{
  ramData zsynth_b0;
  ramData zsynth_b1;
  ramData zsynth_b2;
  ramData zsynth_a1;
  ramData zsynth_a2;
  uInt8 ra;
} Si3219x_Zsynth_Cfg;

/*
** Defines structure for configuring hybrid
*/
typedef struct
{
  ramData ecfir_c2;
  ramData ecfir_c3;
  ramData ecfir_c4;
  ramData ecfir_c5;
  ramData ecfir_c6;
  ramData ecfir_c7;
  ramData ecfir_c8;
  ramData ecfir_c9;
  ramData ecfir_b0;
  ramData ecfir_b1;
  ramData ecfir_a1;
  ramData ecfir_a2;
} Si3219x_hybrid_Cfg;

/*
** Defines structure for configuring audio eq
*/

typedef struct
{
  ramData txaceq_c0;
  ramData txaceq_c1;
  ramData txaceq_c2;
  ramData txaceq_c3;

  ramData rxaceq_c0;
  ramData rxaceq_c1;
  ramData rxaceq_c2;
  ramData rxaceq_c3;
} Si3219x_audioEQ_Cfg;

/*
** Defines structure for configuring audio gain
*/
typedef ProSLIC_audioGain_Cfg Si3219x_audioGain_Cfg;

typedef struct
{
  Si3219x_audioEQ_Cfg audioEQ;
  Si3219x_hybrid_Cfg hybrid;
  Si3219x_Zsynth_Cfg zsynth;
  ramData txgain;
  ramData rxgain;
  ramData rxachpf_b0_1;
  ramData  rxachpf_b1_1;
  ramData  rxachpf_a1_1;
  int16 txgain_db; /*overall gain associated with this configuration*/
  int16 rxgain_db;
} Si3219x_Impedance_Cfg;

/*
** Defines structure for configuring tone generator
*/
typedef ProSLIC_Tone_Cfg Si3219x_Tone_Cfg;

/*
** Defines structure for configuring ring generator
*/
typedef struct
{
  ramData rtper;
  ramData freq;
  ramData amp;
  ramData phas;
  ramData offset;
  ramData slope_ring;
  ramData iring_lim;
  ramData rtacth;
  ramData rtdcth;
  ramData rtacdb;
  ramData rtdcdb;
  ramData vov_ring_bat;
  ramData vov_ring_gnd;
  ramData vbatr_expect;
  uInt8 talo;
  uInt8 tahi;
  uInt8 tilo;
  uInt8 tihi;
  ramData adap_ring_min_i;
  ramData counter_iring_val;
  ramData counter_vtr_val;
  ramData ar_const28;
  ramData ar_const32;
  ramData ar_const38;
  ramData ar_const46;
  ramData rrd_delay;
  ramData rrd_delay2;
  ramData dcdc_vref_min_rng;
  uInt8 ringcon;
  uInt8 userstat;
  ramData vcm_ring;
  ramData vcm_ring_fixed;
  ramData delta_vcm;
  ramData dcdc_rngtype;
} Si3219x_Ring_Cfg;

#endif

