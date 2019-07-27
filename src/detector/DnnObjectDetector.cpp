#include <utility>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <iostream>

//
// Created by inisire on 28.06.19.
//

#include "DnnObjectDetector.h"

using namespace std;
using namespace cv;

DnnObjectDetector::DnnObjectDetector(cv::dnn::Net &net, DnnObjectDetector::Params &params) : ObjectDetector() {

    this->net = net;
    this->params = params;

    std::ifstream input(params.labels.c_str());

    if(!input){
        std::cerr << "Error in opening file: " << params.labels << "\n";
        throw std::exception();
    }

    std::string line;

    while(std::getline(input, line)){

        labels.push_back(line);
    }
}

vector<String> getOutputsNames(const cv::dnn::Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        vector<int> outLayers = net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        vector<String> layersNames = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

std::list<Object> DnnObjectDetector::detect(cv::Mat image) {

    std::list<Object> list;
    std::string label;

    auto originalSize = image.size;

    auto blob = cv::dnn::blobFromImage(image, params.scale, params.size, params.mean, params.swapRGB, params.crop);

    net.setInput(blob);

//    vector<Mat> out;
//    net.forward(out, getOutputsNames(net));

    auto result = net.forward();

    for (int i = 0; i < result.size.p[2]; i++)
    {
        auto id = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 1}) );
        auto confidence = result.at<float>(cv::Vec<int, 4> {0, 0, i, 2});

        auto x0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 3}) * originalSize.p[1] );
        auto y0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 4}) * originalSize.p[0] );
        auto x1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 5}) * originalSize.p[1] );
        auto y1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 6}) * originalSize.p[0] );

//        auto x0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 3}) );
//        auto y0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 4}) );
//        auto x1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 5}) );
//        auto y1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 6}) );

        try {
            label = labels.at(id);
        } catch (std::out_of_range &e) {
            label = "";
        }

        Object object;
        object.id = id;
        object.name = label;
        object.rect = cv::Rect(x0, y0, x1 - x0, y1 - y0);
        object.score = confidence;

        list.push_back(object);
    }

    return list;

//    return processResult(Size(originalSize.p[1], originalSize.p[0]), out);
}

std::list<Object> DnnObjectDetector::processResult(const Size& size, const vector<Mat>& outs)
{
    list<Object> list;

    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.

        float* data = (float*)outs[i].data;

        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;

            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            int centerX = (int)(data[0] * size.width);
            int centerY = (int)(data[1] * size.height);
            int width = (int)(data[2] * size.width);
            int height = (int)(data[3] * size.height);
            int left = centerX - width / 2;
            int top = centerY - height / 2;

            std::string label;
            try {
                label = labels.at(classIdPoint.x);
            } catch (std::out_of_range &e) {
                label = "";
            }

            auto object = Object();
            object.id = classIdPoint.x;
            object.score = (float) confidence;
            object.rect = Rect(left, top, width, height);
            object.name = label;

            list.push_back(object);
        }
    }

    vector<int> indices;
//    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

    return list;
}
