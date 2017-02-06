#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");

static int mydev_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int mydev_release(struct inode *inode, struct file *file)
{
	return 0;
}

struct file_operations mydev_fops = {
	.owner = THIS_MODULE,
	.open = mydev_open,
	.release = mydev_release,
};

static struct miscdevice mydev =
{
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "mydev",
	.fops		= &mydev_fops,
};

static int mydev_init(void)
{
	int ret;

	printk(KERN_ALERT "mydev_init\n");
	
	ret = misc_register(&mydev);
	if (ret < 0) {
		printk(KERN_ALERT "misc register failed\n");
		return -1;
	}


	return 0;
}

static void mydev_exit(void)
{
	misc_deregister(&mydev);

	printk(KERN_ALERT "exit\n");
}

module_init(mydev_init);
module_exit(mydev_exit);
