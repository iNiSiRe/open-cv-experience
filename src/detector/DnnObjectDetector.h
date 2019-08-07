//
// Created by inisire on 28.06.19.
//

#ifndef CV_DNNOBJECTDETECTOR_H
#define CV_DNNOBJECTDETECTOR_H


#include <opencv2/dnn.hpp>
#include "ObjectDetector.h"

class DnnObjectDetector: public ObjectDetector {

public:
    struct Params {
        std::string labels;
        float scale = 0.0;
        cv::Size size;
        cv::Scalar mean;
        bool swapRGB = false;
        bool crop = false;
    };

protected:
    cv::dnn::Net net;
    Params params;
    std::vector<std::string> labels;

    std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);

public:

    DnnObjectDetector(cv::dnn::Net &net, Params &params);

    std::list<Object> detect(cv::Mat image) override;

};


#endif //CV_DNNOBJECTDETECTOR_H
