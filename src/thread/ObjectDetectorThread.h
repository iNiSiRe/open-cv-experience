//
// Created by inisire on 01.08.19.
//

#ifndef CV_OBJECTDETECTORTHREAD_H
#define CV_OBJECTDETECTORTHREAD_H

#include <thread>
#include <condition_variable>
#include <mutex>
#include <opencv2/core/mat.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include "../detector/ObjectDetector.h"

using namespace cv;
using namespace std;

class ObjectDetectorThread
{
private:
    ObjectDetector * detector;
    std::thread thread;
    bool stop = false;

    std::mutex mtx;
    std::condition_variable condition;
    
    struct Task {
        const Mat & frame;
        const vector<Rect> & boxes;
    };
    
    queue<Task> tasks;

public:
    bool running = false;

    std::list<Object> objects;

    explicit ObjectDetectorThread(ObjectDetector * detector) :
        detector(detector),
        thread()
    { }

    ~ObjectDetectorThread()
    {
        stop = true;

        if (thread.joinable())
        {
            thread.join();
        }
    }

    void feed(const Mat & frame, const vector<Rect> & list)
    {
        std::unique_lock<std::mutex> lck(mtx);
        tasks.push(Task{frame, list});
        condition.notify_all();
    }

    void run()
    {
        running = true;
        thread = std::thread(&ObjectDetectorThread::loop, this);
    }

    void loop()
    {
        while(!stop)
        {
            std::unique_lock<std::mutex> lck(mtx);

            if (tasks.empty())
            {
                condition.wait(lck);
            }

            const auto & task = tasks.front();
            const auto & frame = task.frame;

            for (auto & box: task.boxes)
            {
                if (box.area() < 2000)
                {
                    continue;
                }

                std::cout << "- 1" << std::endl;
                std::cout << box << std::endl;

                std::cout << "frame: " << task.frame.cols << " x " << task.frame.rows << std::endl;

                std::cout << "- 2" << std::endl;

                Rect rect = box;

                int dw = 500 - rect.width;
                int dh = 500 - rect.height;

                if (dw > 0 && dh > 0)
                {
                    rect.x -= dw / 2;
                    rect.width = 500;
                    rect.y -= dh / 2;
                    rect.height = 500;

                    if (rect.x < 0)
                    {
                        rect.x = 0;
                        rect.width = 500;
                    }

                    if (rect.y < 0)
                    {
                        rect.y = 0;
                        rect.height = 500;
                    }

                    if (rect.br().x > frame.cols)
                    {
                        rect.x -= rect.br().x - frame.cols;
                    }

                    if (rect.br().y > frame.rows)
                    {
                        rect.y -= rect.br().y - frame.rows;
                    }
                }

                std::cout << rect << std::endl;

                cv::imshow("task", task.frame(rect));

                std::cout << "- 3" << std::endl;

                for (auto & detected : detector->detect(task.frame(box)))
                {
                    detected.rect.x += box.x;
                    detected.rect.y += box.y;

                    objects.push_back(detected);
                }

                std::cout << "- 4" << std::endl;
            }

            tasks.pop();

//            float scale = 1280.0 / frame.cols;
//
//            cv::resize(frame, frame, Size(), scale, scale);
//
//            vector<Rect> boxes;
//            vector<float> confidences;
//            vector<Object> detections;
//
//            for (int i = 0; i < frame.rows - 700; i += 50)
//            {
//                for (int j = 0; j < frame.cols - 700; j += 50)
//                {
//                    int height = 700;
//                    int width = 700;
//
//                    Mat box = frame(cv::Rect(j, i, width, height));
//
//                    auto detected = detector->detect(box);
//
//                    for (auto & object: detected)
//                    {
//                        object.rect.x += j;
//                        object.rect.y += i;
//
//                        object.rect.x /= scale;
//                        object.rect.y /= scale;
//                        object.rect.width /= scale;
//                        object.rect.height /= scale;
//
//                        detections.push_back(object);
//                        boxes.push_back(object.rect);
//                        confidences.push_back(object.score);
//                    }
//                }
//            }
//
//            vector<int> indices;
//            cv::dnn::NMSBoxes(boxes, confidences, 0.3, 0.3, indices);
//
//            objects.clear();
//
//            for (auto id : indices)
//            {
//                objects.push_back(detections[id]);
//            }
        }
    }
};


#endif //CV_OBJECTDETECTORTHREAD_H
