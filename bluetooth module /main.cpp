#include <iostream>
#include "bluetoothModule.h"

using namespace std;

// extern variables in bM.cpp , you may change them if you need to
extern int fd;
extern char dev1[];
extern char dev2[];


int main() {

    int ret;
    fd = UART_Open(fd,dev1);
    ret = UART_Init(fd,115200,0,8,1,'N');

    while (fd == -1) {
        fd = UART_Open(fd,dev1);
        ret = UART_Init(fd,115200,0,8,1,'N');
    }
    
    // insert your code here
    

    turnleft(fd);
    for (int i = 0 ; i  <100000; ++i) {
        cout<<i*i;
    }
    
    
    UART_Close(fd);
    
    return 0;
    
}