/*
** Copyright (c) 2016 Silicon Laboratories, Inc.
** 2016-12-20 21:43:11
**
** Si3217x ProSLIC API Configuration Tool Version 4.0.7
** Last Updated in API Release: 8.2.0
** source XML file: si3217x_BKBT_constants.xml
**
** Auto generated file from configuration tool.
*/


#include "proslic.h"
#include "si3217x.h"

Si3217x_General_Cfg Si3217x_General_Configuration  = {
0x73,                       /* DEVICE_KEY */
BO_DCDC_BUCK_BOOST,         /* BOM_OPT */
BO_GDRV_NOT_INSTALLED,      /* GDRV_OPTION */
VDC_8P0_16P0,               /* VDC_RANGE_OPT */
VDAA_ENABLED,               /* DAA_ENABLE */
AUTO_ZCAL_ENABLED,          /* ZCAL_ENABLE */
BO_STD_BOM,                 /* PM_BOM */
1400L,                      /* I_OITHRESH (1400mA) */
900L,                       /* I_OITHRESH_LO (900mA) */
1400L,                      /* I_OITHRESH_HI (1400mA) */
94000L,                     /* V_OVTHRESH (94000mV) */
5000L,                      /* V_UVTHRESH (5000mV) */
1000L,                      /* V_UVHYST (1000mV) */
0x00000000L,                /* DCDC_FSW_VTHLO */
0x00000000L,                /* DCDC_FSW_VHYST */
0x0048D15BL,                /* P_TH_HVIC */
0x07FEB800L,                /* COEF_P_HVIC */
0x00083120L,                /* BAT_HYST */
0x03D70A20L,                /* VBATH_EXPECT (60.00V) */
0x06147AB2L,                /* VBATR_EXPECT (95.00V) */
0x0FFF0000L,                /* PWRSAVE_TIMER */
0x01999A00L,                /* OFFHOOK_THRESH */
0x00C00000L,                /* VBAT_TRACK_MIN */
0x01800000L,                /* VBAT_TRACK_MIN_RNG */
0x00200000L,                /* DCDC_FSW_NORM */
0x00200000L,                /* DCDC_FSW_NORM_LO */
0x00200000L,                /* DCDC_FSW_RINGING */
0x00200000L,                /* DCDC_FSW_RINGING_LO */
0x0D980000L,                /* DCDC_DIN_LIM */
0x00C00000L,                /* DCDC_VOUT_LIM */
0x0ADD5500L,                /* DCDC_ANA_SCALE */
0x00800000L,                /* THERM_DBI */
0x00FFFFFFL,                /* VOV_DCDC_SLOPE */
0x00A18937L,                /* VOV_DCDC_OS */
0x00A18937L,                /* VOV_RING_BAT_DCDC */
0x00E49BA5L,                /* VOV_RING_BAT_MAX */
0x01018900L,                /* DCDC_VERR */
0x0080C480L,                /* DCDC_VERR_HYST */
0x00400000L,                /* PD_UVLO */
0x00400000L,                /* PD_OVLO */
0x00200000L,                /* PD_OCLO */
0x00400000L,                /* PD_SWDRV */
0x00000000L,                /* DCDC_UVPOL */
0x00000000L,                /* DCDC_RNGTYPE */
0x00600000L,                /* DCDC_ANA_TOFF */
0x00400000L,                /* DCDC_ANA_TONMIN */
0x01500000L,                /* DCDC_ANA_TONMAX */
0x50,                       /* IRQEN1 */
0x13,                       /* IRQEN */
0x03,                       /* IRQEN3 */
0x00,                       /* IRQEN4 */
0x30,                       /* ENHANCE */
0x01,                       /* DAA_CNTL */
0x3F,                       /* AUTO */
};

Si3217x_GPIO_Cfg Si3217x_GPIO_Configuration = {
0x00,     /* GPIO_OE */
0x06,     /* GPIO_ANA */
0x00,     /* GPIO_DIR */
0x00,     /* GPIO_MAN */
0x00,     /* GPIO_POL */
0x00,     /* GPIO_OD */
0x00     /* BATSELMAP */
};
Si3217x_CI_Cfg Si3217x_CI_Presets [] = {
{0}
};
Si3217x_audioGain_Cfg Si3217x_audioGain_Presets [] = {
{0x1377080L,0, 0x0L, 0x0L, 0x0L, 0x0L},
{0x80C3180L,0, 0x0L, 0x0L, 0x0L, 0x0L}
};

Si3217x_Ring_Cfg Si3217x_Ring_Presets[] ={
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 3, Rcpe = 600 ohms
	Rprot = 54 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001D9D5DL,	/* RINGAMP (50.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x006C94D6L,	/* IRING_LIM (70.000 mA) */
0x004AAAA9L,	/* RTACTH (41.233 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x0051EB82L,	/* VOV_RING_BAT (5.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x053A8C42L,	/* VBATR_EXPECT (81.699 v) */
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
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x58,			/* RINGCON */
0x01,			/* USERSTAT */
0x029D4621L,	/* VCM_RING (39.599 v) */
0x029D4621L,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00000000L,	/* DCDC_RNGTYPE */
},  /* RING_MAX_VBAT_PROVISIONING */
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 54 ohms, Type = LPR, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001C0AFCL,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x006C94D6L,	/* IRING_LIM (70.000 mA) */
0x0068A9B9L,	/* RTACTH (57.798 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x0051EB82L,	/* VOV_RING_BAT (5.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04F7DA57L,	/* VBATR_EXPECT (77.628 v) */
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
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x58,			/* RINGCON */
0x01,			/* USERSTAT */
0x027BED2BL,	/* VCM_RING (37.564 v) */
0x027BED2BL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00000000L,	/* DCDC_RNGTYPE */
},  /* RING_F20_45VRMS_0VDC_LPR */
{
/*
	Loop = 500.0 ft @ 0.044 ohms/ft, REN = 5, Rcpe = 600 ohms
	Rprot = 54 ohms, Type = BALANCED, Waveform = SINE
*/ 
0x00050000L,	/* RTPER */
0x07EFE000L,	/* RINGFR (20.000 Hz) */
0x001C0AFCL,	/* RINGAMP (45.000 vrms)  */
0x00000000L,	/* RINGPHAS */
0x00000000L,	/* RINGOF (0.000 vdc) */
0x15E5200EL,	/* SLOPE_RING (100.000 ohms) */
0x006C94D6L,	/* IRING_LIM (70.000 mA) */
0x0068A9B9L,	/* RTACTH (57.798 mA) */
0x0FFFFFFFL,	/* RTDCTH (450.000 mA) */
0x00006000L,	/* RTACDB (75.000 ms) */
0x00006000L,	/* RTDCDB (75.000 ms) */
0x0051EB82L,	/* VOV_RING_BAT (5.000 v) */
0x00000000L,	/* VOV_RING_GND (0.000 v) */
0x04F7DA57L,	/* VBATR_EXPECT (77.628 v) */
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
0x01893740L,	/* DCDC_VREF_MIN_RNG */
0x58,			/* RINGCON */
0x00,			/* USERSTAT */
0x027BED2BL,	/* VCM_RING (37.564 v) */
0x027BED2BL,	/* VCM_RING_FIXED */
0x003126E8L,	/* DELTA_VCM */
0x00000000L,	/* DCDC_RNGTYPE */
}   /* RING_F20_45VRMS_0VDC_BAL */
};

Si3217x_DCfeed_Cfg Si3217x_DCfeed_Presets[] = {
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

Si3217x_Impedance_Cfg Si3217x_Impedance_Presets[] ={
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=54 rfuse=0 emi_cap=10*/
{
{0x07EF4080L, 0x00184A00L, 0x0000E100L, 0x1FFED900L,    /* TXACEQ */
 0x07E8F080L, 0x001B8400L, 0x1FFCF300L, 0x1FFD2680L},   /* RXACEQ */
{0x00218680L, 0x1FAB6080L, 0x021C1200L, 0x00080300L,    /* ECFIR/ECIIR */
 0x02322C00L, 0x1F354100L, 0x003AB400L, 0x00133200L,
 0x1FD87980L, 0x00213380L, 0x0288F400L, 0x0554C800L},
{0x0050B500L, 0x1FBD9980L, 0x1FF20D80L, 0x0A13AD00L,    /* ZSYNTH */
 0x1DEA1780L, 0x5F}, 
 0x08EEBA80L,   /* TXACGAIN */
 0x0153CB80L,   /* RXACGAIN */
 0x07B13100L, 0x184ECF80L, 0x07626200L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_600_0_0_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=270_750_150 rprot=54 rfuse=0 emi_cap=10*/
{
{0x0753B880L, 0x1FC7CD80L, 0x000B3680L, 0x1FFCD900L,    /* TXACEQ */
 0x0A8D4600L, 0x1B920480L, 0x0083CF00L, 0x1FDAEC80L},   /* RXACEQ */
{0x002FAD00L, 0x1F696080L, 0x0254A300L, 0x1F73A480L,    /* ECFIR/ECIIR */
 0x03D2FE00L, 0x1F440300L, 0x01627E00L, 0x00026700L,
 0x00302800L, 0x1FCDAE00L, 0x0C287B80L, 0x1BCDE380L},
{0x1F60AD80L, 0x00A52700L, 0x1FF9F980L, 0x0D90F100L,    /* ZSYNTH */
 0x1A6E1880L, 0xB4}, 
 0x08000000L,   /* TXACGAIN */
 0x0110FF80L,   /* RXACGAIN */
 0x07BBF000L, 0x18441080L, 0x0777E000L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_270_750_150_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=370_620_310 rprot=54 rfuse=0 emi_cap=10*/
{
{0x08372B80L, 0x1FAF8D00L, 0x1FFBF000L, 0x1FFCE600L,    /* TXACEQ */
 0x0A0E7580L, 0x1BE93180L, 0x1F9E0980L, 0x1FE13200L},   /* RXACEQ */
{0x002E9D00L, 0x1F678F00L, 0x0264ED00L, 0x1F440F80L,    /* ECFIR/ECIIR */
 0x0361BA80L, 0x1F493A00L, 0x01162F00L, 0x00349000L,
 0x003D0D80L, 0x1FC1F700L, 0x0DADA000L, 0x1A4CA200L},
{0x00446800L, 0x1F4CD480L, 0x006EB700L, 0x0F08CB00L,    /* ZSYNTH */
 0x18F6D280L, 0x94}, 
 0x08089580L,   /* TXACGAIN */
 0x01319B00L,   /* RXACGAIN */
 0x07B5CC80L, 0x184A3400L, 0x076B9980L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_370_620_310_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=220_820_120 rprot=54 rfuse=0 emi_cap=10*/
{
{0x0717EE80L, 0x1FCA0F00L, 0x0009D680L, 0x1FFC6480L,    /* TXACEQ */
 0x0A81BF80L, 0x1BB57B80L, 0x00A50E00L, 0x1FD37200L},   /* RXACEQ */
{0x00374D80L, 0x1F3CB280L, 0x02B76980L, 0x1F0B9400L,    /* ECFIR/ECIIR */
 0x043A9F80L, 0x1F5EA000L, 0x01302180L, 0x0026C200L,
 0x001FF400L, 0x1FDDA200L, 0x0C605A00L, 0x1B969400L},
{0x0086A800L, 0x1D845180L, 0x01F48A00L, 0x0A137400L,    /* ZSYNTH */
 0x1DEA2780L, 0xAF}, 
 0x08000000L,   /* TXACGAIN */
 0x01086980L,   /* RXACGAIN */
 0x07BCAC80L, 0x18435400L, 0x07795880L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_220_820_120_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=54 rfuse=0 emi_cap=10*/
{
{0x07EF4080L, 0x00184A00L, 0x0000E100L, 0x1FFED900L,    /* TXACEQ */
 0x07E8F080L, 0x001B8400L, 0x1FFCF300L, 0x1FFD2680L},   /* RXACEQ */
{0x00218680L, 0x1FAB6080L, 0x021C1200L, 0x00080300L,    /* ECFIR/ECIIR */
 0x02322C00L, 0x1F354100L, 0x003AB400L, 0x00133200L,
 0x1FD87980L, 0x00213380L, 0x0288F400L, 0x0554C800L},
{0x0050B500L, 0x1FBD9980L, 0x1FF20D80L, 0x0A13AD00L,    /* ZSYNTH */
 0x1DEA1780L, 0x5F}, 
 0x08EEBA80L,   /* TXACGAIN */
 0x0153CB80L,   /* RXACGAIN */
 0x07B13100L, 0x184ECF80L, 0x07626200L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_600_0_1000_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=200_680_100 rprot=54 rfuse=0 emi_cap=10*/
{
{0x077B1700L, 0x1FBD9780L, 0x00032800L, 0x1FFC7180L,    /* TXACEQ */
 0x09C89800L, 0x1D164880L, 0x0075BB80L, 0x1FDE9200L},   /* RXACEQ */
{0x1FFC3D80L, 0x001ED880L, 0x01220380L, 0x00975280L,    /* ECFIR/ECIIR */
 0x03A78C80L, 0x1E090F80L, 0x02D4B480L, 0x1E7F2D80L,
 0x00BE6F80L, 0x1F3CA600L, 0x0643AC00L, 0x01A39C00L},
{0x01596600L, 0x1C593F80L, 0x024D1E00L, 0x0A12AC00L,    /* ZSYNTH */
 0x1DEAF780L, 0x95}, 
 0x08000000L,   /* TXACGAIN */
 0x0115E000L,   /* RXACGAIN */
 0x07BA6580L, 0x18459B00L, 0x0774CA80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_200_680_100_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=220_820_115 rprot=54 rfuse=0 emi_cap=10*/
{
{0x070B3080L, 0x1FCF2780L, 0x0009B700L, 0x1FFCC000L,    /* TXACEQ */
 0x0A674600L, 0x1BEA8C80L, 0x009DEF80L, 0x1FD58380L},   /* RXACEQ */
{0x00021480L, 0x00020180L, 0x01381700L, 0x00FF9B00L,    /* ECFIR/ECIIR */
 0x028E7580L, 0x00657A00L, 0x00C32D00L, 0x0039ED00L,
 0x001A1200L, 0x1FE37080L, 0x0C89C080L, 0x1B6D9800L},
{0x1FEF6480L, 0x1EA98680L, 0x01669600L, 0x0A138100L,    /* ZSYNTH */
 0x1DEA2280L, 0xB2}, 
 0x08000000L,   /* TXACGAIN */
 0x01070500L,   /* RXACGAIN */
 0x07BDBB80L, 0x18424500L, 0x077B7680L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 },  /* ZSYN_220_820_115_30_0 */
/* Source: Database file: cwdb.db */
/* Database information: */
/* parameters: zref=600_0_0 rprot=54 rfuse=4 emi_cap=0*/
{
{0x0833E500L, 0x1FC59680L, 0x00059C00L, 0x1FFEE900L,    /* TXACEQ */
 0x08133900L, 0x1FE40380L, 0x1FF0F100L, 0x00044A00L},   /* RXACEQ */
{0x004ED980L, 0x1F517400L, 0x0288BE80L, 0x00091280L,    /* ECFIR/ECIIR */
 0x01907D80L, 0x00072400L, 0x1FCE6400L, 0x00072A00L,
 0x001E7F80L, 0x1FE78480L, 0x1FE75000L, 0x1FF10D80L},
{0x011E6500L, 0x1DFF7E80L, 0x00E36D00L, 0x09F49700L,    /* ZSYNTH */
 0x1DF25180L, 0x65}, 
 0x08B95A00L,   /* TXACGAIN */
 0x0147F600L,   /* RXACGAIN */
 0x07BF2D00L, 0x1840D380L, 0x077E5A80L,    /* RXACHPF */
#ifdef ENABLE_HIRES_GAIN
 0, 0  /* TXGAIN*10, RXGAIN*10 (hi_res) */
#else
 0, 0  /* TXGAIN, RXGAIN */
#endif
 }   /* WB_ZSYN_600_0_0_20_0 */
};

Si3217x_FSK_Cfg Si3217x_FSK_Presets[] ={
{
{
0x02232000L,	 /* FSK01 */
0x077C2000L 	 /* FSK10 */
},
{
0x003C0000L,	 /* FSKAMP0 (0.220 vrms )*/
0x00200000L 	 /* FSKAMP1 (0.220 vrms) */
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

Si3217x_PulseMeter_Cfg Si3217x_PulseMeter_Presets[] ={
{
0x007A2B6AL,  /* PM_AMP_THRESH (1.000) */
0,            /* Freq (12kHz) */ 
0,            /* PM_RAMP (24kHz)*/
0,            /* PM_FORCE (First)*/
0,            /* PWR_SAVE (off)*/
0,            /* PM_AUTO (off)*/
0x07D00000L,  /* PM_active (2000 ms) */
0x07D00000L   /* PM_inactive (2000 ms) */
 }   /* DEFAULT_PULSE_METERING */
};

Si3217x_Tone_Cfg Si3217x_Tone_Presets[] = {
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

Si3217x_PCM_Cfg Si3217x_PCM_Presets[] ={
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

