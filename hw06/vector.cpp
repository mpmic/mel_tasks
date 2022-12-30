#include "vector.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <numeric>
#include <iterator>
namespace linalg {

/// Construct non-initialized vector with given size
Vector::Vector(std::size_t n) : data_{container(n)} {
}

/// Construct vector with given size and initialized with the given value
Vector::Vector(std::size_t n, float val) : data_{container(n, val)} {

}

///	Copy Constructor
Vector::Vector(Vector const &x) {
  data_ = x.data_;
}

/// Construct vector with initialize list
Vector::Vector(std::initializer_list<float> list) : data_{container(list)} {}

/// Assign the given value to the vector, all coefficients in the vector are
/// then equal to `val`
auto Vector::operator=(float val) -> Vector & {
  std::fill(data_.begin(), data_.end(), val);

  return *this;
}

/// Assign a value to the vector, all coefficients in the vector are then
/// equal to `val`
auto Vector::assign(float val) -> void {
  std::fill(data_.begin(), data_.end(), val);
}

/// Assign `v` to this vector. The size and all coefficients are then equal to
/// the coefficients of `v`.
auto Vector::assign(Vector v) -> void {
  data_ = v.data_;
}

/// Return the size of the vector
auto Vector::size() const -> std::size_t {
  return data_.size();
}

/// Return an begin iterator to the vector
auto Vector::begin() -> iterator {
  return data_.begin();
}

/// Return an end iterator to the vector
auto Vector::end() -> iterator {
  return data_.end();
}

/// Return an begin const_iterator to the vector
auto Vector::begin() const -> const_iterator {
  return data_.begin();
}

/// Return an end const_iterator to the vector
auto Vector::end() const -> const_iterator {
  return data_.end();
}

/// Return an begin const_iterator to the vector
auto Vector::cbegin() const -> const_iterator {
  return data_.cbegin();
}

/// Return an end const_iterator to the vector
auto Vector::cend() const -> const_iterator {
  return data_.cend();
}

/// Access a modifiable reference to the idx-th element of the vector.#
///
/// Additionally a python like behaviour access of negative indices should be
/// possible. I.e. `vec[-1]` should return the last value in the vector It
/// should only be wrapped once. I.e. given a vector of length 10, `vec[-15]`
/// does not need to be supported. Also note that accessing values above the
/// size of the vector is also undefined.
auto Vector::operator[](int idx) -> float & {
  size_t size_of_vector = data_.size();
  if (idx < 0) {
	return data_[size_of_vector + static_cast<unsigned long>(idx)];
  }
  return data_[static_cast<unsigned long>(idx)];
}

/// Access a non-modifiable reference to the idx-th element of the vector.
///
/// Additionally a python like behaviour access of negative indices should be
/// possible. I.e. `vec[-1]` should return the last value in the vector. It
/// should only be wrapped once. I.e. given a vector of length 10, `vec[-15]`
/// does not need to be supported. Also note that accessing values above the
/// size of the vector is also undefined.
auto Vector::operator[](int idx) const -> const float & {

  size_t size_of_vector = data_.size();
  if (idx < 0) {
	return data_[size_of_vector + static_cast<unsigned long>(idx)];
  }
  return data_[static_cast<unsigned long>(idx)];

}

/// Access a modifiable reference to the idx-th element of the vector. No
/// wrapping behaviour should be implemented.
///
/// Throw an `std::out_of_range` exception if the index out of bounds.

auto Vector::coeff(int idx) -> float & {

  if (idx < 0 || idx >= static_cast<int>(data_.size())) {
	throw std::out_of_range("Index out of range");
  }
  return data_[static_cast<unsigned long>(idx)];
}

/// Access a non-modifiable reference to the idx-th element of the vector. No
/// wrapping behaviour should be implemented.
///
/// Throw an `std::out_of_range` exception if the index out of bounds.
auto Vector::coeff(int idx) const -> const float & {

  if (idx < 0 || idx >= static_cast<int>(data_.size())) {
	throw std::out_of_range("Index out of range");
  }
  return data_[static_cast<unsigned long>(idx)];
}

/* In place operators, modify the given Vector in-place, rather than a copy */

/// Add a scalar value to the vector, i.e. for each coefficient `v_i` of the
/// vector, the values after this operators are `v_i + val`
auto Vector::operator+=(float val) -> Vector & {

  std::transform(data_.begin(), data_.end(), data_.begin(),
				 [&val](float elem) { return elem + val; });

  Vector &current = *this;
  return current;
}

/// Subtract a scalar value from the vector, i.e. for each coefficient `v_i`
/// of the vector, the values after this operators are `v_i - val`
auto Vector::operator-=(float val) -> Vector & {
  std::transform(data_.begin(), data_.end(), data_.begin(),
				 [&val](float elem) { return elem - val; });

  Vector &current = *this;
  return current;
}

/// Multiply vector with a scalar, i.e. for each coefficient `v_i`
/// of the vector, the values after this operators are `v_i * val`
auto Vector::operator*=(float val) -> Vector & {
  std::transform(data_.begin(), data_.end(), data_.begin(),
				 [&val](float elem) { return elem * val; });

  Vector &current = *this;
  return current;
}

/// Divide vector by a scalar, i.e. for each coefficient `v_i`
/// of the vector, the values after this operators are `v_i / val`
auto Vector::operator/=(float val) -> Vector & {
  std::transform(data_.begin(), data_.end(), data_.begin(),
				 [&val](float elem) { return elem / val; });

  Vector &current = *this;
  return current;
}

/// In-place addition of two vectors, given the coefficients `x_i` and `y_i`
/// of the vectors respectively, a new vector is returned, where the
/// coefficients are equal to `x_i` + `y_i`
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto Vector::operator+=(const Vector &y) -> Vector & {
  if (data_.size() != y.size()) {
	throw std::invalid_argument("Sizes don't match");
  }

  for (int i{0}; i < static_cast<int>(data_.size()); ++i) {
	data_[i] += y[i];
  }

  Vector &current = *this;
  return current;
}

/// In-place addition of two vectors, given the coefficients `x_i` and `y_i`
/// of the vectors respectively, a new vector is returned, where the
/// coefficients are equal to `x_i` - `y_i`
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto Vector::operator-=(const Vector &y) -> Vector & {
  if (data_.size() != y.size()) {
	throw std::invalid_argument("Sizes don't match");
  }

  for (int i{0}; i < static_cast<int>(data_.size()); ++i) {
	data_[i] -= y[i];
  }
  Vector &current = *this;
  return current;

}

/*
 *
 *
 * end of class
 */


/// Return the minimum value of Vector
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto min(const Vector &x) -> float {
  if (x.size() == 0) {
	throw std::invalid_argument("Empty Vector");
  }
  return *std::min_element(x.begin(), x.end());

}

/// Return the maximum value of Vector
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto max(const Vector &x) -> float {
  if (x.size() == 0) {
	throw std::invalid_argument("Empty Vector");
  }

  return *std::max_element(x.begin(), x.end());
}

/// Return the index into the vector of the minimum value of Vector
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto argmin(const Vector &x) -> std::size_t {
  if (x.size() == 0) {
	throw std::invalid_argument("Empty Vector");
  }
  auto arg_min = std::min_element(x.begin(), x.end());
  return static_cast<size_t>(std::distance(x.begin(), arg_min));
}

/// Return the index into the vector of the maximum value of Vector
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto argmax(const Vector &x) -> std::size_t {
  if (x.size() == 0) {
	throw std::invalid_argument("Empty Vector");

  }
  auto arg_max = std::max_element(x.begin(), x.end());
  return static_cast<size_t>(std::distance(x.begin(), arg_max));
}

/// Return the number of non-zero elements in the vector
auto non_zeros(const Vector &x) -> std::size_t {
  return x.size() - static_cast<unsigned long>(std::count(x.begin(), x.end(), 0));
}

/// Return the sum of the coefficients of the given vector
auto sum(const Vector &x) -> float {
  return static_cast<float>(std::reduce(x.begin(), x.end(), 0.0, [](float x, float y) -> float {
	return x + y;
  }));
}

/// Return the product of the coefficients of the given vector
auto prod(const Vector &x) -> float {
  return static_cast<float>(std::reduce(x.begin(), x.end(), 1, [](float x, float y) -> float {
	return x * y;
  }));
}

/// Return the dot product of the two vectors. i.e. the sum of products of the
/// coefficients: `sum(x_i * y_i) forall i in [0, x.size())`
///
/// Throw an `std::invalid_argument` exceptions, if the given vector is of a
/// different size
auto dot(const Vector &x, const Vector &y) -> float {
  if (x.size() != y.size()) {
	throw std::invalid_argument("Vector sizes don't match");
  }
  return static_cast<float>(std::inner_product(std::begin(x), std::end(x), std::begin(y), 0.0));
}

/// Return the euclidean norm of the vector. i.e. the sum of the square of the
/// coefficients: `sum(x_i * x_i) forall i in [0, x.size())`
auto norm(const Vector &x) -> float {
  return std::sqrt(dot(x, x));
}

/// Normalize the vector, i.e. the norm should be 1 after the normalization
auto normalize(Vector &x) -> void {
  x /= norm(x);
}

/// Return a normalized copy of the vector
auto normalized(const Vector &x) -> Vector {
  auto new_object = Vector(x);
  return new_object /= norm(new_object);
}

/// Return a copy for which every coefficient is the floored, i.e. `v_i =
/// floor(x_i)`
auto floor(const Vector &x) -> Vector {
  auto new_object = Vector(x);
  std::for_each(
	  new_object.begin(),
	  new_object.end(),
	  [](auto &item) {
		item = std::floor(item);
	  });

  return new_object;
}

/// Return a copy for which every coefficient is the ceiled, i.e. `v_i =
/// ceil(x_i)`
auto ceil(const Vector &x) -> Vector {
  auto new_object = Vector(x);
  std::for_each(
	  new_object.begin(),
	  new_object.end(),
	  [](auto &item) {
		item = std::ceil(item);
	  });

  return new_object;
}

/// Unary operator+, returns a copy of x
auto operator+(const Vector &x) -> Vector {
  auto new_object = Vector(x);
  return new_object;
}

/// Unary operator-, returns a copy of x, where all values are negated , i.e.
/// `v_i = -x_i`
auto operator-(const Vector &x) -> Vector {
  auto new_object = Vector(x);
  std::for_each(
	  new_object.begin(),
	  new_object.end(),
	  [](auto &&item) {
		item = -item;
	  });

  return new_object;
}

/// Return a vector, for which every coefficient is the sum of the coefficients
/// of the arguments
auto operator+(const Vector &x, const Vector &y) -> Vector {
  if (x.size() != y.size()) {
	throw std::invalid_argument("Vector sizes don't match");
  }
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), y.begin(), new_object.begin(), std::plus<>());

  return new_object;
}

/// Return a vector, for which every coefficient is the subtraction of the
/// coefficients of the arguments
auto operator-(const Vector &x, const Vector &y) -> Vector {

  if (x.size() != y.size()) {
	throw std::invalid_argument("Vector sizes don't match");
  }
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), y.begin(), new_object.begin(), std::minus<>());

  std::cout << new_object;
  return new_object;

}

/// Return a vector, which is the addition of each coefficient of the given
/// vector and the scalar
auto operator+(const Vector &x, float val) -> Vector {

  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), new_object.begin(),
				 [&val](float elem) { return elem + val; });

  return new_object;
}

/// Return a vector, which is the subtraction of each coefficient of the given
/// vector and the scalar
auto operator-(const Vector &x, float val) -> Vector {
  auto new_object = Vector(x);

  std::for_each(
	  new_object.begin(),
	  new_object.end(),
	  [&val](auto &&item) {
		item = item - val;
	  });

  return new_object;
}

/// Return a vector, which is the multiplication of each coefficient of the
/// given vector and the scalar
auto operator*(const Vector &x, float val) -> Vector {
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), new_object.begin(),
				 [&val](float elem) { return elem * val; });

  return new_object;
}

/// Return a vector, which is the division of each coefficient of the given
/// vector and the scalar
auto operator/(const Vector &x, float val) -> Vector {
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), new_object.begin(),
				 [&val](float elem) { return elem / val; });

  return new_object;
}

/// Return a vector, which is the addition of each coefficient of the given
/// vector and the scalar
auto operator+(float val, const Vector &x) -> Vector {
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), new_object.begin(),
				 [&val](float elem) { return elem + val; });

  return new_object;
}

/// Return a vector, which is the subtraction of each coefficient of the given
/// vector and the scalar
auto operator-(float val, const Vector &x) -> Vector {
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), new_object.begin(),
				 [&val](float elem) { return val - elem; });

  return new_object;
}

/// Return a vector, which is the multiplication of each coefficient of the
/// given vector and the scalar
auto operator*(float val, const Vector &x) -> Vector {
  auto new_object = Vector(x);

  std::transform(new_object.begin(), new_object.end(), new_object.begin(),
				 [&val](float elem) { return elem * val; });

  return new_object;
}

auto operator<<(std::ostream &ostr, const Vector &x) -> std::ostream & {
  ostr << "[ ";
  std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, ", "));
  ostr << "]";
  return ostr;
}
}

/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */
