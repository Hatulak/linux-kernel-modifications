#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/malloc.h>
#include <asm/semaphore.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/ctype.h>
#include <linux/timer.h>

#include <linux/delay.h>
#include "/usr/src/linux/drivers/char/console_struct.h"

#define currcons fg_console

#define MAX_NUM_OF_DEVICES 8 
#define BUFFER_START_SIZE 256
#define DOT_START_TIME 250
#define DASH_START_TIME 500
#define PAUSE_START_TIME 1000

#define CHANGE_BUFF _IOW(61,1,int)
#define CHANGE_DOT_TIME _IOW(61,2,int)
#define CHANGE_DASH_TIME _IOW(61,3,int)
#define CHANGE_PAUSE_TIME _IOW(61,4,int)

static char *morse_chars[] = {
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

static char *morse_digits[] = {
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

static char* morse_space[] = {" "}; //(space)



struct body {
    int* cons_nr;
    int dot_time, dash_time, pause_time;
    int usecount;
    int buffersize;
    char *buffer;
    int buffercount;
    int start, end;
    int transmitting;
    struct wait_queue *write_queue;
};

struct timer_list timer_reader;

struct semaphore sem = MUTEX;
struct semaphore sem_transmission = MUTEX;
struct body transmitter[8];


char* getMorseCode(char c){
    if(c == ' ')
        return morse_space[0];
    if(isalpha(c))
        return morse_chars[tolower(c) - 'a'];
    if(isdigit(c))
        return morse_digits[c - '0' + 26];
}


int morse_open(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return -ENODEV;
    
    down(&sem);
    MOD_INC_USE_COUNT;
    transmitter[number].usecount++;
    *transmitter[number].cons_nr = currcons;
    up(&sem);
    return 0;
}

void morse_release(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return;
    transmitter[number].usecount--;
    MOD_DEC_USE_COUNT;
}

void read_morse(unsigned long arg){
    unsigned short *topleft;
    char* print_string;
    char tmp;
    int j;
    
    topleft = (unsigned short *) vc_cons[*transmitter[arg].cons_nr].d->vc_origin;

    if(transmitter[arg].transmitting == 1){
//        transmitter[arg].transmitting = 1;
	
        if(transmitter[arg].buffercount == 0) {
            transmitter[arg].transmitting = 0;
            return;
        }
        tmp = transmitter[arg].buffer[transmitter[arg].start];

        print_string = getMorseCode(tmp);

        for(j = 0; j < strlen(print_string); j++){
            if(print_string[j] == '.'){
		printk(".");
//	        *topleft = (*topleft) | 0xf000;
//                current->state=TASK_INTERRUPTIBLE;
//                current->timeout=jiffies+transmitter[arg].dot_time*HZ/1000;
//                schedule();

//	            *topleft = (*topleft) & 0x0fff;
//		        current->state=TASK_INTERRUPTIBLE;
//                current->timeout=jiffies+transmitter[arg].dot_time*HZ/1000;
//                schedule();
           	} else if (print_string[j] == '-'){
		    printk("-");
//	    	    *topleft = (*topleft) | 0xf000;
//	    	    current->state=TASK_INTERRUPTIBLE;
//               current->timeout=jiffies+transmitter[arg].dash_time*HZ/1000;
//                schedule();

//	    	    *topleft = (*topleft) & 0x0fff;
//	    	    current->state=TASK_INTERRUPTIBLE;
//                current->timeout=jiffies+transmitter[arg].dot_time*HZ/1000;
 //               schedule();
 	        } else {
			printk(" ");
//		        current->state=TASK_INTERRUPTIBLE;
//                current->timeout=jiffies+transmitter[arg].pause_time*HZ/1000;
//                schedule();
	        }
	    }


        transmitter[arg].start++;
        if (transmitter[arg].start == transmitter[arg].buffersize)
            transmitter[arg].start = 0;
        transmitter[arg].buffercount--;
        wake_up(&transmitter[arg].write_queue);
        

        init_timer(&timer_reader);
        timer_reader.expires=jiffies;
        timer_reader.data = arg;
        timer_reader.function = read_morse;
        add_timer(&timer_reader);

    }

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

        if(transmitter[number].transmitting == 0){
	    transmitter[number].transmitting = 1;
            init_timer(&timer_reader);
            timer_reader.expires=jiffies;
            timer_reader.data = number;
            timer_reader.function = read_morse;
            add_timer(&timer_reader);
        }
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
            printk("Not implemented yet");
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

#define MORSE_MAJOR 61

int morse_init(void) {
    int i;
    for (i = 0; i < MAX_NUM_OF_DEVICES; i++) {
        transmitter[i].usecount = 0;
        transmitter[i].dot_time = DOT_START_TIME;
        transmitter[i].dash_time = DASH_START_TIME;
        transmitter[i].pause_time = PAUSE_START_TIME;
        init_waitqueue(&transmitter[i].write_queue);
        transmitter[i].buffersize = 256;
        transmitter[i].transmitting = 0;
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
