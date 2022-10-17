add_library(eigen_utils INTERFACE
    eigen_utils.h
)
target_link_libraries(eigen_utils INTERFACE eigen)


add_library(file_utils INTERFACE
    file_utils.h
)
target_link_libraries(file_utils INTERFACE absl::optional)


add_library(type_traits INTERFACE
    type_traits.h
)

add_library(image INTERFACE image.h)
target_include_directories(image INTERFACE ${PROJECT_SOURCE_DIR})
target_link_libraries(image INTERFACE file_utils stb_image absl::optional)

add_executable(image_test image_test.cpp)
target_link_libraries(image_test PRIVATE image GTest::gtest_main)

add_library(macro_utils INTERFACE macro_utils.h)
target_include_directories(macro_utils INTERFACE ${PROJECT_SOURCE_DIR})

add_executable(macro_utils_test macro_utils_test.cpp)
target_link_libraries(macro_utils_test PRIVATE macro_utils GTest::gtest_main)
