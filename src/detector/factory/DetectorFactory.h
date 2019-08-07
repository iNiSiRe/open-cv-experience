//
// Created by inisire on 29.06.19.
//

#ifndef CV_DETECTORFACTORY_H
#define CV_DETECTORFACTORY_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "../ObjectDetector.h"
#include "../DnnObjectDetector.h"
#include "../YoloObjectDetector.h"

class DetectorFactory {

public:

    enum Type {
        MOBILENET_SSD_1 = 0, // Best for now
        ALEXNET_1,
        MOBILENET_SSD_2,
        MASK_RCNN,
        FACE,
        HOG,
        YOLO_V3,
        YOLO_TINY_V3,
        COCO_SSD_300
    };

    static ObjectDetector* create(Type type) {

        switch (type) {

            case MOBILENET_SSD_1: {

                auto net = cv::dnn::readNetFromCaffe(
                        "resources/models/mobilenet_ssd_1/MobileNetSSD_deploy.prototxt",
                        "resources/models/mobilenet_ssd_1/MobileNetSSD_deploy.caffemodel"
                );

                DnnObjectDetector::Params params {
                    "resources/models/mobilenet_ssd_1/classes.txt",
                    1.0 / 127.5,
                    cv::Size(300, 300),
                    cv::Scalar(127.5, 127.5, 127.5),
                    false,
                    false,
                };

                return new DnnObjectDetector(net, params);

            } break;

            case ALEXNET_1: {

                auto net = cv::dnn::readNetFromCaffe(
                        "resources/models/alexnet/deploy.prototxt",
                        "resources/models/alexnet/model.caffemodel"
                );

                DnnObjectDetector::Params params {
                        "resources/models/alexnet/imagenet1000_clsidx_to_labels.txt",
                        0,
                        cv::Size(300,300),
                        cv::Scalar(127.5, 127.5, 127.5),
                        false
                };

                return new DnnObjectDetector(net, params);

            } break;

            case MOBILENET_SSD_2: {

                auto net = cv::dnn::readNetFromTensorflow(
                        "resources/models/ssdlite_mobilenet_v2_coco/frozen_inference_graph.pb",
                        "resources/models/ssdlite_mobilenet_v2_coco/ssdlite_mobilenet_v2_coco.pbtxt"
                );

                DnnObjectDetector::Params params {
                        "resources/models/ssdlite_mobilenet_v2_coco/classes.txt",
                        0.007843,
                        cv::Size(300,300),
                        cv::Scalar(127.5, 127.5, 127.5),
                        false
                };

                return new DnnObjectDetector(net, params);

            } break;

            case MASK_RCNN: {

                auto net = cv::dnn::readNetFromTensorflow(
                        "resources/models/mask_rcnn_inception_v2_coco_2018_01_28/frozen_inference_graph.pb",
                        "resources/models/mask_rcnn_inception_v2_coco_2018_01_28/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt"
                );

                DnnObjectDetector::Params params {
                        "resources/models/mobilenet_ssd_1/classes.txt",
                        0.007843,
                        cv::Size(300,300),
                        cv::Scalar(127.5, 127.5, 127.5),
                        false
                };

                return new DnnObjectDetector(net, params);

            } break;

            case FACE: {

                auto net = cv::dnn::readNetFromCaffe(
                        "resources/models/face/ssd/res10_300x300_ssd_deploy.prototxt",
                        "resources/models/face/ssd/res10_300x300_ssd_iter_140000.caffemodel"
                );

                DnnObjectDetector::Params params {
                        "resources/models/mobilenet_ssd_1/classes.txt",
                        0.007843,
                        cv::Size(300,300),
                        cv::Scalar(127.5, 127.5, 127.5),
                        false
                };

                return new DnnObjectDetector(net, params);

            } break;

            case HOG: {

                cv::HOGDescriptor hog( cv::Size(64, 128),
                                       cv::Size(16, 16),
                                       cv::Size(8, 8),
                                       cv::Size(8, 8),
                                       9,
                                       1,
                                       -1,
                                       cv::HOGDescriptor::L2Hys,
                                       0.2,
                                       true,
                                       cv::HOGDescriptor::DEFAULT_NLEVELS);

                hog.setSVMDetector( cv::HOGDescriptor::getDefaultPeopleDetector() );

            } break;

            case YOLO_V3: {

                auto net = cv::dnn::readNetFromDarknet(
                        "resources/models/yolo/yolov3.cfg",
                        "resources/models/yolo/yolov3.weights"
                );

                DnnObjectDetector::Params params {
                        "resources/models/yolo/classes.txt",
                        1 / 255.0,
                        cv::Size(416,416),
                        cv::Scalar(0, 0, 0),
                        true,
                        false
                };

                return new YoloObjectDetector(net, params);
            } break;

            case YOLO_TINY_V3: {

                auto net = cv::dnn::readNetFromDarknet(
                        "resources/models/yolo/yolov3-tiny.cfg",
                        "resources/models/yolo/yolov3-tiny.weights"
                );

                DnnObjectDetector::Params params {
                        "resources/models/yolo/classes.txt",
                        1 / 255.0,
                        cv::Size(416,416),
                        cv::Scalar(),
                        true,
                        false
                };

                return new YoloObjectDetector(net, params);
            } break;

            case COCO_SSD_300: {

                auto net = cv::dnn::readNetFromCaffe(
                        "resources/models/coco/SSD_300x300/deploy.prototxt",
                        "resources/models/coco/SSD_300x300/model.caffemodel"
                );

                DnnObjectDetector::Params params {
                        "resources/models/mobilenet_ssd_1/classes.txt",
                        1.0 / 127.5,
                        cv::Size(300, 300),
                        cv::Scalar(127.5, 127.5, 127.5),
                        false,
                        false,
                };

                return new DnnObjectDetector(net, params);

            } break;
        }

    }

};


#endif //CV_DETECTORFACTORY_H
