//
//  tracking.hpp
//  工科创IIB-prototype
//
//  Created by Apollo on 11/7/15.
//  Copyright © 2015 Apollo. All rights reserved.
//

#ifndef tracking_h
#define tracking_h
#include "image.h"

const char monitor[] = "monitor";
const char trackwindowName[] = "tracking";
const char carfront[] = "/Users/jingweixi/Desktop/IMG_1034.JPG";
const char carback[] =  "/Users/jingweixi/Desktop/IMG_1033.JPG";


void controlling();
void calcHistogram(const char * tarPath, CvHistogram * &tarHistogram);




#endif /* tracking_hpp */
