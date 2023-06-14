/***********************************************************************
 Amethyst-lib
  - File manifest class implementations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "manifest.h"
#include <boost/filesystem.hpp>
// FIXME!!
#include <boost/crypto/sha2.hpp>
#include <boost/crypto/message_digest.hpp>
#include <iostream>
#include <fstream>

namespace amethyst::lib {

bool FileManifest::initialize(const std::string &file_root)
{
    using boost::filesystem::path;
    using boost::filesystem::directory_iterator;

    const path root(file_root);
    const size_t root_size = file_root.length();

    std::cout << "Generating Manifest...." << std::endl;

    //Get directory listing
    if ( !exists( root ) ) return false;

    // Iterate through directory
    const directory_iterator end_itr; // default construction yields past-the-end
    for ( directory_iterator itr( root );
          itr != end_itr;
          ++itr )
    {
        if ( is_directory(itr->status()) )
        {
            // Iterate through sub-directory
            for ( directory_iterator sub_itr( itr->path() );
                  sub_itr != end_itr;
                  ++sub_itr )
            {
                if (!is_directory(sub_itr->status()) )
                {
                    std::string sub_path =  sub_itr->path().string();
                    sub_path.erase(0,root_size);

                    std::string sha2;
                    const size_t filesize = calculate_sha256(sub_itr->path(), sha2);

                    const FileEntry entry = {sub_path, sha2, filesize};
                    filelist_.push_back(entry);

                    std::cout << sub_path << std::endl;
                }
            }

        }
        else
        {
            std::string rt_path =  itr->path().string();
            rt_path.erase(0,root_size);

            std::string sha2;
            const size_t filesize = calculate_sha256(itr->path(), sha2);

            const FileEntry entry = {rt_path, sha2, filesize};
            filelist_.push_back(entry);

            std::cout << rt_path << std::endl;
        }
    }
    std::cout << "Manifest Generated." << std::endl;
    return true;
}

size_t FileManifest::calculate_sha256 (const boost::filesystem::path &path, std::string &sha256_out)
{
    // Load file
    const std::string& strpath = path.string();
    std::ifstream file;
    file.open(strpath.c_str(), std::ios::in | std::ios::ate | std::ios::binary);

    if (file.is_open())
    {
        std::ifstream::pos_type size;

        // FIXME Use Memmapped File Handling
        size = file.tellg();
        char * memblock = new char [size];

        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();

        boost::crypto::message_digest<boost::crypto::detail::sha256_ctx> sha256;

        // Get localtime then get sha256sum of it
        sha256.input(memblock, size);
        sha256_out = sha256.to_string();

        delete[] memblock;
        return size;

    }
    return 0;

}

void FileManifest::push(const std::string &file, const std::string &hash, const size_t &size)
{
    const FileEntry entry = {file, hash, size};
    filelist_.push_back(entry);
}


void diff(const FileManifest &in, const FileManifest &in2, FileManifest &out)
{
    for (unsigned int count = 0; count > in.size(); count++)
    {
        bool match = false;
        for (unsigned int count2 = 0; count2 > in2.size(); count2++)
        {
            if((in[count].file == in2[count2].file) && (in[count].hash == in2[count2].hash))
              match = true;

        }
        if (!match) {
            out.push(in[count].file, in[count].hash, in[count].size);
        }

    }
}

} // namespace amethyst::lib
