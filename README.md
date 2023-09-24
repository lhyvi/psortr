# psortr - Pixel Sorting Tool

psortr is a command-line tool for sorting pixels in an image based on their luminance values. It uses the [raylib](https://github.com/raysan5/raylib) library for image manipulation and sorting.

## Usage

`psortr <image_path> <threshold> (x | y | xr | yr) (lumin | red | green | blue)`

- `<image_path>`: The path to the input image file.
- `<threshold>`: A floating-point value between 0 and 1 that determines the sorting threshold. Pixels with luminance values greater than or equal to the threshold will be sorted.
- `(x | y | xr | yr)`: The sorting direction :
  - `x`: Sort pixels horizontally (left to right).
  - `y`: Sort pixels vertically (top to bottom).
  - `xr`: Sort pixels horizontally in reverse order (right to left).
  - `yr`: Sort pixels vertically in reverse order (bottom to top).
- `(lumin | red | green | blue)`: Sorts the color using selected mode

## How to Install

### Requirements

- CMake (version 3.21 or higher)
- GCC or another C compiler

### Installation Steps

1. Clone the psortr repository:

   ```bash
   git clone https://github.com/lhyvi/psortr.git
   cd psortr
   ```

2. Create a build directory and navigate to it:

   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the build:

   ```bash
   cmake ..
   ```

4. Build psortr:

   ```bash
   cmake --build .
   ```

5. You should now have the `psortr` executable in your build directory. You can copy it to a directory in your system's PATH for easy access:

   ```bash
   sudo cp psortr /usr/local/bin/
   ```

## Example

To sort an image named "input.png" with a threshold of 0.5 in reverse horizontal order, run the following command:

```bash
psortr input.png 0.5 xr
```

This will produce a sorted image and display the message indicating the success of the sorting operation.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
