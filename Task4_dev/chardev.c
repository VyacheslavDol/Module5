/*
 * chardev.c: создаёт символьное устройство, которое сообщает, сколько
 * раз происходило считывание из файла.
 */
 
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/uaccess.h>
 
/*  Prototypes – обычно помещается в файл .h */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);
 
#define SUCCESS 0
#define DEVICE_NAME "chardev" /* Имя устройства, как оно показано в /proc/devices   */
#define BUF_LEN 80 /* Максимальная длина сообщения устройства. */
 
/* Глобальные переменные объявляются как static, поэтому являются глобальными в пределах файла. */
 
static int major; /* Старший номер, присвоенный драйверу устройства */
 
enum {
    CDEV_NOT_USED = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};
 
/* Устройство открыто? Используется для предотвращения множественных обращений к устройству. */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);
 
static char msg[BUF_LEN]; /* msg, которое устройство будет выдавать при запросе. */
static char buff_reverse[BUF_LEN]; 
static struct class *cls;

static int i = 0, ind = 0; /* переменные для перемещения по строкам */
static int len_msg; /* длина сообщения от пользователя */

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);
 
    if (major < 0) {
        pr_alert("Registering char device failed with %d\n", major);
        return major;
    }
 
    pr_info("I was assigned major number %d.\n", major);
 
    cls = class_create(DEVICE_NAME);
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("Device created on /dev/%s\n", DEVICE_NAME);
 
    return SUCCESS;
}
 
static void __exit chardev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
 
    /* Отмена регистрации устройства. */
    unregister_chrdev(major, DEVICE_NAME);
    printk("Dev %s was istalled\n", DEVICE_NAME);
}
 
/* Методы. */
 
/* Вызывается, когда процесс пытается открыть файл устройства */
static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;
 
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
        return -EBUSY;
    
    printk("Times device opened: %d\n", ++counter);
    sprintf(msg, "I already told you %d times Hello world!\n", counter);
    try_module_get(THIS_MODULE);
    return SUCCESS;
}
 
/* Вызывается, когда процесс закрывает файл устройства. */
static int device_release(struct inode *inode, struct file *file)
{
    /* Теперь можно принимать следующий вызов. */
    atomic_set(&already_open, CDEV_NOT_USED);
 
    /* Декрементируйте число использований, иначе, открыв файл, вы уже
     * не сможете извлечь модуль.
     */
    module_put(THIS_MODULE);

    return SUCCESS;
}
 
/* Вызывается, когда процесс, который уже открыл файл устройства, пытается из него считать.
 */
static ssize_t device_read(struct file *filp, /* см. include/linux/fs.h   */
                           char __user *buffer, /* буфер для данных. */
                           size_t length, /* длина буфера.   */
                           loff_t *offset)
{   
    memset(buff_reverse, 0, BUF_LEN);   /* очистка буффера */
    while(ind >= 0)     /* реверсирование полученного сообщения */
    {
        buff_reverse[ind--] = msg[i++];
    }
    
    if(*offset >= len_msg){     /* проверка по смещению и кол-ву отправленных байт */
        pr_info("Succes reverse to user\n");
        return 0;
    }
    if(copy_to_user(buffer, buff_reverse, len_msg)){ /* отправка в user space */
        pr_info("copy_to_user failed\n");
        return -EFAULT;
    }else{
        *offset += len_msg; /* смещение */
    }
    return len_msg;     /* возвращаемое значение - кол-во записанных байт */
}
 
/* Вызывается, когда процесс производит запись в файл устройства*/
static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
    i = 0;
    ind = len - 1;
    len_msg = len;
    memset(msg, 0, BUF_LEN);
    if(copy_from_user(msg, buff, len)){   /* получение данных из user space */
        return -EFAULT; // Ошибка в случае недоступной памяти
    }
    msg[len] = '\0';    //установка нулевого терминатора
    return len;

}
 
module_init(chardev_init);
module_exit(chardev_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dolgikh Vyacheslav");