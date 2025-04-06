#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

int hellobpf(void *ctx)
{
    bpf_printk("ePFF Module Hello");
    return 0;
}
