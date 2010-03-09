
//          Copyright Kevin Sopp 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Based on Federal Information Processing Standard (FIPS) PUB 180-3

// All hash functions of the SHA-2 family are very similar so we encapsulate the
// differences in classes and pass these as template parameter to sha2_ctx.
// The difference between SHA-224 and SHA-256 are the initialization constants
// and the shorter message digest for SHA-224, the same goes for SHA-384 and
// SHA-512.
// SHA-256 and SHA-512 differ in word_type, buffer sizes and constants used.
// The algorithm for the hash function stays the same though.

#ifndef BOOST_CRYPTO_DETAIL_SHA2_CTX_HPP
#define BOOST_CRYPTO_DETAIL_SHA2_CTX_HPP

#include <cstring>
#include <boost/cstdint.hpp>
#include <boost/crypto/detail/md_utils.hpp>


namespace boost {
namespace crypto {
namespace detail {

struct sha256_base
{
  typedef boost::uint32_t word_type;

  static const int message_schedule_length = 64;
  static const int block_size = 64;
  static const int padding_size = 64;
  static const int input_length_size = 8;
  static const int bit_count_size = 1;

  static const word_type K[64];

  // values for Sigma and sigma functions
  static const word_type S0[3];
  static const word_type S1[3];
  static const word_type s0[3];
  static const word_type s1[3];
};

struct sha512_base
{
  typedef boost::uint64_t word_type;

  static const int message_schedule_length = 80;
  static const int block_size = 128;
  static const int padding_size = 128;
  static const int input_length_size = 16;
  static const int bit_count_size = 2;

  static const word_type K[80];

  // values for Sigma and sigma functions
  static const word_type S0[3];
  static const word_type S1[3];
  static const word_type s0[3];
  static const word_type s1[3];
};


struct sha224_impl : sha256_base
{
  static const int digest_length = 28;
  static const word_type init_values[8];
};

struct sha256_impl : sha256_base
{
  static const int digest_length = 32;
  static const word_type init_values[8];
};

struct sha384_impl : sha512_base
{
  static const int digest_length = 48;
  static const word_type init_values[8];
};

struct sha512_impl : sha512_base
{
  static const int digest_length = 64;
  static const word_type init_values[8];
};


template<class Impl>
struct sha2_ctx
{
  static const int digest_length = Impl::digest_length;
  static const int block_size = Impl::block_size;
  static const int padding_size = Impl::padding_size;
  static const int input_length_size = Impl::input_length_size;
  static const int bit_count_size = Impl::bit_count_size;
  static const int message_schedule_length = Impl::message_schedule_length;

  typedef boost::uint8_t  uint8_t;
  typedef boost::uint32_t uint32_t;
  typedef boost::uint64_t uint64_t;
  typedef boost::uint8_t  element_type;
  typedef typename Impl::word_type word_type;
  static const int word_type_size = sizeof(word_type);
  typedef boost::uint64_t size_type;

  sha2_ctx();

  sha2_ctx(const sha2_ctx&);
  
  sha2_ctx& operator = (const sha2_ctx&);

  void reset();
  void clear();

  void process_block(const void* msg);
  void store_msg_digest(void* digest) const;
  void store_bit_count(void* dst) const;
  
  void add_to_bit_count(size_type x)
  {
    bit_count_.add(x);
  }
  
  unsigned int bytes_in_buf() const
  {
    return bit_count_[0] / 8 % block_size;
  }

  unsigned int bits_in_buf() const
  {
    return bit_count_[0] % block_size;
  }

  static word_type rotate_right(word_type x, word_type bits)
  {
    return (x >> bits) | (x << (word_type_size * 8 - bits));
  }

  static word_type Ch(word_type x, word_type y, word_type z)
  {
    return (x & y) ^ (~x & z);
  }

  static word_type Maj(word_type x, word_type y, word_type z)
  {
    return (x & y) ^ (x & z) ^ (y & z);
  }

  static word_type Sigma(word_type x, const word_type y[3])
  {
    return rotate_right(x, y[0]) ^ rotate_right(x, y[1]) ^ rotate_right(x, y[2]);
  }

  static word_type sigma(word_type x, const word_type y[3])
  {
    return rotate_right(x, y[0]) ^ rotate_right(x, y[1]) ^ (x >> y[2]);
  }

  word_type H[8];
  bit_count<size_type, bit_count_size> bit_count_;
};


template<class Impl>
sha2_ctx<Impl>::sha2_ctx()
{
  std::memcpy(H, Impl::init_values, sizeof(H));
}

template<class Impl>
sha2_ctx<Impl>::sha2_ctx(const sha2_ctx<Impl>& copy)
:
  bit_count_(copy.bit_count_)
{
  std::memcpy(H, copy.H, sizeof(H));
}

template<class Impl>
sha2_ctx<Impl>& sha2_ctx<Impl>::operator = (const sha2_ctx<Impl>& rhs)
{
  bit_count_ = rhs.bit_count_;
  std::memcpy(H, rhs.H, sizeof(H));
  return *this;
}

template<class Impl>
void sha2_ctx<Impl>::reset()
{
  std::memcpy(H, Impl::init_values, sizeof(H));
  bit_count_ = 0;
}

template<class Impl>
void sha2_ctx<Impl>::clear()
{
  reset();
}

template<class Impl>
void sha2_ctx<Impl>::process_block(const void* msg)
{
  word_type W[message_schedule_length];

  native_to_big_endian<word_type>(W, msg, 16 * sizeof(word_type));

  for (int t = 16; t < message_schedule_length; ++t)
    W[t] = sigma(W[t -  2], Impl::s1) + W[t - 7]
         + sigma(W[t - 15], Impl::s0) + W[t - 16];
  
  word_type Y[8];
  std::memcpy(Y, H, sizeof(Y));

  for (int t = 0; t < message_schedule_length; ++t)
  {
    word_type T1 = Y[7] + Sigma(Y[4], Impl::S1) + Ch(Y[4], Y[5], Y[6])
                 + Impl::K[t] + W[t];
    word_type T2 = Sigma(Y[0], Impl::S0) + Maj(Y[0], Y[1], Y[2]);
    Y[7] = Y[6];
    Y[6] = Y[5];
    Y[5] = Y[4];
    Y[4] = Y[3] + T1;
    Y[3] = Y[2];
    Y[2] = Y[1];
    Y[1] = Y[0];
    Y[0] = T1 + T2;
  }

  for (int i = 0; i < 8; ++i)
    H[i] += Y[i];
}

template<class Impl>
void sha2_ctx<Impl>::store_msg_digest(void* digest) const
{
  native_to_big_endian<word_type>(digest, H, digest_length);
}

template<class Impl>
void sha2_ctx<Impl>::store_bit_count(void* dst) const
{
  bit_count_.store_as_big_endian(dst);
}


typedef sha2_ctx<sha224_impl> sha224_ctx;
typedef sha2_ctx<sha256_impl> sha256_ctx;
typedef sha2_ctx<sha384_impl> sha384_ctx;
typedef sha2_ctx<sha512_impl> sha512_ctx;


} // namespace detail
} // namespace crypto
} // namespace boost

#endif

