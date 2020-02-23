#pragma once

#include <cstddef>
#include <string>
#include <filesystem>

#include <boost/iostreams/device/mapped_file.hpp>

namespace amethyst {
namespace lib {


class Resource {
public:
    Resource(const char *start, const char *end, const std::filesystem::path &path, const std::string& filename);
    virtual ~Resource();

    const char * const &data() const;
    const size_t &size() const;

    const char *begin() const;
    const char *end() const;
    const char &operator[](size_t idx) const;
    std::string to_str() const;
    std::string name() const;

private:
    const char *mData;
    size_t mSize;
    std::string mName;
    boost::iostreams::mapped_file_source mmFile;
};

} // namespace lib
} // namespace amethyst

#define LOAD_RESOURCE(x,y,z) amethyst::lib::Resource(&_binary_##x##_start, &_binary_##x##_end, y, z)

