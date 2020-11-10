#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int speed_def[] = { B115200, B57600, B38400, B19200, B9600};
int speed_num[] = { 115200, 57600, 38400, 19200, 9600};
void set_speed(int fd, int speed)
{
	int i;
	int status;
	struct termios opt;

	tcgetattr(fd, &opt);

	for ( i= 0; i < sizeof(speed_def) / sizeof(int); i++) {
		if (speed == speed_num[i]) {
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt, speed_def[i]);
			cfsetospeed(&opt, speed_def[i]);
			status = tcsetattr(fd, TCSANOW, &opt);
			if (status != 0)
				perror("tcsetattr");
			return;
		}
		tcflush(fd,TCIOFLUSH);
	}
}

void set_parity(int fd, int databits, int stopbits, int parity)
{
	struct termios opt;

	if (tcgetattr(fd, &opt) != 0) {
		perror("tcgetattr");
		return;
	}

	opt.c_cflag &= ~CSIZE;
	switch (databits) {
		case 7:
			opt.c_cflag |= CS7;
			break;
		case 8:
			opt.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported databits %d\n", databits);
			return;
	}

	switch (parity) {
		case 'n':
		case 'N':
			opt.c_cflag &= ~PARENB;
			opt.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			opt.c_cflag |= (PARODD | PARENB);
			opt.c_iflag |= INPCK;
			break;
		case 'e':
		case 'E':
			opt.c_cflag |= PARENB;
			opt.c_cflag &= ~PARODD;
			opt.c_iflag |= INPCK;
			break;
		case 'S':
		case 's':
			opt.c_cflag &= ~PARENB;
			opt.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported parity %c\n", parity);
			return;
	}

	switch (stopbits) {
		case 1:
			opt.c_cflag &= ~CSTOPB;
			break;
		case 2:
			opt.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported stopbits %d\n", stopbits);
			return;
	}

	if (parity != 'n')
		opt.c_iflag |= INPCK;

	opt.c_cc[VTIME] = 150; // 15 seconds
	opt.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);
	if (tcsetattr(fd, TCSANOW, &opt) != 0) {
		perror("tcsetattr");
		return;
	}
 }

int OpenDev(char *Dev)
{
int	fd = open( Dev, O_RDWR );				 //| O_NOCTTY | O_NDELAY
	if (-1 == fd)
		{ /*设置数据位数*/
			perror("Can't Open Serial Port");
			return -1;
		}
	else
	return fd;

}

int serialsenddata(char *path)
{
	int fd;
	int speed = 115200;
	char *dev;

	dev = argv[1];
	sscanf(argv[2], "%d", &speed);

	fd = open(path, O_RDWR);
	if (fd < 0) {
		perror("Can't open serial device");
		return -1;
	}

	set_speed(fd, speed);
	set_parity(fd, 8, 1, 'N');
	close(fd);
	return 0;
}
