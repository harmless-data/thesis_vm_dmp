sudo perf record -e syscalls:sys_enter_ioctl \
    -e syscalls:sys_exit_ioctl \
    -e syscalls:sys_enter_open \
    -e syscalls:sys_exit_open \
    -e syscalls:sys_enter_close  \
    -e syscalls:sys_exit_close  \
    -e syscalls:sys_enter_mmap \
    -e syscalls:sys_exit_mmap \
    -- $1
