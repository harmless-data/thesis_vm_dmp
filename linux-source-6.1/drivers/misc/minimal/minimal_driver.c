#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init minimal_driver_init(void)
{
	printk(KERN_INFO "Minimal driver: Hello, kernel!\n");
	return 0;
}

static void __exit minimal_driver_exit(void)
{
	printk(KERN_INFO "Minimal driver: Goodbye, kernel!\n");
}

module_init(minimal_driver_init);
module_exit(minimal_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jacob Thiessen");
MODULE_DESCRIPTION("A minimal driver that logs to kernel log.");