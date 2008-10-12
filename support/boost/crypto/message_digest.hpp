
//          Copyright Kevin Sopp 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CRYPTO_MESSAGE_DIGEST_HPP
#define BOOST_CRYPTO_MESSAGE_DIGEST_HPP

#include <cstdio>
#include <cstring>
#include <ostream>
#include <stdexcept> // overflow_error
#include <sstream>
#include <string>
#include <boost/type_traits/is_arithmetic.hpp>


namespace boost {
namespace crypto {

template<class Context>
class message_digest
{
public:

  typedef Context context_type;
  typedef typename context_type::size_type size_type;

  static const int block_size    = context_type::block_size;
  static const int digest_length = context_type::digest_length;

  message_digest() : digest_calculated_(false) {}

  template<typename RandomAccessIterator>
  message_digest(RandomAccessIterator first, RandomAccessIterator last)
  {
    input(first, last);
  }

  explicit message_digest(const std::string& s)
  {
    input(s);
  }

  explicit message_digest(const std::wstring& s)
  {
    input(s);
  }

  template<typename charT, class traits, class alloc>
  explicit message_digest(const std::basic_string<charT, traits, alloc>& s)
  {
    input(s);
  }

  message_digest(const void* data, size_type len)
  {
    input(data, len);
  }

  message_digest(const message_digest&);

  ~message_digest()
  {
    ctx_.clear();
    std::memset(buffer_, 0, context_type::block_size);
  }

  message_digest& operator = (const message_digest&);

  const void* digest() const
  {
    if (!digest_calculated_)
      calculate_digest();
    return msg_digest_;
  }
  
  std::string to_string() const;

  template<typename RandomAccessIterator>
  void input(RandomAccessIterator first, RandomAccessIterator last)
  {
    input(&*first, last - first);
  }

  void input(const std::string& s)
  {
    input(s.begin(), s.end());
  }
  
  void input(const std::wstring& s)
  {
    input(s.begin(), s.end());
  }
  
  template<typename charT, class traits, class alloc>
  void input(const std::basic_string<charT, traits, alloc>& s)
  {
    input(s.begin(), s.end());
  }

  void input(const void* data, size_type len);
  
  // resets internal context
  void reset();

  // low level interface
  void process_block(const void* b)
  {
    digest_calculated_ = false;
    ctx_.process_block(b);
    ctx_.add_to_bit_count(block_size * 8);
  }
  
private:

  typedef typename context_type::element_type element_type;

  void calculate_digest() const;

  mutable context_type ctx_;
  element_type         buffer_    [context_type::block_size   ];
  mutable uint8_t      msg_digest_[context_type::digest_length];
  mutable bool         digest_calculated_;
};


template<class Context>
message_digest<Context>::message_digest(const message_digest& copy)
:
  ctx_(copy.ctx_),
  digest_calculated_(copy.digest_calculated_)
{
  std::memcpy(buffer_, copy.buffer_, context_type::block_size);
  std::memcpy(msg_digest_, copy.msg_digest_, context_type::digest_length);
}

template<class Context>
message_digest<Context>& message_digest<Context>::operator = (const message_digest<Context>& rhs)
{
  ctx_ = rhs.ctx_;
  std::memcpy(buffer_, rhs.buffer_, context_type::block_size);
  std::memcpy(msg_digest_, rhs.msg_digest_, context_type::digest_length);
  digest_calculated_ = rhs.digest_calculated_;
  return *this;
}

template<class Context>
std::string message_digest<Context>::to_string() const
{
  char buf[context_type::digest_length * 2];
  for (int i = 0; i < context_type::digest_length; ++i)
    std::sprintf(buf + i * 2, "%02x",
        static_cast<const unsigned char*>(digest())[i]);
  return std::string(buf, context_type::digest_length * 2);
}

template<class Context>
void message_digest<Context>::input(const void* vdata, size_type len)
{
  digest_calculated_ = false;
	if (!vdata)
		return;
  if (len * 8 < len)
    throw std::overflow_error("message too long");
  const uint8_t* data = static_cast<const uint8_t*>(vdata);
  
  size_type index = 0;                             // index into data array
  unsigned int bytes_in_buf = ctx_.bytes_in_buf();

  const unsigned int bs = context_type::block_size;

  // process any bytes that may still be left in the context from a previous
  // invocation
  if (bytes_in_buf)
  {
    const unsigned int bytes_to_copy =
      len > bs - bytes_in_buf ? bs - bytes_in_buf : len;
    std::memcpy(buffer_ + bytes_in_buf, data, bytes_to_copy);
    index += bytes_to_copy;
    ctx_.add_to_bit_count(bytes_to_copy * 8);
    if (len <= bs && len + bytes_in_buf >= bs)
    {
      ctx_.process_block(buffer_);
      ctx_.add_to_bit_count((len - index) * 8);
      bytes_in_buf = 0;
    }
    else
      bytes_in_buf += bytes_to_copy;
  }
  else
    ctx_.add_to_bit_count(len * 8);

  // now process the data in blocks
  for (; len - index >= bs; index += bs)
    ctx_.process_block(data + index);

  // copy remaining bytes into buffer
  const size_type remaining_bytes = len - index;
  std::memcpy(buffer_, data + index, remaining_bytes);
}

template<class Context>
void message_digest<Context>::calculate_digest() const
{
  const unsigned int bytes_in_buf = ctx_.bytes_in_buf();
  // TODO does padding size always equal buffer size?
  uint8_t padding[context_type::padding_size];
  std::memcpy(padding, buffer_, bytes_in_buf);
  padding[bytes_in_buf] = 0x80;
  const unsigned int pad_end = context_type::padding_size - context_type::input_length_size;
  std::memset(&padding[bytes_in_buf + 1], 0, context_type::padding_size - bytes_in_buf - 1);
  // check if there's enough room to store the bit_count
  if (bytes_in_buf >= pad_end)
  {
    ctx_.process_block(padding);
    std::memset(padding, 0, context_type::padding_size);
  }
  
  ctx_.store_bit_count(padding + pad_end);
  //for (int i = 0; i < context_type::padding_size; ++i)
  //  std::printf("%02x", padding[i]);

  ctx_.process_block(padding);
  ctx_.store_msg_digest(msg_digest_);
  digest_calculated_ = true;
}

template<class Context>
void message_digest<Context>::reset()
{
  ctx_.reset();
  digest_calculated_ = false;
}

template<class Context>
inline bool operator == (
    const message_digest<Context>& lhs, const message_digest<Context>& rhs)
{
  return !std::memcmp(lhs.digest(), rhs.digest(), Context::digest_length);
}

template<class Context>
inline bool operator != (
    const message_digest<Context>& lhs, const message_digest<Context>& rhs)
{
  return std::memcmp(lhs.digest(), rhs.digest(), Context::digest_length);
}

template<class Context>
inline bool operator == (
    const message_digest<Context>& m, const std::string& digest)
{
  return m.to_string() == digest;
}

template<class Context>
inline bool operator != (
    const message_digest<Context>& m, const std::string& digest)
{
  return m.to_string() != digest;
}

template<class Context>
inline bool operator == (
    const std::string& digest, const message_digest<Context>& m)
{
  return digest == m.to_string();
}

template<class Context>
inline bool operator != (
    const std::string& digest, const message_digest<Context>& m)
{
  return digest != m.to_string();
}

template<typename charT, class traits, class Context>
inline std::basic_ostream<charT, traits>&
operator << (std::basic_ostream<charT, traits>& out, const message_digest<Context>& m)
{
  return out << m.to_string();
}


} // namespace crypto
} // namespace boost

#endif

