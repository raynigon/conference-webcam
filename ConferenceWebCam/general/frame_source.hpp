//
//  frame_source.hpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#pragma once
#include "opencv2/opencv.hpp"

namespace cwc {

namespace general {

class FrameSource {
public:
    virtual ~FrameSource() {};
    virtual std::string getName() = 0;
    virtual cv::Mat next() = 0;
};

using FrameSourceRef = std::shared_ptr<FrameSource>;

}

}

