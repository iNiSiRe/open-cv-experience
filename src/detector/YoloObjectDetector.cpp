#include <opencv2/imgproc.hpp>
#include "YoloObjectDetector.h"

std::list<Object> YoloObjectDetector::detect(cv::Mat image)
{
    std::list<Object> list;
    std::string label;

    auto originalSize = image.size;

    if (image.channels() == 4)
    {
        cvtColor(image, image, cv::COLOR_BGRA2BGR);
    }

    Mat blob;
//    auto blob = cv::dnn::blobFromImage(image, params.scale, params.size, params.mean, params.swapRGB, params.crop);
    cv::dnn::blobFromImage(image, blob, params.scale, params.size, params.mean, params.swapRGB, false);

    net.setInput(blob);

    vector<Mat> out;
    net.forward(out, getOutputsNames(net));

    static std::vector<int> outLayers = net.getUnconnectedOutLayers();
    static std::string outLayerType = net.getLayer(outLayers[0])->type;

    return processResult(Size(originalSize.p[1], originalSize.p[0]), out);
}

std::list<Object> YoloObjectDetector::processResult(const Size& size, const vector<Mat>& outs)
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

            auto object = Object();
            object.id = classIdPoint.x;
            object.score = (float) confidence;
            object.rect = Rect(left, top, width, height);

            std::string label;
            try {
                label = labels.at(object.id);
            } catch (std::out_of_range &e) {
                label = "";
            }

            object.name = label;

            list.push_back(object);
        }
    }


    return list;
}