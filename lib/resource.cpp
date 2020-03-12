
#include "resource.h"

#include <iostream> // FIXME
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/crypto/sha2.hpp>
#include <boost/crypto/message_digest.hpp>

namespace amethyst {
namespace lib {

namespace fs = std::filesystem;
namespace bio = boost::iostreams; 
using Sha256 = boost::crypto::message_digest<boost::crypto::detail::sha256_ctx>;



Resource::Resource(const char *start, const char *end, const fs::path &path, const std::string& filename)
    : mData(start),
      mSize(end - start),
      mName("Builtin: " + filename),
      mSBuf(nullptr, nullptr)
{

    // Attempt to load overidden resource, don't throw if fails since we fallback on builtin
    open_fs(path, filename, false);    
    mmStream.open(begin(), size());

    mSBuf = MemBuf(const_cast<char *>(begin()), const_cast<char *>(begin()+mSize));
    //std::istream in(&sbuf);

    std::cout << "Resource: " << name() << " opened." << std::endl;
}

Resource::Resource(const fs::path &path, const std::string& filename, bool throws)
: mData(nullptr),
  mSize(0),
  mName(),
  mSBuf(nullptr, nullptr)
{
    open_fs(path, filename, true);
    mmStream.open(begin(), size());

    mSBuf = MemBuf(const_cast<char *>(begin()), const_cast<char *>(begin()+mSize));

    std::cout << "Resource: " << name() << " opened." << std::endl;
}

bool
Resource::open_fs(const fs::path &path, const std::string& filename, bool throws)
{

    fs::path filepath = path / filename;

    if (!fs::exists(filepath)) { 
        if (throws)
            throw std::runtime_error("Resource doesn't exist: " + std::string(filepath));

        return false;
    }

    mmFile.open(filepath);

    if (!mmFile.is_open()) {
        if (throws)
            throw std::runtime_error("Unable to open resource: " + std::string(filepath));

        return false;
    }

    mData = mmFile.begin();
    mSize = mmFile.size();

    mName = filepath;

    return true;    
}

Resource::~Resource()
{
    // Check stream references for open instances
    for (auto& s_ptr: vistream_) {
        if (!s_ptr.expired())
            std::cout << "ERROR! Stream open while resource teardown! " << name() << std::endl;
    }

    mmFile.close();
    std::cout << "Resource: " << name() << " closed." << std::endl;

}


const char * const &
Resource::data() const {
    return mData;
}


const size_t &
Resource::size() const {
    return mSize;
}


const char *
Resource::begin() const {
    return mData;
}


const char *
Resource::end() const {
    return mData + mSize;
}


const char &
Resource::operator[](size_t idx) const {
    if (idx >= size()) {
        throw std::runtime_error("Buffer Overflow accessing: " + name() + ", size:1 offset: " + std::to_string(idx) + "/" + std::to_string(size()));
    }
    return mData[idx];
}


std::string 
Resource::to_str() const {
    return std::string(begin(),size());
}


std::string
Resource::name() const {
    return mName;
}

uint16_t
Resource::getUInt16(size_t off) const {
    if ((off + sizeof(uint16_t)) >= size()) {
        throw std::runtime_error("Buffer Overflow accessing: " + name() + ", size:2, offset: " + std::to_string(off) + "/" + std::to_string(size()));
    }

    return *(uint16_t*)(mData + off);
}

uint32_t
Resource::getUInt32(size_t off) const {
    if ((off + sizeof(uint32_t)) >= size()) {
        throw std::runtime_error("Buffer Overflow accessing: " + name() + ", size:4, offset: " + std::to_string(off) + "/" + std::to_string(size()));
    }

    return *(uint32_t*)(mData + off);
}

Resource::ArrayStream&
Resource::get_stream() const {
    return const_cast<ArrayStream&>(mmStream);
}


std::shared_ptr<std::istream>
Resource::get_istream() const {

    std::shared_ptr<std::istream> is_ptr = std::make_shared<std::istream>(&mSBuf);
    std::weak_ptr<std::istream> wptr = is_ptr;
    vistream_.push_back(wptr);
    return is_ptr;
}


const std::string&
Resource::getSHA256() const {

    std::lock_guard<std::mutex> g(mMutex);

    // Return calculated sum if it already exists
    if (!mSha256sum.empty())
        return mSha256sum;

    Sha256 hash;

    hash.input(begin(), size());
    mSha256sum = hash.to_string();

    return mSha256sum;
}


} // namespace lib
} // namespace amethyst
