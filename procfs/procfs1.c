#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#define procfs_name "helloworld"

struct proc_dir_entry *proc_file;

int procfile_read(struct file *, char __user buffer*, size_t offset, loff_t *)
{
	int ret;

	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

	if (offset > 0) {
		ret = 0;
	} else {
		ret = sprintf(buffer, "HelloWorld!\n");
	}

	return ret;
}

int init_module()
{
	proc_file = create_proc_entry(procfs_name, 0644, NULL);

	if (proc_file == NULL) {
		remove_proc_entry(procfs_name, &proc_root);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}

	proc_file->proc_ops->proc_read = procfile_read;
	proc_file->owner = THIS_MODULE;
	proc_file->mode = S_IFREG | S_IRUGO;
	proc_file->uid = 0;
	proc_file->gid = 0;
	proc_file->size = 37;

	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;
}

void cleanup_module()
{
	remove_proc_entry(procfs_name, &proc_root);
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
