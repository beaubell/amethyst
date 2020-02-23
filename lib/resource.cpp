
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

    fs::path filepath = path / filename;
    //FIXME
    //std::cout << filepath << std::string(fs::exists(filepath)?" Exists":" Not Found") << std::endl;

    if (fs::exists(filepath)) {
        mmFile.open(filepath);
        
        if (mmFile.is_open()) {
            mData = mmFile.begin();
            mSize = mmFile.size();
            
            mName = filepath;
            
            //std::cout << to_str() << std::endl;
        }
        
    }
    
    std::cout << "Resource: " << name() << " opened." << std::endl;
    
    
}

Resource::~Resource()
{
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


} // namespace lib
} // namespace amethyst
