//
//  main.cpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <thread>
#include "general/frame_source.hpp"
#include "input/input_device.hpp"
#include "input/webcam.hpp"
#include "input/screen.hpp"
#include "transform/frame_combinator.hpp"
#include "transform/frame_resize.hpp"

using namespace cwc::general;
using namespace cwc::input;
using namespace cwc::transform;


InputDeviceRef createWebcam() {
    auto pWebcam = std::make_shared<Webcam>();
    return std::dynamic_pointer_cast<InputDevice>(pWebcam);
}

InputDeviceRef createScreen() {
    auto pWebcam = std::make_shared<Screen>();
    return std::dynamic_pointer_cast<InputDevice>(pWebcam);
}

/*
 while(!inputDevice->open()){
 if( cv::waitKey(250) == 27 ) return 0;
 }
 for(;;)
 {
 cv::Mat frame = inputDevice->grabFrame();
 if( frame.empty() ) continue; // end of video stream
 cv::imshow("this is you, smile! :)", frame);
 if( cv::waitKey(10) == 27 ) break; // stop capturing by pressing ESC
 }
 */

FrameSourceRef createFrameCombinator(const std::vector<FrameSourceRef>& frameSources) {
    cv::Mat backgroundImage = cv::imread("background.jpg");
    cv::cvtColor(backgroundImage, backgroundImage, cv::COLOR_BGR2BGRA);
    auto pFrameCombinator = std::make_shared<FrameCombinator>(CombinationDefinition{
        backgroundImage,
        {
            cv::Rect(0, 0, 1920, 1080),
            cv::Rect(1720, 964, 192, 108) // 1920-192-8
        }
    }, frameSources);
    return std::dynamic_pointer_cast<FrameSource>(pFrameCombinator);
}

FrameSourceRef createFrameSizeTranformer(const FrameSourceRef& frameSource) {
    auto pFrameCombinator = std::make_shared<FrameSizeTranformer>(cv::Size(1920, 1080), frameSource);
    return std::dynamic_pointer_cast<FrameSource>(pFrameCombinator);
}

FrameSourceRef selectFrameSource(const std::vector<FrameSourceRef>& frameSources){
    size_t index = 1;
    for (const auto& source : frameSources) {
        std::cout << index << ") " << source->getName() << std::endl;
        index++;
    }
    std::cout << "Select Frame Source:" << std::endl;
    std::cin >> index;
    index--;
    auto source = frameSources[index];
    std::cout << "Selected: " << source->getName() << std::endl;
    return source;
}

int main(int argc, const char * argv[]) {
    InputDeviceRef inputDevice0 = createScreen();
    InputDeviceRef inputDevice1 = createWebcam();
    FrameSourceRef frameCombinator = createFrameCombinator({
        createFrameSizeTranformer(std::dynamic_pointer_cast<FrameSource>(inputDevice0)),
        createFrameSizeTranformer(std::dynamic_pointer_cast<FrameSource>(inputDevice1))
    });
    
    while(!inputDevice0->open()){
        if( cv::waitKey(250) == 27 ) return 0;
    }
    while(!inputDevice1->open()){
        if( cv::waitKey(250) == 27 ) return 0;
    }
    
    FrameSourceRef activeSource = frameCombinator;
    bool running = true;
    
    std::thread cli([&](){
        while(running){
            activeSource = selectFrameSource({
                createFrameSizeTranformer(std::dynamic_pointer_cast<FrameSource>(inputDevice0)),
                createFrameSizeTranformer(std::dynamic_pointer_cast<FrameSource>(inputDevice1))1,
                frameCombinator
            });
        }
    });
    
    while(true){
        cv::Mat frame = activeSource->next();
        if(frame.empty()){
            break;
        }
        cv::imshow("this is you, smile! :)", frame);
        if( cv::waitKey(10) == 27 ){
            break;
        }
    }
    running = false;
    cli.join();
    
    inputDevice0->close();
    inputDevice1->close();
    return 0;
}
