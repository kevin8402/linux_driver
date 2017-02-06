#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

struct mydevice{
	struct miscdevice miscdev;
	int value;
};

static struct mydevice mydev;

static int mydev_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int mydev_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations mydev_fops = {
	.owner = THIS_MODULE,
	.open = mydev_open,
	.release = mydev_release,
};

static ssize_t mydev_proc_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	char *str = "value=100\n";
	size_t len = 0;

	if (*ppos == 0) {
		len = strlen(str) + 1;
		copy_to_user(buf, str, len);
		*ppos += len;
	} else {
		len = 0;
	}

	printk(KERN_DEBUG "proc, len=%d\n", len);

	return len;
}

static struct file_operations mydev_proc_fops = {
	.owner = THIS_MODULE,
	.read = mydev_proc_read,
};

static int mydev_init(void)
{
	int ret;
	struct miscdevice *miscdev;

	printk(KERN_DEBUG "mydev init\n");
	
	miscdev = &mydev.miscdev;
	miscdev->minor = MISC_DYNAMIC_MINOR;
	miscdev->name = "mydev";
	miscdev->fops = &mydev_fops;
	mydev.value = 100;

	ret = misc_register(miscdev);
	if (ret < 0) {
		printk(KERN_DEBUG "misc register failed\n");
		return -1;
	}

	proc_create("mydev", 0, NULL, &mydev_proc_fops);


	return 0;
}

static void mydev_exit(void)
{
	remove_proc_entry("mydev", NULL);

	misc_deregister(&mydev.miscdev);

	printk(KERN_DEBUG "mydev exit\n");
}

module_init(mydev_init);
module_exit(mydev_exit);
