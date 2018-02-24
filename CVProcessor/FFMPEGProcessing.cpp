/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <sstream>
#include "SystemHelper.h"
#include "FFMPEGProcessing.h"

VideoMetadata FFMPEGProcessing::extractMetadata(const std::string& videoFile)
{
    const char* format =
        "ffprobe -v error -count_frames -select_streams v:0 "
        "-show_entries stream=avg_frame_rate,nb_read_frames,width,height "
        "-of default=noprint_wrappers=1:nokey=1 %s";
    // TODO: Validate that we wont overflow buffer
    char buffer[256];
    sprintf(buffer, format, videoFile.c_str());
    
    std::istringstream result;
    result.str(exec(std::string(buffer)));
    
    std::string widthStr, heightStr, avgFramesStr, frameCountStr;
    std::getline(result, widthStr);    
    std::getline(result, heightStr);
    std::getline(result, avgFramesStr);
    std::getline(result, frameCountStr);

    int width = std::stoi(widthStr);
    int height = std::stoi(heightStr);

    int sep = avgFramesStr.find('/');
    int num = std::stoi(avgFramesStr.substr(0,sep));
    int denom = std::stoi(avgFramesStr.substr(sep + 1));

    int frameCount = std::stoi(frameCountStr);
    
    return VideoMetadata(width, height, frameCount, num, denom);
}

void FFMPEGProcessing::extractFrames(const std::string& targetVideo, 
                 const std::string& outputFolder, 
                 const VideoMetadata& metadata)
{
    // TODO: create outputFolder if needed
    // TODO: Validate that we wont overflow buffer
    const char* format = "ffmpeg -i %s -vf fps=%d/%d %s/out%s.png 2>&1";
    char buffer[256];
    sprintf(buffer, format, targetVideo.c_str(), metadata.frameRateNum, 
            metadata.frameRateDenom, outputFolder.c_str(), "%d");
    exec(std::string(buffer));
}
