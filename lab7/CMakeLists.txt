cmake_minimum_required(VERSION 3.14)
project(inverse
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Создаем исполняемый файл
add_executable(InversionTest inversion_test.cpp matrix.h)

# Установка флагов компиляции для конкретной цели
target_compile_options(InversionTest PRIVATE -Wall -O3 -mavx -mfma -lblas)

# Находим и подключаем библиотеки
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(GTest REQUIRED)

# Находим библиотеку BLAS
find_package(BLAS REQUIRED)

# Подключаем библиотеки к целям
target_link_libraries(InversionTest Eigen3::Eigen gtest gtest_main ${BLAS_LIBRARIES})

# Регистрация теста
add_test(NAME InversionTest COMMAND InversionTest)
enable_testing()
