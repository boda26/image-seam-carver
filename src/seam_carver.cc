#include "seam_carver.hpp"

const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}

int SeamCarver::GetHeight() const {
  return height_;
}

int SeamCarver::GetWidth() const {
  return width_;
}

int SeamCarver::GetEnergy(int row, int col) const {
  int row_minus_1 = 0;
  int row_plus_1 = 0;
  int col_minus_1 = 0;
  int col_plus_1 = 0;
  if (row == 0) {row_minus_1 = height_ - 1;} 
  else {row_minus_1 = row - 1;}
  if (row == height_ - 1) {row_plus_1 = 0;} 
  else {row_plus_1 = row + 1;}
  if (col == 0) {col_minus_1 = width_ - 1;} 
  else {col_minus_1 = col - 1;}
  if (col == width_ - 1) {col_plus_1 = 0;} 
  else {col_plus_1 = col + 1;}
  int r_col = image_.GetPixel(row, col_minus_1).GetRed() - image_.GetPixel(row, col_plus_1).GetRed();
  int g_col = image_.GetPixel(row, col_minus_1).GetGreen() - image_.GetPixel(row, col_plus_1).GetGreen();
  int b_col = image_.GetPixel(row, col_minus_1).GetBlue() - image_.GetPixel(row, col_plus_1).GetBlue();
  int col_squared = r_col*r_col + g_col*g_col + b_col*b_col;
  int r_row = image_.GetPixel(row_minus_1, col).GetRed() - image_.GetPixel(row_plus_1, col).GetRed();
  int g_row = image_.GetPixel(row_minus_1, col).GetGreen() - image_.GetPixel(row_plus_1, col).GetGreen();
  int b_row = image_.GetPixel(row_minus_1, col).GetBlue() - image_.GetPixel(row_plus_1, col).GetBlue();
  int row_squared = r_row*r_row + g_row*g_row + b_row*b_row;
  return col_squared+row_squared; 
}

int** SeamCarver::FindVerticalMinPath() const {
  int** values = new int*[height_];
  for (int a = 0; a < height_; a++) {
    values[a] = new int[width_];
  }
  for (int i = 0; i < width_; i++) {
    values[height_-1][i] = GetEnergy(height_-1, i);
  }
  for (int row = height_-2; row >= 0; row--) {
    for (int col = 0; col < width_; col++) {
      int best = 0;
      if (col == 0) {
        best = std::min(values[row+1][col], values[row+1][col+1]);
      } else if (col == width_-1) {
        best = std::min(values[row+1][col], values[row+1][col-1]);
      } else {
        best = std::min(std::min(values[row+1][col-1], values[row+1][col]), values[row+1][col+1]);
      }
      values[row][col] = GetEnergy(row,col) + best;
    }
  }
  return values;
}

int* SeamCarver::GetVerticalSeam() const {
  int* vertical_seam = new int[height_];
  int** values = FindVerticalMinPath();
  int min_col = 0;
  int min_value = values[0][0];
  for (int c = 1; c < width_; c++) {
    if (values[0][c] < min_value) {min_col = c;min_value = values[0][c];}
  }
  vertical_seam[0] = min_col;
  int last_col = min_col;
  for (int row = 1; row < height_; row++) {
    int next_col = 0;
    if (last_col == 0) {
      if (values[row][last_col] > values[row][last_col+1]) {
        next_col = last_col + 1;
      } else {
        next_col = last_col;
      }
    } else if (last_col == width_-1) {
      if (values[row][last_col] > values[row][last_col-1]) {
        next_col = last_col - 1;
      } else {
        next_col = last_col;
      }
    } else {
      if (((values[row][last_col-1] == values[row][last_col+1]) && (values[row][last_col-1]
        < values[row][last_col])) || ((values[row][last_col-1] < values[row][last_col]) && (values[row][last_col-1] < values[row][last_col+1]))) {
        next_col = last_col-1;
      } else if (values[row][last_col+1] < values[row][last_col] && values[row][last_col+1] < values[row][last_col-1]) {
        next_col = last_col+1;
      } else {next_col = last_col;}
    }
    vertical_seam[row] = next_col;
    last_col = next_col;
  }
  for (int r = 0; r < height_; r++) {
    delete[] values[r];
  }
  delete[] values;
  return vertical_seam;
}

int** SeamCarver::FindHorizontalMinPath() const {
  int** values = new int*[height_];
  for (int a = 0; a < height_; a++) {
    values[a] = new int[width_];
  }
  for (int r = 0; r < height_; r++) {
    values[r][width_-1] = GetEnergy(r,width_-1);
  }
  for (int col = width_-2; col >= 0; col--) {
    for (int row = 0; row < height_; row++) {
      int best = 0;
      if (row == 0) {
        best = std::min(values[row][col+1], values[row+1][col+1]);
      } else if (row == height_-1) {
        best = std::min(values[row][col+1], values[row-1][col+1]);
      } else {
        best = std::min(std::min(values[row][col+1], values[row-1][col+1]), values[row+1][col+1]);
      }
      values[row][col] = best + GetEnergy(row,col);
    }
  }
  return values;
}

int* SeamCarver::GetHorizontalSeam() const {
  int* horizontal_seam = new int[width_];
  int** values = FindHorizontalMinPath();
  int min_row = 0;
  int min_value = values[0][0];
  for (int r = 1; r < height_; r++) {
    if (values[r][0] < min_value) {min_row = r;min_value = values[r][0];}
  }
  horizontal_seam[0] = min_row;
  int last_row = min_row;
  for (int col = 1; col < width_; col++) {
    int next_row = 0;
    if (last_row == 0) {
      if (values[last_row+1][col] < values[last_row][col]) {
        next_row = last_row+1;
      } else {next_row = last_row;}
    } else if (last_row == height_-1) {
      if (values[last_row-1][col] < values[last_row][col]) {
        next_row = last_row-1;
      } else {
        next_row = last_row;
      }
    } else {
      if (((values[last_row-1][col] == values[last_row+1][col]) && (values[last_row-1][col]
        < values[last_row][col])) || ((values[last_row-1][col] < values[last_row][col]) 
        && (values[last_row-1][col] < values[last_row+1][col]))) {
        next_row  = last_row-1;
      } else if (values[last_row+1][col] < values[last_row][col] && values[last_row+1][col]
        < values[last_row-1][col]) {
        next_row = last_row+1;
      } else {
        next_row = last_row;
      }
    }
    horizontal_seam[col] = next_row;
    last_row = next_row;
  }
  for (int i = 0; i < height_; i++) {delete[] values[i];}
  delete[] values;
  return horizontal_seam;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* horizontal_seam = GetHorizontalSeam();
  auto** new_pixel = new Pixel*[height_-1];
  for (int i = 0; i < height_-1; i++) {
    new_pixel[i] = new Pixel[width_];
  }
  int max_value = INT32_MIN;
  for (int col = 0; col < width_; col++) {
    for (int row = 0; row < height_-1; row++) {
      if (row >= horizontal_seam[col]) {
        new_pixel[row][col] = image_.GetPixel(row+1,col);
      } else {
        new_pixel[row][col] = image_.GetPixel(row,col);
      }
      if (new_pixel[row][col].GetRed() > max_value) {
        max_value = new_pixel[row][col].GetRed();
      }
      if (new_pixel[row][col].GetGreen() > max_value) {
        max_value = new_pixel[row][col].GetGreen();
      }
      if (new_pixel[row][col].GetBlue() > max_value) {
        max_value = new_pixel[row][col].GetBlue();
      }
    }
  }
  //image_.SetMaxColorValue(max_value);
  image_.SetPixels(new_pixel);
  image_.SetHeight(height_-1);
  height_ = height_-1;
  delete[] horizontal_seam;
}

void SeamCarver::RemoveVerticalSeam() {
  int* vertical_seam = GetVerticalSeam();
  auto** new_pixel = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    new_pixel[i] = new Pixel[width_-1];
  }
  int max_value = INT32_MIN;
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_-1; col++) {
      if (col >= vertical_seam[row]) {
        new_pixel[row][col] = image_.GetPixel(row,col+1);
      } else {
        new_pixel[row][col] = image_.GetPixel(row,col);
      }
      if (new_pixel[row][col].GetRed() > max_value) {
        max_value = new_pixel[row][col].GetRed();
      }
      if (new_pixel[row][col].GetGreen() > max_value) {
        max_value = new_pixel[row][col].GetGreen();
      }
      if (new_pixel[row][col].GetBlue() > max_value) {
        max_value = new_pixel[row][col].GetBlue();
      }
    }
  }
  image_.SetPixels(new_pixel);
  image_.SetWidth(width_-1);
  width_ = width_-1;
  delete[] vertical_seam;
}


SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
