//
//  frame_resize.cpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#include "frame_resize.hpp"

using namespace cwc::general;
using namespace cwc::transform;

FrameSizeTranformer::FrameSizeTranformer(const cv::Size& outputSize, const general::FrameSourceRef& input) :
outputSize(outputSize),
input(input) {
    
}


cv::Mat FrameSizeTranformer::next() {
    return input->next();
}
