
/***********************************************************************
 Amethyst-lib
  - HDF5 Class definitions

 Authors (c):
 2010-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "io_hdf5.h"

#include <boost/lexical_cast.hpp>


#define QUOTEME_(x) #x
#define QUOTEME(x) QUOTEME_(x)


namespace amethyst {
namespace lib {


HDF5::HDF5(const std::string &filename_in, const unsigned int flags)
    : H5File( filename_in.c_str(), flags )
{
}

HDF5::~HDF5()
{
}


// Read a Scalar Float from a dataset path.
unsigned int HDF5::read_scalar_uint(const H5std_string &dataset_name) const
{
    unsigned int data_out[1] = {0};

    // Get dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_INTEGER )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
        std::string("Expecting UINT Type from ") + dataset_name + " in " + getFileName()));

    // Read scalar float
    dataset.read(data_out, H5::PredType::NATIVE_UINT);

    return data_out[0];
}

// Read a Scalar Float from a dataset path.
float HDF5::read_scalar_float(const H5std_string &dataset_name) const
{

    float data_out[1] = {0.0f};

   // Get dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

   // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

   // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting FLOAT Type from ") + dataset_name + " in " + getFileName()));

   // Read scalar float
    dataset.read(data_out, H5::PredType::NATIVE_FLOAT);

    return data_out[0];
}


void  HDF5::write_scalar_float(const H5std_string &dataset_name, float out)
{
    // Create dataspace
    H5::DataSpace dataspace(H5S_SCALAR);

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_FLOAT );

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(&out, datatype);
}


void  HDF5::write_scalar_double(const H5std_string &dataset_name, double out)
{
    // Create dataspace
    H5::DataSpace dataspace(H5S_SCALAR);
    
    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_DOUBLE );
    
    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);
    
    // Write data
    dataset.write(&out, datatype);
}


void  HDF5::write_scalar_uint(const H5std_string &dataset_name, unsigned int out)
{
    // Create dataspace
    H5::DataSpace dataspace(H5S_SCALAR);

    // Define Datatype
    H5::IntType datatype( H5::PredType::NATIVE_UINT );

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(&out, datatype);
}


// Read a String Array from a dataset path.
std::string HDF5::read_string(const H5std_string &dataset_name) const
{

    H5std_string buffer("");

   // Get dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

   // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

   // Check to see if we are dealing with floats
    if( type_class != H5T_STRING )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting STRING Type from ") + dataset_name + " in " + getFileName()));

    H5::DataType dtype = dataset.getDataType();

   buffer.reserve(dataset.getStorageSize());
   //std::cout << "Storage size required: " << size << std::endl;

   // Read string
    dataset.read( buffer, dtype);

    return buffer;
}


// Read a String Array from a dataset path.
void HDF5::write_string(const H5std_string &dataset_name, const std::string &out)
{
    // Create dataspace
    H5::DataSpace dataspace(H5S_SCALAR);

    // Define Datatype
    H5::StrType datatype( H5::PredType::C_S1, H5T_VARIABLE );

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out, datatype);
}


// Read a double vector from a dataset path.
void HDF5::read_1D_double(const H5std_string &dataset_name, std::vector<double> &in) const
{
    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_FLOAT Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::FloatType floattype = dataset.getFloatType();
    size_t size = floattype.getSize();
    if(size != 8)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 8 (double) in ") + dataset_name + " from " + getFileName()));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();

    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 1)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 1 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[1];
    dataspace.getSimpleExtentDims( dimsm, NULL);

    // Initialize vector;
    in.resize(dimsm[0]);

    hsize_t j;
    for (j = 0; j < dimsm[0]; j++)
    {
        in[j] = 0.0;
    }

    // Get data
    dataset.read(in.data(), H5::PredType::NATIVE_DOUBLE);

    return;
}


void HDF5::write_1D_string(const H5std_string &dataset_name, const std::vector<std::string> &out)
{
    const hsize_t rank = 1;
    const hsize_t shape = out.size();
    
    hsize_t dimsf[rank];
    dimsf[0] = shape;
    
    // Create dataspace
    H5::DataSpace dataspace( rank, dimsf );
    H5::DataSpace memspace( rank, dimsf );

    // Define Datatype
    H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);


    hsize_t      offset_out[1]; // hyperslab offset in memory
    hsize_t      count_out[1];  // size of the hyperslab in memory
    offset_out[0] = 0;
    count_out[0]  = 1;
    memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

    for (unsigned int i = 0; i < out.size(); i++)
    {
        offset_out[0] = i;

        dataspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

        // Write data
        dataset.write(out[i], datatype, memspace, dataspace);
    }
}


void HDF5::write_1D_double(const H5std_string &dataset_name, const std::vector<double> &out)
{
    const hsize_t rank = 1;
    const hsize_t shape = out.size();

    hsize_t dimsf[rank];
    dimsf[0] = shape;

    // Create dataspace
    H5::DataSpace dataspace( rank, dimsf );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_DOUBLE );
    datatype.setOrder( H5T_ORDER_LE);;

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_DOUBLE);
  
}


// Read a 2D Unsigned Integer Array from a dataset path.
void HDF5::read_2D_uint(const H5std_string &dataset_name, Am2DArrayUI &in) const
{

    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str() );

    // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_INTEGER )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_INTEGER Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::IntType inttype = dataset.getIntType();
    size_t size = inttype.getSize();
    if(size != 4)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 4 (integer) in ") + dataset_name + " from " + getFileName()));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();

    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 2)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 2 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[2];
    dataspace.getSimpleExtentDims( dimsm, NULL);

    // Initialize boost multi_array;
    in.resize(boost::extents[dimsm[0]][dimsm[1]]);

    hsize_t i, j;
    for (j = 0; j < dimsm[0]; j++)
    {
        for (i = 0; i < dimsm[1]; i++)
        {
            in[j][i] = 0;
        }
    }

    // Get data
    dataset.read(in.data(), H5::PredType::NATIVE_UINT);

    return;
}


/// NOT IMPLEMENTED
//void HDF5::read_3D_uint(const H5std_string &dataset_name, Am3DArrayUI &in) const
//{
//}


/// NOT IMPLEMENTED
//void HDF5::read_4D_uint(const H5std_string &dataset_name, Am4DArrayUI &in) const
//{
//}


// Write a 2D Unsigned Integer Array to a dataset path.
void HDF5::write_2D_uint(const H5std_string &dataset_name, const Am2DArrayUI &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto *shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::IntType datatype( H5::PredType::NATIVE_UINT );
    datatype.setOrder( H5T_ORDER_LE);;

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_UINT);
}


/// NOT IMPLEMENTED
//void HDF5::write_3D_uint(const H5std_string &dataset_name, const Am3DArrayUI &out)
//{
//}


/// NOT IMPLEMENTED
//void HDF5::write_4D_uint(const H5std_string &dataset_name, const Am4DArrayUI &out)
//{
//}



// Read a 2D Float Array from a dataset path.
void HDF5::read_2D_float(const H5std_string &dataset_name, Am2DArrayF &in) const
{

    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_FLOAT Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::FloatType floattype = dataset.getFloatType();
    size_t size = floattype.getSize();
    if(size != 4)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 4 (float) in ") + dataset_name + " from " + getFileName()));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();

    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 2)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 2 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[2];
    dataspace.getSimpleExtentDims( dimsm, NULL);

    // Initialize boost multi_array;
    in.resize(boost::extents[dimsm[0]][dimsm[1]]);

    hsize_t i, j;
    for (j = 0; j < dimsm[0]; j++)
    {
        for (i = 0; i < dimsm[1]; i++)
        {
            in[j][i] = 0;
        }
    }

    // Get data
    dataset.read(in.data(), H5::PredType::NATIVE_FLOAT);

    return;
}


// Read a 3D Float Array from a dataset path.
void HDF5::read_3D_float(const H5std_string &dataset_name, Am3DArrayF &in) const
{

    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type classype::NATIVE_FLOAT
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_FLOAT Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::FloatType floattype = dataset.getFloatType();
    size_t size = floattype.getSize();
    if(size != 4)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 4 (float) in ") + dataset_name + " from " + getFileName()));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();

    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 3)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 4 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[3];
    dataspace.getSimpleExtentDims( dimsm, NULL);

    // Initialize boost multi_array;
    in.resize(boost::extents[dimsm[0]][dimsm[1]][dimsm[2]]);


    hsize_t i, j, k;

    for (j = 0; j < dimsm[0]; j++)
    {
        for (i = 0; i < dimsm[1]; i++)
        {
            for (k = 0; k < dimsm[2]; k++)
                in[j][i][k] = 0;
        }
    }

    // Get data
    dataset.read(in.data(), H5::PredType::NATIVE_FLOAT);

    return;
}


// Read a 4D Float Array from a dataset path.
void HDF5::read_4D_float(const H5std_string &dataset_name, Am4DArrayF &in) const
{

    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type classype::NATIVE_FLOAT
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_FLOAT Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::FloatType floattype = dataset.getFloatType();
    size_t size = floattype.getSize();
    if(size != 4)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 4 (float) in ") + dataset_name + " from " + getFileName()));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();

    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 4)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 4 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[4];
    dataspace.getSimpleExtentDims( dimsm, NULL);

    // Initialize boost multi_array;
    in.resize(boost::extents[dimsm[0]][dimsm[1]][dimsm[2]][dimsm[3]]);


    hsize_t i, j, k, l;

    for (j = 0; j < dimsm[0]; j++)
    {
        for (i = 0; i < dimsm[1]; i++)
        {
            for (k = 0; k < dimsm[2]; k++)
                for (l = 0; l < dimsm[3]; l++) 
                    in[j][i][k][l] = 0;
        }
    }

    // Get data
    dataset.read(in.data(), H5::PredType::NATIVE_FLOAT);

    return;
}


// Write a 2D Float Array to a dataset path.
void HDF5::write_2D_float(const H5std_string &dataset_name, const Am2DArrayF &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto *shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_FLOAT );
    datatype.setOrder( H5T_ORDER_LE);

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_FLOAT);
}


// Write a 3D Float Array to a dataset path.
void HDF5::write_3D_float(const H5std_string &dataset_name, const Am3DArrayF &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto *shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_FLOAT );
    datatype.setOrder( H5T_ORDER_LE);

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_FLOAT);
}


// Write a 4D Float Array to a dataset path.
void HDF5::write_4D_float(const H5std_string &dataset_name, const Am4DArrayF &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto *shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_FLOAT );
    datatype.setOrder( H5T_ORDER_LE);

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_FLOAT);

}


// Read a 2D Double-Precision Float Array from a dataset path.
void HDF5::read_2D_double(const H5std_string &dataset_name, Am2DArrayD &in) const
{

    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type class
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_FLOAT Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::FloatType floattype = dataset.getFloatType();
    size_t size = floattype.getSize();
    if(size != 8)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 8 (double) in ") + dataset_name + " from " + getFileName()));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();
    
    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 2)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 2 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[2];
    int ndims = dataspace.getSimpleExtentDims( dimsm, NULL);

    #ifndef NDEBUG
    assert((rank == 2) && (ndims == 2));
    #endif

     // Initialize boost multi_array;
    in.resize(boost::extents[dimsm[0]][dimsm[1]]);

    for (hsize_t j = 0; j < dimsm[0]; j++)
    {
        for (hsize_t i = 0; i < dimsm[1]; i++)
        {
            in[j][i] = 0;
        }
    }

    // get data
    dataset.read( in.data(), H5::PredType::NATIVE_DOUBLE );

    return;
}



void HDF5::read_3D_double(const H5std_string &dataset_name, Am3DArrayD &in) const
{
    // Get Dataset
    H5::DataSet dataset = openDataSet( dataset_name.c_str());

    // Get Type classype::NATIVE_FLOAT
    H5T_class_t type_class = dataset.getTypeClass();

    // Check to see if we are dealing with floats
    if( type_class != H5T_FLOAT )
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting H5T_FLOAT Type in ") + dataset_name + " from " + getFileName()));

    // Get size of datatpe and verify
    H5::FloatType floattype = dataset.getFloatType();
    size_t size = floattype.getSize();
    if(size != 8)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting float size to be 8 (double) in ") + dataset_name + " from " + getFileName() + " but got " + boost::lexical_cast<std::string>(size) + " instead"));

    // Get dataspace handle
    H5::DataSpace dataspace = dataset.getSpace();

    // Get rank and verify
    int rank = dataspace.getSimpleExtentNdims();
    if(rank != 3)
        throw(std::runtime_error(std::string(__FILE__) + ":" + std::string(QUOTEME(__LINE__)) + "  " +
                std::string("Expecting rank to be 4 dimensions in ") + dataset_name + " from " + getFileName()));

    // Get dimensions and verify
    hsize_t dimsm[3];
    dataspace.getSimpleExtentDims( dimsm, NULL);

    // Initialize boost multi_array;
    in.resize(boost::extents[dimsm[0]][dimsm[1]][dimsm[2]]);


    hsize_t i, j, k;

    for (j = 0; j < dimsm[0]; j++)
    {
        for (i = 0; i < dimsm[1]; i++)
        {
            for (k = 0; k < dimsm[2]; k++)
                in[j][i][k] = 0;
        }
    }

    // Get data
    dataset.read(in.data(), H5::PredType::NATIVE_DOUBLE);

    return;
}


/// NOT IMPLEMENTED
//void HDF5::read_4D_double(const H5std_string &dataset_name, Am4DArrayD &in) const
//{
//}


// Write a 2D Double-Precision Float Array to a dataset path.
void HDF5::write_2D_double(const H5std_string &dataset_name, const Am2DArrayD &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto *shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_DOUBLE );
    datatype.setOrder( H5T_ORDER_LE);;

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_DOUBLE);
}



void HDF5::write_3D_double(const H5std_string &dataset_name, const Am3DArrayD &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto* shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_DOUBLE );
    datatype.setOrder( H5T_ORDER_LE);

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_DOUBLE);
}



void HDF5::write_4D_double(const H5std_string &dataset_name, const Am4DArrayD &out)
{
    const hsize_t rank = out.num_dimensions();
    const auto* shape = out.shape();

    // Create dataspace
    H5::DataSpace dataspace( rank, shape );

    // Define Datatype
    H5::FloatType datatype( H5::PredType::NATIVE_DOUBLE );
    datatype.setOrder( H5T_ORDER_LE);

    // Create a new dataset within the file...
    H5::DataSet dataset = createDataSet( dataset_name.c_str(), datatype, dataspace);

    // Write data
    dataset.write(out.data(), H5::PredType::NATIVE_DOUBLE);
}


} // namespace lib
} // namespace amethyst

