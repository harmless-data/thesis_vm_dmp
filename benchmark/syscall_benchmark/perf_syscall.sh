#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <binary_to_run>"
    exit 1
else
    echo "$1"
fi

    # -e syscalls:sys_exit_ioctl \
    # -e logmodule:ioctl \

taskset -c 3 sudo perf record -F 1000 -g -k mono --call-graph fp -m 1024 \
    -e syscalls:sys_enter_ioctl \
    -- taskset -c 2 "$1"