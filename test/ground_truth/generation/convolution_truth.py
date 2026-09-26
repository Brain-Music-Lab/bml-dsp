import os
from pathlib import Path
import csv


import numpy as np
import scipy as sp


def gen_convolution_truth():

    # test case 1
    fs = 512
    t = np.arange(0, 2, 1/fs)
    t2 = np.arange(0, 0.5, 1/fs)

    x_n = np.sin(100 * 2 * np.pi * t)
    h_n = np.sin(0.3 * 2 * np.pi * t2)

    y_out = np.convolve(x_n, h_n)

    with open(os.path.join(Path(__file__).parent, "../data/convolution_test_x.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(x_n)

    with open(os.path.join(Path(__file__).parent, "../data/convolution_test_h.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(h_n)

    with open(os.path.join(Path(__file__).parent, "../data/convolution_test_y.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y_out)
        

if __name__ == "__main__":
    gen_convolution_truth()
