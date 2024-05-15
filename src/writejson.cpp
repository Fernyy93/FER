#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
namespace fs = std::filesystem;

int main(int argc, const char *argv[]) {
    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <directory_path>\n";
        return 1;
    }
    fs::path myPath = argv[1];
    //fs::path myPath = "../new/";
    std::string img_path;
    std::string delimiter = "/";
    
    std::cout << "file path is: " << myPath << std::endl;
    for (const auto& dirEntry : fs::recursive_directory_iterator(myPath)) {
        if (dirEntry.is_regular_file()){
            std::string filename = dirEntry.path().filename().string();
            std::string emotion = dirEntry.path().parent_path().filename().string();
            std::string extension = dirEntry.path().extension().string(); // save extension to the JSON too? and also add to image class?
            std::cout << "Filename: " << filename << " emotion: " << emotion << " extension: " << extension << std::endl;
        }
    }

    // for each path in fs::recursive_directory_iterator
    // separate the path by //
    // discard new
    // save emotion and image num to a data structure (map)
    // imread and create an image object
    // save the image object properties to the json

    return 0;
}