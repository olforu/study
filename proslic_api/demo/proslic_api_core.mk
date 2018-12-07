########################################################################
#
# Generic Makefile include for ProSLIC API - defines dependencies.
# Copyright 2014-2018, Silicon Labs
#
# $Id: proslic_api_core.mk 7093 2018-04-18 22:56:17Z nizajerk $
#
# Customers are encouraged to include this file in their makefiles since
# it defines the dependencies for the various chipsets for several of the 
# commonly used converter topologies.  The PROSLIC_CFG_SRC variable
# can be overwritten to use whichever constants file(s) one wants.
# 
# For multiple chipsets (3217x + 3218x for example), you WILL need
# to define PROSLIC_CFG_SRC for the configuration files that are needed
# for your particular requirements. In addition, the patch files will
# need to be called out.  For example, for 3217x+3218x, You would
# need to define PROSLIC_PATCH_SRC = $(PROSLIC_17X_PATCH_FB) $(PROSLIC_18X_LCCB) 
#
########################################################################

# For backward compatbility, set PROSLIC_XXX_CONVERTER to PROSLIC_XXX_CONVERTER
ifdef PROSLIC_XXX_CONVERTER
PROSLIC_17X_CONVERTER=$(PROSLIC_CONVERTER)
PROSLIC_18X_CONVERTER=$(PROSLIC_CONVERTER)
PROSLIC_19X_CONVERTER=$(PROSLIC_CONVERTER)
PROSLIC_26X_CONVERTER=$(PROSLIC_CONVERTER)
PROSLIC_28X_CONVERTER=$(PROSLIC_CONVERTER)
endif

########################################################################
#
# 3217x options
# 

ifeq ($(findstring 3217x,$(PROSLIC_CHIPSET)),3217x)
PROSLIC_CHIPSET_SRC += si3217x_intf.c 
PROSLIC_FXS_SET=1

PROSLIC_CFLAGS += -DSI3217X

#
# Common Patch file definitions...
#
ifdef SI3217X_REVB_ONLY		
PROSLIC_17X_PATCH_FB ?= si3217x_patch_B_FB_2017MAY25.c 
PROSLIC_17X_PATCH_BB ?= si3217x_patch_B_BB_2012DEC10.c 
PROSLIC_CFLAGS += -DSI3217X_REVB_ONLY=1
PROSLIC_CHIPSET_SRC += si3217x_revb_intf.c
else

ifdef SI3217X_REVC_ONLY
PROSLIC_17X_PATCH_FB ?=	si3217x_patch_C_FB_2017MAY25.c
PROSLIC_17X_PATCH_BB ?= si3217x_patch_C_FB_2017MAR22.c
PROSLIC_CFLAGS += -DSI3217X_REVC_ONLY=1
PROSLIC_CHIPSET_SRC += si3217x_revc_intf.c 

else # default... both versions..
PROSLIC_17X_PATCH_FB ?=	si3217x_patch_B_FB_2017MAY25.c \
			si3217x_patch_C_FB_2017MAY25.c

PROSLIC_17X_PATCH_BB ?= si3217x_patch_B_BB_2012DEC10.c \
			si3217x_patch_C_FB_2017MAY25.c
PROSLIC_CHIPSET_SRC += si3217x_revb_intf.c si3217x_revc_intf.c 
endif # SI3217X_REVC_ONLY
endif # SI3217X_REVB_ONLY

ifeq ($(PROSLIC_17X_CONVERTER),FB_GD)
PROSLIC_CFG_SRC_LIST +=si3217x_FLBK_GDRV_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),BBB)
PROSLIC_CFG_SRC_LIST +=si3217x_BKBT_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_BB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),BBC)
PROSLIC_CFG_SRC_LIST +=si3217x_BKBT_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),FB_NOGD)
PROSLIC_CFG_SRC_LIST +=si3217x_FLBK_NO_GDRV_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),LCQC3W)
PROSLIC_CFG_SRC_LIST +=si3217x_LCQC3W_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),LCQC6W)
PROSLIC_CFG_SRC_LIST +=si3217x_LCQC6W_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),LCQC7P6W)
PROSLIC_CFG_SRC_LIST +=si3217x_LCQC7P6W_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),PBB_GD)
PROSLIC_CFG_SRC_LIST +=si3217x_PBB_VDC_9P0_24P0_GDRV_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),PBB_NOGD)
PROSLIC_CFG_SRC_LIST +=si3217x_PBB_VDC_9P0_24P0_NO_GDRV_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),LCUB)
PROSLIC_CFG_SRC_LIST +=si3217x_LCUB_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),LCFB)
PROSLIC_CFG_SRC_LIST +=si3217x_LCFB_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB)
endif

ifeq ($(PROSLIC_17X_CONVERTER),MULTI_BOM)
PROSLIC_CFG_SRC_LIST +=si3217x_MULTI_BOM_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_17X_PATCH_FB) $(PROSLIC_17X_PATCH_BB)
endif

endif # 3217x


########################################################################
#
# 3218x options
# 

ifeq ($(findstring 3218x,$(PROSLIC_CHIPSET)),3218x)
PROSLIC_FXS_SET=1
PROSLIC_CHIPSET_SRC += si3218x_intf.c

PROSLIC_CFLAGS += -DSI3218X

PROSLIC_18X_PATCH ?=	si3218x_patch_A_2017MAY25.c
PROSLIC_18X_PATCH_LCCB ?= $(PROSLIC_18X_PATCH)

ifeq ($(PROSLIC_18X_CONVERTER),LCCB)
PROSLIC_CFG_SRC_LIST +=si3218x_LCCB_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_18X_PATCH)
endif

ifeq ($(PROSLIC_18X_CONVERTER),LCCB110)
PROSLIC_CFG_SRC_LIST   += si3218x_LCCB110_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_18X_PATCH)
endif

ifeq ($(PROSLIC_18X_CONVERTER),MULTI_BOM)
PROSLIC_CFG_SRC_LIST +=si3218x_MULTI_BOM_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_18X_PATCH_LCCB)
endif

endif # 3218x

########################################################################
#
# 3219x options
# 

ifeq ($(findstring 3219x,$(PROSLIC_CHIPSET)),3219x)
PROSLIC_FXS_SET=1
PROSLIC_CHIPSET_SRC += si3219x_intf.c

PROSLIC_CFLAGS += -DSI3219X

PROSLIC_19X_PATCH ?=	si3219x_patch_A_2017MAY25.c
PROSLIC_19X_PATCH_LCCB ?= $(PROSLIC_19X_PATCH)

ifeq ($(PROSLIC_19X_CONVERTER),LCCB)
PROSLIC_CFG_SRC_LIST +=si3219x_LCCB_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_19X_PATCH)
endif

ifeq ($(PROSLIC_19X_CONVERTER),MULTI_BOM)
PROSLIC_CFG_SRC_LIST +=si3219x_MULTI_BOM_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_19X_PATCH_LCCB)
endif

endif # 3219x

########################################################################
#
# 3226x options
# 

ifeq ($(findstring 3226x,$(PROSLIC_CHIPSET)),3226x)
PROSLIC_FXS_SET=1
PROSLIC_CHIPSET_SRC += si3226x_intf.c
PROSLIC_CFLAGS += -DSI3226X

PROSLIC_26X_PATCH_FB ?= si3226x_patch_C_FB_2017MAY26.c
PROSLIC_26X_PATCH_TSS ?= si3226x_patch_C_TSS_2014JUN18.c

ifeq ($(PROSLIC_26X_CONVERTER),FB)
PROSLIC_CFG_SRC_LIST +=si3226x_FLBK_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),BB)
PROSLIC_CFG_SRC_LIST +=si3226x_BB_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),LCQC3W)
PROSLIC_CFG_SRC_LIST +=si3226x_LCQC3W_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),LCQC6W)
PROSLIC_CFG_SRC_LIST +=si3226x_LCQC6W_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),LCQC7P6W)
PROSLIC_CFG_SRC_LIST +=si3226x_LCQC7P6W_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),CK)
PROSLIC_CFG_SRC_LIST +=si3226x_CUK_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),QSS)
PROSLIC_CFG_SRC_LIST +=si3226x_QSS_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_TSS)
endif

ifeq ($(PROSLIC_26X_CONVERTER),PBB)
PROSLIC_CFG_SRC_LIST +=si3226x_PBB_7P0_20P0_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),LCUB)
PROSLIC_CFG_SRC_LIST +=si3226x_LCUB_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),LCFB)
PROSLIC_CFG_SRC_LIST +=si3226x_LCFB_constants.c
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB)
endif

ifeq ($(PROSLIC_26X_CONVERTER),MULTI_BOM)
PROSLIC_CFG_SRC_LIST +=si3226x_MULTI_BOM_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_26X_PATCH_FB) $(PROSLIC_26X_PATCH_TSS) si3226x_patch_C_TSS_ISO_2014JUN18.c
endif

endif # si3226x


########################################################################
#
# 3228x options
# 

ifeq ($(findstring 3228x,$(PROSLIC_CHIPSET)),3228x)
PROSLIC_FXS_SET=1
PROSLIC_CHIPSET_SRC += si3228x_intf.c

PROSLIC_CFLAGS += -DSI3228X

PROSLIC_28X_PATCH ?=	si3228x_patch_A_2017MAY26.c
PROSLIC_28X_PATCH_LCCB ?= $(PROSLIC_28X_PATCH)

ifeq ($(PROSLIC_28X_CONVERTER),LCCB)
PROSLIC_CFG_SRC_LIST +=si3228x_LCCB_SR_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_28X_PATCH)
endif

ifeq ($(PROSLIC_28X_CONVERTER),LCCB110)
PROSLIC_CFG_SRC_LIST +=si3228x_LCCB110_SR_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_28X_PATCH)
endif

ifeq ($(PROSLIC_28X_CONVERTER),MULTI_BOM)
PROSLIC_CFG_SRC_LIST +=si3228x_MULTI_BOM_constants.c 
PROSLIC_PATCH_SRC_LIST +=$(PROSLIC_28X_PATCH_LCCB)
endif

endif # 3228x

########################################################################
#
# Si3050 options
#
ifeq ($(findstring 3050,$(PROSLIC_CHIPSET)),3050)
PROSLIC_DAA_SUPPORT=1
PROSLIC_CFLAGS += -DSI3050_CHIPSET
endif # Si3050

########################################################################
#
# Common
#

PROSLIC_SRC_CORE= si_voice.c si_voice_version.c

ifeq ($(PROSLIC_FXS_SET),1)
PROSLIC_SRC_CORE += proslic.c
ifeq ($(PROSLIC_TSTIN_SUPPORT),1)
PROSLIC_SRC_CORE += proslic_tstin.c
endif

ifndef PROSLIC_CFG_SRC
PROSLIC_CFG_SRC := $(PROSLIC_CFG_SRC_LIST)
endif
ifndef PROSLIC_PATCH_SRC
PROSLIC_PATCH_SRC := $(PROSLIC_PATCH_SRC_LIST)
endif
endif # FXS 

ifeq ($(PROSLIC_DAA_SUPPORT),1)
PROSLIC_CHIPSET_SRC += vdaa.c 
DAA_CFG_SRC_LIST ?= vdaa_constants.c
ifndef DAA_CFG_SRC
DAA_CFG_SRC := $(DAA_CFG_SRC_LIST)
endif
endif

ifndef PROSLIC_CORE_MULTI_INCLUDE
PROSLIC_CFLAGS += -I$(PROSLIC_API_DIR)/inc 
endif

