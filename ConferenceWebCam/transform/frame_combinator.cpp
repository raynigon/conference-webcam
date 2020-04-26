//
//  frame_combinator.cpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#include "frame_combinator.hpp"

using namespace cwc::general;
using namespace cwc::transform;

FrameCombinator::FrameCombinator(const CombinationDefinition& definition, const std::vector<FrameSourceRef>& devices) :
definition(definition),
input_devices(devices) {
    
}

cv::Mat FrameCombinator::next() {
    cv::Mat result = definition.background.clone();
    
    for (size_t i = 0;i<definition.boxes.size();i++){
        cv::Rect box = definition.boxes[i];
        cv::Mat frame = input_devices[i]->next();
        cv::resize(frame, frame, box.size());
        
        frame.copyTo(result(box));
    }
    
    return result;
}
