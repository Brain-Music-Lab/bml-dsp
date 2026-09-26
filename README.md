# BML-DSP
This is a signal processing library designed to be used with the custom max externals developed by the Brain Music Lab at the University of Colorado Boulder

# Status of Resampling
- Upsampling works
- Downsampling and fractional resampling need to be validated through additional test cases in `test/resample_test.cpp`

## Testing
This package utilizes Python for ground truth. To generate the ground truth necessary:
- Go into the test directory
- Create a virtual environment and activate it
- `pip install numpy scipy`
- `python -m generate.ground_truth`

Afterwards leave that directory (`cd ..`), create and enter a build directory (`mkdir build; cd build`), and build the package:
- `cmake ..`
- `cmake --build .`

Then, to run tests:
- `ctest`
