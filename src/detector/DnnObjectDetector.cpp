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

std::list<Object> DnnObjectDetector::detect(cv::Mat image)
{
    std::list<Object> list;
    std::string label;

    auto originalSize = image.size;

//    auto blob = cv::dnn::blobFromImage(image, params.scale, params.size, params.mean, params.swapRGB, params.crop);
//    net.setInput(blob);

    Mat blob;
    cv::dnn::blobFromImage(image, blob, 1.0, params.size, Scalar(), params.swapRGB, false, CV_8U);
    net.setInput(blob, "", params.scale, params.mean);

    //    auto result = net.forward();

    vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));

    static std::vector<int> outLayers = net.getUnconnectedOutLayers();
    static std::string outLayerType = net.getLayer(outLayers[0])->type;

    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    // Network produces output blob with a shape 1x1xNx7 where N is a number of
    // detections and an every detection is a vector of values
    // [batchId, classId, confidence, left, top, right, bottom]
    CV_Assert(outs.size() > 0);

    for (size_t k = 0; k < outs.size(); k++)
    {
        float* data = (float*) outs[k].data;

        for (size_t i = 0; i < outs[k].total(); i += 7)
        {
            int id = (int) (data[i + 1]);
            float confidence = data[i + 2];

            if (confidence > 0.0)
            {
                int left   = (int)data[i + 3];
                int top    = (int)data[i + 4];
                int right  = (int)data[i + 5];
                int bottom = (int)data[i + 6];
                int width  = right - left + 1;
                int height = bottom - top + 1;

                if (width * height <= 1)
                {
                    left   = (int)(data[i + 3] * image.cols);
                    top    = (int)(data[i + 4] * image.rows);
                    right  = (int)(data[i + 5] * image.cols);
                    bottom = (int)(data[i + 6] * image.rows);
                    width  = right - left + 1;
                    height = bottom - top + 1;
                }

                classIds.push_back(id);
                boxes.push_back(Rect(left, top, width, height));
                confidences.push_back(confidence);
            }
        }
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.0, 0.0, indices);

    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];

        try {
            label = labels.at(classIds[idx]);
        } catch (std::out_of_range &e) {
            label = "";
        }

        auto object = Object();
        object.id = classIds[idx];
        object.score = confidences[idx];
        object.rect = boxes[idx];
        object.name = label;

        list.push_back(object);
    }

    return list;

//    for (int i = 0; i < result.size.p[2]; i++)
//    {
//        auto id = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 1}) );
//        auto confidence = result.at<float>(cv::Vec<int, 4> {0, 0, i, 2});
//
//        auto x0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 3}) * originalSize.p[1] );
//        auto y0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 4}) * originalSize.p[0] );
//        auto x1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 5}) * originalSize.p[1] );
//        auto y1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 6}) * originalSize.p[0] );
//
////        auto x0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 3}) );
////        auto y0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 4}) );
////        auto x1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 5}) );
////        auto y1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 6}) );
//
//        try {
//            label = labels.at(id);
//        } catch (std::out_of_range &e) {
//            label = "";
//        }
//
//        Object object;
//        object.id = id;
//        object.name = label;
//        object.rect = cv::Rect(x0, y0, x1 - x0, y1 - y0);
//        object.score = confidence;
//
//        list.push_back(object);
//    }
//
//    return list;
}

std::vector<std::string> DnnObjectDetector::getOutputsNames(const cv::dnn::Net & net)
{
    static vector<string> names;
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
