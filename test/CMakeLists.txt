cmake_minimum_required(VERSION 3.27)


set(TEST_NAMES
    bml-math-test
    circular-buffer-test
    fft-test
    realtime-test)

foreach (TEST_NAME IN LISTS TEST_NAMES)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    add_test(${TEST_NAME} ${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE bml-dsp)
endforeach()

target_link_libraries(bml-math-test PRIVATE bml-dsp)