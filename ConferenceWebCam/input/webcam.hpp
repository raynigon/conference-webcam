//
//  webcam.hpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#pragma once
#include "input_device.hpp"
#include "opencv2/opencv.hpp"

namespace cwc {

namespace input {

class Webcam : public InputDevice {
public:
    virtual ~Webcam();
    virtual bool open() override;
    virtual void close() override {};
    virtual cv::Mat next() override;
    virtual std::string getName() override { return "Webcam"; };
private:
    cv::VideoCapture capture_device;
};

}

}
