
//Link to Boost
//#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "HDF5ClassTests"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "lib/io_hdf5.h"

#include <iostream>

using namespace amethyst::lib;
using namespace boost::unit_test;

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE ( test_hdf5_float_rw )
{
    std::string binpath ( framework::master_test_suite().argv[2] );

    // Open File
    HDF5 h5file(binpath + "/" + std::string("floattest.h5"), H5F_ACC_TRUNC);
    h5file.createGroup(std::string("/Test"));

    // 2D TESTS
    {
        std::string datatag = "/Test/2df";

        // Create and Write Data to HDF5 File
        constexpr int dims = 2;
        std::array<int, dims> shape = {{20,4}};
        Am2DArrayF test2d(shape);
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                test2d[i][j] = (float)(i) * 1.0f + (float)(j) * 0.001f;
            }
        }

        h5file.write_MDA_float(datatag, test2d);

        // Read Data Back from HDF5 File
        std::array<int, dims> shape_read = {{1,1}};
        Am2DArrayF test2d_read(shape_read);

        h5file.read_MDA_float(datatag, test2d_read);
        auto shape_in = test2d_read.shape();

        // Check Shape
        for (int i = 0; i < dims; i++) {
            BOOST_CHECK_EQUAL( shape[i], shape_in[i]);
        }

        // Check Data
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                BOOST_CHECK_EQUAL(test2d[i][j], test2d_read[i][j]);
            }
        }
    }

    // 3D TESTS
    {
        std::string datatag = "/Test/3df";

        // Create and Write Data to HDF5 File
        constexpr int dims = 3;
        std::array<int, dims> shape = {{20,4,8}};
        Am3DArrayF test3d(shape);
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                for (size_t k = 0; k < shape[2]; k++) {
                    test3d[i][j][k] = (float)(i) * 1.0f + (float)(j) * 0.001f + (float)(k) * 100.0f;
                }
            }
        }

        h5file.write_MDA_float(datatag, test3d);

        // Read Data Back from HDF5 File
        std::array<int, dims> shape_read = {{1,1,1}};
        Am3DArrayF test3d_read(shape_read);

        h5file.read_MDA_float(datatag, test3d_read);
        auto shape_in = test3d_read.shape();

        // Check Shape
        for (int i = 0; i < dims; i++) {
            BOOST_CHECK_EQUAL( shape[i], shape_in[i]);
        }

        // Check Data
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                for (size_t k = 0; k < shape[2]; k++) {
                    BOOST_CHECK_EQUAL(test3d[i][j][k], test3d_read[i][j][k]);
                }
            }
        }
    }
}

BOOST_AUTO_TEST_CASE ( test_hdf5_double_rw )
{
    std::string binpath ( framework::master_test_suite().argv[2] );

    // Open File
    HDF5 h5file(binpath + "/" + std::string("doubletest.h5"), H5F_ACC_TRUNC);
    h5file.createGroup(std::string("/Test"));

    // 2D TESTS
    {
        std::string datatag = "/Test/2dd";

        // Create and Write Data to HDF5 File
        constexpr int dims = 2;
        std::array<int, dims> shape = {{20,4}};
        Am2DArrayD test2d(shape);
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                test2d[i][j] = (double)(i) * 1.0 + (double)(j) * 0.000000001;
            }
        }

        h5file.write_MDA_double(datatag, test2d);

        // Read Data Back from HDF5 File
        std::array<int, dims> shape_read = {{1,1}};
        Am2DArrayD test2d_read(shape_read);

        h5file.read_MDA_double(datatag, test2d_read);
        auto shape_in = test2d_read.shape();

        // Check Shape
        for (int i = 0; i < dims; i++) {
            BOOST_CHECK_EQUAL( shape[i], shape_in[i]);
        }

        // Check Data
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                BOOST_CHECK_EQUAL(test2d[i][j], test2d_read[i][j]);
            }
        }
    }

    // 3D TESTS
    {
        std::string datatag = "/Test/3dd";

        // Create and Write Data to HDF5 File
        constexpr int dims = 3;
        std::array<int, dims> shape = {{20,4,8}};
        Am3DArrayD test3d(shape);
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                for (size_t k = 0; k < shape[2]; k++) {
                    test3d[i][j][k] = (double)(i) * 1.0 + (double)(j) * 0.000000001 + (double)(k) * 1000.0;
                }
            }
        }

        h5file.write_MDA_double(datatag, test3d);

        // Read Data Back from HDF5 File
        std::array<int, dims> shape_read = {{1,1,1}};
        Am3DArrayD test3d_read(shape_read);

        h5file.read_MDA_double(datatag, test3d_read);
        auto shape_in = test3d_read.shape();

        // Check Shape
        for (int i = 0; i < dims; i++) {
            BOOST_CHECK_EQUAL( shape[i], shape_in[i]);
        }

        // Check Data
        for (size_t i = 0; i < shape[0]; i++) {
            for (size_t j = 0; j < shape[1]; j++) {
                for (size_t k = 0; k < shape[2]; k++) {
                    BOOST_CHECK_EQUAL(test3d[i][j][k], test3d_read[i][j][k]);
                }
            }
        }
    }
}

