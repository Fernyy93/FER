/**
 * Filename: writeyaml.cpp
 * Writes the photos into a yaml file containing their cv::Mat object, file location and emotion
 * Not to be confused with the function write_yaml that writes histogram objects to a yaml file
 * Author: Alex Fernicola
 *  
 **/
#include "image.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stdint.h>
#include <string.h>

namespace fs = std::filesystem;

int main(int argc, const char *argv[]) {
    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <images directory path>" << "<path to yaml file> " << std::endl;
        
        return 1;
    }
    fs::path myPath = argv[1];
    std::string img_path;
    std::string delimiter = "/";

    cv::FileStorage yaml_file(argv[2], cv::FileStorage::WRITE);
    
    std::cout << "file path is: " << myPath << std::endl;
    yaml_file << "images" << "[";
    for (const auto& dirEntry : fs::recursive_directory_iterator(myPath)) {
        if (dirEntry.is_regular_file()){
            std::stringstream ss;
            std::string filename = dirEntry.path().filename().string();
            std::string emotion = dirEntry.path().parent_path().filename().string();
            std::string extension = dirEntry.path().extension().string(); 
            ss << argv[1] << emotion << "/" << filename;
            filename = ss.str();
            Image image(filename, emotion);
            yaml_file << "{" << "Image_path" << filename << "Emotion" << emotion << "Matrix"<< image.getImage() << "}";
        }
    }
    yaml_file << "]";
    yaml_file.release();
    return 0;
}