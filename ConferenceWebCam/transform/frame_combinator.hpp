//
//  frame_combinator.hpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#pragma once
#include <vector>
#include "../general/frame_source.hpp"

namespace cwc {

namespace transform {

struct CombinationDefinition {
    cv::Mat                 background;
    std::vector<cv::Rect>   boxes;
};

class FrameCombinator : public general::FrameSource {
public:
    FrameCombinator(const CombinationDefinition& definition, const std::vector<general::FrameSourceRef>&);
    cv::Mat next() override;
    virtual std::string getName() override { return "Combined"; };
private:
    CombinationDefinition                   definition;
    std::vector<general::FrameSourceRef>    input_devices;
};

}

}
