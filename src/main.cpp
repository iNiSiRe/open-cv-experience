#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include "detector/factory/DetectorFactory.h"
#include "detector/ChangeDetector.h"
#include "thread/ObjectDetectorThread.h"
#include "recorder/VideoRecorder.h"

int main(int, char**)
{
    long frames = 0;

    cv::VideoCapture vcap;
    cv::VideoWriter video;

    cv::Mat source, window, image, result, previous;

    cv::Mat & frame = image;

//    const char *stream = "video.mkv";
//    const char *stream = "run.mp4";
//    const char *stream = "motor_bike.mp4";
    const char *stream = "rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream";

    if(!vcap.open(stream))
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    auto detector = new ObjectDetectorThread(DetectorFactory::create(DetectorFactory::Type::YOLO_V3));

    auto changesDetector = ChangeDetector();

    auto changes_start = std::chrono::high_resolution_clock::now();
    auto objects_start = std::chrono::high_resolution_clock::now() - std::chrono::seconds(30);

    int motions_in_row = 0;
    vector<Mat> motion_frames;

    vector<Rect> changes;
    std::list<Object> objects;

    vcap.read(image);
    changesDetector.setup(image);

    auto recorder = new VideoRecorder(image);

    auto thread = std::thread([detector, recorder] () {

        std::this_thread::sleep_for( std::chrono::seconds(10) );
        // detector->run();
        recorder->start();
        std::cout << "Detector runned!" << std::endl;

    });

    bool recording = false;
    int remain = 0;
    bool person = false;
    bool write = false;
    int videos = 0;
    bool big_motion = false;

    for(;;)
    {
        if(!vcap.read(image))
        {
            std::cout << "No frame" << std::endl;
            
            vcap.release();
            
            if(!vcap.open(stream)) 
            {
                std::cout << "Error opening video stream or file" << std::endl;
                return -1;
            }

            if(cv::waitKey(1) == 27) 
            {
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

            if (detector->running)
            {
                std::cout << "task frame: " << image.cols << " x " << image.rows << std::endl;
                detector->feed(image, changes);
            }

            big_motion = false;

            for (const auto & rect : changes)
            {
                if (rect.area() >= 2000)
                {
                    big_motion = true;
                }
            }

            if (big_motion)
            {
                motions_in_row++;
                cout << "motions in row: " << motions_in_row << endl;
            }
            else
            {
                motions_in_row = 0;
                motion_frames.clear();
            }

            if (motions_in_row >= 3 && !recording)
            {
//                recording = true;
//                videos++;
//                video = cv::VideoWriter("out_" + std::to_string(videos) + ".avi", cv::VideoWriter::fourcc('M','J','P','G'), 15, cv::Size(1920, 1080));
//
//                for (const auto & motion_frame : motion_frames)
//                {
//                    video.write(motion_frame);
//                }
//
//                motion_frames.clear();
            }

            if (recording && motions_in_row < 3)
            {
                recording = false;
                video.release();
            }

            cout << changes.size() << endl;
        }

        if (objects_diff > 15000)
        {
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

        if (big_motion && !recording)
        {
            motion_frames.push_back(image.clone());
        }

        if (recording)
        {
            video.write(image);
        }

        for (const auto & object : detector->objects)
        {
            if (object.score < 0.4)
            {
                continue;
            }

            std::string label = object.name + " (" + std::to_string(object.score) + ")";
            cv::rectangle(window, object.rect, cv::Scalar(0, 0, 255), 1);
            cv::putText(window, label, cv::Point(object.rect.x, object.rect.y + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 255, 255));
        }

        cv::resize(window, window, cv::Size(1280, 720));

        cv::imshow("Output", window);

        if (stream == "video.mkv" || stream == "run.mp4")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        if(cv::waitKey(1) == 27) {
            break;
        }
    }

}