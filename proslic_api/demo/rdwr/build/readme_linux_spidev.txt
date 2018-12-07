The SPIDEV code will need to be modified to use whatever GPIO path you have.  It assumes sysfs for GPIO userspace access has been compiled into your kernel.  Furthermore, it assumes the direction has been set.

Example shell script for gpio2:

echo 2 >/sys/class/gpio/export #set GPIO 2 as being accessbile...
echo out > /sys/class/gpio/gpio2/direction #default...

Please see https://www.kernel.org/doc/Documentation/gpio/sysfs.txt for more information on usage.

PCM bus setup
-------------

Demo code assumes that the TDM bus has been configured externally and is up prior to running any demo.


