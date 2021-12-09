include_directories(${CPR_INCLUDE_DIRS})

add_library(geocoder geocoder.cpp)
add_executable(efs efs.cpp)
target_link_libraries(efs PUBLIC matplot)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(geocoder ${CPR_LIBRARIES})
target_link_libraries(efs PUBLIC geocoder)
