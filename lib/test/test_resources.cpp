
//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "ResourceClassTests"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "lib/resource.h"
#include "lib/cl.h"

#include <iostream>

using namespace amethyst::lib;
using namespace boost::unit_test;

// ------------- Tests Follow --------------
BOOST_AUTO_TEST_CASE ( res_builtin )
{
    Resource res1 = LOAD_RESOURCE ( lib_histdist_cl, "./", "test.res" );


    //Use BOOST_CHECK for small equal checks - true or false
    BOOST_CHECK_EQUAL ( res1[0], '#' );
    BOOST_CHECK_EQUAL ( res1.size(), 700 );

    BOOST_CHECK_THROW ( int i = res1[700], std::runtime_error );

    BOOST_CHECK_EQUAL (res1.getSHA256(), std::string("29293222a97c71e833d8daa6a80e95f98f27a2638fff4a9d29cd790390ff8f10"));
}

BOOST_AUTO_TEST_CASE ( res_external )
{

    std::string binpath ( framework::master_test_suite().argv[1] );
    Resource res1 ( binpath, "override.res" );

    BOOST_CHECK_EQUAL ( res1[0], '1' );
    BOOST_CHECK_EQUAL ( res1.size(), 15 );

    BOOST_CHECK_EQUAL ( res1.getSHA256(), std::string("033253b0eb5cb274b409784c2fbef36ffa9f1e76a4b3255c3c70391b3f446433"));
}

BOOST_AUTO_TEST_CASE ( res_override )
{

    std::string binpath ( framework::master_test_suite().argv[1] );
    Resource res1 = LOAD_RESOURCE ( lib_histdist_cl, binpath, "override.res" );

    BOOST_CHECK_EQUAL ( res1[0], '1' );
    BOOST_CHECK_EQUAL ( res1.size(), 15 );

}

