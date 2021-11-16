//
// Created by CVSLab on 11/15/2021.
//

#include "utils.h"

ImageProcessing::ImageProcessing(char **path, char *ex, std::int32_t th){
    this -> number = 0;
    Initial(path, ex);
    setTrimImage();
    setThreshold(th);
}

ImageProcessing::~ImageProcessing(){
    delete[] image;
    delete[] standard;
}

void ImageProcessing::Initial(char **path, char *ex){
    setExtension(ex);
    setInitialPath(path);
    this -> hf = FindFirstFileA(initial_path, &wfd);
    setNumber();
    setImage(path);
    setCoordinateInitial();
}

void ImageProcessing::call(){
    Graph();
}

void ImageProcessing::setInitialPath(char **path){
    sprintf_s(initial_path, "%s/*%s", path[1], extension);
}

void ImageProcessing::setNumber(){
    std::size_t file_counter = 0;
    if(hf != INVALID_HANDLE_VALUE){
        do {
            ++file_counter;
        } while (FindNextFileA(hf, &wfd) != NULL);
        FindClose(hf);
    }
    std::cout << "IMAGE NUMBER : " << file_counter << std::endl;
    this -> number = file_counter;
}

void ImageProcessing::setImage(char **path){
    this -> image = new cv::Mat_<uchar>[number];
    std::size_t counter = 0;
    this -> hf = FindFirstFileA(initial_path, &wfd);

    if(hf != INVALID_HANDLE_VALUE){
        do {
            sprintf_s(image_path, "%s/%s", path[1], wfd.cFileName);
            this -> image[counter] = cv::imread(image_path,
                                                cv::IMREAD_GRAYSCALE);
            ++counter;
            FindNextFileA(hf, &wfd);
        } while (counter != number);
        FindClose(hf);
    }
}

void ImageProcessing::setCoordinateInitial(){
    this -> start_x = image[0].rows / 4;
    this -> start_y = image[0].cols / 4;
    this -> end_x = (image[0].rows / 4) * 3;
    this -> end_y = (image[0].cols / 4) * 3;
}

void ImageProcessing::setTrimImage(){
    setStart();
    setEnd();
    cv::Rect roi(start_x, start_y, end_x - start_x, end_y - start_y);
    showTrimmingRange();
    for (std::int32_t i=0; i < number; ++i){
        this -> image[i] = image[i](roi);
    }
    cv::namedWindow("Image", cv::WINDOW_NORMAL);
    cv::imshow("Image", image[0]);
    cv::waitKey();
    cv::destroyWindow("Window");
}

void ImageProcessing::saveTrimImage(char *save_path) {
    std::size_t counter = 0;
    this -> hf = FindFirstFileA(initial_path, &wfd);

    if(hf != INVALID_HANDLE_VALUE){
        do {
            sprintf_s(image_path, "%s/%s", save_path, wfd.cFileName);
//            std::cout << image_path << std::endl;
            cv::imwrite(image_path, image[counter]);
            ++counter;
            FindNextFileA(hf, &wfd);
        } while (counter != number);
        FindClose(hf);
    }
}


void ImageProcessing::setStart(){
    cv::Mat_<cv::Vec3b> img;
    cv::cvtColor(getImage()[0], img, CV_GRAY2RGB);
    std::int32_t key;
    cv::namedWindow("Window", cv::WINDOW_NORMAL);
    cv::imshow("Window", img);
    std::cout << std::endl;
    std::cout << "up : w" << std::endl;
    std::cout << "down : s" << std::endl;
    std::cout << "left : a" << std::endl;
    std::cout << "right : d" << std::endl;
    std::cout << "Decision : Enter" << std::endl;
    std::cout << std::endl;

    do{
        key = cv::waitKey(10);
        for (std::int32_t i = 0; i < image[0].rows; ++i) {
            for (std::int32_t j = 0; j < image[0].cols; ++j) {
                if (i == start_y){
                    if (i != 0) img.at<cv::Vec3b>(i-1, j)[0] = 255;
                    img.at<cv::Vec3b>(i, j)[0] = 255;
                    if (i != image[0].cols-1)  img.at<cv::Vec3b>(i+1, j)[0] = 255;
                }
                if (j == start_x){
                    if (j != 0) img.at<cv::Vec3b>(i, j-1)[2] = 255;
                    img.at<cv::Vec3b>(i, j)[2] = 255;
                    if (j != image[0].rows -1) img.at<cv::Vec3b>(i, j+1)[2] = 255;
                }
            }
        }
        cv::imshow("Window", img);
        std::cout << "\r" << "Start x : " << start_x << ", Y : " << start_y << " ";
        if (key==119){
            if (ColsChecker(start_y - 1)) this -> start_y -= 1;
        }
        else if (key==115){
            if (ColsChecker(start_y + 1)) this -> start_y += 1;
        }
        else if (key==97){
            if (RowsChecker(start_x -1)) this -> start_x -= 1;
        }
        else if (key==100){
            if (RowsChecker(start_x + 1)) this -> start_x += 1;
        }
        cv::cvtColor(getImage()[0], img, CV_GRAY2RGB);
    } while (key !=13);

    std::cout << "\r" << "Start x : " << start_x << ", Y : " << start_y;
    std::cout << std::endl;
}

cv::Mat_<cv::Vec3b> ImageProcessing::getStartImage(){
    cv::Mat_<cv::Vec3b> img;
    cv::cvtColor(getImage()[0], img, CV_GRAY2RGB);
    for (std::int32_t i = 0; i < image[0].rows; ++i) {
        for (std::int32_t j = 0; j < image[0].cols; ++j) {
            if (i == start_y){
                img.at<cv::Vec3b>(i-1, j)[1] = 255;
                img.at<cv::Vec3b>(i, j)[1] = 255;
                img.at<cv::Vec3b>(i+1, j)[1] = 255;
            }
            if (j == start_x){
                img.at<cv::Vec3b>(i, j-1)[1] = 255;
                img.at<cv::Vec3b>(i, j)[1] = 255;
                img.at<cv::Vec3b>(i, j+1)[1] = 255;
            }
        }
    }
    return img;
}

void ImageProcessing::setEnd(){
    std::int32_t key;
    cv::namedWindow("Window", cv::WINDOW_NORMAL);
    cv::Mat_<cv::Vec3b> img = getStartImage();
    cv::imshow("Window", img);

    do{
        for (std::int32_t i = 0; i < image[0].rows; ++i) {
            for (std::int32_t j = 0; j < image[0].cols; ++j) {
                if (i == end_y){
                    if (i != 0) img.at<cv::Vec3b>(i-1, j)[0] = 255;
                    img.at<cv::Vec3b>(i, j)[0] = 255;
                    if (i != image[0].cols-1)  img.at<cv::Vec3b>(i+1, j)[0] = 255;
                }
                if (j == end_x){
                    if (j != 0) img.at<cv::Vec3b>(i, j-1)[2] = 255;
                    img.at<cv::Vec3b>(i, j)[2] = 255;
                    if (j != image[0].cols -1) img.at<cv::Vec3b>(i, j+1)[2] = 255;
                }
            }
        }
        cv::imshow("Window", img);
        key = cv::waitKey(0);
        if (key==119){
            if (ColsChecker(end_y -1) && end_y - 1 > start_y) this -> end_y -= 1;
        }
        else if (key==115) {
            if (ColsChecker(end_y +1 )) this -> end_y += 1;
        }
        else if (key==97){
            if (RowsChecker(end_x - 1) && end_x - 1 > start_x)  this -> end_x -= 1;
        }
        else if (key==100) {
            if (RowsChecker(end_x + 1)) this -> end_x += 1;
        }
        img = getStartImage();
        std::cout << "\r" << "End   x : " << end_x << ", Y : " << end_y;
    } while (key !=13);

    std::cout << "\r" << "End   x : " << end_x << ", Y : " << end_y;
    std::cout << std::endl;
}

void ImageProcessing::showTrimmingRange(){
    cv::Mat_<cv::Vec3b> img;
    cv::cvtColor(getImage()[0], img, CV_GRAY2RGB);
    for (std::int32_t i = 0; i < image[0].rows; ++i) {
        for (std::int32_t j = 0; j < image[0].cols; ++j) {
            if (j < start_x || end_x < j){
                img.at<cv::Vec3b>(i, j)[1] = 255;
            }
            if (i < start_y || end_y < i){
                img.at<cv::Vec3b>(i, j)[1] = 255;
            }
        }
    }

    std::cout << std::endl;
    std::cout << "Height Size : " << end_y - start_y << std::endl;
    std::cout << "Width  Size : " << end_x - start_x << std::endl;

    cv::imshow("Window", img);
}

bool ImageProcessing::RowsChecker(std::int32_t _x){
    if (_x > 0 && _x < image[0].rows) return true;
    else return false;
}

bool ImageProcessing::ColsChecker(std::int32_t _x){
    if (_x > 0 && _x < image[0].cols) return true;
    else return false;
}

void ImageProcessing::ImageChecker(){
    cv::Mat_<uchar>*src = getImage();
    for(std::int32_t i=0; i < number; ++i){
        plot_image(src[i]);
    }
}

void ImageProcessing::Graph(){
    struct MyFunc{
        static void visualize(const std::string& title, cv::Mat& x){
            cv::namedWindow(title, cv::WINDOW_NORMAL);
            cv::imshow(title, x);
            cv::waitKey(1);
        }
    };

    std::cout << std::endl;
    std::vector<std::int32_t> x(image[0].cols);
    std::vector<uchar> one_dimension(image[0].cols);

    std::int32_t std_height = 500;

    cv::Mat_<cv::Vec3b> graph = cv::Mat::zeros(cv::Size(image[0].cols, 255), CV_8UC3);
//    cv::Mat_<cv::Vec3b> std_graph = cv::Mat::zeros(cv::Size(image[0].cols, std_height), CV_8UC3);
    cv::Mat_<cv::Vec3b> dst;
    cv::Mat_<cv::Vec3b> mask;
    for (std::int32_t o = 0; o < number; ++o){
        cv::cvtColor(getImage()[o],  dst, CV_GRAY2RGB);
        cv::cvtColor(getImage()[o],  mask, CV_GRAY2RGB);

        for (std::int32_t i=0; i<image[0].rows;++i) {
            for (std::int32_t j=0; j<image[0].cols;++j) {
                //            x[j] = j;
                //            one_dimension[j] = image.at<uchar>(j, i);
                for (std::int32_t k=0; k < 3;++k){

                    graph.at<cv::Vec3b>(255 - (image[o].at<uchar>(i, j)), j)[k] = 255;
                    if (threshold < image[o].at<uchar>(i, j)){
                        for (std::int32_t p=0; p < 2; p+=2){
                            graph.at<cv::Vec3b>(255 - (image[o].at<uchar>(i, j)), j)[p] = 0;
                        }
                    }
                    if (k ==2 ) {
                        graph.at<cv::Vec3b>((255 - threshold), j)[2] = 225;
                    }
                }

                if(i !=0) dst.at<cv::Vec3b>(i-1, j)[2] = 255;
                dst.at<cv::Vec3b>(i, j)[2] = 255;
                if(i != image[0].rows - 1) dst.at<cv::Vec3b>(i+1, j)[2] = 255;

                if (threshold < image[o].at<uchar>(i, j)){
                    for (std::int32_t q = 0; q < 3; ++q){
                        mask.at<cv::Vec3b>(i, j)[q] = 0;
                    }
                    for (std::int32_t r = 0; r < 2; ++r){
                        mask.at<cv::Vec3b>(i, j)[r] = 255;
                    }
                }
            }
            MyFunc::visualize("Graph", graph);
            MyFunc::visualize("Image", dst);
//            MyFunc::visualize("Standard", std_graph);
            //        ShowGraph(x, one_dimension);
            cv::cvtColor(getImage()[o],  dst, CV_GRAY2RGB);
            for (std::int32_t l=0; l<graph.rows; ++l) {
                for (std::int32_t m=0; m<graph.cols; ++m){
                    for (std::int32_t n=0; n < 3; ++n){
                        graph.at<cv::Vec3b>(l, m)[n] = 0;
                    }
                }
            }
            MyFunc::visualize("Mask", mask);
        }
    }
    if (cv::waitKey() == 27) exit(1);
}


GenerateNoiseImage::GenerateNoiseImage(char* path, char* extension,
                                       std::int32_t width, std::int32_t height,
                                       std::int32_t generate_number) {
    sprintf_s(sample_path, "%s", path);
    _mkdir(path);
    sprintf_s(image_extension, "%s", extension);
    this -> width = width;
    this -> height = height;
    this -> number = generate_number;
}

void GenerateNoiseImage::call(){
    resetRandom();
    for (std::int32_t i = 0; i < number; ++i) {
        setImagePath();
        setImage();
    }
}

void GenerateNoiseImage::setImagePath(){
    sprintf_s(image_path, "%s/%04d%s", sample_path, count, image_extension);
    this -> count += 1;
}

void GenerateNoiseImage::setImage(){
    image.create(cv::Size(width, height));

    for (std::int32_t i = 0; i < image.rows; ++i) {
        for (std::int32_t j = 0; j < image.cols; ++j) {
            image.at<uchar>(i, j) = getRandom();
        }
    }
    cv::imwrite(getImagePath(), image);
}

void GenerateNoiseImage::resetRandom(){
    srand((std::uint32_t) time(NULL));
}

std::int32_t GenerateNoiseImage::getRandom(){
    std::int32_t min = 0;
    std::int32_t max = 255;
    return (min + (std::int32_t)(rand() * (max - min + 1.0)/(1.0 + RAND_MAX)));
}

void plot_image(cv::Mat_<uchar>& image){
    char title[32] = "test";
    cv::namedWindow(title,
                    cv::WINDOW_NORMAL);
    cv::imshow(title, image);
    cv::waitKey(5);
}