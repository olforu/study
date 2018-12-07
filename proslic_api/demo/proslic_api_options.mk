########################################################################
#
# Generic Makefile include for ProSLIC API - defines dependencies.
# Copyright 2014-2017, Silicon Labs
#
# $Id: proslic_api_options.mk 7093 2018-04-18 22:56:17Z nizajerk $
#
# Customers are encouraged to include this file in their makefiles since
# it converts a general options to specific options used by proslic_api_core.mk
#
########################################################################

########################################################################
# Si3217x Chipset Options
########################################################################

# Si3217x RevB Flyback
ifdef SI3217X_B_FB
PROSLIC_CHIPSET=3217x 3050
PROSLIC_17X_CONVERTER=FB_GD
OUT_FILE_PREFIX=si3217x_b_fb
endif

# Si3217x RevB BJT Buck-Boost
ifdef SI3217X_B_BB
PROSLIC_CHIPSET=3217x 3050
PROSLIC_17X_CONVERTER=BBB
OUT_FILE_PREFIX=si3217x_b_bb
endif

# Si3217x RevB LCQC3W
ifdef SI3217X_B_LCQC3W
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=LCQC3W
OUT_FILE_PREFIX=si3217x_b_lcqc3w
endif

# Si3217x RevC Flyback
ifdef SI3217X_C_FB
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=FB_NOGD
OUT_FILE_PREFIX=si3217x_c_fb
endif

# Si3217x RevC BJT Buck-Boost
ifdef SI3217X_C_BB
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=BBC
OUT_FILE_PREFIX=si3217x_c_bb
endif

# Si3217x RevC LCQC 3W (was LCQCUK)
ifdef SI3217X_C_LCQC3W
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=LCQC3W
OUT_FILE_PREFIX=si3217x_c_lcqc3w
endif

# Si3217x RevC LCQC 6W
ifdef SI3217X_C_LCQC6W
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=LCQC6W
OUT_FILE_PREFIX=si3217x_c_lcqc6w
endif

# Si3217x Rev B PMOS Buck-Boost
ifdef SI3217X_B_PBB
PROSLIC_CHIPSET=3217x 3050
PROSLIC_17X_CONVERTER=PBB_GD
OUT_FILE_PREFIX=si3217x_b_pbb
endif

# Si3217x Rev C PMOS Buck-Boost
ifdef SI3217X_C_PBB
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=PBB_NOGD
OUT_FILE_PREFIX=si3217x_c_pbb
endif

# Si3217x RevC LCQC 7.6W (was LCQCUK)
ifdef SI3217X_C_LCQC7P6W
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=LCQC7P6W
OUT_FILE_PREFIX=si3217x_c_lcqc7p6w
endif

# Si3217x RevC LCUB
ifdef SI3217X_C_LCUB
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=LCUB
OUT_FILE_PREFIX=si3217x_c_lcub
endif

# Si3217x RevC LCFB
ifdef SI3217X_C_LCFB
PROSLIC_CHIPSET=3217x
PROSLIC_17X_CONVERTER=LCFB
OUT_FILE_PREFIX=si3217x_c_lcfb
endif

########################################################################
# Si3226x Chipset Options
########################################################################

# Si3226x RevC Flyback
ifdef SI3226X_C_FB
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=FB
OUT_FILE_PREFIX=si3226x_c_fb
endif

# Si3226x RevC QCUK/LCQC 7.6W
ifdef SI3226X_C_LCQC7P6W
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=LCQC7P6W
OUT_FILE_PREFIX=si3226x_c_lcqc7p6w
endif

# Si3226x RevC BJT Buck Boost
ifdef SI3226X_C_BB
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=BB
OUT_FILE_PREFIX=si3226x_c_bb
endif

# Si3226x RevC LCQCUK/LCQC 3W
ifdef SI3226X_C_LCQC3W
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=LCQC3W
OUT_FILE_PREFIX=si3226x_c_lcqc3w
endif

# Si3226x RevC LCQCUK/LCQC 6W
ifdef SI3226X_C_LCQC6W
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=LCQC6W
OUT_FILE_PREFIX=si3226x_c_lcqc6w
endif

# Si3226x RevC CUK
ifdef SI3226X_C_CK
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=CK
OUT_FILE_PREFIX=si3226x_c_ck
endif

# Si3226x RevC QSS
ifdef SI3226X_C_QS
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=QSS
OUT_FILE_PREFIX=si3226x_c_qs
endif

# Si3226x RevC PMOS Buck-Boost
ifdef SI3226X_C_PBB
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=PBB
OUT_FILE_PREFIX=si3226x_c_pbb
endif

ifdef SI3226X_C_LCUB
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=LCUB
OUT_FILE_PREFIX=si3226x_c_lcub
endif

ifdef SI3226X_C_LCFB
PROSLIC_CHIPSET=3226x
PROSLIC_26X_CONVERTER=LCFB
OUT_FILE_PREFIX=si3226x_c_lcfb
endif

########################################################################
# Si3218x Chipset Options
########################################################################

# Si3218x Rev A LCCB
ifdef SI3218X_A_LCCB
PROSLIC_CHIPSET=3218x
PROSLIC_18X_CONVERTER=LCCB
OUT_FILE_PREFIX=si3218x_a_lccb
endif

# Si3218x Rev A LCCB110
ifdef SI3218X_A_LCCB110
PROSLIC_CHIPSET=3218x
PROSLIC_18X_CONVERTER=LCCB110
OUT_FILE_PREFIX=si3218x_a_lccb110
endif

########################################################################
# Si3219x Chipset Options
########################################################################

# Si3219x Rev A LCCB
ifdef SI3219X_A_LCCB
PROSLIC_CHIPSET=3219x
PROSLIC_19X_CONVERTER=LCCB
OUT_FILE_PREFIX=si3219x_a_lccb
endif

########################################################################
# Si3228x Chipset Options
########################################################################

# Si3228x Rev A LCCB
ifdef SI3228X_A_LCCB
PROSLIC_CHIPSET=3228x
PROSLIC_28X_CONVERTER=LCCB
OUT_FILE_PREFIX=si3228x_a_lccb
endif

# Si3228x Rev A LCCB110
ifdef SI3228X_A_LCCB110
PROSLIC_CHIPSET=3228x
PROSLIC_28X_CONVERTER=LCCB110
OUT_FILE_PREFIX=si3228x_a_lccb110
endif

########################################################################
# Si3050 Chipset Options
########################################################################

# Si3050 
ifdef SI3050
PROSLIC_CHIPSET=3050
OUT_FILE_PREFIX=si3050
endif

########################################################################
# CUSTOM configuration example.
ifdef 26x_3050
PROSLIC_CHIPSET=3050 3226x 
PROSLIC_26X_CONVERTER=FB
OUT_FILE_PREFIX=26x_3050
endif
