#pragma once

#include <cstddef>
#include <string>
#include <filesystem>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace amethyst {
namespace lib {


class Resource {
public:
    typedef boost::iostreams::stream<boost::iostreams::array_source> ArrayStream;
    
    Resource(const char *start, const char *end, const std::filesystem::path &path, const std::string& filename);
    Resource(const std::filesystem::path &path, const std::string& filename, bool throws = true);
    virtual ~Resource();

    const char * const &data() const;
    const size_t &size() const;

    const char *begin() const;
    const char *end() const;
    const char &operator[](size_t idx) const;
    std::string to_str() const;
    std::string name() const;
    uint16_t getUInt16(size_t off) const;
    uint32_t getUInt32(size_t off) const;
    ArrayStream& get_stream() const;

private:
    Resource(const Resource&) = delete; // non construction-copyable
    Resource& operator=( const Resource& ) = delete; // non copyable
    bool open_fs(const std::filesystem::path &path, const std::string& filename, bool throws = true);
    const char *mData;
    size_t mSize;
    std::string mName;
    boost::iostreams::mapped_file_source mmFile;
    ArrayStream mmStream;
};

} // namespace lib
} // namespace amethyst

#define LOAD_RESOURCE(x,y,z) amethyst::lib::Resource(&_binary_##x##_start, &_binary_##x##_end, y, z)

