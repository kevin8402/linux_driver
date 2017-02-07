#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>

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

#if 0
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
#endif

static int mydev_proc_show(struct seq_file *m, void *v)
{
	struct mydevice *dev = (struct mydevice *)m->private;

	seq_printf(m, "value=%d\n", dev->value);

	return 0;
}

static int mydev_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, mydev_proc_show, &mydev);
}

static struct file_operations mydev_proc_fops = {
	.owner = THIS_MODULE,
	.open = mydev_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t mydev_value_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%d\n", mydev.value);
}

static ssize_t mydev_value_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	int x;

	if (sscanf(buf, "%d\n", &x) == 1)
		mydev.value = x;

	return count;
}

static DEVICE_ATTR(value, 0600, mydev_value_show,
			mydev_value_store);

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

	device_create_file(mydev.miscdev.this_device, &dev_attr_value);

	proc_create("mydev", 0, NULL, &mydev_proc_fops);


	return 0;
}

static void mydev_exit(void)
{
	remove_proc_entry("mydev", NULL);

	device_remove_file(mydev.miscdev.this_device, &dev_attr_value);

	misc_deregister(&mydev.miscdev);

	printk(KERN_DEBUG "mydev exit\n");
}

module_init(mydev_init);
module_exit(mydev_exit);
