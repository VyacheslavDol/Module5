/* 
 * hello-sysfs.c - пример использования sysfs 
 */ 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/kobject.h> 
#include <linux/module.h> 
#include <linux/string.h> 
#include <linux/sysfs.h>
#include <linux/configfs.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>

struct timer_list my_timer;
struct tty_driver *my_driver;
static int _kbledstatus = 0;
#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF

static struct kobject *mymodule; 
 
/* Переменная, которую нужно будет изменять И которая отвечает за моргание. */ 
static int myvariable = 0; 
 
static ssize_t myvariable_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{ 
    return sprintf(buf, "%d\n", myvariable); 
} 
 
static ssize_t myvariable_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count) 
{ 
    sscanf(buf, "%du", &myvariable);
    if(myvariable > 7 || myvariable < 0)
        myvariable = 0; 
    return count; 
} 
 
static struct kobj_attribute myvariable_attribute = __ATTR(myvariable, 0660, myvariable_show, (void *)myvariable_store); 

static void my_timer_func(struct timer_list *ptr)
{
        int *pstatus = &_kbledstatus;
        if (*pstatus == myvariable)
                *pstatus = RESTORE_LEDS;
        else
                *pstatus = myvariable;
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            *pstatus);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static int __init kbledssysfs_init(void) 
{ 
    int error = 0; 
 
    pr_info("kbleds_sysfs: initialised\n"); 

    printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);
    for (int i = 0; i < MAX_NR_CONSOLES; i++) {
            if (!vc_cons[i].d)
                    break;
            printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
                   MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                   (unsigned long)vc_cons[i].d->port.tty);
    }
    printk(KERN_INFO "kbleds: finished scanning consoles\n");
    my_driver = vc_cons[fg_console].d->port.tty->driver;
    printk(KERN_INFO "kbleds: tty driver name %s\n", my_driver->name);

    mymodule = kobject_create_and_add("mymodule", kernel_kobj); 
    if (!mymodule) 
        return -ENOMEM; 
 
    error = sysfs_create_file(mymodule, &myvariable_attribute.attr); 
    if (error) { 
        pr_info("failed to create the myvariable file " 
                "in /sys/kernel/mymodule\n"); 
    } 

    /* Set up the LED blink timer the first time */        
    timer_setup(&my_timer, my_timer_func, 0);
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);

    return error; 
}

static void __exit kbledssysfs_exit(void) 
{ 
    pr_info("kbleds_sysfs: Exit success\n");
    del_timer(&my_timer);
    (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
    kobject_put(mymodule); 
} 
 
module_init(kbledssysfs_init); 
module_exit(kbledssysfs_exit); 
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dolgikh V.");