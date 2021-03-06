/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "EyeLike/findEyes.h"

#include "EyeLike/constants.h"
#include "EyeLikeProcessing.h"
#include "Config.h"

#include "Utilities.h"

const std::string FaceCascadeLocation = "haarcascade_frontalface_alt.xml";

float kEyeCornerKernel[4][6] = {
  {-1,-1,-1, 1, 1, 1},
  {-1,-1,-1,-1, 1, 1},
  {-1,-1,-1,-1, 0, 3},
  { 1, 1, 1, 1, 1, 1},
};

cv::CascadeClassifier EyeLikeProcessing::createClassifier()
{
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load(FaceCascadeLocation))
    {
        Config::output.log("Unable to load facial cascade - check path\n",
                           OutputWriter::LogLevel::Debug);
    }
    return faceCascade;
}

std::tuple<cv::Point, cv::Point> EyeLikeProcessing::detectPupils(const cv::Mat_<uchar>& grayFrame)
{
    cv::CascadeClassifier classifier = createClassifier();
    return detectPupils(grayFrame, classifier);
}

std::tuple<cv::Point, cv::Point> EyeLikeProcessing::detectPupils(const cv::Mat_<uchar>& grayFrame,
                                                                 cv::CascadeClassifier& classifier)
{
    std::vector<cv::Rect> faces;
    unsigned int imageWidth = grayFrame.size().width;
    
    classifier.detectMultiScale(grayFrame, faces, 1.1, 2,
                                 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, 
                                 cv::Size(imageWidth/4, imageWidth/4)); 
    
    if (faces.size() > 0)
    {
        return findEyes(grayFrame, faces[0]);
    }
    
    return std::make_tuple(cv::Point(0,0), cv::Point(0,0));
}

std::tuple<cv::Point, cv::Point> EyeLikeProcessing::detectPupils(const cv::Mat_<uchar>& grayFrame,
                                                                 const cv::Rect& face,
                                                                 const cv::Rect& leftEyeRect,
                                                                 const cv::Rect& rightEyeRect)
{
    return findEyes(grayFrame, face, leftEyeRect, rightEyeRect);
}

void EyeLikeProcessing::applyEyeCentersToImage(cv::Mat& outputImage, 
                                               const std::tuple<cv::Point,cv::Point>& pupilLocations,
                                               double scaleFactor)
{
    const cv::Scalar PUPIL_COLOR(255, 255, 255);
    
    cv::Point left;
    cv::Point right;
    std::tie(left, right) = pupilLocations;
    cv::circle(outputImage, cv::Point(left.x * scaleFactor, left.y * scaleFactor), 3, PUPIL_COLOR);
    cv::circle(outputImage, cv::Point(right.x * scaleFactor, right.y * scaleFactor), 3, PUPIL_COLOR);
}