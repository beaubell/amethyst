
#include "resource.h"

#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>

namespace amethyst {
namespace lib {

namespace fs = std::filesystem;
namespace bio = boost::iostreams; 

Resource::Resource(const char *start, const char *end, const fs::path &path, const std::string& filename)
    : mData(start),
      mSize(end - start),
      mName("Builtin: " + filename)
{

    // Attempt to load overidden resource, don't throw if fails since we fallback on builtin
    Resource(path, filename, false);    
    
    std::cout << "Resource: " << name() << " opened." << std::endl;
    
}

Resource::Resource(const fs::path &path, const std::string& filename, bool throws)
{

    fs::path filepath = path / filename;

    if (!fs::exists(filepath)) { 
        if (throws)
            throw std::runtime_error("Resource doesn't exist: " + std::string(filepath));
        
        return;
    }
    
    mmFile.open(filepath);
    
    if (!mmFile.is_open()) {
        if (throws)
            throw std::runtime_error("Unable to open resource: " + std::string(filepath));

        return;
    }
        
    mData = mmFile.begin();
    mSize = mmFile.size();
    
    mName = filepath;
    
    std::cout << "Resource: " << name() << " opened." << std::endl;
    
    
}

Resource::~Resource()
{
    mmFile.close();
    std::cout << "Resource: " << name() << " closed." << std::endl;
    
    if (name() == "") {
        //throw std::runtime_error("WHere");
    }
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

} // namespace lib
} // namespace amethyst
