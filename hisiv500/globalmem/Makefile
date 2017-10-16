
ifeq ($(EXTDRV_PARAM_FILE), ) 
    EXTDRV_PARAM_FILE:=../Makefile.param
    include $(EXTDRV_PARAM_FILE)
endif



#SRCS = $(wildcard *.c)
SRCS = gmem.c

#INC := -I$(REL_INC)
#INC += -I./arch/$(ARCH_DIR)/
#INC += -I$(DRV_ROOT)/extdrv/sensor_i2c/arch/$(ARCH_DIR)/
#INC += -I$(OSAL_ROOT)/include

#EXTDRV_CFLAGS += $(INC)



		
#*************************************************************************
TARGET := globalmem

#*************************************************************************
# compile linux or HuaweiLite
include $(PWD)/../Make.$(OSTYPE)

