
cmake_minimum_required(VERSION 3.16)

project(UAS_Pratikum_Pemograman C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(perpustakaan
    src/main.c
    src/admin.c
    src/penyimpanan.c
    src/user.c
    src/sorting.c
)

target_include_directories(perpustakaan PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
