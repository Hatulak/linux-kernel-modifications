#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>


#define WRITE_IOCTL _IOW(60,1,int)

int main() {
    int fd = open("/dev/ring");
    ioctl(fd, WRITE_IOCTL,1 );
}