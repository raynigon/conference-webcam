//
//  screen.hpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#pragma once
#include "input_device.hpp"
#include "opencv2/opencv.hpp"
#include <mutex>
#include <QuartzCore/QuartzCore.h>

namespace cwc {

namespace input {

class Screen : public InputDevice {
public:
    Screen();
    virtual ~Screen();
    virtual bool open() override;
    virtual void close() override;
    virtual cv::Mat next() override;
    virtual std::string getName() override { return "Screen"; };
private:
    dispatch_queue_t    queue;
    int                 screen_width;
    int                 screen_height;
    CGDisplayStreamRef  stream_ref;
    std::mutex          mtx_last_frame;
    cv::Mat             last_frame;
};

}

}
