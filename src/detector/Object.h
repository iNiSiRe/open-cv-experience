//
// Created by inisire on 28.06.19.
//

#ifndef CV_OBJECT_H
#define CV_OBJECT_H


#include <string>
#include <opencv2/core/types.hpp>

class Object {

public:
    int id;
    float score;
    std::string name;
    cv::Rect rect;
};


#endif //CV_OBJECT_H
