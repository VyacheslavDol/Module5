#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

static int len, temp;
static char *msg;

ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp) {
    ssize_t ret;
    if (count > temp) {
        count = temp;
    }
    temp = temp - count;
    ret = copy_to_user(buf, msg, count);
    if (ret != 0) {
        // Обработка ошибки, ret - количество байтов, которые не удалось скопировать
        return -EFAULT; // Возвращаем соответствующий код ошибки
    }
    if (count == 0)
        temp = len;
    return count;
}

ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    ssize_t ret;
    ret = copy_from_user(msg, buf, count);
    if (ret != 0) {
        // Обработка ошибки, ret - количество байтов, которые не удалось скопировать
        return -EFAULT; // Возвращаем соответствующий код ошибки
    }
    len = count;
    temp = len;
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};

void create_new_proc_entry(void) { // использование void для отсутствия аргументов теперь обязательно
    proc_create("hello", 0, NULL, &proc_fops);
    msg = kmalloc(10 * sizeof(char), GFP_KERNEL);
}

int proc_init(void) {
    create_new_proc_entry();
    pr_info("proc module loaded\n");  // Сообщение при загрузке модуля
    return 0;
}

void proc_cleanup(void) {
    remove_proc_entry("hello", NULL);
    kfree(msg);
    pr_info("proc module removed\n");  // Сообщение при выгрузке модуля
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dolgikh");
module_init(proc_init);
module_exit(proc_cleanup);