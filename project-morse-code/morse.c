#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/malloc.h>
#include <asm/semaphore.h>
#include <linux/module.h>
#include <linux/ioctl.h>

#define MAX_NUM_OF_DEVICES 8 
#define BUFFER_START_SIZE 256
#define DOT_START_TIME 50
#define DASH_START_TIME 100
#define PAUSE_START_TIME 200

#define CHANGE_BUFF _IOW(60,1,int)
#define CHANGE_DOT_TIME _IOW(60,2,int)
#define CHANGE_DASH_TIME _IOW(60,3,int)
#define CHANGE_PAUSE_TIME _IOW(60,4,int)

static const char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};
static const char *num[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

struct body {
    int buffersize;
    char *buffer;
    int buffercount;
    int start, end;
    int dot_time, dash_time, pause_time;
    int usecount;
    struct wait_queue *write_queue;
};
struct semaphore sem = MUTEX;
struct body transmitter[8];


int morse_open(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return -ENODEV;
    
    down(&sem);
    MOD_INC_USE_COUNT;
    transmitter[number].usecount++;
    if (transmitter[number].usecount == 1) {
        // kmalloc moze uspic proces - uwaga na synchronizacje
        transmitter[number].buffer = kmalloc(transmitter[number].buffersize, GFP_KERNEL);
        transmitter[number].buffercount = transmitter[number].start = transmitter[number].end = 0;
    }
    up(&sem);
    return 0;
}

void morse_release(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return;
    transmitter[number].usecount--;
    MOD_DEC_USE_COUNT;
    if (transmitter[number].usecount == 0)
        kfree(transmitter[number].buffer);
}


int morse_write(struct inode *inode, struct file *file, const char *pB, int count) {
    int number = MINOR(inode->i_rdev);
    int i;
    char tmp;
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return -ENODEV;
    for (i = 0; i < count; i++) {
        tmp = get_user(pB + i);
        while (transmitter[number].buffercount == transmitter[number].buffersize) {
            interruptible_sleep_on(&transmitter[number].write_queue);
            if (current->signal & ~current->blocked) {
                if (i == 0)
                    return -ERESTARTSYS;
                return i;
            }
        }
        transmitter[number].buffer[transmitter[number].end] = tmp;
        transmitter[number].buffercount++;
        transmitter[number].end++;
        if (transmitter[number].end == transmitter[number].buffersize)
            transmitter[number].end = 0;
        
        //Zapisane do bufora. Jak odczytywać?
        wake_up(&transmitter[number].read_queue);
    }
    return count;
}

int morse_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg) {
    int number;
    char *temp;
    int i;
    number = MINOR(inode->i_rdev);
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return -ENODEV;

    switch (cmd) {
        case CHANGE_BUFF:
            if ((arg != 0) && ((arg & (arg - 1)) == 0)) {
                printk("Argument nie jest potegą dwojki z przedzialu <1,1024>");
                return EPERM;
            }
            if (arg < 1 || arg > 1024) {
                printk("Argument wielkosci wykracza poza przedzial <1,1024>");
                return EPERM;
            }

            if (arg < transmitter[number].buffersize && transmitter[number].buffercount > arg) {
                printk("Nowy buffor mniejszy od starego oraz zapelniony! Nie mozna zmienic rozmiaru buffora");
                return EPERM;
            }
            down(&sem);
            temp = kmalloc(arg, GFP_KERNEL);

            if (transmitter[number].end == 0 && transmitter[number].buffercount > 0) {
                transmitter[number].end = transmitter[number].start + transmitter[number].buffercount;
            }

            if (transmitter[number].buffercount < arg) {
                for (i = transmitter[number].start; i < transmitter[number].end; i++) {
                    temp[i] = transmitter[number].buffer[i];
                }
            }
            kfree(transmitter[number].buffer);
            transmitter[number].buffersize = arg;
            transmitter[number].buffer = temp;
            up(&sem);
            wake_up(&transmitter[number].write_queue);
            break;
        case CHANGE_DOT_TIME:
            number = MINOR(inode->i_rdev);
            //Zastanowić się czy tutaj semafor
            transmitter[number].dot_time = arg;
            break;
        case CHANGE_DASH_TIME:
            number = MINOR(inode->i_rdev);
            //Zastanowić się czy tutaj semafor
            transmitter[number].dash_time = arg;
            break;
        case CHANGE_PAUSE_TIME:
            number = MINOR(inode->i_rdev);
            //Zastanowić się czy tutaj semafor
            transmitter[number].pause_time = arg;
            break;
        default:
            printk("Niepoprawna komenda");
            break;
    }
    return 0;
}

struct file_operations morse_ops = {
        write : morse_write,
        open : morse_open,
        release : morse_release,
        ioctl: morse_ioctl
};

#define MORSE_MAJOR 60

int morse_init(void) {
    int i;

    for (i = 0; i < MAX_NUM_OF_DEVICES; i++) {
        init_waitqueue(&transmitter[i].write_queue);
        transmitter[i].usecount = 0;
        transmitter[i].buffersize = BUFFER_START_SIZE;
        transmitter[i].dot_time = DOT_START_TIME;
        transmitter[i].dash_time = DASH_START_TIME;
        transmitter[i].pause_time = PAUSE_START_TIME;
    }

    register_chrdev(MORSE_MAJOR, "morse", &morse_ops);
    printk("Morse device initialized\n");
    return 0;
}

int init_module() {
    return morse_init();
}

void cleanup_module() {
    unregister_chrdev(MORSE_MAJOR, "morse");
}
