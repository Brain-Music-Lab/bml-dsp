import numpy as np
import scipy as sp
import csv
from pathlib import Path
import os

from .generation import (
    gen_fft_truth, 
    gen_lerp_truth, 
    gen_resample_truth, 
    gen_convolution_truth, 
    gen_filter_truth, 
    gen_blackman_window_truth, 
    gen_sinc_truth,
    gen_csv_truth)


if __name__ == "__main__":

    data_path = Path(os.path.join(Path(__file__).parent, "data"))
    if not data_path.exists():
        data_path.mkdir()

    gen_fft_truth()
    gen_lerp_truth()
    gen_resample_truth()
    gen_convolution_truth()
    gen_filter_truth()
    gen_blackman_window_truth()
    gen_sinc_truth()
    gen_csv_truth()
