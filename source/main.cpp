//
// Created by CVSlab on 11/15/2021
//

#include "params.h"
#include "utils.h"

void generate_image();
void trimming(char** argv);

int main(std::int32_t argc, char **argv) {
    generate_image();
//    trimming(argv);
    return 0;
}

void generate_image(){
    GenerateNoiseImage gni(sample_dir,
                           extension,
                           width, height,
                           number);
    gni.call();
}

void trimming(char** argv){
    ImageProcessing ip(argv, extension, threshold);
    ip.call();
}