#pragma once

#include <cstddef>
#include <string>
#include <filesystem>
#include <istream>
#include <vector>
#include <mutex>

#include <boost/iostreams/device/mapped_file.hpp>

namespace amethyst {
namespace lib {

struct MemBuf : std::streambuf
{
    MemBuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};


class Resource {
  public:

    Resource(const char *start, const char *end, const std::filesystem::path &path, const std::string& filename);
    Resource(const std::filesystem::path &path, const std::string& filename, bool throws = true);
    Resource(const Resource&) = delete;
    Resource(Resource&&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource& operator=(Resource&&) = delete;
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

    auto getIStreamPtr() const -> std::shared_ptr<std::istream>;
    auto getSHA256() const -> const std::string&;

private:
    bool open_fs(const std::filesystem::path &path, const std::string& filename, bool throws = true);
    const char *mData;
    size_t mSize;
    std::string mName;
    mutable std::string mSha256sum;
    boost::iostreams::mapped_file_source mmFile;
    mutable std::mutex mMutex;
    mutable MemBuf mSBuf;
    mutable std::vector<std::weak_ptr<std::istream>> vistream_;
};

} // namespace lib
} // namespace amethyst

#define LOAD_RESOURCE(x,y,z) amethyst::lib::Resource(_binary_##x##_begin, _binary_##x##_end, y, z)
#define DECLARE_RESOURCE(x) extern const char *_binary_##x##_begin, *_binary_##x##_end


