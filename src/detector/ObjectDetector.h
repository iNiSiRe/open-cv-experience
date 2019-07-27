//
// Created by inisire on 28.06.19.
//

#ifndef CV_OBJECTDETECTOR_H
#define CV_OBJECTDETECTOR_H


#include <list>
#include <opencv2/core/mat.hpp>
#include "Object.h"

class ObjectDetector {

public:

    virtual std::list<Object> detect(cv::Mat image) = 0;

};


#endif //CV_OBJECTDETECTOR_H
