

// main control function for PCCC
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "image.h"
#include "perspectiveTransform.h"
#include "binarization.h"
#include "hough.h"
#include "controlling.h"
#include "bluetoothModule2.h"


#include "controlling.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include "timecalc.h"
using namespace std;
using namespace cv;
//注:窗口是全局变量，无论在哪个文件中调用窗口的名字都是对同一个窗口进行操作

extern int fd;
extern char dev1[];
extern char dev2[];

int main() {
    
    // connecting car
    
     
     int ret;
     fd = UART_Open(fd,dev1);
     ret = UART_Init(fd,115200,0,8,1,'N');
     
     while (fd == -1) {
     fd = UART_Open(fd,dev1);
     ret = UART_Init(fd,115200,0,8,1,'N');
     }
    
     stop(fd);
    std::cout << "stop the car" << std::endl;
    stop(fd);
    
    
    
    // capture camera
    
    extern IplImage *windowImage;
    extern IplImage *processImage;
    IplImage *wImage = cvCreateImage(cvSize(squareWindowSize, squareWindowSize), IPL_DEPTH_8U, 3);
    CvCapture * cam;
    cam = cvCreateCameraCapture(0);
    
    //cvNamedWindow("monitor", 0);
    
    if (!cam) {
        return -1;
    }

    
    while (1) {
        
        
        wImage = cvQueryFrame(cam);
        
        if (cvWaitKey(50) == '\r') {
            windowImage = cvLoadImage("/Users/jingweixi/Desktop/sss.png");
            cvShowImage("monitor", windowImage);
            break;
        }
        cvShowImage("monitor", wImage);
    }
    

    // perspective transform, 变换一帧图像大约需要不到5ms(对于1020 × 790的图像需要3.577ms)
     extern CvMat *transmat;                                                                  //这个变量在后面还会用到
    //perspective
    perspectiveTransform();
    
    bin();
    thin();
    
    
    hough();
    
     // tracking
     controlling(fd);
     
     
     cvReleaseCapture(&cam);
    
    
    return (int)0;
    
}




