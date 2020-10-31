#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>


#define READ_IOCTL _IOR(60,3,int)

int main() {
    int fd = open("/dev/ring");
    ioctl(fd, READ_IOCTL, 1);
    close(fd);
    return 0;
}