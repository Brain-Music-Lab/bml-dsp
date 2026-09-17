import numpy as np
import scipy as sp
import csv
from pathlib import Path
import os

import matplotlib.pyplot as plt


if __name__ == "__main__":
    
    # sine wave 1 for FFT test
    t = np.arange(0, 3, 1 / 22050)
    f0, f1, f2 = 250, 500, 1000
    y = np.sin(f0 * 2 * np.pi * t) + np.sin(f1 * 2 * np.pi * t) + np.sin(f2 * 2 * np.pi * t)
    N = len(y)
    output = sp.fft.fft(y)

    with open(os.path.join(Path(__file__).parent, "fft_test_real.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(output.real)

    with open(os.path.join(Path(__file__).parent, "fft_test_imag.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(output.imag)

    # sine wave 2 for IFFT Test
    t = np.arange(0, 2, 1 / 44100)
    f0, f1, f2 = 40, 103, 724
    y = np.sin(f0 * 2 * np.pi * t) + 0.8*np.sin(f1 * 2 * np.pi * t) + 0.5*np.sin(f2 * 2 * np.pi * t)
    output = sp.fft.fft(y)

    ifft_out = sp.fft.ifft(output)

    with open(os.path.join(Path(__file__).parent, "ifft_test_real.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(ifft_out.real)

    with open(os.path.join(Path(__file__).parent, "ifft_test_imag.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(ifft_out.imag)