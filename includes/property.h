#ifndef PROPERTY_H
#define PROPERTY_H

template <typename T>
class Property {
 public:
  Property() = default;
  ~Property() = default;
  T& operator= (const T& f) { return value = f; }
  const T operator() () const { return value; }
 private:
  explicit operator const T& () const { return value; }
  T* operator->() { return &value; }
 protected:
  T value;
};

#endif //PROPERTY_H
