//
//  perspectiveTransform.hpp
//  工科创IIB-prototype
//
//  Created by Apollo on 11/3/15.
//  Copyright © 2015 Apollo. All rights reserved.
//

#ifndef perspectiveTransform_hpp
#define perspectiveTransform_hpp
#include "image.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



void perspectiveTransform();

void perspective_mouse(int mouseAction , int x , int y , int flags , void * param);        //视角变换鼠标回调函数

void setVariables();

void setPoints();

#endif /* perspectiveTransform_hpp */

