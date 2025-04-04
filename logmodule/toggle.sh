#!/bin/bash
MODULE="logmodule"  # Replace with your module name
DEV_NODE="/dev/mydevice"  # Replace with your device node if applicable

if lsmod | grep -q "^$MODULE"; then
    echo "Module '$MODULE' is loaded. Unloading..."
    sudo rmmod $MODULE
    echo "Module '$MODULE' unloaded."
else
    echo "Module '$MODULE' is not loaded. Loading..."
    sudo insmod "$MODULE.ko"

    echo "Module '$MODULE' loaded."
    sudo chmod 666 /dev/$MODULE
fi
