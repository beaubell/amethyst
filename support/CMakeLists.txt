add_subdirectory(boost/crypto)
add_subdirectory(yaml-cpp)

set_property(TARGET yaml-cpp PROPERTY POSITION_INDEPENDENT_CODE ON)


add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
target_include_directories(glm
  INTERFACE
    ./
)
