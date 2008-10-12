
//          Copyright Kevin Sopp 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define __STDC_CONSTANT_MACROS
#include <boost/cstdint.hpp>
#include <boost/crypto/detail/sha2_ctx.hpp>

namespace boost {
namespace crypto {
namespace detail {

const sha256_base::word_type sha256_base::K[64] = {
  UINT32_C(0x428a2f98), UINT32_C(0x71374491), UINT32_C(0xb5c0fbcf),
  UINT32_C(0xe9b5dba5), UINT32_C(0x3956c25b), UINT32_C(0x59f111f1),
  UINT32_C(0x923f82a4), UINT32_C(0xab1c5ed5), UINT32_C(0xd807aa98),
  UINT32_C(0x12835b01), UINT32_C(0x243185be), UINT32_C(0x550c7dc3),
  UINT32_C(0x72be5d74), UINT32_C(0x80deb1fe), UINT32_C(0x9bdc06a7),
  UINT32_C(0xc19bf174), UINT32_C(0xe49b69c1), UINT32_C(0xefbe4786),
  UINT32_C(0x0fc19dc6), UINT32_C(0x240ca1cc), UINT32_C(0x2de92c6f),
  UINT32_C(0x4a7484aa), UINT32_C(0x5cb0a9dc), UINT32_C(0x76f988da),
  UINT32_C(0x983e5152), UINT32_C(0xa831c66d), UINT32_C(0xb00327c8),
  UINT32_C(0xbf597fc7), UINT32_C(0xc6e00bf3), UINT32_C(0xd5a79147),
  UINT32_C(0x06ca6351), UINT32_C(0x14292967), UINT32_C(0x27b70a85),
  UINT32_C(0x2e1b2138), UINT32_C(0x4d2c6dfc), UINT32_C(0x53380d13),
  UINT32_C(0x650a7354), UINT32_C(0x766a0abb), UINT32_C(0x81c2c92e),
  UINT32_C(0x92722c85), UINT32_C(0xa2bfe8a1), UINT32_C(0xa81a664b),
  UINT32_C(0xc24b8b70), UINT32_C(0xc76c51a3), UINT32_C(0xd192e819),
  UINT32_C(0xd6990624), UINT32_C(0xf40e3585), UINT32_C(0x106aa070),
  UINT32_C(0x19a4c116), UINT32_C(0x1e376c08), UINT32_C(0x2748774c),
  UINT32_C(0x34b0bcb5), UINT32_C(0x391c0cb3), UINT32_C(0x4ed8aa4a),
  UINT32_C(0x5b9cca4f), UINT32_C(0x682e6ff3), UINT32_C(0x748f82ee),
  UINT32_C(0x78a5636f), UINT32_C(0x84c87814), UINT32_C(0x8cc70208),
  UINT32_C(0x90befffa), UINT32_C(0xa4506ceb), UINT32_C(0xbef9a3f7),
  UINT32_C(0xc67178f2) };                     
                     
const sha256_base::word_type sha256_base::S0[3] = { 2, 13, 22};
const sha256_base::word_type sha256_base::S1[3] = { 6, 11, 25};
const sha256_base::word_type sha256_base::s0[3] = { 7, 18,  3};
const sha256_base::word_type sha256_base::s1[3] = {17, 19, 10};

const sha224_impl::word_type sha224_impl::init_values[8] =
{
  UINT32_C(0xc1059ed8), UINT32_C(0x367cd507),
  UINT32_C(0x3070dd17), UINT32_C(0xf70e5939),
  UINT32_C(0xffc00b31), UINT32_C(0x68581511),
  UINT32_C(0x64f98fa7), UINT32_C(0xbefa4fa4)
};

const sha256_impl::word_type sha256_impl::init_values[8] =
{
  UINT32_C(0x6a09e667), UINT32_C(0xbb67ae85),
  UINT32_C(0x3c6ef372), UINT32_C(0xa54ff53a),
  UINT32_C(0x510e527f), UINT32_C(0x9b05688c),
  UINT32_C(0x1f83d9ab), UINT32_C(0x5be0cd19)
};


const sha512_base::word_type sha512_base::K[80] = {
  UINT64_C(0x428a2f98d728ae22), UINT64_C(0x7137449123ef65cd),
  UINT64_C(0xb5c0fbcfec4d3b2f), UINT64_C(0xe9b5dba58189dbbc),
  UINT64_C(0x3956c25bf348b538), UINT64_C(0x59f111f1b605d019),
  UINT64_C(0x923f82a4af194f9b), UINT64_C(0xab1c5ed5da6d8118),
  UINT64_C(0xd807aa98a3030242), UINT64_C(0x12835b0145706fbe),
  UINT64_C(0x243185be4ee4b28c), UINT64_C(0x550c7dc3d5ffb4e2),
  UINT64_C(0x72be5d74f27b896f), UINT64_C(0x80deb1fe3b1696b1),
  UINT64_C(0x9bdc06a725c71235), UINT64_C(0xc19bf174cf692694),
  UINT64_C(0xe49b69c19ef14ad2), UINT64_C(0xefbe4786384f25e3),
  UINT64_C(0x0fc19dc68b8cd5b5), UINT64_C(0x240ca1cc77ac9c65),
  UINT64_C(0x2de92c6f592b0275), UINT64_C(0x4a7484aa6ea6e483),
  UINT64_C(0x5cb0a9dcbd41fbd4), UINT64_C(0x76f988da831153b5),
  UINT64_C(0x983e5152ee66dfab), UINT64_C(0xa831c66d2db43210),
  UINT64_C(0xb00327c898fb213f), UINT64_C(0xbf597fc7beef0ee4),
  UINT64_C(0xc6e00bf33da88fc2), UINT64_C(0xd5a79147930aa725),
  UINT64_C(0x06ca6351e003826f), UINT64_C(0x142929670a0e6e70),
  UINT64_C(0x27b70a8546d22ffc), UINT64_C(0x2e1b21385c26c926),
  UINT64_C(0x4d2c6dfc5ac42aed), UINT64_C(0x53380d139d95b3df),
  UINT64_C(0x650a73548baf63de), UINT64_C(0x766a0abb3c77b2a8),
  UINT64_C(0x81c2c92e47edaee6), UINT64_C(0x92722c851482353b),
  UINT64_C(0xa2bfe8a14cf10364), UINT64_C(0xa81a664bbc423001),
  UINT64_C(0xc24b8b70d0f89791), UINT64_C(0xc76c51a30654be30),
  UINT64_C(0xd192e819d6ef5218), UINT64_C(0xd69906245565a910),
  UINT64_C(0xf40e35855771202a), UINT64_C(0x106aa07032bbd1b8),
  UINT64_C(0x19a4c116b8d2d0c8), UINT64_C(0x1e376c085141ab53),
  UINT64_C(0x2748774cdf8eeb99), UINT64_C(0x34b0bcb5e19b48a8),
  UINT64_C(0x391c0cb3c5c95a63), UINT64_C(0x4ed8aa4ae3418acb),
  UINT64_C(0x5b9cca4f7763e373), UINT64_C(0x682e6ff3d6b2b8a3),
  UINT64_C(0x748f82ee5defb2fc), UINT64_C(0x78a5636f43172f60),
  UINT64_C(0x84c87814a1f0ab72), UINT64_C(0x8cc702081a6439ec),
  UINT64_C(0x90befffa23631e28), UINT64_C(0xa4506cebde82bde9),
  UINT64_C(0xbef9a3f7b2c67915), UINT64_C(0xc67178f2e372532b),
  UINT64_C(0xca273eceea26619c), UINT64_C(0xd186b8c721c0c207),
  UINT64_C(0xeada7dd6cde0eb1e), UINT64_C(0xf57d4f7fee6ed178),
  UINT64_C(0x06f067aa72176fba), UINT64_C(0x0a637dc5a2c898a6),
  UINT64_C(0x113f9804bef90dae), UINT64_C(0x1b710b35131c471b),
  UINT64_C(0x28db77f523047d84), UINT64_C(0x32caab7b40c72493),
  UINT64_C(0x3c9ebe0a15c9bebc), UINT64_C(0x431d67c49c100d4c),
  UINT64_C(0x4cc5d4becb3e42b6), UINT64_C(0x597f299cfc657e2a),
  UINT64_C(0x5fcb6fab3ad6faec), UINT64_C(0x6c44198c4a475817) };

const sha512_base::word_type sha512_base::S0[3] = {28, 34, 39};
const sha512_base::word_type sha512_base::S1[3] = {14, 18, 41};
const sha512_base::word_type sha512_base::s0[3] = { 1,  8,  7};
const sha512_base::word_type sha512_base::s1[3] = {19, 61,  6};

const sha384_impl::word_type sha384_impl::init_values[8] =
{
  UINT64_C(0xcbbb9d5dc1059ed8),
  UINT64_C(0x629a292a367cd507),
  UINT64_C(0x9159015a3070dd17),
  UINT64_C(0x152fecd8f70e5939),
  UINT64_C(0x67332667ffc00b31),
  UINT64_C(0x8eb44a8768581511),
  UINT64_C(0xdb0c2e0d64f98fa7),
  UINT64_C(0x47b5481dbefa4fa4)
};

const sha512_impl::word_type sha512_impl::init_values[8] =
{
  UINT64_C(0x6a09e667f3bcc908),
  UINT64_C(0xbb67ae8584caa73b),
  UINT64_C(0x3c6ef372fe94f82b),
  UINT64_C(0xa54ff53a5f1d36f1),
  UINT64_C(0x510e527fade682d1),
  UINT64_C(0x9b05688c2b3e6c1f),
  UINT64_C(0x1f83d9abfb41bd6b),
  UINT64_C(0x5be0cd19137e2179)
};


} // namespace detail
} // namespace crypto
} // namespace boost

