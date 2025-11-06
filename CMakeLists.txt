cmake_minimum_required(VERSION 3.22)

project(bml-dsp)

# include(GNUInstallDirs)

# add_subdirectory()

# Specify the directory where the library is located
# set(FFTW_LIB_DIR "${PROJECT_SOURCE_DIR}/vendor/fftw/lib")

# Use find_library to locate the library
# find_library(FFTW_LIB NAMES fftw3-3  PATHS ${FFTW_LIB_DIR})

# Optionally, print the path of the library if found
# if(FFTW_LIB)
#     message(STATUS "Found FFTW library: ${FFTW_LIB}")
# else()
#     message(FATAL_ERROR "FFTW library not found!")
# endif()

add_library(bml-dsp 
  ${PROJECT_SOURCE_DIR}/src/circular-buffer.cpp
    # ${PROJECT_SOURCE_DIR}/src/fft.cpp
    # ${PROJECT_SOURCE_DIR}/src/filter.cpp
    # ${PROJECT_SOURCE_DIR}/src/realtime.cpp
  )

target_include_directories(bml-dsp 
  PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/bml-dsp

  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    # ${PROJECT_SOURCE_DIR}/vendor/fftw/include/fftw
  )

target_compile_options(bml-dsp PUBLIC /MTd)

target_link_libraries(bml-dsp
  PUBLIC
  # ${FFTW_LIB}
)


# set_property(TARGET ${project_dir} PROPERTY
#             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# if (UNIX)
#   find_package(LSL 1.13.0 REQUIRED
#     HINTS "/usr/local"
#     "${CMAKE_CURRENT_LIST_DIR}/../../LSL/liblsl/build/"
#     "${CMAKE_CURRENT_LIST_DIR}/../../LSL/liblsl/build/install"
#     PATH_SUFFIXES share/LSL)

#   target_link_libraries(bml-dsp PUBLIC fftw3 liblsl LSL::lsl)
# endif(UNIX)

# if (WIN32)

    # target_include_directories(bml-dsp 
    # PUBLIC
    #   ${PROJECT_SOURCE_DIR}/vendor/fftw/include
    #   "C:\\Program Files (x86)\\liblsl\\include")



