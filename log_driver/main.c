
#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/io_uring.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/delay.h>

#define DEVICE_NAME "logmodule"
#define BUF_SIZE PAGE_SIZE

#define CREATE_TRACE_POINTS
#include "trace.h"

#ifdef ENABLE_LOGMODULE_LOGGING
#define LM_LOG(fmt, ...) \
    pr_info("logmodule: " fmt, ##__VA_ARGS__)
#else
#define LM_LOG(fmt, ...) \
    nop();
#endif

#ifdef ENABLE_IOCTL_DELAY
#define IOCTL_DELAY() mdelay(1)
#else
#define IOCTL_DELAY() nop()

#endif

static ktime_t anchor_time;

static uint open_n = 0;
static uint write_n = 0;

static struct class *cls;
static int major;

void *kbuf;
dma_addr_t dma_handle;
static struct device *dma_dummy_dev;

enum io_cmd
{
    HELLO
};

enum
{
    CDEV_FREE,
    CDEV_EXCLUSIVE,
};

struct WriteWrapper
{
    int index;
    char value;
};

static atomic_t already_open = ATOMIC_INIT(CDEV_FREE);

static ssize_t logmodule_write(struct file *, const char __user *, size_t len, loff_t *);
static int logmodule_uring_cmd(struct io_uring_cmd *cmd, unsigned int issue_flags);
static int logmodule_open(struct inode *inode, struct file *file);
static int logmodule_release(struct inode *inode, struct file *file);
long logmodule_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int logmodule_mmap(struct file *file, struct vm_area_struct *vma);
static void dma_dummy_device_release(struct device *dev);

static int __init logmodule_init(void);
static void __exit logmodule_exit(void);

void dump_kbuf(int bytes);

static struct file_operations fops = {
    write : logmodule_write,
    open : logmodule_open,
    release : logmodule_release,
    uring_cmd : logmodule_uring_cmd,
    unlocked_ioctl : logmodule_ioctl,
    mmap : logmodule_mmap,
};

void dump_kbuf(int bytes)
{
    int i;
    char buf[256];
    int len = 0;

    if (!kbuf)
    {
        LM_LOG("kbuf is NULL!\n");
        return;
    }

    if (bytes > BUF_SIZE)
        bytes = BUF_SIZE;

    for (i = 0; i < bytes; ++i)
    {
        len += snprintf(buf + len, sizeof(buf) - len, "%02x ", ((unsigned char *)kbuf)[i]);

        if ((i + 1) % 16 == 0 || i == bytes - 1)
        {
            LM_LOG("kbuf: [%03d] \t%s\n", i - (i % 16), buf);
            len = 0;
        }
    }
}

static ssize_t logmodule_write(struct file *, const char __user *, size_t len, loff_t *)
{
    trace_write(current->pid);

    LM_LOG("wrt \t[ktime: %lld][write_n: %d][pid: %d]\n", ktime_get(), write_n, current->pid);
    write_n += 1;
    return len;
}

static int logmodule_uring_cmd(struct io_uring_cmd *cmd, unsigned int issue_flags)
{
    trace_uring_cmd(current->pid);
    LM_LOG("iou \t[ktime: %lld][opcode: %d][pid: %d]\n", ktime_get(), cmd->cmd_op, current->pid);

    io_uring_cmd_done(cmd, 0, 0, issue_flags);
    return 0;
}

static int logmodule_open(struct inode *inode, struct file *file)
{
    trace_open(current->pid);
    if (atomic_cmpxchg(&already_open, CDEV_FREE, CDEV_EXCLUSIVE))
    {
        return -EBUSY;
    }

    try_module_get(THIS_MODULE);

    LM_LOG("opn \t[ktime: %lld][open_n: %d][pid: %d]\n", ktime_get(), open_n, current->pid);

    open_n += 1;
    return 0;
}

static int logmodule_release(struct inode *inode, struct file *file)
{
    trace_release(current->pid);

    atomic_set(&already_open, CDEV_FREE);
    module_put(THIS_MODULE);

    LM_LOG("rel \t[ktime: %lld][pid: %d]\n", ktime_get(), current->pid);
    return 0;
}

long logmodule_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    trace_ioctl(current->pid);

    LM_LOG("ioc \t[ktime: %lld][cmd: %d][pid: %d]\n", ktime_get(), cmd, current->pid);

    if (cmd >= BUF_SIZE)
    {
        LM_LOG("ioc \t[cmd ouf of reference range (%d/%ld)]", cmd, BUF_SIZE);
        return -EINVAL;
    }

    if (!kbuf)
    {
        LM_LOG("ioc \t[kbuf is NULL]");
        return -EINVAL;
    }

    if (copy_from_user(((char *)kbuf) + cmd, (void __user *)arg, sizeof(char)))
    {
        LM_LOG("ioc \t[copy failed]");
        return -EFAULT;
    }

    IOCTL_DELAY();
    return 0;
}

int logmodule_mmap(struct file *file, struct vm_area_struct *vma)
{
    trace_mmap(current->pid);

    LM_LOG("mmp \t[ktime: %lld][pid: %d]\n", ktime_get(), current->pid);
    unsigned long pfn = virt_to_phys(kbuf) >> PAGE_SHIFT;
    size_t size = vma->vm_end - vma->vm_start;

    if (size > BUF_SIZE)
        return -EINVAL;

    return remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
}

static void dma_dummy_device_release(struct device *dev)
{
    LM_LOG("rdv \t[ktime: %lld][pid: %d]\n", ktime_get(), current->pid);
}

static int __init logmodule_init(void)
{
    trace_initf(current->pid);

    dma_dummy_dev = kzalloc(sizeof(struct device), GFP_KERNEL);
    dma_dummy_dev->coherent_dma_mask = DMA_BIT_MASK(32);
    dma_dummy_dev->release = dma_dummy_device_release;

    dev_set_name(dma_dummy_dev, "dma_dummy_dev");
    dev_set_drvdata(dma_dummy_dev, NULL);
    device_initialize(dma_dummy_dev);

    kbuf = dma_alloc_coherent(dma_dummy_dev, BUF_SIZE, &dma_handle, GFP_KERNEL);

    if (!kbuf)
    {
        put_device(dma_dummy_dev);
        kfree(dma_dummy_dev);

        pr_err("logmodule: Failed to allocate DMA buffer\n");
        return -ENOMEM;
    }
    pr_info("logmodule: dmy [ktime: %lld][virt %p][phys 0x%llx]\n", ktime_get(), kbuf, (unsigned long long)dma_handle);

    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major > 0)
    {
        anchor_time = ktime_get();

        cls = class_create(THIS_MODULE, DEVICE_NAME);
        device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

        LM_LOG("lod \t[ktime: %lld][major: %d]\n", anchor_time, major);
    }
    else
    {
        LM_LOG("fal \t[ktime: %lld]\n", anchor_time);
        return major;
    }

    return 0;
}

static void __exit logmodule_exit(void)
{
    dump_kbuf(BUF_SIZE);

    trace_exitf(current->pid);

    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    unregister_chrdev(0, DEVICE_NAME);

    if (kbuf)
    {
        dma_free_coherent(dma_dummy_dev, BUF_SIZE, kbuf, dma_handle);
        put_device(dma_dummy_dev);
        pr_info("Freed DMA buffer\n");
        kbuf = NULL;
    }

    if (dma_dummy_dev)
    {
        put_device(dma_dummy_dev);
        kfree(dma_dummy_dev);
    }

    LM_LOG("ext \t[open_n: %d][write_n: %d][ktime: %lld]\n", open_n, write_n, ktime_get());
}

module_init(logmodule_init);
module_exit(logmodule_exit);

MODULE_LICENSE("GPL");
