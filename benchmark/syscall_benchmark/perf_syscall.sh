#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <binary_to_run>"
    exit 1
else
    echo "$1"
fi

sudo perf record -e syscalls:sys_enter_ioctl \
    -e syscalls:sys_exit_ioctl \
    -e syscalls:sys_enter_open \
    -e syscalls:sys_exit_open \
    -e syscalls:sys_enter_close  \
    -e syscalls:sys_exit_close  \
    -e syscalls:sys_enter_mmap \
    -e syscalls:sys_exit_mmap \
    -e logmodule:* \
    -- "$1"
