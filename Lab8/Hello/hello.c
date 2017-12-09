#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
static int hello_init(void) {
	printk("KERN_ALERT “HELLO WORLD!\n");
	return 0;
}
static void hello_exit(void) {
	printk(KERN_ALERT "THIS MODULE DESTROYED!\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("Modric@BUAA");
MODULE_DESCRIPTION("HELLO");
