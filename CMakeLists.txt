cmake_minimum_required(VERSION 3.22)

project(bml-dsp)

# include(GNUInstallDirs)

if (WIN32)
# Specify the directory where the library is located
  set(FFTW_LIB_DIR "${PROJECT_SOURCE_DIR}/deps/fftw/lib/")
  find_library(FFTW_LIB NAMES fftw3-3  PATHS ${FFTW_LIB_DIR})

  # Optionally, print the path of the library if found
  if(FFTW_LIB)
      message(STATUS "Found FFTW library: ${FFTW_LIB}")
  else()
      message(FATAL_ERROR "FFTW library not found!")
  endif()
elseif(UNIX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(FFTW3 REQUIRED IMPORTED_TARGET fftw3)
endif()

add_library(bml-dsp 
  ${PROJECT_SOURCE_DIR}/src/circular-buffer.cpp
  ${PROJECT_SOURCE_DIR}/src/fft.cpp
  ${PROJECT_SOURCE_DIR}/src/filter.cpp
  ${PROJECT_SOURCE_DIR}/src/realtime.cpp
  ${PROJECT_SOURCE_DIR}/src/util/csv.cpp

  # inline includes
)

target_include_directories(bml-dsp 
  PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/bml-dsp

  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    # ${PROJECT_SOURCE_DIR}/vendor/fftw/include/fftw
  )

# target_compile_options(bml-dsp PUBLIC /MTd)

if (WIN32)
  target_link_libraries(bml-dsp
    PUBLIC
    # ${FFTW_LIB}
  )
elseif (UNIX)
  target_link_libraries(bml-dsp
    PRIVATE
    PkgConfig::FFTW3
  )
endif()


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

# Tests
find_package(Catch2 3 REQUIRED)

set(TEST_DIR test)

set(TEST_NAMES 
  bml_math_test
  circular_buffer_test
  read_csv_test
  fft_test
)

enable_testing()

foreach (test_name ${TEST_NAMES})
  message(${test_name})
  add_executable(${test_name} test/${test_name}.cpp)
  target_link_libraries(${test_name} 
    PRIVATE 
      bml-dsp
      Catch2::Catch2WithMain
  )

  add_test(
    NAME ${test_name} 
    COMMAND $<TARGET_FILE:${test_name}>
  )
endforeach()
