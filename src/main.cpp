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
#include <thread>
#include "detector/factory/DetectorFactory.h"
#include "detector/ChangeDetector.h"

int main(int, char**)
{
    long frames = 0;

    cv::VideoCapture vcap;
    cv::VideoWriter video;

    cv::Mat source, window, image, result, previous;

//    const char *stream = "video.mkv";
    const char *stream = "rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream";

    if(!vcap.open(stream))
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    auto detector = DetectorFactory::create(DetectorFactory::Type::MOBILENET_SSD_1);

    auto changesDetector = ChangeDetector();

    auto changes_start = std::chrono::high_resolution_clock::now();
    auto objects_start = std::chrono::high_resolution_clock::now() - std::chrono::seconds(30);

    int remain = 0;
    bool person = false;
    bool write = false;
    int videos = 0;

    vector<Rect> changes;
    std::list<Object> objects;

    vcap.read(image);
    changesDetector.setup(image);

    for(;;)
    {
        if(!vcap.read(image))
        {
            std::cout << "No frame" << std::endl;
            
            vcap.release();
            
            if(!vcap.open(stream)) {
                std::cout << "Error opening video stream or file" << std::endl;
                return -1;
            }

            if(cv::waitKey(1) >= 0) {
                break;
            } else {
                continue;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto changes_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - changes_start).count();
        auto objects_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - objects_start).count();

        if (changes_diff > 500)
        {
            changes_start = now;
            changes = changesDetector.detect(image);

            cout << changes.size() << endl;
        }

        if (objects_diff > 300000)
        {
            objects_start = now;

            vector<Rect> boxes;
            vector<float> confidences;
            vector<Object> detections;

//            int size = std::min(image.rows, image.cols);
//
//            Mat box1 = image(cv::Rect(0, 0, size, size));
//
//            for (auto & object : detector->detect(box1))
//            {
//                objects.push_back(object);
//            }
//
//            Mat box2 = image(cv::Rect(image.cols - size, image.rows - size, size, size));
//
//            for (auto & object : detector->detect(box2))
//            {
//                object.rect.x += image.cols - size;
//                object.rect.y += image.rows - size;
//                objects.push_back(object);
//            }

            for (int i = 0; i < image.rows - 700; i += 50)
            {
                for (int j = 0; j < image.cols - 700; j += 50)
                {
                    int height = 700;
                    int width = 700;

                    Mat box = image(cv::Rect(j, i, width, height));

                    auto detected = detector->detect(box);

                    for (auto & object: detected)
                    {
                        object.rect.x += j;
                        object.rect.y += i;

                        detections.push_back(object);

                        boxes.push_back(object.rect);
                        confidences.push_back(object.score);
                    }
                }
            }

            vector<int> indices;
            cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.1, indices);

            objects.clear();

            for (auto id : indices)
            {
                objects.push_back(detections[id]);
            }

//            person = false;
//
//            for (auto & object : list)
//            {
//                if (object.score < 0.5) {
//                    continue;
//                }
//
//                if (object.id == 0) {
//                    person = true;
//                }
//
//                std::string label = object.name + " (" + std::to_string(object.score) + ")";
//                cv::rectangle(image, object.rect, cv::Scalar(0, 0, 255), 1);
//                cv::putText(image, label, cv::Point(object.rect.x, object.rect.y + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 255, 255));
//            }
//
//            if (!write && person)
//            {
//                write = true;
//                remain = 10;
//                videos++;
//                video = cv::VideoWriter("out_" + std::to_string(videos) + ".avi", cv::VideoWriter::fourcc('M','J','P','G'), 1, cv::Size(1280, 720));
//            }
//
//            if (write && !person) {
//                remain--;
//
//                if (remain == 0) {
//                    write = false;
//                    video.release();
//                }
//            }
//
//            if (write) {
//                video.write(image);
//            }
        }

        window = image.clone();

        for (const auto & rect : changes)
        {
            if (rect.area() < 2000)
            {
                cv::rectangle(window, rect, Scalar(255, 0, 0), 2);
            }
            else
            {
                cv::rectangle(window, rect, Scalar(0, 0, 255), 2);
            }
        }

        for (const auto & object : objects)
        {
            if (object.score < 0.8)
            {
                continue;
            }

            std::string label = object.name + " (" + std::to_string(object.score) + ")";
            cv::rectangle(window, object.rect, cv::Scalar(0, 0, 255), 1);
            cv::putText(window, label, cv::Point(object.rect.x, object.rect.y + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 255, 255));
        }

        cv::resize(window, result, cv::Size(1280, 720));
        cv::imshow("Output", result);

        if (stream == "video.mkv")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        if(cv::waitKey(1) >= 0) {
            break;
        }
    }

}