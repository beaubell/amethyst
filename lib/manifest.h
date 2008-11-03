#ifndef AMETHYST_LIB_MANIFEST_H
#define AMETHYST_LIB_MANIFEST_H

/***********************************************************************
 Amethyst-lib
  - File manifest class declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <string>
#include <list>
#include <boost/filesystem.hpp>

namespace amethyst
{
namespace lib
{

typedef struct
{
    std::string file;
    std::string hash;
    size_t size;

} fileentry;

class filemanifest
{
   public:
    filemanifest(void)
    {};
    filemanifest(const std::string &file_root)
    {
        initialize(file_root);
    };

    void clear();
    bool initialize(const std::string &fileroot);

    const fileentry &operator[](int pos) const
    {
        return filelist_[pos];
    }

    size_t size() const
    {
        return filelist_.size();
    }

    void push(const std::string &file, const std::string &hash, const size_t &size);

   private:
    size_t calculate_sha256 (const boost::filesystem::path &path, std::string &sha256_out);

    std::vector<fileentry> filelist_;
};

// Minus 'in2' from 'in' assign whats left to out 
void diff(const filemanifest &in, const filemanifest &in2, filemanifest &out);

} // namespace lib
} // namespace amethyst

#endif //AMETHSYT_LIB_MANIFEST_H
