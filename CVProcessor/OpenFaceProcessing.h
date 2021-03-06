/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OpenFaceProcessing.h
 * Author: dean
 *
 * Created on February 23, 2018, 11:43 PM
 */

#ifndef OPENFACEPROCESSING_H
#define OPENFACEPROCESSING_H

#include <string>
#include <opencv2/core/core.hpp>
#include <OpenFace/LandmarkDetectorModel.h>

#include "VideoMetadata.h"

namespace OpenFaceProcessing
{

struct FaceDataPointsRecord
{
public:
    cv::Mat_<double> landmarks;
    cv::Mat_<int> visibilities;
    
    FaceDataPointsRecord();
    FaceDataPointsRecord(const cv::Mat_<double>& landmarks, 
                         const cv::Mat_<int>& visibilities);
};

cv::Mat openImage(const std::string& inputPath);
void saveImage(const std::string& path, const cv::Mat& image);

// Extracts the facial data points from a single image
FaceDataPointsRecord extractFaceDataPoints(const cv::Mat_<uchar> grayImage,
                                           const VideoMetadata& metadata);

// Extracts the facial data points from a frame of a video
// using the provided CLNF model that is tracking each frame
FaceDataPointsRecord extractFaceDataPoints(const cv::Mat_<uchar> grayFrame,
                                           const VideoMetadata& metadata,
                                           LandmarkDetector::CLNF& model);

// Extracts the head pose data of the first face detected
// by the given model
cv::Vec6f extractHeadPose(const LandmarkDetector::CLNF& model, 
                          const VideoMetadata& metadata);

// Gets the delaunay trianlges constructed from the given dataPoints.
// Discards triangles not entirely contained within a frame of the video.
std::vector<cv::Vec6f> getDelaunayTriangles(const FaceDataPointsRecord& dataPoints,
                          const VideoMetadata& metadata);

// Draws the given face data points on the target image
void applyFaceDataPointsToImage(cv::Mat& outputImage,
                                const FaceDataPointsRecord& dataPoints,
                                const VideoMetadata& metadata,
                                double scaleFactor = 1.0);

// Draws the given triangles on the target image
void applyDelaunayTrianlgesToImage(cv::Mat& outputImage,
                                   const std::vector<cv::Vec6f> triangles,
                                   const VideoMetadata& metadata,
                                   double scaleFactor = 1.0);

}

#endif /* OPENFACEPROCESSING_H */

