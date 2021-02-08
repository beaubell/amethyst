add_subdirectory(boost/crypto)
add_subdirectory(yaml-cpp)

set_property(TARGET yaml-cpp PROPERTY POSITION_INDEPENDENT_CODE ON)

add_subdirectory(glm)
