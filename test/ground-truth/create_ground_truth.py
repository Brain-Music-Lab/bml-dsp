import numpy as np
import scipy as sp
import csv
from pathlib import Path
import os


if __name__ == "__main__":
    t = np.arange(0, 3, 1 / 22050)
    
    # sine wave 1
    f0 = 250
    f1 = 500
    f2 = 1000

    y = np.sin(f0 * 2 * np.pi * t) + np.sin(f1 * 2 * np.pi * t) + np.sin(f2 * 2 * np.pi * t)
    output = sp.fft.fft(y)

    with open(os.path.join(Path(__file__).parent, "fft_test_1.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(np.abs(output))
