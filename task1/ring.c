#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/malloc.h>
#include <asm/semaphore.h>
#include <linux/module.h>
#include <linux/ioctl.h>

#define CHANGE_BUFF _IOW(60,1,int)
#define READ_BUFF _IOR(60,3,int)

struct body {
    int buffersize;
    char *buffer;
    int buffercount;
    int start, end;
    int usecount;;
    struct wait_queue *read_queue, *write_queue;
};
struct semaphore sem = MUTEX;
struct body sth[4];

int ring_open(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    down(&sem);
    MOD_INC_USE_COUNT;
    sth[number].usecount++;
    if (sth[number].usecount == 1) {
        // kmalloc moze uspic proces - uwaga na synchronizacje
        sth[number].buffer = kmalloc(sth[number].buffersize, GFP_KERNEL);
        sth[number].buffercount = sth[number].start = sth[number].end = 0;
    }
    up(&sem);
    return 0;
}

void ring_release(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    sth[number].usecount--;
    MOD_DEC_USE_COUNT;
    if (sth[number].usecount == 0)
        kfree(sth[number].buffer);
}

int ring_read(struct inode *inode, struct file *file, char *pB, int count) {
    int number = MINOR(inode->i_rdev);
    int i;
    char tmp;
    for (i = 0; i < count; i++) {
        while (sth[number].buffercount == 0) {
            if (sth[number].usecount == 1)
                return i;

            interruptible_sleep_on(&sth[number].read_queue);
            if (current->signal & ~current->blocked) {
                if (i == 0)
                    return -ERESTARTSYS;
                return i;
            }
        }

        tmp = sth[number].buffer[sth[number].start];
        sth[number].start++;
        if (sth[number].start == sth[number].buffersize)
            sth[number].start = 0;
        sth[number].buffercount--;
        wake_up(&sth[number].write_queue);
        put_user(tmp, pB + i);
    }
    return count;
}

int ring_write(struct inode *inode, struct file *file, const char *pB, int count) {
    int number = MINOR(inode->i_rdev);
    int i;
    char tmp;
    for (i = 0; i < count; i++) {
        tmp = get_user(pB + i);
        while (sth[number].buffercount == sth[number].buffersize) {
            interruptible_sleep_on(&sth[number].write_queue);
            if (current->signal & ~current->blocked) {
                if (i == 0)
                    return -ERESTARTSYS;
                return i;
            }
        }
        sth[number].buffer[sth[number].end] = tmp;
        sth[number].buffercount++;
        sth[number].end++;
        if (sth[number].end == sth[number].buffersize)
            sth[number].end = 0;
        wake_up(&sth[number].read_queue);
    }
    return count;
}

int ring_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg) {
    int number;
    char *temp;
    int i;
    printk("%u \n", cmd);
    printk("%u \n", CHANGE_BUFF);
    printk("%u \n", READ_BUFF);
    switch (cmd) {
        case CHANGE_BUFF:
            number = MINOR(inode->i_rdev);
            printk("Numer podrzedny: %u \n", number);

            printk("%lu", arg);
            printk("\n");
            printk("%u", cmd);
            printk("\n");
            printk("caly %d \n", sth[number].buffersize);
            printk("zapelnione %d \n", sth[number].buffercount);

            down(&sem);
            temp = kmalloc(arg, GFP_KERNEL);

            if (arg < sth[number].buffersize)
            {
                printk("Nowy nie moze byc mniejszy od starego");
                return 0;
            }
            if (sth[number].end == 0 && sth[number].buffercount > 0)
            {
                sth[number].end = sth[number].start + sth[number].buffercount;
            }

            printk("poczatek %d \n", sth[number].start);
            printk("koniec %d \n", sth[number].end);

            for (i = sth[number].start; i < sth[number].end; i++)
            {
                temp[i] = sth[number].buffer[i];
            }
            kfree(sth[number].buffer);
            sth[number].buffersize = arg;
            sth[number].buffer = temp;
            up(&sem);
            wake_up(&sth[number].write_queue);
            printk("caly: %d \n", sth[number].buffersize);
            printk("zapelnion: %d \n", sth[number].buffercount);
            break;
        case READ_BUFF:
            number = MINOR(inode->i_rdev);
            printk("caly: %d \n", sth[number].buffersize);
            printk("zapelnione: %d \n", sth[number].buffercount);
            break;
        default:
            printk("Niepoprawna komenda");
            break;
    }
    return 0;
}

struct file_operations ring_ops = {
        read : ring_read,
        write : ring_write,
        open : ring_open,
        release : ring_release,
        ioctl: ring_ioctl
};

#define RING_MAJOR 60

int ring_init(void) {
    int i;

    for (i = 0; i < 4; i++) {
        init_waitqueue(&sth[i].write_queue);
        init_waitqueue(&sth[i].read_queue);
        sth[i].usecount = 0;
        sth[i].buffersize = 1024;
    }

    register_chrdev(RING_MAJOR, "ring", &ring_ops);
    printk("Ring device initialized\n");
    return 0;
}

int init_module() {
    return ring_init();
}

void cleanup_module() {
    unregister_chrdev(RING_MAJOR, "ring");
}
