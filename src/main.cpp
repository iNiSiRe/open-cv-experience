//#include <iostream>
//#include <sstream>

//#include <openssl/md5.h>
//
//string build_digest(string user, string pwd, string realm, string method,  string uri, string nonce)
//{
//    ostringstream s1, s2;
//
//    s1 << user << ":" << realm << ":" << pwd;
//    s2 << method << ":" << uri;
//
//    MD5_CTX md5handler;
//    unsigned char md5digest[MD5_DIGEST_LENGTH];
//
//    MD5(in, 10, md5digest);
////    h2 = md5("$method:$uri");
////
////    return md5("$h1:$nonce:$h2");
//
//    return "";
//}

//int main(int argc, char** argv) {
//
////    cv::VideoCapture * stream = new cv::VideoCapture("rtsp://192.168.31.197:554/onvif1");
//
//    TCPClient c;
//    const string host = "192.168.31.197";
//
//    c.conn(host , 554);
//
//    //send some data
//    ostringstream data;
//
//    cout << "----------------------------\n\n";
//
//    data.clear();
//    data << "OPTIONS 192.168.31.197:554 RTSP/1.0\r\n";
//    data << "CSeq: 1" << "\r\n";
//    data << "User-Agent: custom" << "\r\n";
//    data << "\r\n";
//
//    c.send_data(data.str());
//    cout << c.receive(1024);
//
//    cout << "\n\n----------------------------\n\n";
//
//    cout << "----------------------------\n\n";
//
//    data.str(std::string());
//    data << "DESCRIBE 192.168.31.197:554 RTSP/1.0\r\n";
//    data << "CSeq: 2" << "\r\n";
//    data << "User-Agent: custom" << "\r\n";
//    data << "Accept: application/sdp" << "\r\n";
//    data << "\r\n";
//
//    c.send_data(data.str());
//    cout << c.receive(1024);
//
//    cout << "\n\n----------------------------\n\n";
//
//    return 0;
//
//}

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

int main(int, char**)
{
//    auto net = cv::dnn::readNetFromTensorflow(
//            "resources/models/ssdlite_mobilenet_v2_coco/frozen_inference_graph.pb",
//            "resources/models/ssdlite_mobilenet_v2_coco/ssdlite_mobilenet_v2_coco.pbtxt"
//    );

        // Good results
    auto net = cv::dnn::readNetFromCaffe(
            "resources/models/mobilenet_ssd_1/MobileNetSSD_deploy.prototxt",
            "resources/models/mobilenet_ssd_1/MobileNetSSD_deploy.caffemodel"
    );

//    auto net = cv::dnn::readNetFromTensorflow(
//            "resources/models/mask_rcnn_inception_v2_coco_2018_01_28/frozen_inference_graph.pb",
//            "resources/models/mask_rcnn_inception_v2_coco_2018_01_28/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt"
//    );


//    auto net = cv::dnn::readNetFromCaffe(
//            "resources/models/face/ssd/res10_300x300_ssd_deploy.prototxt",
//            "resources/models/face/ssd/res10_300x300_ssd_iter_140000.caffemodel"
//    );

//    std::string filename = "resources/models/ssdlite_mobilenet_v2_coco/classes.txt";

    std::string filename = "resources/models/mobilenet_ssd_1/classes.txt";

    std::ifstream input(filename.c_str());

    if(!input){
        std::cerr << "Error in opening file: " << filename << "\n";
        return 1;
    }
    std::vector<std::string> words;
    std::string line;

    while(std::getline(input, line)){

        words.push_back(line);
    }

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

    long frames = 0;

    cv::VideoCapture vcap;
    cv::Mat image;
    cv::Mat result;

    //open the video stream and make sure it's opened
//    if(!vcap.open("rtsp://admin:pwd@127.0.0.1:8000/onvif1"))
    if(!vcap.open("video.mkv"))
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }


    for(;;)
    {
        if(!vcap.read(image)) {
            std::cout << "No frame" << std::endl;

            if(cv::waitKey(1) >= 0) {
                break;
            } else {
                continue;
            }
        }

//        if (frames % 25 == 0) {

            cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
            cv::resize(image, image, cv::Size(), 0.8, 0.8);

            std::vector<cv::Rect> rects;
            std::vector<double> weights;

            hog.detectMultiScale(image, rects, weights);

            cv::rectangle(image, cv::Point(10, 10), cv::Point(10 + 64, 10 + 128), cv::Scalar(0, 0, 255), 2);

            for (int i = 0; i < rects.size(); i++) {

//                if (weights[i] > 0.5) {
                    auto rect = rects[i];
                    cv::rectangle(image, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(0, 0, 255), 2);
                    std::string label = std::to_string(weights[i]);
                    cv::putText(image, label, cv::Point(rect.x, rect.y + 30), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 2);
//                }

            }

//            auto blob = cv::dnn::blobFromImage(image, 0.017, cv::Size(300,300), cv::Scalar(127.5, 127.5, 127.5), true, false);

            // For mobilenet_ssd_1 (good results)
//            auto blob = cv::dnn::blobFromImage(image, 0.007843, cv::Size(300,300), cv::Scalar(127.5, 127.5, 127.5), false);

//            auto blob = cv::dnn::blobFromImage(image, 0.007843, cv::Size(300,300), cv::Scalar(127.5, 127.5, 127.5), false, false);

//            auto blob = cv::dnn::blobFromImage(image, 1, cv::Size(300,300), cv::Scalar(104, 177, 123), false, false);

//            net.setInput(blob);
//            result = net.forward();

//        for (int i = 0; i < result.size.p[2]; i++)
//        {
//            auto id = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 1}) );
//            auto confidence = result.at<float>(cv::Vec<int, 4> {0, 0, i, 2});
//
//            auto x0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 3}) * 1280 );
//            auto y0 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 4}) * 720 );
//            auto x1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 5}) * 1280 );
//            auto y1 = int ( result.at<float>(cv::Vec<int, 4> {0, 0, i, 6}) * 720 );
//
//            if (confidence > 0.3) {
//                cv::rectangle(image, cv::Point(x0, y0), cv::Point(x1, y1), cv::Scalar(0, 0, 255), 2);
//
//                try {
//
//                    std::string label = words.at(id);
//                    label.append("(" + std::to_string(confidence) + ")");
//
//                    cv::putText(image, label, cv::Point(x0, y0 + 40), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255), 2);
//
//                } catch (std::out_of_range &e) {
//
//                }
//            }
//        }

        frames++;

        cv::imshow("Output Window", image);

        if(cv::waitKey(1) >= 0) {
            break;
        }

    }

}