#include <iostream>
#include <png.h>
#include <vector>

// Function to load a PNG image into a buffer
bool load_png(const std::string& filename, std::vector<unsigned char> &image,
              unsigned int &width, unsigned int &height) {
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    return false;
  }

  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    fclose(fp);
    std::cerr << "Failed to initialize libpng" << std::endl;
    return false;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_read_struct(&png, nullptr, nullptr);
    fclose(fp);
    std::cerr << "Failed to initialize info structure" << std::endl;
    return false;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    std::cerr << "Error during PNG reading" << std::endl;
    return false;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  width = png_get_image_width(png, info);
  height = png_get_image_height(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
  png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
  image.resize(width * height * 4);

  for (unsigned int y = 0; y < height; ++y) {
    row_pointers[y] = &image[y * width * 4];
  }

  png_read_image(png, row_pointers);
  png_read_end(png, nullptr);

  free(row_pointers);
  png_destroy_read_struct(&png, &info, nullptr);
  fclose(fp);
  return true;
}

// Function to save a PNG image from a buffer
bool save_png(const std::string& filename, const std::vector<unsigned char> &image,
              unsigned int width, unsigned int height) {
  FILE *fp = fopen(filename.c_str(), "wb");
  if (!fp) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    return false;
  }

  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    fclose(fp);
    std::cerr << "Failed to initialize libpng" << std::endl;
    return false;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, nullptr);
    fclose(fp);
    std::cerr << "Failed to initialize info structure" << std::endl;
    return false;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    std::cerr << "Error during PNG writing" << std::endl;
    return false;
  }

  png_init_io(png, fp);

  png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png, info);

  std::vector<png_bytep> row_pointers(height);
  for (unsigned int y = 0; y < height; ++y) {
    row_pointers[y] = const_cast<png_bytep>(&image[y * width * 4]);
  }

  png_write_image(png, row_pointers.data());
  png_write_end(png, nullptr);

  png_destroy_write_struct(&png, &info);
  fclose(fp);
  return true;
}

bool save_apng(const std::string& filename, const std::vector<unsigned char> &image1,
               const std::vector<unsigned char> &image2, unsigned int width,
               unsigned int height) {

  FILE *fp = fopen(filename.c_str(), "wb");
  if (!fp) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    return false;
  }

  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    fclose(fp);
    std::cerr << "Failed to initialize libpng" << std::endl;
    return false;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, nullptr);
    fclose(fp);
    std::cerr << "Failed to initialize info structure" << std::endl;
    return false;
  }

  if (setjmp(png_jmpbuf(png))) {
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    std::cerr << "Error during PNG writing" << std::endl;
    return false;
  }

  png_init_io(png, fp);

  // Write APNG header: IHDR and acTL
  png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_set_acTL(png, info, 2, 0);
  png_write_info(png, info);

  // Animation defs
  int x_offset = 0, y_offset = 0, delay_num = 41, delay_den = 1000, dispose_op = 0, blend_op = 0;

  // Write fcTL chunk for the 1st frame
  png_write_frame_head(png, info, (png_bytepp)NULL, width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

  // Write first frame as IDAT
  std::vector<png_bytep> row_pointers(height);
  for (unsigned int y = 0; y < height; ++y) {
    row_pointers[y] = const_cast<png_bytep>(&image1[y * width * 4]);
  }
  png_write_image(png, row_pointers.data());
  png_write_frame_tail(png, info);

  // Write fcTL chunk for the 2nd frame
  png_write_frame_head(png, info, (png_bytepp)NULL, width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

  // Write second frame as fdAT
  std::vector<png_bytep> row_pointers2(height);
  for (unsigned int y = 0; y < height; ++y) {
    row_pointers2[y] = const_cast<png_bytep>(&image2[y * width * 4]);
  }
  png_write_image(png, row_pointers2.data());
  png_write_frame_tail(png, info);

  // Write IEND chunk
  png_write_end(png, nullptr);

  png_destroy_write_struct(&png, &info);
  fclose(fp);
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <input_file1> <input_file2> <output_file>" << std::endl;
    return 1;
  }

  std::string input_file1 = argv[1];
  std::string input_file2 = argv[2];
  std::string output_file = argv[3];

  std::vector<unsigned char> image1;
  std::vector<unsigned char> image2;
  unsigned int width, height;

  if (!load_png(input_file1, image1, width, height)) return 1;
  if (!load_png(input_file2, image2, width, height)) return 1;

  if (!save_apng(output_file, image1, image2, width, height)) return 1;

  std::cout << "Image has been saved as OUTPUT1.png, OUTPUT2.png and the animation to OUTPUT.png"
            << std::endl;
  return 0;
}
