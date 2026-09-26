import os
import csv
from pathlib import Path

import numpy as np
import scipy as sp


def gen_filter_truth():

    old_sr = 50
    nyquist = old_sr / 2
    new_sr = 20000
    bw = (old_sr / new_sr)

    fc = nyquist / new_sr

    N = int(np.ceil(4 / bw))

    if not N % 2:
       N += 1  # Make sure that N is odd.
    n = np.arange(N)
    
    # Compute sinc filter.
    h = np.sinc(fc * (n - (N - 1) / 2))
    
    # Compute Blackman window.
    w = 0.42 - 0.5 * np.cos(2 * np.pi * n / (N - 1)) + \
        0.08 * np.cos(4 * np.pi * n / (N - 1))
    
    # Multiply sinc filter by window.
    h = h * w
    
    # Normalize to get unity gain.
    h = h / np.sum(h)

    with open(os.path.join(Path(__file__).parent, "../data/filter_h.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(h)


if __name__ == "__main__":
    gen_filter_truth()