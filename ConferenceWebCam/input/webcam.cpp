//
//  webcam.cpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#include "webcam.hpp"

using namespace cwc::input;

Webcam::~Webcam() {
    
}

bool Webcam::open() {
    return capture_device.open(0);
}

cv::Mat Webcam::next() {
    cv::Mat frame;
    capture_device >> frame;
    cv::cvtColor(frame, frame, cv::COLOR_BGR2BGRA);
    return frame;
}

