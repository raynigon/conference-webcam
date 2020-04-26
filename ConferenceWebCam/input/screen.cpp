//
//  screen.cpp
//  ConferenceWebCam
//
//  Created by Simon Schneider on 4/26/20.
//  Copyright © 2020 Simon Schneider. All rights reserved.
//

#include "screen.hpp"

#include <iostream>

using namespace cwc::input;


Screen::Screen() :
queue(dispatch_queue_create("com.raynigon.conference-web-cam.screencapture", DISPATCH_QUEUE_SERIAL)),
screen_width(0),
screen_height(0),
stream_ref(nullptr)
{
    
}

Screen::~Screen() {
    
}

bool Screen::open() {
    CGDirectDisplayID mainDisplayId = CGMainDisplayID();
    auto bounds = CGDisplayBounds(mainDisplayId);
    screen_width = bounds.size.width;
    screen_height = bounds.size.height;
    
    CGDisplayStreamFrameAvailableHandler handler = ^void(CGDisplayStreamFrameStatus status,
                                                         uint64_t timestamp,
                                                         IOSurfaceRef frameSurface,
                                                         CGDisplayStreamUpdateRef updateRef) {
        /*
         IOPhysicalAddress           baseAddress;
         UInt32                      activeWidth;
         UInt32                      activeHeight;
         IOByteCount                 bytesPerRow;
         IOByteCount                 bytesPerPlane;
         UInt32                      bitsPerPixel;
         UInt32                      pixelType;
         UInt32                      flags;
         UInt32                      reserved[ 4 ];
         */
        IOSurfaceLock(frameSurface, kIOSurfaceLockReadOnly, nullptr);
        int width = int(IOSurfaceGetWidth(frameSurface));
        int height = int(IOSurfaceGetHeight(frameSurface));
        auto baseAddress = IOSurfaceGetBaseAddress(frameSurface);
        auto bufferSize = IOSurfaceGetAllocSize(frameSurface);
        std::lock_guard<std::mutex> lock(mtx_last_frame);
        this->last_frame = cv::Mat(height, width, CV_8UC4, baseAddress).clone();
        IOSurfaceUnlock(frameSurface, kIOSurfaceLockReadOnly, nullptr);
    };
    
    int32_t pixel_format = reinterpret_cast<int32_t>('BGRA');
    stream_ref = CGDisplayStreamCreateWithDispatchQueue(mainDisplayId, screen_width, screen_height, pixel_format, nullptr, queue, handler);
    CGError error = CGDisplayStreamStart(stream_ref);
    return error == kCGErrorSuccess;
}

void Screen::close() {
    CGDisplayStreamStop(stream_ref);
}

cv::Mat Screen::next() {
    static int counter = 0;
    int count = (counter++)%768;
    if (last_frame.empty()){
        int b = count < 256 ? count : (count < 512 ? 512 - count : 0);
        int g = count > 255 && count < 512 ? count - 256 : (count > 255 && count < 768 ? 768 - count : 0);
        int r = count > 511 && count < 768 ? count - 512 : (count < 256 ? 256 - count : 0);
        cv::Mat frame(screen_height, screen_width, CV_8UC4, cv::Scalar(b, g, r, 0));
        return frame;
    }
    std::lock_guard<std::mutex> lock(mtx_last_frame);
    return last_frame;
}


/*
 
 struct __block StreamFrameHandler {
 
 };
 
 StreamFrameHandler handlerBlock;
 
 CGDisplayStreamFrameAvailableHandler handler = (CGDisplayStreamFrameStatus status,
 uint64_t timestamp,
 IOSurfaceRef frameSurface,
 CGDisplayStreamUpdateRef updateRef){
 return;
 };
 
 CGDisplayStreamFrameAvailableHandler handler = [](CGDisplayStreamFrameStatus status,
 uint64_t timestamp,
 IOSurfaceRef frameSurface,
 CGDisplayStreamUpdateRef updateRef){
 return;
 };
 int32_t pixel_format = reinterpret_cast<int32_t>('BGRA');
 auto stream = CGDisplayStreamCreateWithDispatchQueue(mainDisplayId, 1920, 1080, pixel_format, nullptr, queue, handler);*/
