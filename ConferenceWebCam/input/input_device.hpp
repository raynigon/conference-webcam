//
//  input_device.hpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#pragma once
#include "opencv2/opencv.hpp"
#include "../general/frame_source.hpp"

namespace cwc {

namespace input {

class InputDevice : public general::FrameSource{
public:
    virtual ~InputDevice() {};
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual cv::Mat next() = 0;
};

using InputDeviceRef = std::shared_ptr<InputDevice>;

}

}
