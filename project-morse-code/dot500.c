#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

#define CHANGE_DOT_TIME _IOW(61,2,int)

int main() {
	int fd = open("/dev/morse");
	ioctl(fd, CHANGE_DOT_TIME, 500);
	close(fd);
	return 0;
}
