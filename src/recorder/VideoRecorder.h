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

class VideoRecorder {

private:
    const cv::Mat & frame;
    int count = 0;
    cv::VideoWriter writer;
    bool running = false;
    std::thread thread;
    int started_at;

public:
    explicit VideoRecorder(const cv::Mat & frame) : frame(frame), thread() {}

    void loop()
    {
        while(running)
        {
            std::time_t result = std::time(nullptr);
            auto tm = std::localtime(&result);

            if (!writer.isOpened())
            {
                continue;
            }

            if (tm->tm_min > started_at)
            {
                writer.release();
            
                writer.open(
                    "out_" + std::to_string(tm->tm_min) + ".avi",
                    cv::VideoWriter::fourcc('H','2','6','4'),
                    5,
                    cv::Size(1920, 1080)
                );

                started_at = tm->tm_min;
            }

            writer.write(frame);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    void start()
    {
        std::time_t result = std::time(nullptr);
        auto tm = std::localtime(&result);
        started_at = tm->tm_min;

        running = true;
        count++;

        writer.open(
                "out_" + std::to_string(tm->tm_min) + ".avi",
                cv::VideoWriter::fourcc('H','2','6','4'),
                // cv::VideoWriter::fourcc('M','J','P','G'),
                5,
                cv::Size(1920, 1080)
        );

        thread = std::thread(&VideoRecorder::loop, this);
    };

    void stop()
    {
        running = false;
        writer.release();
    }

    bool isRunning() const {
        return running;
    }
};


#endif //CV_VIDEORECORDER_H
