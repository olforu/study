/*
** Copyright (c) 2018 Silicon Laboratories, Inc.
** 2018-04-18 20:50:28
**
** Si3226x ProSLIC API Configuration Tool Version 4.2.1
** Last Updated in API Release: 9.2.0
** source XML file: si3226x_CUK_constants.xml
**
** Auto generated file from configuration tool.
*/


#include "proslic.h"
#include "si3226x.h"

Si3226x_General_Cfg Si3226x_General_Configuration  = {
0x65,     /* DEVICE_KEY */
BO_DCDC_CUK,    /* BOM_OPT */
BO_DCDC_TRACKING,    /* BAT_RAIL_TYPE */
0x000AD000L,    /* bat_hyst */
0x0851EB45L,    /* VBATR_EXPECT (130.00) */
0x03D70A20L,    /* VBATH_EXPECT (60.00) */
0x0FFF0000L,    /* PWRSAVE_TIMER */
0x01999A00L,    /* OFFHOOK_THRESH */
0x00F00000L,    /* VBAT_TRACK_MIN */
0x00F00000L,    /* VBAT_TRACK_MIN_RNG */
0x00014000L,    /* PWRSAVE_DBI */
0x0ADD5500L,    /* DCDC_ANA_SCALE */
0x00312700L,    /* VOV_BAT_PWRSAVE_MIN */
0x00395800L,    /* VOV_BAT_PWRSAVE_MIN_RNG */
0x00800000L,    /* THERM_DBI */
0x00004000L,    /* CPUMP_DBI */
0x00F18900L,    /* DCDC_VERR */
0x00809D80L,    /* DCDC_VERR_HYST */
0x01200000L,    /* DCDC_OITHRESH_LO */
0x02100000L,    /* DCDC_OITHRESH_HI */
0x00400000L,    /* PD_UVLO */
0x00400000L,    /* PD_OVLO */
0x00200000L,    /* PD_OCLO */
0x00400000L,    /* PD_SWDRV */
0x00500000L,    /* DCDC_UVHYST */
0x00000000L,    /* DCDC_UVTHRESH */
0x01400000L,    /* DCDC_OVTHRESH */
0x00800000L,    /* DCDC_OITHRESH */
0x00000000L,    /* DCDC_SWDRV_POL */
0x00000000L,    /* DCDC_UVPOL */
0x00000000L,    /* DCDC_VREF_MAN */
0x00000000L,    /* DCDC_VREF_CTRL */
0x00200000L,    /* DCDC_RNGTYPE */
0x05B00000L,    /* DCDC_ANA_VREF */
0x00300000L,    /* DCDC_ANA_GAIN */
0x00300000L,    /* DCDC_ANA_TOFF */
0x00100000L,    /* DCDC_ANA_TONMIN */
0x00FFC000L,    /* DCDC_ANA_TONMAX */
0x00F00000L,    /* DCDC_ANA_DSHIFT */
0x0FDA4000L,    /* DCDC_ANA_LPOLY */
0x00000000L,    /* DCDC_AUX_INVERT */
0x00400000L,    /* DCDC_CPUMP_LP */
0x00000000L,    /* DCDC_CPUMP_PULLDOWN */
0x00000000L,    /* DCDC_LIFT_EN */
0x07FEB800L,    /* COEF_P_HVIC */
0x005B05B2L,    /* P_TH_HVIC */
VDC_27P0_42P0,    /* VDC_RANGE_OPT */
0x3F,     /* AUTO */
0x50,     /* IRQEN1 */
0x13,     /* IRQEN2 */
0x03,     /* IRQEN3 */
0x00,     /* IRQEN4 */
0x10,     /* ENHANCE */
0x03A2E8BAL,    /* SCALE_KAUDIO */
0x04,    /* ZCAL_EN */
0x03000000L,    /* LKG_OFHK_OFFSET */
0x05000000L,    /* LKG_LB_OFFSET */
0x01000000L,    /* VBATH_DELTA */
0x03700000L,    /* UVTHRESH_MAX */
0x04B80200L,    /* UVTHRESH_SCALE */
0x00823000L     /* UVTHRESH_BIAS */
};

Si3226x_GPIO_Cfg Si3226x_GPIO_Configuration = {
0x00,     /* GPIO_OE */
0x06,     /* GPIO_ANA */
0x00,     /* GPIO_DIR */
0x00,     /* GPIO_MAN */
0x00,     /* GPIO_POL */
0x00,     /* GPIO_OD */
0x00     /* BATSELMAP */
};
Si3226x_CI_Cfg Si3226x_CI_Presets [] = {
{0}
};
Si3226x_audioGain_Cfg Si3226x_audioGain_Presets [] = {
{0x1377080L,0, 0x0L, 0x0L, 0x0L, 0x0L},
{0x80C3180L,0, 0x0L, 0x0L, 0x0L, 0x0L}
};

Si3226x_Ring_Cfg Si3226x_Ring_Presets[] ={
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 20 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x0027A50BL,	/* RINGAMP (65.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) */
0x0097B1F0L,	/* RTACTH (83.771 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x0051EB82L,	/* VOV_RING_BAT (5.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x06E4258BL,	/* VBATR_EXPECT (107.675 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* VBAT_TRACK_MIN_RNG */
0x98,			/* RINGCON */
0x00,			/* USERSTAT */
0x037212C5L,	/* VCM_RING (52.588 v) */
0x037212C5L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
0x00FFFFFFL,	/* VOV_DCDC_SLOPE */
0x00A18937L,	/* VOV_DCDC_OS */
0x00E49BA5L,	/* VOV_RING_BAT_MAX */
},  /* RING_MAX_VBAT_PROVISIONING */
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 20 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001B7243L,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) */
0x00690508L,	/* RTACTH (57.995 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x0051EB82L,	/* VOV_RING_BAT (5.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04DE89D7L,	/* VBATR_EXPECT (76.083 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* VBAT_TRACK_MIN_RNG */
0x98,			/* RINGCON */
0x00,			/* USERSTAT */
0x026F44EBL,	/* VCM_RING (36.791 v) */
0x026F44EBL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
0x00FFFFFFL,	/* VOV_DCDC_SLOPE */
0x00A18937L,	/* VOV_DCDC_OS */
0x00E49BA5L,	/* VOV_RING_BAT_MAX */
},  /* RING_F20_45VRMS_0VDC_LPR */
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 20 ohms, Type = BALANCED, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001B7243L,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x008B9ACAL,	/* IRING_LIM (90.000 mA) */
0x00690508L,	/* RTACTH (57.995 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x0051EB82L,	/* VOV_RING_BAT (5.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04DE89D7L,	/* VBATR_EXPECT (76.083 v) */
0x80,			/* RINGTALO (2.000 s) */
0x3E,			/* RINGTAHI */
0x00,			/* RINGTILO (4.000 s) */
0x7D,			/* RINGTIHI */
0x00000000L,	/* ADAP_RING_MIN_I */
0x00003000L,	/* COUNTER_IRING_VAL */
0x00051EB8L,	/* COUNTER_VTR_VAL */
0x00000000L,	/* CONST_028 */
0x00000000L,	/* CONST_032 */
0x00000000L,	/* CONST_038 */
0x00000000L,	/* CONST_046 */
0x00000000L,	/* RRD_DELAY */
0x00000000L,	/* RRD_DELAY2 */
0x01893740L,	/* VBAT_TRACK_MIN_RNG */
0x18,			/* RINGCON */
0x00,			/* USERSTAT */
0x026F44EBL,	/* VCM_RING (36.791 v) */
0x026F44EBL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00200000L,	/* DCDC_RNGTYPE */
0x00FFFFFFL,	/* VOV_DCDC_SLOPE */
0x00A18937L,	/* VOV_DCDC_OS */
0x00E49BA5L,	/* VOV_RING_BAT_MAX */
}   /* RING_F20_45VRMS_0VDC_BAL */
};

Si3226x_DCfeed_Cfg Si3226x_DCfeed_Presets[] = {
{
0x1C8A024CL,	/* SLOPE_VLIM */
0x1F909679L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1D5B21A9L,	/* SLOPE_DELTA1 */
0x1DD87A3EL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x050D2839L,	/* V_RFEED (43.000 v) */
0x03FE7F0FL,	/* V_ILIM  (34.000 v) */
0x00B4F3C3L,	/* CONST_RFEED (15.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
},  /* DCFEED_48V_20MA */
{
0x1C8A024CL,	/* SLOPE_VLIM */
0x1EE08C11L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1C940D71L,	/* SLOPE_DELTA1 */
0x1DD87A3EL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x050D2839L,	/* V_RFEED (43.000 v) */
0x03FE7F0FL,	/* V_ILIM  (34.000 v) */
0x01241BC9L,	/* CONST_RFEED (15.000 mA) */
0x0074538FL,	/* CONST_ILIM (25.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
},  /* DCFEED_48V_25MA */
{
0x1E655196L,	/* SLOPE_VLIM */
0x001904EFL,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1B4CAD9EL,	/* SLOPE_DELTA1 */
0x1BB0F47CL,	/* SLOPE_DELTA2 */
0x05A38633L,	/* V_VLIM (48.000 v) */
0x043AA4A6L,	/* V_RFEED (36.000 v) */
0x025977EAL,	/* V_ILIM  (20.000 v) */
0x0068B19AL,	/* CONST_RFEED (18.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
0x002D8D96L,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
},  /* DCFEED_PSTN_DET_1 */
{
0x1A10433FL,	/* SLOPE_VLIM */
0x1C206275L,	/* SLOPE_RFEED */
0x0040A0E0L,	/* SLOPE_ILIM */
0x1C1F426FL,	/* SLOPE_DELTA1 */
0x1EB51625L,	/* SLOPE_DELTA2 */
0x041C91DBL,	/* V_VLIM (35.000 v) */
0x03E06C43L,	/* V_RFEED (33.000 v) */
0x038633E0L,	/* V_ILIM  (30.000 v) */
0x022E5DE5L,	/* CONST_RFEED (10.000 mA) */
0x005D0FA6L,	/* CONST_ILIM (20.000 mA) */
0x0021373DL,	/* I_VLIM (0.000 mA) */
0x005B0AFBL,	/* LCRONHK (10.000 mA) */
0x006D4060L,	/* LCROFFHK (12.000 mA) */
0x00008000L,	/* LCRDBI (5.000 ms) */
0x0048D595L,	/* LONGHITH (8.000 mA) */
0x003FBAE2L,	/* LONGLOTH (7.000 mA) */
0x00008000L,	/* LONGDBI (5.000 ms) */
0x000F0000L,	/* LCRMASK (150.000 ms) */
0x00080000L,	/* LCRMASK_POLREV (80.000 ms) */
0x00140000L,	/* LCRMASK_STATE (200.000 ms) */
0x00140000L,	/* LCRMASK_LINECAP (200.000 ms) */
0x01BA5E35L,	/* VCM_OH (27.000 v) */
0x0051EB85L,	/* VOV_BAT (5.000 v) */
0x00418937L,	/* VOV_GND (4.000 v) */
}   /* DCFEED_PSTN_DET_2 */
};

Si3226x_Impedance_Cfg Si3226x_Impedance_Presets[] ={
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=20 rfuse=0 emi_cap=10*/
{
{0x07F97D80L, 0x0006CC00L, 0x1FFC1480L, 0x1FFC7B80L,    /* TXACEQ */
 0x07F36B80L, 0x000A8E00L, 0x1FF90F00L, 0x1FFAE500L},   /* RXACEQ */
{0x001AF400L, 0x1FC86A80L, 0x01E9AE00L, 0x00652F00L,    /* ECFIR/ECIIR */
 0x01F4AF00L, 0x1F57E000L, 0x00485E00L, 0x1FF3A680L,
 0x1FF83700L, 0x00011D00L, 0x01706980L, 0x066A8480L},
{0x00920F00L, 0x1EE31980L, 0x008ADF00L, 0x0F92E500L,    /* ZSYNTH */
 0x186CE880L, 0x53}, 
 0x085C6880L,   /* TXACGAIN */
 0x013E3100L,   /* RXACGAIN */
 0x07AF6F80L, 0x18509100L, 0x075EDF00L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_600_0_0_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=20 rfuse=0 emi_cap=10*/
{
{0x06FC3800L, 0x1FDE9280L, 0x000BC980L, 0x1FFEC900L,    /* TXACEQ */
 0x0A7A3180L, 0x1BB98500L, 0x0078FC00L, 0x1FDDF500L},   /* RXACEQ */
{0x000A9280L, 0x0008DA80L, 0x00EEDC80L, 0x01C2E400L,    /* ECFIR/ECIIR */
 0x01447F00L, 0x016F6000L, 0x001D5400L, 0x007BF480L,
 0x001F5680L, 0x1FDEE780L, 0x0CCDB980L, 0x1B23B100L},
{0x00857800L, 0x1E6C8180L, 0x010DD200L, 0x0D0AEC00L,    /* ZSYNTH */
 0x1AF3BB80L, 0x92}, 
 0x08000000L,   /* TXACGAIN */
 0x01062800L,   /* RXACGAIN */
 0x07BADA00L, 0x18452680L, 0x0775B400L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_270_750_150_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=370_620_310 rprot=20 rfuse=0 emi_cap=10*/
{
{0x07C28C80L, 0x1FE04E80L, 0x1FFF9400L, 0x1FFE2D80L,    /* TXACEQ */
 0x09E96680L, 0x1C31D800L, 0x1F927A80L, 0x1FDD9100L},   /* RXACEQ */
{0x0026B300L, 0x1F8CA780L, 0x0217B000L, 0x1FD05600L,    /* ECFIR/ECIIR */
 0x02DE4380L, 0x1FB44980L, 0x00E05B80L, 0x00441F80L,
 0x003D1080L, 0x1FC20D80L, 0x0DAB3E80L, 0x1A4EFE80L},
{0x00416100L, 0x1F5A4680L, 0x00644E00L, 0x0F172B00L,    /* ZSYNTH */
 0x18E87980L, 0x87}, 
 0x08000000L,   /* TXACGAIN */
 0x0123FD80L,   /* RXACGAIN */
 0x07B39180L, 0x184C6F00L, 0x07672280L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_370_620_310_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=220_820_120 rprot=20 rfuse=0 emi_cap=10*/
{
{0x06C91B00L, 0x1FE6B380L, 0x00090B80L, 0x1FFE1B80L,    /* TXACEQ */
 0x0A672A00L, 0x1BEA4880L, 0x00943000L, 0x1FD81D00L},   /* RXACEQ */
{0x001C2A80L, 0x1FBF9880L, 0x01816280L, 0x01023680L,    /* ECFIR/ECIIR */
 0x02273400L, 0x0103EA00L, 0x00523800L, 0x006BD480L,
 0x00188980L, 0x1FE58180L, 0x0C9F8080L, 0x1B4FDD00L},
{0x1F289780L, 0x00B99100L, 0x001D8400L, 0x0C20FC00L,    /* ZSYNTH */
 0x1BDD7980L, 0xB0}, 
 0x08000000L,   /* TXACGAIN */
 0x00FF7E00L,   /* RXACGAIN */
 0x07BB4B80L, 0x1844B500L, 0x07769700L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_220_820_120_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=20 rfuse=0 emi_cap=10*/
{
{0x07F97D80L, 0x0006CC00L, 0x1FFC1480L, 0x1FFC7B80L,    /* TXACEQ */
 0x07F36B80L, 0x000A8E00L, 0x1FF90F00L, 0x1FFAE500L},   /* RXACEQ */
{0x001AF400L, 0x1FC86A80L, 0x01E9AE00L, 0x00652F00L,    /* ECFIR/ECIIR */
 0x01F4AF00L, 0x1F57E000L, 0x00485E00L, 0x1FF3A680L,
 0x1FF83700L, 0x00011D00L, 0x01706980L, 0x066A8480L},
{0x00920F00L, 0x1EE31980L, 0x008ADF00L, 0x0F92E500L,    /* ZSYNTH */
 0x186CE880L, 0x53}, 
 0x085C6880L,   /* TXACGAIN */
 0x013E3100L,   /* RXACGAIN */
 0x07AF6F80L, 0x18509100L, 0x075EDF00L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_600_0_1000_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=200_680_100 rprot=20 rfuse=0 emi_cap=10*/
{
{0x0721FD00L, 0x1FD2AA80L, 0x00027E80L, 0x1FFD4300L,    /* TXACEQ */
 0x09B7CF80L, 0x1D33A080L, 0x006DD680L, 0x1FE08A00L},   /* RXACEQ */
{0x1FF93B80L, 0x00365F80L, 0x00DDBA00L, 0x0129A800L,    /* ECFIR/ECIIR */
 0x0311AA80L, 0x1E718200L, 0x02D3AB80L, 0x1E303E80L,
 0x01181C00L, 0x1EE22300L, 0x051CA700L, 0x02C7AE00L},
{0x01484900L, 0x1C91C280L, 0x0225C700L, 0x0A131400L,    /* ZSYNTH */
 0x1DEA8F80L, 0x8B}, 
 0x08000000L,   /* TXACGAIN */
 0x010ABA80L,   /* RXACGAIN */
 0x07BA3000L, 0x1845D080L, 0x07745F80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_200_680_100_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=220_820_115 rprot=20 rfuse=0 emi_cap=10*/
{
{0x06BFCA00L, 0x1FE56180L, 0x00094A00L, 0x1FFE0400L,    /* TXACEQ */
 0x0A544B80L, 0x1C108680L, 0x0093D180L, 0x1FD7FC80L},   /* RXACEQ */
{0x00227280L, 0x1FB49D80L, 0x01826F80L, 0x01397480L,    /* ECFIR/ECIIR */
 0x01D0C900L, 0x01665C00L, 0x000EA200L, 0x007D2480L,
 0x0014DC00L, 0x1FE90800L, 0x0C998B00L, 0x1B584400L},
{0x00331500L, 0x1E4B8B80L, 0x0180EF00L, 0x0A138100L,    /* ZSYNTH */
 0x1DEA2380L, 0xA3}, 
 0x08000000L,   /* TXACGAIN */
 0x00FDFD80L,   /* RXACGAIN */
 0x07BB1C80L, 0x1844E400L, 0x07763880L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_220_820_115_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=20 rfuse=0 emi_cap=0*/
{
{0x0817F080L, 0x1FE39600L, 0x0005A280L, 0x1FFC7600L,    /* TXACEQ */
 0x07FA6E80L, 0x1FFD4F80L, 0x1FF80500L, 0x1FFDE780L},   /* RXACEQ */
{0x00538E00L, 0x1F47A800L, 0x02957D00L, 0x00128700L,    /* ECFIR/ECIIR */
 0x0194DD80L, 0x000E7100L, 0x1FC5B180L, 0x00061600L,
 0x0021B480L, 0x1FE88600L, 0x1FCAD600L, 0x1FD00E80L},
{0x006A0F00L, 0x1F499F80L, 0x004C8900L, 0x0A152400L,    /* ZSYNTH */
 0x1DE9B580L, 0x68}, 
 0x08419500L,   /* TXACGAIN */
 0x01365F00L,   /* RXACGAIN */
 0x07BB5700L, 0x1844A980L, 0x0776AE80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 }   /* WB_ZSYN_600_0_0_20_0 */
};

Si3226x_FSK_Cfg Si3226x_FSK_Presets[] ={
{
{
0x02232000L,	 /* FSK01 */
0x077C2000L 	 /* FSK10 */
},
{
0x0015C000L,	 /* FSKAMP0 (0.080 vrms )*/
0x000BA000L 	 /* FSKAMP1 (0.080 vrms) */
},
{
0x06B60000L,	 /* FSKFREQ0 (2200.0 Hz space) */
0x079C0000L 	 /* FSKFREQ1 (1200.0 Hz mark) */
},
0x00,			 /* FSK8 */
0x00,			 /* FSKDEPTH (1 deep fifo) */
},  /* DEFAULT_FSK */
{
{
0x026E4000L,	 /* FSK01 */
0x0694C000L 	 /* FSK10 */
},
{
0x0014C000L,	 /* FSKAMP0 (0.080 vrms )*/
0x000CA000L 	 /* FSKAMP1 (0.080 vrms) */
},
{
0x06D20000L,	 /* FSKFREQ0 (2100.0 Hz space) */
0x078B0000L 	 /* FSKFREQ1 (1300.0 Hz mark) */
},
0x00,			 /* FSK8 */
0x00,			 /* FSKDEPTH (1 deep fifo) */
}   /* ETSI_FSK */
};

Si3226x_PulseMeter_Cfg Si3226x_PulseMeter_Presets[] ={
{
0x007A2B6AL,  /* PM_AMP_THRESH (1.000) */
0,            /* Freq (12kHz) */ 
0,            /* PM_AUTO (off)*/
0x07D00000L,  /* PM_active (2000 ms) */
0x07D00000L   /* PM_inactive (2000 ms) */
 }   /* DEFAULT_PULSE_METERING */
};

Si3226x_Tone_Cfg Si3226x_Tone_Presets[] = {
{
	{
	0x07B30000L,	 /* OSC1FREQ (350.000 Hz) */
	0x000C6000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x00,			 /* O1TALO (0 ms) */
	0x00,			 /* O1TAHI */
	0x00,			 /* O1TILO (0 ms) */
	0x00			 /* O1TIHI */
	},
	{
	0x07870000L,	 /* OSC2FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x00,			 /* O2TALO (0 ms) */
	0x00,			 /* O2TAHI */
	0x00,			 /* O2TILO (0 ms) */
	0x00 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_DIAL */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0xA0,			 /* O1TALO (500 ms) */
	0x0F,			 /* O1TAHI */
	0xA0,			 /* O1TILO (500 ms) */
	0x0F			 /* O1TIHI */
	},
	{
	0x07120000L,	 /* OSC2FREQ (620.000 Hz) */
	0x00164000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0xA0,			 /* O2TALO (500 ms) */
	0x0F,			 /* O2TAHI */
	0xA0,			 /* O2TILO (500 ms) */
	0x0F 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_BUSY */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x80,			 /* O1TALO (2000 ms) */
	0x3E,			 /* O1TAHI */
	0x00,			 /* O1TILO (4000 ms) */
	0x7D			 /* O1TIHI */
	},
	{
	0x07870000L,	 /* OSC2FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x80,			 /* O2TALO (2000 ms) */
	0x3E,			 /* O2TAHI */
	0x00,			 /* O2TILO (4000 ms) */
	0x7D 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_RINGBACK */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x60,			 /* O1TALO (300 ms) */
	0x09,			 /* O1TAHI */
	0x60,			 /* O1TILO (300 ms) */
	0x09			 /* O1TIHI */
	},
	{
	0x07120000L,	 /* OSC2FREQ (620.000 Hz) */
	0x00164000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x60,			 /* O2TALO (300 ms) */
	0x09,			 /* O2TAHI */
	0x40,			 /* O2TILO (200 ms) */
	0x06 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_REORDER */
{
	{
	0x07700000L,	 /* OSC1FREQ (480.000 Hz) */
	0x00112000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x40,			 /* O1TALO (200 ms) */
	0x06,			 /* O1TAHI */
	0x40,			 /* O1TILO (200 ms) */
	0x06			 /* O1TIHI */
	},
	{
	0x07120000L,	 /* OSC2FREQ (620.000 Hz) */
	0x00164000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x40,			 /* O2TALO (200 ms) */
	0x06,			 /* O2TAHI */
	0x40,			 /* O2TILO (200 ms) */
	0x06 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_CONGESTION */
{
	{
	0x1F2F0000L,	 /* OSC1FREQ (2130.000 Hz) */
	0x0063A000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x80,			 /* O1TALO (80 ms) */
	0x02,			 /* O1TAHI */
	0x80,			 /* O1TILO (80 ms) */
	0x02			 /* O1TIHI */
	},
	{
	0x1B8E0000L,	 /* OSC2FREQ (2750.000 Hz) */
	0x00A84000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x80,			 /* O2TALO (80 ms) */
	0x02,			 /* O2TAHI */
	0x40,			 /* O2TILO (1000 ms) */
	0x1F 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_FCC_CAS */
{
	{
	0x07870000L,	 /* OSC1FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC1AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x60,			 /* O1TALO (300 ms) */
	0x09,			 /* O1TAHI */
	0x00,			 /* O1TILO (8000 ms) */
	0xFA			 /* O1TIHI */
	},
	{
	0x1B8E0000L,	 /* OSC2FREQ (2750.000 Hz) */
	0x00A84000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x80,			 /* O2TALO (2000 ms) */
	0x3E,			 /* O2TAHI */
	0x00,			 /* O2TILO (4000 ms) */
	0x7D 			 /* O2TIHI */
	},
	0x06 			 /* OMODE */
},  /* TONEGEN_FCC_SAS */
{
	{
	0x1F2F0000L,	 /* OSC1FREQ (2130.000 Hz) */
	0x01BD0000L,	 /* OSC1AMP (-5.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x20,			 /* O1TALO (100 ms) */
	0x03,			 /* O1TAHI */
	0x20,			 /* O1TILO (100 ms) */
	0x03			 /* O1TIHI */
	},
	{
	0x1B8E0000L,	 /* OSC2FREQ (2750.000 Hz) */
	0x02EFC000L,	 /* OSC2AMP (-5.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x20,			 /* O2TALO (100 ms) */
	0x03,			 /* O2TAHI */
	0x20,			 /* O2TILO (100 ms) */
	0x03 			 /* O2TIHI */
	},
	0x66 			 /* OMODE */
},  /* TONEGEN_ETSI_DTAS */
{
	{
	0x05A40000L,	 /* OSC1FREQ (1004.000 Hz) */
	0x005DE000L,	 /* OSC1AMP (-10.000 dBm) */
	0x00000000L,	 /* OSC1PHAS (0.000 rad) */
	0x00,			 /* O1TALO (8000 ms) */
	0xFA,			 /* O1TAHI */
	0x00,			 /* O1TILO (8000 ms) */
	0xFA			 /* O1TIHI */
	},
	{
	0x07870000L,	 /* OSC2FREQ (440.000 Hz) */
	0x000FA000L,	 /* OSC2AMP (-18.000 dBm) */
	0x00000000L,	 /* OSC2PHAS (0.000 rad) */
	0x80,			 /* O2TALO (2000 ms) */
	0x3E,			 /* O2TAHI */
	0x00,			 /* O2TILO (4000 ms) */
	0x7D 			 /* O2TIHI */
	},
	0x47 			 /* OMODE */
}   /* TONEGEN_1004 */
};

Si3226x_PCM_Cfg Si3226x_PCM_Presets[] ={
	{
	0x01, 	 /* PCM_FMT - u-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_8ULAW */
	{
	0x00, 	 /* PCM_FMT - A-Law */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_8ALAW */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x00, 	 /* WIDEBAND - DISABLED (3.4kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	},  /* PCM_16LIN */
	{
	0x03, 	 /* PCM_FMT - 16-bit Linear */
	0x01, 	 /* WIDEBAND - ENABLED (7kHz BW) */
	0x00, 	 /* PCM_TRI - PCLK RISING EDGE */
	0x00, 	 /* TX_EDGE - PCLK RISING EDGE */
	0x00 	 /* A-LAW -  INVERT NONE */
	}   /* PCM_16LIN_WB */
};

