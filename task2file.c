#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/mm.h>		
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mount.h>

struct file *get_mm_exe_file(struct mm_struct *mm)
{       
        struct file *exe_file;

        /* We need mmap_sem to protect against races with removal of
         * VM_EXECUTABLE vmas */
        down_read(&mm->mmap_sem);
        exe_file = mm->exe_file;
        if (exe_file)
                get_file(exe_file);
        up_read(&mm->mmap_sem);
        return exe_file;
}

struct mm_struct *get_task_mm(struct task_struct *task)
{
	struct mm_struct *mm;
	task_lock(task);
	mm = task->mm;
	if(mm){
		if (task->flags & PF_KTHREAD)
			mm = NULL;
		else
			atomic_inc(&mm->mm_users);
	}
	task_unlock(task);
	return mm;
}

static int __init task2file_init(void)
{
    	printk("[+task2file] loading...\n");
	struct mm_struct *mm = get_task_mm(current);
	struct file *exe_file = get_mm_exe_file(mm);
	struct inode *i = exe_file->f_path.dentry->d_inode;
	char *pathname = exe_file->f_path.dentry->d_name.name;
	if(i)
	    	printk("current->cmd: %s, pathname:%s, inode:%lu\n", current->comm, pathname, i->i_ino);
    	return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit task2file_cleanup(void)
{
  printk(KERN_INFO "[+task2file] exiting...\n");
}

module_init(task2file_init);
module_exit(task2file_cleanup);

