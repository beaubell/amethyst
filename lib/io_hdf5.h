#ifndef AMETHYST_IO_HDF5_H
#define AMETHYST_IO_HDF5_H

/***********************************************************************
 Amethyst-lib
  - HDF5 Class declarations

 Authors (c):
 2010-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "types.h"

#include <H5Cpp.h>
#include <string>


namespace amethyst {
namespace lib {


class HDF5 : public H5::H5File
{
  public:
    HDF5(const std::string &filename_in, const unsigned int flags);
    virtual ~HDF5();

    const std::string &filename() const;

    unsigned int read_scalar_uint(const H5std_string &dataset_name) const;
    float read_scalar_float(const H5std_string &dataset_name) const;

    void  write_scalar_uint(const H5std_string &dataset_name, unsigned int out);
    void  write_scalar_float(const H5std_string &dataset_name, float out);
    void  write_scalar_double(const H5std_string &dataset_name, double out);

    std::string read_string(const H5std_string &dataset_name) const;
    void        write_string(const H5std_string &dataset_name, const std::string &out);

    void read_1D_double(const H5std_string &dataset_name, std::vector<double> &in) const;

    void write_1D_string(const H5std_string &dataset_name, const std::vector<std::string> &out);
    void write_1D_double(const H5std_string &dataset_name, const std::vector<double> &out);

    void read_2D_uint(const H5std_string &dataset_name, Am2DArrayUI &in) const;
    void read_3D_uint(const H5std_string &dataset_name, Am3DArrayUI &in) const;
    void read_4D_uint(const H5std_string &dataset_name, Am4DArrayUI &in) const;

    void write_2D_uint(const H5std_string &dataset_name, const Am2DArrayUI &out);
    void write_3D_uint(const H5std_string &dataset_name, const Am3DArrayUI &out);
    void write_4D_uint(const H5std_string &dataset_name, const Am4DArrayUI &out);

    template <typename MDA_T>
    void read_MDA_float(const std::string &dataset_name, MDA_T &in) const;

    template <typename MDA_T>
    void write_MDA_float(const std::string &dataset_name, const MDA_T &out);

    template <typename MDA_T>
    void read_MDA_double(const std::string &dataset_name, MDA_T &in) const;

    template <typename MDA_T>
    void write_MDA_double(const std::string &dataset_name, const MDA_T &out);

  private:
    // No copying
    HDF5(const HDF5 &in);
    HDF5& operator= (const HDF5 &right);

    //std::string _filename;
    //H5::H5File  _h5file;

};

} // namespace lib
} // namespace amethyst

#endif // #ifndef MUIR_HD5_H
