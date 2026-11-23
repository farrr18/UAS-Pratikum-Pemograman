cmake_minimum_required(VERSION 3.10)
project(Perpustakaan C)

set(CMAKE_C_STANDARD 11)

add_executable(perpustakaan main.c admin.c user.c penyimpanan.c sorting.c)
