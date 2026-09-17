import numpy as np
import scipy as sp
import csv
from pathlib import Path
import os
import matplotlib.pyplot as plt


if __name__ == "__main__":
    
    # sine wave 1
    t = np.arange(0, 3, 1 / 22050)
    f0, f1, f2 = 250, 500, 1000
    y = np.sin(f0 * 2 * np.pi * t) + np.sin(f1 * 2 * np.pi * t) + np.sin(f2 * 2 * np.pi * t)
    N = len(y)
    output = sp.fft.fft(y)

    with open(os.path.join(Path(__file__).parent, "fft_test.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(np.abs(output))
