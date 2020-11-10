# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <termios.h>
# include <errno.h>
# include <unistd.h>
# include <string.h>

int main(void)
{
    int fd=open("/dev/ttyAMA1",O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd<0)
    {
        printf("open the uart ttyAMA1 failed\n");
        return -1;
    }

//get the serial port attr.
    struct termios old;
    int ret=tcgetattr(fd,&old);
    if(ret<0)
    {
        printf("get the serial attr fail");
        return -1;
    }

//make the old activity(send and receiver data),and set baudrate.
    old.c_cflag |=(CLOCAL|CREAD);
    cfsetispeed(&old,B115200);
    cfsetospeed(&old,B115200);

//set the data(8bit,1 byte==char)
    old.c_cflag &= ~CSIZE;
    old.c_cflag |= CS8;
//set the stop(1bit)
    old.c_cflag &= ~CSTOPB;
//set the check(no)
    old.c_cflag &= ~PARENB;

//set timeout
    old.c_cc[VTIME]=10;
    old.c_cc[VMIN]=0;

//make the stream control no effect.hardware
//    old.c_cflag &= ~CNEW_RTSCTS;
//    old.c_cflag |= CNEW_RTSCTS;

//make the stream control no effect.software
    old.c_iflag &= ~(IXON|IXOFF|IXANY);
//    old.c_iflag |= (IXON|IXOFF|IXANY);//effect 

//set the attr to serialport.and make the setting effect now!
    ret=tcsetattr(fd,TCSANOW,&old);
    if(ret<0)
    {
        printf("set attr fail\n");
        return -1;
    }

//write data.
   char data[]={0x7E,0x11,0x02,0x20,0x11,0x00,0x02,0x04,0x00,0x00,0x00,0x0D};
   *(data+8)=1099&0XFF;
   *(data+7)=(1099>>8)&0XFF;
    ret=write(fd,data,12);
    if(ret<0)
    {
        printf("write fail\n");
        return -1;
    }

    int i;
    char recvbuf[128]={0};
 //   while(1)
 //   {
        sleep(1);
        ret=read(fd,recvbuf,128);
        if(ret>0)
        {
            for(i=0;i<ret;i++)
            {
               printf("%02x ",recvbuf[i]);
            }
            printf("\n");
        }		
 //   }
    close(fd);
}
