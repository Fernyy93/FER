// base64.hpp
// Abdarhman Ibrahim, abdalrahman.m5959@gmail.com

#ifndef BASE64_HPP
#define BASE64_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <optional>
#include <stdexcept>


/**
 * Base64 encoding and decoding class.
 * Provides functionalities to encode and decode data using Base64 encoding scheme.
 */
class Base64
{
public:

    // Encode a cv::Mat to a Base64 string
    static std::string encodeMat(const cv::Mat &mat)
    {
        std::vector<uchar> buf;
        if (!cv::imencode(".png", mat, buf))
        {
            throw std::runtime_error("cv::imencode() failed.");
        }
        return encodeBuffer(buf.data(), buf.size());
    }

    // Decode a Base64 string to a cv::Mat
    static std::optional<cv::Mat> decodeToMat(const std::string &base64_string)
    {
        auto decoded_bytes = decodeBuffer(base64_string);
        if (!decoded_bytes)
        {
            return std::nullopt;
        }
        cv::Mat img = cv::imdecode(*decoded_bytes, cv::IMREAD_COLOR);
        if (img.empty())
        {
            return std::nullopt;
        }
        return img;
    }

    // Encode any buffer to a Base64 string
    static std::string encodeBuffer(const uchar *buf, size_t len)
    {
        std::string ret;
        ret.reserve((len + 2) / 3 * 4); // Precalculate the size of the output
        for (size_t i = 0; i < len; i += 3)
        {
            uint32_t val = buf[i] << 16;
            if (i + 1 < len)
                val |= buf[i + 1] << 8;
            if (i + 2 < len)
                val |= buf[i + 2];
            for (int j = 0; j < 4; j++)
            {
                if (i * 8 + j * 6 < len * 8)
                {
                    ret.push_back(base64_chars[(val >> (18 - j * 6)) & 0x3F]);
                }
                else
                {
                    ret.push_back('='); // Add padding
                }
            }
        }
        return ret;
    }

    // Decode a Base64 string to a buffer
    static std::optional<std::vector<uchar>> decodeBuffer(const std::string& encoded_string) {
        std::string clean_encoded_string;
        clean_encoded_string.reserve(encoded_string.size());
        for (char c : encoded_string) {
            if (base64_chars.find(c) != std::string::npos) {
                clean_encoded_string += c;
            }
        }

        // Check for valid Base64 length
        if (clean_encoded_string.length() % 4 != 0) {
            throw std::invalid_argument("Invalid Base64 input length.");
        }

        std::vector<uchar> ret;
        int bits_collected = 0;
        unsigned int accumulator = 0;

        for (char c : clean_encoded_string) {
            const int value = base64_chars.find(c);
            if (value == std::string::npos) {
                continue; // Skip invalid characters
            }

            accumulator = (accumulator << 6) | value;
            bits_collected += 6;
            if (bits_collected >= 8) {
                bits_collected -= 8;
                ret.push_back((accumulator >> bits_collected) & 0xFF);
            }
        }

        // No need to explicitly check for padding ('=') as we've already cleaned the input string.
        // If the input is malformed (e.g., incorrect padding), this simplistic approach might not detect it.

        return ret;
    }


private:
    static const std::string base64_chars;
};

const std::string Base64::base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

#endif // BASE64_HPP
