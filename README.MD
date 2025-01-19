# libpng_apng Write Demo

## Purpose
This repository contains a demonstration program that uses the **libpng** library to create a two-layer APNG (Animated PNG) file from two input PNG files.

## Usage
```
APNGWriteDemo <input_file1> <input_file2> <output_file>
```

### Parameters
- `<input_file1>`: The path to the first input PNG file.
- `<input_file2>`: The path to the second input PNG file.
- `<output_file>`: The path to the output APNG file that will be generated.

## Requirements
- **libpng-apng**: Ensure you have the apng patched `libpng` library installed on your system.

## Build Instructions
1. Clone the repository:
   ```
   git clone https://github.com/your-username/libpng_apng_write_demo.git
   cd libpng_apng_write_demo
   ```

2. Build the program:
   ```
   mkdir build ; cd build
   cmake ..
   make
   ```

3. Run the program using the usage instructions above.

## Example
To generate an APNG file `output.apng` from two PNG files `image1.png` and `image2.png`, run:
```
APNGWriteDemo image1.png image2.png output.apng
```

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.

## Author
Piotr D. Kaczorowski

## Dependencies
- Patched libpng from: https://github.com/peterkaczorowski/libpng/tree/v1.6.44-apng

