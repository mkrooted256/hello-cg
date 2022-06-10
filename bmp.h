/*
I took this code from 
https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/
*/

#pragma once
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};          // File type always BM which is 0x4D42
    uint32_t file_size{0};               // Size of the file (in bytes)
    uint16_t reserved1{0};               // Reserved, always 0
    uint16_t reserved2{0};               // Reserved, always 0
    uint32_t offset_data{0};             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size{ 0 };                      // Size of this header (in bytes)
    int32_t width{ 0 };                      // width of bitmap in pixels
    int32_t height{ 0 };                     // height of bitmap in pixels
                                             //       (if positive, bottom-up, with origin in lower left corner)
                                             //       (if negative, top-down, with origin in upper left corner)
    uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 0 };                 // No. of bits per pixel
    uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader {
    uint32_t red_mask  { 0x00ff0000 };         // Bit mask for the red channel
    uint32_t green_mask{ 0x0000ff00 };       // Bit mask for the green channel
    uint32_t blue_mask { 0x000000ff };        // Bit mask for the blue channel
    uint32_t alpha_mask{ 0xff000000 };       // Bit mask for the alpha channel
    uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
    uint32_t unused[16]{ 0 };                // Unused data for sRGB color space
};

struct BMPPixel {
    // Order reversed intentionally - this is how colors are stored in BMP
    uint8_t b = 0;
    uint8_t g = 0;
    uint8_t r = 0;

    BMPPixel() = default;
    BMPPixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};
#pragma pack(pop)

struct BMP {
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header;
    std::vector<BMPPixel> data;

    BMP(int32_t width, int32_t height, const std::vector<Color>& img) {
        data.reserve(img.size());
        for (const Color& c : img) {
            data.emplace_back( c.x * 255,c.y * 255,c.z * 255 );
        }

        bmp_info_header.width = width;
        bmp_info_header.height = height;
        bmp_info_header.size = sizeof(BMPInfoHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

        bmp_info_header.bit_count = 24;
        bmp_info_header.compression = 0;

        data_size = data.size() * 3;
        row_stride = width * 3;
        row_padding_size = (4 - row_stride % 4) % 4; // = - row_stride (mod 4)

        file_header.file_size = file_header.offset_data + data_size + bmp_info_header.height * row_padding_size;
    }

    void write(const char *fname) {
       std::ofstream of{ fname, std::ios_base::binary | std::ios_base::trunc };
       if (of) {
           if (bmp_info_header.bit_count == 32 || bmp_info_header.bit_count == 24) {
               write_headers_and_data(of);
           }
           else {
               throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
           }
       }
       else {
           throw std::runtime_error("Unable to open the output image file.");
       }
   }
   
private:
    uint32_t row_stride{ 0 };
    size_t data_size;
    uint32_t row_padding_size;
    const char* row_padding = "\0\0\0\0"; // max padding size = 4. yeah, yeah, i remember about null-termination, but just in case.

    void write_headers(std::ofstream &of) {
        of.write((const char*)&file_header, sizeof(file_header));
        of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
        if(bmp_info_header.bit_count == 32) {
            of.write((const char*)&bmp_color_header, sizeof(bmp_color_header));
        }
    }

    void write_headers_and_data(std::ofstream &of) {
        write_headers(of);
        const char * start = (const char*) data.data();

        if (row_padding_size)
        {
            for (const char* row_start = start + data_size - row_stride; row_start >= start; row_start -= row_stride ) {
                of.write(row_start, row_stride);
                of.write(row_padding, row_padding_size);
            }
        }
        else
        {
            for (const char* row_start = start + data_size - row_stride; row_start >= start; row_start -= row_stride ) {
                of.write(row_start, row_stride);
            }
        }
    }
};
