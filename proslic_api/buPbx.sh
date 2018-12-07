#!/bin/bash

make -C demo/uPbx/build SI3217X_C_FB=1  SPIDEV=1  RESET_GPIO="1020" SPIDEV_NO="3.0" ENABLE_CID=1 $1
