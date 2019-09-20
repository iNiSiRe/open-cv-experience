//
// Created by inisire on 8/9/19.
//

#ifndef CV_VIDEORECORDER_H
#define CV_VIDEORECORDER_H

#include <thread>
#include <condition_variable>
#include <mutex>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <ctime>
#include <queue>
#include <iostream>

using namespace std;
using namespace cv;

class VideoRecorder {

private:
    const cv::Mat & screen;
    int count = 0;
    bool running = false;
    std::thread thread;
    int started_at = 0;

    std::queue<cv::Mat> queue;

    std::mutex mtx;
    std::condition_variable cv;

public:
    explicit VideoRecorder(const cv::Mat & screen) : screen(screen), thread() {}
    
    void loop()
    {
        cout << "Start recorder" << endl;

        std::thread local_thread;
        
        while(running)
        {
            cout << "Start minute thread" << endl;

            local_thread = std::thread([this]
            {
                std::time_t result = std::time(nullptr);
                auto tm = std::localtime(&result);

                int minutes = tm->tm_min;
                int seconds = tm->tm_sec;

                cv::VideoWriter writer;

                const std::string &filename = "out_" + std::to_string(minutes) + ".mkv";

                cout << "Start video " << filename << endl;

                writer.open(
                        filename,
                        cv::VideoWriter::fourcc('H','2','6','4'),
                        // cv::VideoWriter::fourcc('M','J','P','G'),
                        5,
                        cv::Size(1920, 1080)
                );

                int count = seconds * 5;

                while (count < 300)
                {
                    unique_lock<mutex> lock(mtx);

                    while (queue.empty())
                    {
                        cout << "Wait for frame" << endl;
                        cv.wait(lock);
                    }

                    cout << "Write frame" << endl;

                    writer.write(queue.front());
                    queue.pop();

                    lock.unlock();

                    count++;
                }

                cout << "Release video " << filename << endl;

                writer.release();
            });

            local_thread.join();
//            std::this_thread::sleep_for(std::chrono::seconds(60 - seconds));
        }
    }

    void start()
    {
        running = true;
        thread = std::thread(&VideoRecorder::loop, this);

        auto producer = std::thread([this]
        {
            while (running)
            {
                cout << "Write frame to queue" << endl;

                unique_lock<mutex> lock(mtx);
                queue.push(screen.clone());
                cv.notify_one();
                lock.unlock();

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });

        producer.detach();
    };

    void stop()
    {
        running = false;
    }

    bool isRunning() const {
        return running;
    }
};


#endif //CV_VIDEORECORDER_H
