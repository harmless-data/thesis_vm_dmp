#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/io_uring.h>
#include <linux/dma-mapping.h>

#define DEVICE_NAME "logmodule"
#define BUF_SIZE PAGE_SIZE

static ktime_t anchor_time;
static uint open_n = 0;
static uint write_n = 0;
static struct class *cls;
static int major;

void *kbuf;
dma_addr_t dma_handle;

enum io_cmd
{
    DEFAULT,
    HELLO
};

enum
{
    CDEV_FREE,
    CDEV_EXCLUSIVE,
};

static atomic_t already_open = ATOMIC_INIT(CDEV_FREE);

static ssize_t logmodule_write(struct file *, const char __user *, size_t len, loff_t *)
{
    printk(KERN_INFO "logmodule: tch \t[write_n: %d][ktime: %lld]\n", write_n, ktime_get());
    write_n += 1;
    return len;
}

static int logmodule_uring_cmd(struct io_uring_cmd *cmd, unsigned int issue_flags)
{
    printk(KERN_INFO "logmodule: iou \t[opcode: %d][ktime: %lld]\n", cmd->cmd_op, ktime_get());

    io_uring_cmd_done(cmd, 0, 0, issue_flags);
    return 0;
}

static int logmodule_open(struct inode *inode, struct file *file)
{
    if (atomic_cmpxchg(&already_open, CDEV_FREE, CDEV_EXCLUSIVE))
    {
        return -EBUSY;
    }

    try_module_get(THIS_MODULE);

    printk(KERN_INFO "logmodule: opn \t[open_n: %d][ktime: %lld]\n", open_n, ktime_get());

    open_n += 1;
    return 0;
}

static int logmodule_release(struct inode *inode, struct file *file)
{
    atomic_set(&already_open, CDEV_FREE);
    module_put(THIS_MODULE);

    printk(KERN_INFO "logmodule: rel \t[ktime: %lld]\n", ktime_get());
    return 0;
}

long logmodule_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd)
    {
    case HELLO:
        printk(KERN_INFO "logmodule: ioc \t[ktime: %lld]\n", ktime_get());
        return 0;

    case DEFAULT:
        break;
    default:
        return 0;
    }

    return 0;
}

int logmodule_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned long pfn = virt_to_phys(kbuf) >> PAGE_SHIFT;
    size_t size = vma->vm_end - vma->vm_start;

    if (size > BUF_SIZE)
        return -EINVAL;

    return remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
}

static struct file_operations fops = {
    write : logmodule_write,
    open : logmodule_open,
    release : logmodule_release,
    uring_cmd : logmodule_uring_cmd,
    unlocked_ioctl : logmodule_ioctl,
};

static int __init logmodule_init(void)
{
    kbuf = dma_alloc_coherent(NULL, BUF_SIZE, &dma_handle, GFP_KERNEL);

    if (!kbuf)
    {
        pr_err("Failed to allocate DMA buffer\n");
        return -ENOMEM;
    }
    pr_info("DMA buffer allocated: virt=%p, phys=0x%llx\n", kbuf, (unsigned long long)dma_handle);

    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major > 0)
    {
        anchor_time = ktime_get();
        printk(KERN_INFO "logmodule: lod \t[major: %d][ktime: %lld]\n", major, anchor_time);

        cls = class_create(THIS_MODULE, DEVICE_NAME);
        device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
        printk(KERN_INFO "logmodule: Device created on /dev/%s\n", DEVICE_NAME);
    }
    else
    {
        printk(KERN_INFO "logmodule: failed to load");
        return major;
    }

    return 0;
}

static void __exit logmodule_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    unregister_chrdev(0, DEVICE_NAME);

    if (kbuf)
    {
        dma_free_coherent(NULL, BUF_SIZE, kbuf, dma_handle);
        pr_info("Freed DMA buffer\n");
        kbuf = NULL;
    }

    printk(KERN_INFO "logmodule: ext \t[open_n: %d][write_n: %d][ktime: %lld]\n", open_n, write_n, ktime_get());
}

module_init(logmodule_init);
module_exit(logmodule_exit);

MODULE_LICENSE("GPL");
