#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/highmem.h>
#include <linux/scatterlist.h>
#include <linux/crypto.h>
#include <linux/fs.h>

#define KFLAG_SIZE     512

// a single log entry included as part of flag
char glog[128];

static
int show_kflag(struct seq_file *p, void *v)
{
    seq_printf(p, "%s", "\
F38FE749E36CD0437AD0062D6836C393618FCC4A412FDE666C89EB41AC0814D5\n\
B734EBE71127B13BFE569599D6521FF424CEBDF34F07A91CF70B8CB12E324283\n\
788E593AF6DED9DC41A86AB64EA9A7D9A2EF718EB8B6ACFEF7A195EF241649E1\n\
E392715E0E0E936D46E3048A1AA22ADAD0874F6A25092B87D834E2E1E95E20AE\n\
BE020DC6D44A858FA33063F6EDCE7AFFDB858583255CA09CFAC73D73E6AD8A9D\n\
B284D6E905DCBE49D0E24D844B28CEE909A91515E97883A744A854BB60BB50A5\n\
3AFE33A987FC897A39218C553951E87D2E5FEF0D312500DD936A9A0A2F59C450\n\
                      >>> FOR TESTING  <<<                      \n\
D4F31AC01E20589A6D7E1EE3026881F3B01F791B425198A9B0468B094C58797E\n\
EC266EA25DE5EC175CB1E01B4E92830E6CDFA8FB36C8EC58B37B400D4F75B0F9\n\
59CB724D7681B69F5D3A485F0DF03C23AB4B58D566D6849471554FEC5BB76D90\n\
7DB098B8A60C23C81F41923CBF6CB3C23511C258A3BD04DB1FF697A7070E03E8\n\
2747CB047C92E27232B4050A3B6D8A6E8377D939933AD921C6B14BA1FCD2BEA2\n\
3199EC6E3C817041CD61B7B1394CAB1EBB01ACD74D4F1A7E857D22FDA227B834\n\
2AF0AEC61229004D9E8CF6F4E3622419A37F6D410526951D6EB2F639DBEA49AD\n");
    
  return 0;
}

static
ssize_t kflag_proc_write(struct file *file, const char __user *buf,
                         size_t count, loff_t *ppos)
{
  int i;

  if (count > sizeof(glog) - 1)
    return -EFAULT;

  if (copy_from_user(glog, buf, count))
    return -EFAULT;

  glog[count] = '\0';

  // enforce a single line
  for (i = 0; i < count; i ++) {
    if (glog[i] == '\n')
      glog[i] = ' ';
  }

  printk(KERN_INFO "log: %s (%zd)", glog, count);
  return count;
}

static
ssize_t kflag_proc_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
  /* NOTE. inherited */
  if (atomic_long_read(&file->f_count) > 1) {
    printk(KERN_INFO "[kflag] Not allowed to inherit the flag fd\n");
    return -EPERM;
  }
  return seq_read(file, buf, size, ppos);
}

static
int kflag_proc_open(struct inode *inode, struct file *file)
{
  return single_open_size(file, show_kflag, NULL, KFLAG_SIZE);
}

static const struct file_operations proc_kflag_ops = {
  .owner    = THIS_MODULE,
  .open     = kflag_proc_open,
  .read     = kflag_proc_read,
  .write    = kflag_proc_write,
  .llseek   = seq_lseek,
  .release  = single_release,
};

static
int kflag_init(void)
{
  memset(glog, '\0', sizeof(glog));
  if (!proc_create("flag", S_IWUGO | S_IRUGO, NULL, &proc_kflag_ops))
    return -ENOMEM;
  return 0;
}

static
void kflag_exit(void)
{
  remove_proc_entry("flag", NULL);
}

module_init(kflag_init);
module_exit(kflag_exit);

MODULE_AUTHOR("Taesoo Kim");
MODULE_LICENSE("GPL");
