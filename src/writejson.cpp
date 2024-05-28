#include "image.h"

int main(int argc, const char *argv[]) {
    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <directory_path>\n";
        return 1;
    }
    fs::path myPath = argv[1];
    //fs::path myPath = "../new/";
    std::string img_path;
    std::string delimiter = "/";

    json result = json::array();
    
    std::cout << "file path is: " << myPath << std::endl;
    for (const auto& dirEntry : fs::recursive_directory_iterator(myPath)) {
        if (dirEntry.is_regular_file()){
            std::stringstream ss;
            std::string filename = dirEntry.path().filename().string();
            std::string emotion = dirEntry.path().parent_path().filename().string();
            std::string extension = dirEntry.path().extension().string(); // save extension to the JSON too? and also add to image class?
            ss << argv[1] << emotion << "/" << filename;
            filename = ss.str();
            std::cout << "Filename: " << filename << " emotion: " << emotion << " extension: " << extension << std::endl;
            Image image(filename, emotion);
            json image_json = image.create_json_object();
            result.push_back(image_json);

            // I need to open a JSON and write the above to the JSON.
            // need to imread each file to get the height, width, type

        }
    }
    // TODO finish below
    std::ofstream f("all_images.json");
    if (f.is_open()){
        f << std::setw(4) << result << std::endl;
    }

    // for each path in fs::recursive_directory_iterator
    // separate the path by //
    // discard new
    // save emotion and image num to a data structure (map)
    // imread and create an image object
    // save the image object properties to the json

    return 0;
}