#undef TRACE_SYSTEM
#define TRACE_SYSTEM logmodule

#if !defined(_TRACE_SUBSYS_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_SUBSYS_H

#include <linux/tracepoint.h>

TRACE_EVENT(initf,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("init: %d", __entry->pid));

TRACE_EVENT(exitf,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("exit: %d", __entry->pid));

TRACE_EVENT(mmap,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("mmap: %d", __entry->pid));

TRACE_EVENT(write,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("write: %d", __entry->pid));

TRACE_EVENT(open,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("open: %d", __entry->pid));

TRACE_EVENT(release,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("release: %d", __entry->pid));

TRACE_EVENT(uring_cmd,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("uring_cmd: %d", __entry->pid));

TRACE_EVENT(ioctl,
            TP_PROTO(pid_t pid),
            TP_ARGS(pid),
            TP_STRUCT__entry(
                __field(pid_t, pid)),
            TP_fast_assign(
                __entry->pid = pid),
            TP_printk("ioctl: %d", __entry->pid));

#endif /* _TRACE_SUBSYS_H */

// this is a out of tree build thing.. the tracepoints are doing some magic here
// ...

#undef TRACE_INCLUDE_PATH
#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE trace

#include <trace/define_trace.h>

// source:
// docs.kernel.org/trace/tracepoints