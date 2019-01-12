//
//  binarization.cpp
//  工科创IIB-prototype
//

#include "binarization.h"


int binThreshold = 60;
int binIterations = 49;
int houghThreshold;
int houghDistance;
//int binErode;
IplImage * staticImage;
IplImage * binGrayImage;
IplImage * binBinaryImage;
extern IplImage * windowImage;

void bin() {
    std::cout << "bin1" << '\n';
    staticImage = windowImage;
    extern int binIterations;
    extern IplImage * binGrayImage;
    extern IplImage * binBinaryImage;
    binGrayImage =  cvCreateImage(cvGetSize(windowImage), IPL_DEPTH_8U, 1);
    cvCvtColor(windowImage, binGrayImage, CV_BGR2GRAY);
    
    binBinaryImage = cvCreateImage(cvGetSize(binGrayImage), IPL_DEPTH_8U, 1);
    cvNamedWindow("bin");
    cvMoveWindow("bin", middlewindowX, middlewindowY);
    std::cout << "gray" << '\n';
    cvShowImage("bin", binGrayImage);
    //    cvShowImage("binary image", g_pGrayImage);
    // 创建二值图窗口
    std::cout << "gray" << '\n';
    cvWaitKey(0);
    binaryzation(binThreshold);
    // 滑动条
    cvCreateTrackbar("bin threshold", "bin", &binThreshold, 254, binaryzation);
    //处理结束
    
    //    cvWaitKey(0);
    //    cvDestroyWindow("bin");
    //    cvNamedWindow("bin");
    //    cvShowImage("bin", binBinaryImage);
    //    binGrayImage = cvCreateImage(cvGetSize(binBinaryImage), IPL_DEPTH_8U, 1);
    //    cvCreateTrackbar("erode", "bin", &binErode , 20 , binerode);
    
    cvWaitKey(0);
    cvDestroyWindow("bin");
}

void thin() {
    std::cout << "thin1" << '\n';
    cvNamedWindow("thin");
    cvMoveWindow("thin", middlewindowX, middlewindowY);
    cvShowImage("thin", binBinaryImage);
    cvCreateTrackbar("iterations", "thin", &binIterations, 50, thin);
    cvThin(binBinaryImage, binGrayImage,400);
    cvShowImage("thin", binGrayImage);
    cvWaitKey(0);
    cvDestroyWindow("thin");
}



void binaryzation(int pos) {
    std::cout << "bin2" << '\n';
    cvThreshold(binGrayImage, binBinaryImage, pos, 255, CV_THRESH_BINARY_INV);
    cvShowImage("bin", binBinaryImage);
}

void thin(int pos) {
    std::cout << "thin2" << '\n';
    cvThin(binBinaryImage, binGrayImage,pos);
    cvShowImage("thin", binGrayImage);
    std::cout << pos <<std::endl;
}




void cvThin(IplImage * src, IplImage * dst , int iterations ) {
    cvCopy(src, dst);
    BWImage dstdat(dst);
    IplImage * t_image = cvCloneImage(src);
    BWImage t_dat(t_image);
    for (int n = 0; n < iterations; ++n) {
        for (int s = 0 ; s <= 1 ; ++s) {
            cvCopy(dst, t_image);
            for (int i = 0 ; i < src->height; ++i) {
                for (int j = 0 ; j < src->width; ++j) {
                    if (t_dat[i][j]) {
                        int a = 0 , b = 0 ;
                        int d[8][2] = {{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1}};
                        int p[8];
                        p[0] = (i==0)?0:t_dat[i-1][j];
                        for (int k = 1; k <= 8; k++) {
                            if (i+d[k%8][0] < 0 || i+d[k%8][0] >= src->height || j+d[k%8][1] < 0 || j+d[k%8][1] >= src->width) p[k%8] = 0;
                            else p[k%8] = t_dat[ i+d[k%8][0] ][ j+d[k%8][1] ];
                            if (p[k%8]) {
                                b++;
                                if (!p[k-1]) a++;
                            }
                        }
                        if (b>=2&&b<=6&&a==1) {
                            if (!s && !(p[2]&&p[4]&&(p[0]||p[6]))) {
                                dstdat[i][j]=0;
                            } else if (s && !(p[0]&&p[6]&&(p[2]||p[4]))) {
                                dstdat[i][j]=0;
                            }
                        }
                    }
                }
            }
        }
    }
    cvReleaseImage(&t_image);
}
