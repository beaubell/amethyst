#pragma once

#include <cstddef>
#include <string>

class Resource {
public:
    Resource(const char *start, const char *end, const std::string &path, const std::string& filename)
    : mData(start),
      mSize(end - start),
      mName("Builtin: " + filename)
    {
        // TODO, Attempt to load file from resource folder else use bultin.

    }

    const char * const &data() const { return mData; }
    const size_t &size() const { return mSize; }

    const char *begin() const { return mData; }
    const char *end() const { return mData + mSize; }
    const char &operator[](size_t idx) const { return mData[idx]; }
    std::string to_str() const { return std::string(begin(),end()); }
    std::string name() const { return mName; }

private:
    const char *mData;
    size_t mSize;
    std::string mName;
};

#define LOAD_RESOURCE(x,y,z) Resource(&_binary_##x##_start, &_binary_##x##_end, y, z)

