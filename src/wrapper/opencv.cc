#include "opencv.h"

namespace my_cv {
    static inline bool isCorrectClass(unsigned char class_) {
        return class_ == 3 || class_ == 12 || class_ == 15;
    }

    cv::Mat_<uchar> rebuildImageFromVector(
        const std::vector<uchar> &labels, unsigned width) {
        auto resMat = cv::Mat_<uchar>(labels.size() / width, width);

        for (auto y = 0; y < resMat.rows; y++) {
            for (auto x = 0; x < resMat.cols; ++x) {
                unsigned char class_ = labels[y * resMat.cols + x];
                resMat[y][x] = (isCorrectClass(class_))? 255: 0;
            }
        }

        return resMat;
    }


    cv::Mat rebuildImageFromVectorRgb(const std::vector<uchar> &labels,
                                              unsigned width) {
        static cv::Vec3b random_lut[16] = {
            {84,  0,   255},
            {255, 0,   23},
            {0,   116, 255},
            {255, 100, 0},
            {184, 0,   255},
            {255, 200, 0},
            {255, 0,   124},
            {0,   15,  255},
            {255, 0,   0},
            {108, 255, 0},
            {0,   255, 192},
            {0,   255, 92},
            {255, 0,   224},
            {7,   255, 0},
            {208, 255, 0},
            {0,   216, 255}
        };
        auto resMat = cv::Mat(labels.size() / width, width, CV_8UC3);

        for (auto y = 0; y < resMat.rows; y++) {
            for (auto x = 0; x < resMat.cols; ++x) {
                unsigned char class_ = labels[y * resMat.cols + x];
                resMat.at<cv::Vec3b>(y, x) = random_lut[class_];
            }
        }

        return resMat;
    }
}

std::pair<cv::Rect, bool> get_position_barcode(cv::Mat image)
{
    using namespace cv;
    GaussianBlur(image, image, Size(3,3), 6);
    threshold(image, image, 150, 255, THRESH_BINARY);
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(2,4)), Point(-1,-1), 1);
    dilate(image, image, getStructuringElement(MORPH_RECT, Size(16,4)), Point(-1,-1), 2 );
    std::vector<std::vector<Point> > contours;
    findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty())
        return {Rect(), false};

    auto index_max = 0U;
    for (auto i = 1U; i < contours.size(); i++)
    {
        if (contours[i].size() > contours[index_max].size())
        {
            index_max = i;
        }
    }

    std::vector<Point> contours_poly;
    approxPolyDP( contours[index_max], contours_poly, 3, true );
    Rect boundRect = boundingRect( contours_poly );
    Mat res_image = imread("barcode-00-01.jpg");
    boundRect.x *= 16;
    boundRect.y *= 16;
    boundRect.width *= 16;
    boundRect.height *= 16;

    return {boundRect, true};
}
