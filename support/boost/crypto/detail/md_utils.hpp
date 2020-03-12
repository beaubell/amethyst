
//          Copyright Kevin Sopp 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CRYPTO_DETAIL_MD_UTILS_HPP
#define BOOST_CRYPTO_DETAIL_MD_UTILS_HPP

#include <cstring> // memcpy, memset
#include <stdexcept>
#include <boost/predef/other/endian.h>

namespace boost {
namespace crypto {
namespace detail {

template<typename T>
void native_to_big_endian(void* vdst, const void* vsrc, int n)
{
  #if (BOOST_ENDIAN_BIG_BYTE > 0)
    std::memcpy(vdst, vsrc, n);
  #elif (BOOST_ENDIAN_LITTLE_BYTE > 0)
    T* dst = static_cast<T*>(vdst);
    const unsigned char* src = static_cast<const unsigned char*>(vsrc);
    const int size = sizeof(T);
    for (int i = 0; i < n / size; ++i)
    {
      dst[i] = static_cast<T>(src[i * size]) << 8 * (size - 1);
      for (int j = 1; j < size; ++j)
        dst[i] |= static_cast<T>(src[i * size + j])
               << 8 * (size - j - 1);
    }
  #else
    #error unsupported endianness
  #endif
}

template<typename T>
void native_to_little_endian(void* vdst, const void* vsrc, int n)
{
  #if (BOOST_ENDIAN_LITTLE_BYTE > 0)
    std::memcpy(vdst, vsrc, n);
  #elif (BOOST_ENDIAN_BIG_BYTE > 0)
    T* dst = static_cast<T*>(vdst);
    const unsigned char* src = static_cast<const unsigned char*>(vsrc);
    const int size = sizeof(T);
    for (int i = 0; i < n / size; ++i)
    {
      dst[i] = static_cast<T>(src[i * size]);
      for (int j = 1; j < size; ++j)
        dst[i] |= (T)src[i * size + j] << 8 * j;
    }
  #else
    #error unsupported endianness
  #endif
}


// bit count with little endian layout
// this is used so that multiword bitcounts are handled correctly
// and safely (resistant against timing attacks)
// N > 0
template<typename T, int N>
struct bit_count
{
  typedef T value_type;
  static const int elements = N;

  bit_count()
  {
    std::memset(data_, 0, sizeof(data_));
  }

  bit_count(const T& x)
  {
    data_[0] = x;
    std::memset(data_ + 1, 0, sizeof(data_) - sizeof(T));
  }

  bit_count(const bit_count& copy)
  {
    std::memcpy(data_, copy.data_, sizeof(data_));
  }

  ~bit_count()
  {
    std::memset(data_, 0, sizeof(T) * N);
  }

  bit_count& operator = (const bit_count& rhs)
  {
    std::memcpy(data_, rhs.data_, sizeof(data_));
    return *this;
  }

  void add(T x)
  {
    T carry = 0;
    if ((data_[0] += x) < x)
      carry = 1;
    // ripple carry
    for (int i = 1; i < N; ++i)
    {
      T tmp = data_[i];
      if ((data_[i] += carry) < tmp)
        carry = 1;
      else
        carry = 0;
    }
    // wrap around
    data_[0] += carry;
    if(carry)
      throw std::overflow_error("message too long");
  }

  T&       operator [] (unsigned int n)       { return data_[n]; }
  const T& operator [] (unsigned int n) const { return data_[n]; }

  T*       data()       { return data_; }
  const T* data() const { return data_; }

  void store_as_big_endian(void* vdst) const
  {
    T* dst = static_cast<T*>(vdst);
    dst += N-1;
    int i = 0;
    while (i < N)
    {
      boost::crypto::detail::native_to_big_endian<T>(
          dst, &data_[i], sizeof(T));
      ++i;
      --dst;
    }
  }

  void store_as_little_endian(void* vdst) const
  {
    T* dst = static_cast<T*>(vdst);
    int i = 0;
    while (i < N)
    {
      boost::crypto::detail::native_to_little_endian<T>(
          dst, &data_[i], sizeof(T));
      ++i;
      ++dst;
    }
  }

private:

  T data_[N];
};



} // namespace detail
} // namespace crypto
} // namespace boost

#endif
