#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>


#define CHANGE_BUFF _IOW(61,1,int)

int main() {
    int fd = open("/dev/morse");
    ioctl(fd, CHANGE_BUFF,512 );
    close(fd);
    return 0;
}