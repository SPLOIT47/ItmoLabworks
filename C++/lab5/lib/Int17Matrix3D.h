#include <iostream>

class Int17Matrix3D {
public:

  Int17Matrix3D(int x, int y, int z);

  Int17Matrix3D(const Int17Matrix3D& other);

  ~Int17Matrix3D();

  Int17Matrix3D& operator=(int32_t value);  

  Int17Matrix3D& operator=(const Int17Matrix3D& other);

  static Int17Matrix3D make_array(int x, int y, int z);

  void SetValueAtCurrentIndex(int32_t value);

  class Proxy3D;
  class Proxy2D;
  class Proxy1D;

  Proxy1D operator[](int x);

  Int17Matrix3D operator+(const Int17Matrix3D& other);

  Int17Matrix3D operator-(const Int17Matrix3D& other);

  Int17Matrix3D operator*(int32_t mult);

  bool operator==(const Int17Matrix3D& other) const;

  bool operator!=(const Int17Matrix3D& other) const;

  friend std::istream& operator>>(std::istream& stream, Int17Matrix3D& array);

private:
  const int8_t kNumLenght_ = 17;
  int32_t width_;
  int32_t height_;
  int32_t depth_;
  int32_t* array_;

  void copy(int32_t* array1, const int32_t* array2, int size);

  Int17Matrix3D& SetValueAtIndex(int x, int y, int z, int32_t value);

  int32_t GetValueAtIndex(int x, int y, int z) const;

  bool TakeBit(const int32_t* value, uint32_t bit_position) const;

  Int17Matrix3D& ClearBit(int32_t* value, int32_t bit_position);

  Int17Matrix3D& SetBit(int32_t* value, int32_t bit_position);

  int GetIndex(int x, int y, int z) const;
};

class Int17Matrix3D::Proxy3D {
    Int17Matrix3D& parentMatrix_;
    int x_, y_, z_;

public:
    Proxy3D(Int17Matrix3D& parentMatrix_, int x, int y, int z);

    Proxy3D& operator=(int value);

    operator int() const;
};

class Int17Matrix3D::Proxy2D {
  Int17Matrix3D& parentMatrix_;
  int x_, y_;

public:
  Proxy2D(Int17Matrix3D& parentMatrix_, int x, int y);

  Proxy3D operator[](int z);
};

class Int17Matrix3D::Proxy1D {
  Int17Matrix3D& parentMatrix_;
  int x_;

public:
  Proxy1D(Int17Matrix3D& parentMatrix_, int x);

  Proxy2D operator[](int y);
};


