//
//  frame_resize.hpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#pragma once
#include "../general/frame_source.hpp"
#include "opencv2/opencv.hpp"


namespace cwc {

namespace transform {

class FrameSizeTranformer : public general::FrameSource {
public:
    FrameSizeTranformer(const cv::Size&, const general::FrameSourceRef&);
    cv::Mat next() override;
    virtual std::string getName() override { return "ResizeTranformer"; };
private:
    general::FrameSourceRef input;
    cv::Size                outputSize;
};

}

}
