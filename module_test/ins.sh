#!/bin/bash
echo "insmod dependency module..."
insmod ./dependency_module/dependency.ko
echo "insmod test module..."
insmod ./helloworld.ko
sleep 0.005
chmod 666 /dev/zc_cdev
