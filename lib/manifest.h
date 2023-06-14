#ifndef AMETHYST_LIB_MANIFEST_H
#define AMETHYST_LIB_MANIFEST_H

/***********************************************************************
 Amethyst-lib
  - File manifest class declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)
 ***********************************************************************/

#include <string>
#include <list>
#include <boost/filesystem.hpp>

namespace amethyst::lib {

typedef struct
{
    std::string file;
    std::string hash;
    size_t size;

} FileEntry;

class FileManifest
{
   public:
    FileManifest() = default;
    explicit FileManifest(const std::string &file_root)
    {
        initialize(file_root);
    };

    void clear();
    bool initialize(const std::string &fileroot);

    const FileEntry &operator[](std::size_t pos) const
    {
        return filelist_[pos];
    }

    [[nodiscard]] std::size_t size() const
    {
        return filelist_.size();
    }

    void push(const std::string &file, const std::string &hash, const size_t &size);

   private:
    std::vector<FileEntry> filelist_;
};

// Minus 'in2' from 'in' assign whats left to out
void diff(const FileManifest &in, const FileManifest &in2, FileManifest &out);

} // namespace amethyst::lib

#endif //AMETHYST_LIB_MANIFEST_H
