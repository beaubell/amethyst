
//          Copyright Kevin Sopp 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CRYPTO_SHA2_HPP
#define BOOST_CRYPTO_SHA2_HPP

#include <boost/crypto/message_digest.hpp>
#include <boost/crypto/detail/sha2_ctx.hpp>

namespace boost {
namespace crypto {
  typedef message_digest<detail::sha224_ctx> sha224;
  typedef message_digest<detail::sha256_ctx> sha256;
  typedef message_digest<detail::sha384_ctx> sha384;
  typedef message_digest<detail::sha512_ctx> sha512;
}
}

#endif

