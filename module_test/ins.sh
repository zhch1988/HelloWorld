#!/bin/bash
echo "insmod dependency module..."
insmod ./dependency_module/dependency.ko
echo "insmod test module..."
insmod ./helloworld.ko
