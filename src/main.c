#define SUPPORT_FILEFORMAT_BMP 1
#define SUPPORT_FILEFORMAT_PNG 1
#define SUPPORT_FILEFORMAT_TGA 1
#include "raylib.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum {
  SORT_X,
  SORT_Y,
};

typedef enum {
  LUMIN,
  CRED,
  CGREEN,
  CBLUE,
} ColorMode;

ColorMode color_mode = LUMIN;

float Color_to_lumin(Color color) {
  switch (color_mode) {
  case LUMIN:
    return (0.00117254901 * color.r) + (0.00230196078 * color.g) +
           (0.00044705882 * color.b);
    break;
  case CRED:
    return (float)color.r / 255.0f;
    break;
  case CGREEN:
    return (float)color.g / 255.0f;
    break;
  case CBLUE:
    return (float)color.b / 255.0f;
    break;
  }
  return 0;
}

int Color_cmp(const void *a, const void *b) {
  Color ac = *((Color *)a);
  Color bc = *((Color *)b);
  float al, bl;
  switch (color_mode) {
  case LUMIN:
    al = 0.299 * ac.r + 0.587 * ac.g + 0.114 * ac.b;
    bl = 0.299 * bc.r + 0.587 * bc.g + 0.114 * bc.b;
    break;
  case CRED:
    al = ac.r;
    bl = bc.r;
    break;
  case CGREEN:
    al = ac.g;
    bl = bc.g;
    break;
  case CBLUE:
    al = ac.b;
    bl = bc.b;
    break;
  }
  return ceil(al - bl);
}

int Color_cmp_rev(const void *a, const void *b) {
  Color ac = *((Color *)a);
  Color bc = *((Color *)b);
  float al, bl;
  switch (color_mode) {
  case LUMIN:
    al = 0.299 * ac.r + 0.587 * ac.g + 0.114 * ac.b;
    bl = 0.299 * bc.r + 0.587 * bc.g + 0.114 * bc.b;
    break;
  case CRED:
    al = ac.r;
    bl = bc.r;
    break;
  case CGREEN:
    al = ac.g;
    bl = bc.g;
    break;
  case CBLUE:
    al = ac.b;
    bl = bc.b;
    break;
  }
  return ceil(bl - al);
}

void Image_sort_x(Image *img, float threshold, bool reverse) {
  float *lumins;
  lumins = malloc(img->height * img->width * sizeof(float));
  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img->width; j++) {
      int pixel_index = i * img->width + j;
      Color color = *((Color *)(img->data + (pixel_index * sizeof(Color))));
      *(lumins + pixel_index) = Color_to_lumin(color);
    }
  }

  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img->width; j++) {
      int pixel_start = i * img->width + j;
      float curr = *(lumins + pixel_start);
      if ((threshold <= curr)) {
        int pixel_end = i * img->width + j;
        while ((threshold <= curr) && j + 1 < img->width) {
          pixel_end++;
          j++;
          curr = *(lumins + pixel_end);
        }
        qsort(img->data + (pixel_start * sizeof(Color)),
              pixel_end - pixel_start, sizeof(Color),
              reverse ? Color_cmp_rev : Color_cmp);
      }
    }
  }
  free(lumins);
}

void Image_sort_y(Image *img, float threshold, bool reverse) {
  ImageRotateCCW(img);
  Image_sort_x(img, threshold, reverse);
  ImageRotateCW(img);
}

char *save_image_to_file(Image *img, const char *extension) {
  time_t t = time(0);
  struct tm tm;
  localtime_s(&tm, &t);
  char *output_text = malloc(256);
  snprintf(output_text, 256, "output-%d-%02d-%02d %02d-%02d-%02d.png",
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
           tm.tm_sec);
  if (ExportImage(*img, output_text)) {
    return output_text;
  } else {
    return strdup("Image export failed.");
  }
}

int main(int argc, char *argv[]) {

  SetTraceLogLevel(LOG_NONE);

  if (argc < 3) {
    printf("psortr <image_path> <threshold> (x | y | xr | yr) (lumin | red | "
           "green | blue)");
    return 0;
  }

  char *err;
  float threshold = strtof(argv[2], &err);

  if (*err != '\0') {
    printf("Invalid threshold: \"%s\"", argv[2]);
    printf("Err! : %s\n", err);
    return 1;
  }

  if (!FileExists(argv[1])) {
    printf("File doesnt exist: %s\n", argv[1]);
    return 1;
  }
  Image image = LoadImage(argv[1]);
  const char *extension = GetFileExtension(argv[1]);
  if (image.data == NULL) {
    printf("Couldn't load image: \"%s\"", argv[1]);
    return 1;
  }

  int sort = SORT_X;
  bool reverse = false;
  if (argc >= 4) {
    char *curr = argv[3];
    while (*curr != '\0') {
      if (*curr == 'x') {
        sort = SORT_X;
      }
      if (*curr == 'y') {
        sort = SORT_Y;
      }
      if (*curr == 'r') {
        reverse = true;
      }
      curr++;
    }
  }

  if (argc >= 5) {
    const char *lower = TextToLower(argv[4]);
    if (strcmp(argv[4], "lumin") == 0)
      color_mode = LUMIN;
    else if (strcmp(argv[4], "red") == 0)
      color_mode = CRED;
    else if (strcmp(argv[4], "green") == 0)
      color_mode = CGREEN;
    else if (strcmp(argv[4], "blue") == 0)
      color_mode = CBLUE;
    else {
      printf("Invalid color mode: %s\nUse (lumin | red | green | blue)",
             argv[4]);
      return 1;
    }
  }

  ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

  switch (sort) {
  case SORT_X:
    Image_sort_x(&image, threshold, reverse);
    break;
  case SORT_Y:
    Image_sort_y(&image, threshold, reverse);
    break;
  }

  char *message = save_image_to_file(&image, extension);
  printf("%s\n", message);
  free(message);

  return 0;
}
