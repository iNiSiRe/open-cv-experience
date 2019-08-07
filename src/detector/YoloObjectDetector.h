//
// Created by inisire on 01.08.19.
//

#ifndef CV_YOLOOBJECTDETECTOR_H
#define CV_YOLOOBJECTDETECTOR_H


#include "DnnObjectDetector.h"

using namespace std;
using namespace cv;

class YoloObjectDetector : public DnnObjectDetector
{

using DnnObjectDetector::DnnObjectDetector;

public:
    list<Object> detect(cv::Mat image) override;

private:
    list<Object> processResult(const cv::Size& size, const std::vector<cv::Mat>& outs);

};


#endif //CV_YOLOOBJECTDETECTOR_H
