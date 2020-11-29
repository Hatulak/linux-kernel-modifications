#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

#define CHANGE_DASH_TIME _IOW(61,3,int)

int main() {
	int fd = open("/dev/morse");
	ioctl(fd, CHANGE_DASH_TIME, 1000);
	close(fd);
	return 0;
}
