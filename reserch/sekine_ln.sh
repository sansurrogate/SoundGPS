#!/bin/sh

unlink ~/school/3a/bdm/SoundGPS/rpi/lib/arm/lib/newlib
unlink ~/school/3a/bdm/SoundGPS/rpi/lib/arm/lib/gcc
unlink ~/school/3a/bdm/SoundGPS/rpi/lib/arm/include/newlib 

ln -s ~/Library/gcc-arm-none-eabi-5_4-2016q3/arm-none-eabi/include ~/school/3a/bdm/SoundGPS/rpi/lib/arm/include/newlib
ln -s ~/Library/gcc-arm-none-eabi-5_4-2016q3/arm-none-eabi/lib ~/school/3a/bdm/SoundGPS/rpi/lib/arm/lib/newlib
ln -s ~/Library/gcc-arm-none-eabi-5_4-2016q3/lib/gcc/arm-none-eabi/5.4.1/ ~/school/3a/bdm/SoundGPS/rpi/lib/arm/lib/gcc
