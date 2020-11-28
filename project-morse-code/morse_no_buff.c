#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/malloc.h>
#include <asm/semaphore.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include "/usr/src/linux/drivers/char/console_struct.h"

#define currcons fg_console

#define MAX_NUM_OF_DEVICES 8 
#define BUFFER_START_SIZE 256
#define DOT_START_TIME 500
#define DASH_START_TIME 1000
#define PAUSE_START_TIME 2000

#define CHANGE_BUFF _IOW(60,1,int)
#define CHANGE_DOT_TIME _IOW(60,2,int)
#define CHANGE_DASH_TIME _IOW(60,3,int)
#define CHANGE_PAUSE_TIME _IOW(60,4,int)

static const char *morse_chars[] = {
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

static const char *morse_digits[] = {
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

static const char* morse_space[] = {" "}; //(space)

//struct vc vc_cons [MAX_NR_CONSOLES];

struct body {
    int dot_time, dash_time, pause_time;
    int usecount;
};

struct semaphore sem = MUTEX;
struct body transmitter[8];

char* getMorseCode(char c){
    if(c == ' ')
        return morse_space[0];
    if(isalpha(c))
        return morse_chars[tolower(c) - 'a'];
    if(isdigit(c))
        return morse_digits[c - '0' + 26];
    return "";
}


int morse_open(struct inode *inode, struct file *file) {
    int number = MINOR(inode->i_rdev);
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return -ENODEV;
    
    down(&sem);
    MOD_INC_USE_COUNT;
    transmitter[number].usecount++;
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


int morse_write(struct inode *inode, struct file *file, const char *pB, int count) {
    int number = MINOR(inode->i_rdev);
    unsigned short *topleft = origin;
    int i, j;
    char tmp;
    char* print_string;
    if (number >= MAX_NUM_OF_DEVICES || number < 0)
        return -ENODEV;
    for (i = 0; i < count; i++) {
        if (current->signal & ~current->blocked) {
                if (i == 0)
                    return -ERESTARTSYS;
                return i;
        }
        tmp = get_user(pB + i);
        print_string = getMorseCode(tmp);
        
        for(j=0; j< strlen(print_string); j++){
            if(print_string[j] == '.'){
                *topleft = (*topleft) | 0xf000;
                udelay(DOT_START_TIME*1000);
                *topleft = (*topleft) & 0x0fff;
                udelay(DOT_START_TIME*1000); 
            } else if (print_string[j] == '-'){ // -
                *topleft = (*topleft) | 0xf000;
                udelay(DASH_START_TIME*1000);
                *topleft = (*topleft) & 0x0fff;
                udelay(DOT_START_TIME*1000);
            } else {
                udelay(PAUSE_START_TIME*1000);
            }
                 
        }
    
    }

    // Zmiana background koloru lewego gornego na bialy        
    //*topleft = (*topleft) | 0xf000;
    // Zmiana background koloru lewego gornego na czarny
    //*topleft = (*topleft) & 0x0fff; 

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

#define MORSE_MAJOR 60

int morse_init(void) {
    int i;
    for (i = 0; i < MAX_NUM_OF_DEVICES; i++) {
        transmitter[i].usecount = 0;
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
