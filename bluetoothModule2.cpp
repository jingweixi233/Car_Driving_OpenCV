#include "bluetoothModule2.h"

double FALSE_ = -1;
double TRUE_ = 0;

char status = 's';

int fd = FALSE_;

char dev1[] = "/dev/cu.AEGIN-SPPDev";
char dev2[] = "/dev/cu.AEGIN-SPPDev";


void changeStatus() {
    if (status != 's') {
        stop(fd);
        status = 's';
    }
    else {
        go(fd);
        status = 'g';
    }
}




int UART_Send(int fd, char *send_buf,int data_len)
{
    int ret;
    ret = (int)write(fd,send_buf,data_len);
    std::cout<<std::endl<<"ret="<<ret<<std::endl;
    if (data_len == ret ){
        return ret;
    } else {
        tcflush(fd,TCOFLUSH);
        return FALSE_;
    }
}

//有些东西不知道英文怎么说我就用中文写注释了😂

//调用系统的read函数，但是有点小问题，貌似会出现丢包的情况，可以写个包裹函数来解决
int UART_Recv(int fd, char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;
    
    struct timeval time;
    
    FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);
    
    time.tv_sec = 10;
    time.tv_usec = 0;
    
    //使用select实现串口的多路通信
    fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel){
        len = (int)read(fd,rcv_buf,data_len);
        return len;
    } else {
        return FALSE_;
    }
}

int UART_Open(int fd, char* port){
    fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);
    if (FALSE_ == fd){
        perror("Bluetooth device offline");
        return(FALSE_);
    }
    if(fcntl(fd, F_SETFL, 0) < 0){                                                                          //判断串口的状态是否为阻塞状态
        printf("fcntl failed!\n");
        return(FALSE_);
    } else {
        printf("Blutooth device online ,fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
    if(0 == isatty(STDIN_FILENO)){                                                                          //测试是否为终端设备
        printf("standard input is not a terminal device\n");
        return(FALSE_);
    }
    return fd;
}

void UART_Close(int fd)
{
    close(fd);
}

int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
    
    int i;
    
    
    //    int status;
    int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
        B38400, B19200, B9600, B4800, B2400, B1200, B300
    };
    int name_arr[] = {
        38400, 19200, 9600, 4800, 2400, 1200, 300, 38400,
        19200, 9600, 4800, 2400, 1200, 300
    };
    struct termios options;
    
    /*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数,还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
     */
    if(tcgetattr( fd,&options) != 0){
        perror("SetupSerial 1");
        return(FALSE_);
    }
    
    //设置串口输入波特率和输出波特率
    for(i= 0;i < sizeof(speed_arr) / sizeof(int);i++) {
        if (speed == name_arr[i]) {
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
        }
    }
    //修改控制模式，保证程序不会占用串口
    options.c_cflag |= CLOCAL;
    //修改控制模式，使得能够从串口中读取输入数据
    options.c_cflag |= CREAD;
    //设置数据流控制
    switch(flow_ctrl){
        case 0 : //不使用流控制
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1 : //使用硬件流控制
            options.c_cflag |= CRTSCTS;
            break;
        case 2 : //使用软件流控制
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;
    }
    //设置数据位
    options.c_cflag &= ~CSIZE; //屏蔽其他标志位
    switch (databits){
        case 5 :
            options.c_cflag |= CS5;
            break;
        case 6    :
            options.c_cflag |= CS6;
            break;
        case 7    :
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unsupported data size\n");
            return (FALSE_);
    }
    //设置校验位
    switch (parity) {
        case 'n':
        case 'N': //无奇偶校验位。
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 'o':
        case 'O': //设置为奇校验
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;
            break;
        case 'e':
        case 'E': //设置为偶校验
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;
        case 's':
        case 'S': //设置为空格
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported parity\n");
            return (FALSE_);
    }
    // 设置停止位
    switch (stopbits){
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported stop bits\n");
            return (FALSE_);
    }
    //修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;
    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */
    
    //如果发生数据溢出，接收数据，但是不再读取
    tcflush(fd,TCIFLUSH);
    
    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("com set error!/n");
        return (FALSE_);
    }
    return (TRUE_);
}


int UART_Init(int fd, int speed,int flow_ctrlint ,int databits,int stopbits,char parity)
{
    //设置串口数据帧格式
    if (FALSE_ == UART_Set(fd,speed,flow_ctrlint,databits,stopbits,parity)) {
        return FALSE_;
    } else {
        return TRUE_;
    }
}



void go(int fd) {
    printf("go\n");
    write(fd,"2",1);
//    write(fd,"4",1);
}

void stop(int fd) {
    printf("stop\n");
    write(fd,"5",1);
//    write(fd,"5",1);

}


void turnleft(int fd) {
    printf("left\n");
    write(fd,"1",1);
//    write(fd,"8",1);
    
}

void turnright(int fd) {
    printf("right\n");
    write(fd, "3", 1);
//    write(fd, "2", 1);

}



