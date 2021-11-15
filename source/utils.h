//
// Created by CVSLab on 11/15/2021.
//

#ifndef TUTORIAL_UTILS_H
#define TUTORIAL_UTILS_H

#include <iostream>
#include <Windows.h>
#include <direct.h>
#include <ctime>
#include <cstdint>
#include <vector>
#include "opencv2/opencv.hpp"

void plot_image(cv::Mat_<uchar>& image);

class ImageProcessing{
private:
    char initial_path[256];
    char image_path[256];
    char *extension;
    WIN32_FIND_DATA wfd;
    HANDLE hf;
    std::size_t number;
    cv::Mat_<uchar>* image;

    std::int32_t start_x;
    std::int32_t start_y;
    std::int32_t end_x;
    std::int32_t end_y;

    cv::Mat_<std::double_t> mean;
    cv::Mat_<std::double_t> sigma;
    cv::Mat_<std::double_t>* standard;

    std::int32_t threshold;
public:
    ImageProcessing(char **path, char* ex, std::int32_t th);
    void call();
    void setInitialPath(char **path);
    void setNumber();
    std::size_t getNumber(){return number;}
    void setExtension(char* ex){this -> extension = ex;}
    void setImage(char **path);
    void setThreshold(std::int32_t th){this -> threshold = th;}
    std::int32_t getThreshold(){return threshold;}
    void Initial(char **path, char *ex);
    void ImageChecker();
    void setCoordinateInitial();
    void setTrimImage();
    void saveTrimImage(char *save_path);
    void setStart();
    void setEnd();
    void showTrimmingRange();
    bool RowsChecker(std::int32_t _x);
    bool ColsChecker(std::int32_t _x);
    cv::Mat_<cv::Vec3b> getStartImage();
    cv::Mat_<uchar>* getImage(){return image;}
    void Graph();
    ~ImageProcessing();
};

class GenerateNoiseImage{
private:
    char sample_path[256];
    char image_extension[256];
    char image_path[256];
    std::int32_t width;
    std::int32_t height;
    std::int32_t number;
    std::int32_t count = 0;
    cv::Mat_<uchar> image;
public:
    GenerateNoiseImage(char* path,
                       char* extension,
                       std::int32_t width,
                       std::int32_t height,
                       std::int32_t generate_number);
    ~GenerateNoiseImage()= default;;
    void call();
    void setImagePath();
    char* getImagePath(){return image_path;}
    void setImage();
    static void resetRandom();
    std::int32_t getRandom();
};


#endif //TUTORIAL_UTILS_H
