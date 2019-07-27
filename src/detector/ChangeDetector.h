#ifndef CV_CHANGEDETECTOR_H
#define CV_CHANGEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <random>

using namespace cv;
using namespace std;

class Shape
{
public:
    vector<Point> points;

    float calculate_distance(const Point & point1, const Point & point2)
    {
        return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
    }

    Point top_left() const
    {
        Point point = points.front();

        for (const auto & i : points)
        {
            if (i.x < point.x && i.y < point.y)
            {
                point = i;
            }
        }

        return point;
    }

    Point bottom_right() const
    {
        Point point = points.front();

        for (const auto & i : points)
        {
            if (i.x > point.x && i.y > point.y)
            {
                point = i;
            }
        }

        return point;
    }

    bool is_near(const Point & point)
    {
        for (const auto & i : points)
        {
            if (calculate_distance(i, point) <= 10)
            {
                return true;
            }
        }

        return false;
    }

};

class ChangeDetector {

private:
    const float MAX_DISTANCE = sqrt(pow(255, 2) * 3);

    Vec3b swap_rb(const Vec3b & color)
    {
        return Vec3b(color.val[2], color.val[1], color.val[0]);
    }

    Vec3b rgb_to_lab(const Vec3b & color)
    {
        float r = color.val[0] / 255.0;
        float g = color.val[1] / 255.0;
        float b = color.val[2] / 255.0;

        // RGB to XYZ
        r = (r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
        g = (g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
        b = (b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

        // Illuminant D65
        float xn = 0.95047;
        float yn = 1.00000;
        float zn = 1.08883;

        float x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / xn;
        float y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / yn;
        float z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / zn;

        x = (x > 0.008856) ? pow(x, 1.0 / 3.0) : (7.787 * x) + 16.0 / 116.0;
        y = (y > 0.008856) ? pow(y, 1.0 / 3.0) : (7.787 * y) + 16.0 / 116.0;
        z = (z > 0.008856) ? pow(z, 1.0 / 3.0) : (7.787 * z) + 16.0 / 116.0;

        // XYZ to LAB
        uchar l = (uchar) ( (116 * y) - 16 );
        uchar a = (uchar) ( 500 * (x - y) );
        uchar b1 = (uchar) ( 200 * (y - z) );

        return Vec3b(l, a, b1);

    }
    
    float calculate_distance(const Vec3b& color1, const Vec3b& color2)
    {
//        uchar deltaL = color1.val[0] - color2.val[0];
//        uchar deltaA = color1.val[1] - color2.val[1];
//        uchar deltaB = color1.val[2] - color2.val[2];
//
//        float c1 = sqrt(color1.val[1] * color1.val[1] + color1.val[2] * color1.val[2]);
//        float c2 = sqrt(color1.val[1] * color2.val[1] + color2.val[2] * color2.val[2]);
//
//        float deltaC = c1 - c2;
//        float deltaH = deltaA * deltaA + deltaB * deltaB - deltaC * deltaC;
//        deltaH = deltaH < 0 ? 0 : sqrt(deltaH);
//
//        float sc = 1.0 + 0.045 * c1;
//        float sh = 1.0 + 0.015 * c1;
//
//        float deltaLKlsl = deltaL / (1.0);
//        float deltaCkcsc = deltaC / (sc);
//        float deltaHkhsh = deltaH / (sh);
//
//        float i = deltaLKlsl * deltaLKlsl + deltaCkcsc * deltaCkcsc + deltaHkhsh * deltaHkhsh;
//
//        return i < 0 ? 0 : sqrt(i);

        float diff1 = (float) color1.val[0] - (float) color2.val[0];
        float diff2 = (float) color1.val[1] - (float) color2.val[1];
        float diff3 = (float) color1.val[2] - (float) color2.val[2];

        return sqrt(pow(diff1, 2) + pow(diff2, 2) + pow(diff3, 2));
    }

    bool is_changed(const Vec3b& color1, const Vec3b& color2)
    {
        float diff1 = (float) color1.val[0] - (float) color2.val[0];
        float diff2 = (float) color1.val[1] - (float) color2.val[1];
        float diff3 = (float) color1.val[2] - (float) color2.val[2];

        return diff1 != 0 || diff2 != 0 || diff3 != 0;
    }

public:

    const int BOX_SIZE = 1;
    const float SCALE = 0.7;

    map<int, map<int, int>> blur_map;

    Mat base;

    void blur(Mat & image)
    {
        int width = image.cols / 10;
        int height = image.rows / 10;

        for (int i = 0; i < image.rows; i += height)
        {
            for (int j = 0; j < image.cols; j += width)
            {
                int blur = blur_map[i / height][j / width];

                Rect rect(j, i, width, height);

                cv::GaussianBlur(image(rect), image(rect), Size(blur, blur), 0);
            }
        }

        cv::imshow("blur", image);
    }

    void setup(const Mat & image)
    {
        float scale = 1280.0 / image.cols;

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(10, 100);

        for (int i = 0; i < 50; i++)
        {
            for (int j = 0; j < 50; j++)
            {
                int random = uni(rng);

                if (random % 2 != 1)
                {
                    random++;
                }

                blur_map[i][j] = random;
            }
        }

        cv::resize(image, base, Size(), scale, scale);
        cv::cvtColor(base, base, cv::COLOR_BGR2GRAY);
        blur(base);
    }

    vector<Rect> detect(const Mat &next)
    {
        vector<Rect> list;
        vector<Point> changes;

        Mat current, processing, threshold, dilate;
        vector<vector<Point>> contours;

        float scale = 1280.0 / next.cols;

        cv::resize(next, current, Size(), scale, scale);
        cv::cvtColor(current, current, cv::COLOR_BGR2GRAY);
        blur(current);

        cv::absdiff(base, current, processing);
        cv::threshold(processing, processing, 25, 255, cv::THRESH_BINARY);
        cv::dilate(processing, processing, Mat());
        cv::findContours(processing, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        for (auto & contour : contours)
        {
            auto rect = cv::boundingRect(contour);

            rect.x /= scale;
            rect.y /= scale;
            rect.width /= scale;
            rect.height /= scale;

            list.push_back(rect);
        }

        this->base = current.clone();

        return list;
    }

};


#endif //CV_CHANGEDETECTOR_H
