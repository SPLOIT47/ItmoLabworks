#include <iostream>
#include "Int17Matrix3D.h"

Int17Matrix3D::Int17Matrix3D(int x, int y, int z) : width_(x), height_(y), depth_(z), array_(new int32_t[kNumLenght_ * x * y * z]){}

Int17Matrix3D::Int17Matrix3D(const Int17Matrix3D& other) : width_(other.width_), height_(other.height_), depth_(other.depth_), array_(new int32_t[kNumLenght_ * width_ * height_ * depth_]) {
    copy(array_, other.array_, kNumLenght_ * width_ * height_ * depth_);
}
Int17Matrix3D::~Int17Matrix3D() { delete[] array_; }

Int17Matrix3D::Proxy3D::Proxy3D(Int17Matrix3D& parentMatrix_, int x, int y, int z) : parentMatrix_(parentMatrix_), x_(x), y_(y), z_(z) {}

Int17Matrix3D::Proxy2D::Proxy2D(Int17Matrix3D& parentMatrix_, int x, int y) : parentMatrix_(parentMatrix_), x_(x), y_(y) {}

Int17Matrix3D::Proxy1D::Proxy1D(Int17Matrix3D& parentMatrix_, int x) : parentMatrix_(parentMatrix_), x_(x) {}

Int17Matrix3D::Proxy3D::operator int() const {
  return parentMatrix_.GetValueAtIndex(x_, y_, z_);
}

Int17Matrix3D& Int17Matrix3D::operator=(const Int17Matrix3D& other) {
  if (this != &other) {
    delete[] array_;
    width_ = other.width_;
    height_ = other.height_;
    depth_ = other.depth_;
    array_ = new int32_t[kNumLenght_ * width_ * height_ * depth_];
    copy(array_, other.array_, kNumLenght_ * width_ * height_ * depth_);
  }
  return *this;
}

Int17Matrix3D::Proxy3D& Int17Matrix3D::Proxy3D::operator=(int value) {
  parentMatrix_.SetValueAtIndex(x_, y_, z_, value);
  return *this;
}

Int17Matrix3D Int17Matrix3D::make_array(int x, int y, int z) {
  return Int17Matrix3D(x, y, z);
}

void Int17Matrix3D::copy(int32_t* array1, const int32_t* array2, int size) {
  for (int i = 0; i < size; ++i) {
    array1[i] = array2[i];
  }
}

Int17Matrix3D::Proxy1D Int17Matrix3D::operator[](int x) {
  if (x < 0 || x >= this->width_) {
    throw std::out_of_range("Index out of range in x-dimension");
  }
  return Proxy1D(*this, x);
}

Int17Matrix3D::Proxy2D Int17Matrix3D::Proxy1D::operator[](int y) {
  if (y < 0 || y >= parentMatrix_.height_) {
    throw std::out_of_range("Index out of range in y-dimension");
  }
  return Proxy2D(parentMatrix_, x_, y);
}

Int17Matrix3D::Proxy3D Int17Matrix3D::Proxy2D::operator[](int z) {
  if (z < 0 || z >= parentMatrix_.depth_) {
    throw std::out_of_range("Index out of range in z-dimension");
  }
  return Proxy3D(parentMatrix_, x_, y_, z);
}

Int17Matrix3D& Int17Matrix3D::SetValueAtIndex(int x, int y, int z, int32_t value) {
  int bit_index = GetIndex(x, y, z);
  for (int i = 0; i < kNumLenght_; ++i) {
    if (value & (1 << i)) {
      SetBit(array_, bit_index);
    } else {
      ClearBit(array_, bit_index);
    }
    bit_index++;
  }
  return *this;
}

int32_t Int17Matrix3D::GetValueAtIndex(int x, int y, int z) const {
  int32_t value = 0;
  int bit_index = GetIndex(x, y, z);
  for (int i = 0; i < kNumLenght_; ++i) {
    if (TakeBit(array_, bit_index)) {
      value |= (1 << i);
    }
    bit_index++;
  }
  return value;
}

bool Int17Matrix3D::TakeBit(const int32_t* value, uint32_t bit_position) const {
  int array_index = bit_position / 32;
  int bit_index = bit_position % 32;
  return ((value[array_index] >> bit_index) & 1) != 0;
}

Int17Matrix3D& Int17Matrix3D::ClearBit(int32_t* value, int32_t bit_position) {
  value[bit_position / 32] &= ~(1 << (bit_position % 32));
  return *this;
}

Int17Matrix3D& Int17Matrix3D::SetBit(int32_t* value, int32_t bit_position) {
  value[bit_position / 32] |= (1 << (bit_position % 32));
  return *this;
}

int Int17Matrix3D::GetIndex(int x, int y, int z) const {
  return (z * width_ * height_ + y * width_ + x) * kNumLenght_; 
}

Int17Matrix3D Int17Matrix3D::operator+(const Int17Matrix3D& other) {
  if (this->width_ != other.width_ || this->height_ != other.height_ || this->depth_ != other.depth_) {
    throw std::out_of_range("Arrays must be with the same size");
  }

  Int17Matrix3D result(this->width_, this->height_, this->depth_);

  for (int z = 0; z < this->depth_; ++z) {
    for (int y = 0; y < this->height_; ++y) {
      for (int x = 0; x < this->width_; ++x) {
        int index = this->GetIndex(x, y, z);
        result[x][y][z] = this->GetValueAtIndex(x, y, z) + other.GetValueAtIndex(x, y, z);
      }
    }
  }
  return result;
}

Int17Matrix3D Int17Matrix3D::operator-(const Int17Matrix3D& other) {
  if (this->width_ != other.width_ || this->height_ != other.height_ || this->depth_ != other.depth_) {
    throw std::out_of_range("Arrays must be with the same size");
  }

  Int17Matrix3D result(this->width_, this->height_, this->depth_);

  for (int z = 0; z < this->depth_; ++z) {
    for (int y = 0; y < this->height_; ++y) {
      for (int x = 0; x < this->width_; ++x) {
        int index = this->GetIndex(x, y, z);
        result[x][y][z] = this->GetValueAtIndex(x, y, z) - other.GetValueAtIndex(x, y, z);
      }
    }
  }
  return result;
}

Int17Matrix3D Int17Matrix3D::operator*(int32_t mult) {
  Int17Matrix3D result(this->width_, this->height_, this->depth_);

  for (int z = 0; z < this->depth_; ++z) {
    for (int y = 0; y < this->height_; ++y) {
      for (int x = 0; x < this->width_; ++x) {
        int index = this->GetIndex(x, y, z);
        result[x][y][z] = this->GetValueAtIndex(x, y, z) * mult;
      }
    }
  }
  return result;
}

bool Int17Matrix3D::operator==(const Int17Matrix3D& other) const{
  for (int z = 0; z < this->depth_; ++z) {
    for (int y = 0; y < this->height_; ++y) {
      for (int x = 0; x < this->width_; ++x) {
        int index = this->GetIndex(x, y, z);
        if (this->GetValueAtIndex(x, y, z) != other.GetValueAtIndex(x, y, z)) return false;
      }
    }
  }
  return true;
}

bool Int17Matrix3D::operator!=(const Int17Matrix3D& other) const{
  return !(operator==(other));
}

std::istream& operator>>(std::istream& stream, Int17Matrix3D& matrix) {
    int32_t value;
    for (int z = 0; z < matrix.depth_; ++z) {
        for (int y = 0; y < matrix.height_; ++y) {
            for (int x = 0; x < matrix.width_; ++x) {
                if (!(stream >> value)) {
                    return stream;
                }
                matrix.SetValueAtIndex(x, y, z, value);
            }
        }
    }
    return stream;
}