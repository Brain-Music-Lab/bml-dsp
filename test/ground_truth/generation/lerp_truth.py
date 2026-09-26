import os
import csv
import numpy as np
from pathlib import Path


def gen_lerp_truth():
    # For linear interpolation
    np.random.seed(10)
    fs = 512
    n_secs = 3
    t_unsorted = n_secs * np.random.random(n_secs * fs)
    t = np.sort(t_unsorted)
    y = np.sin(3 * 2 * np.pi * t) + 0.7 * np.sin(2.6 * 2 * np.pi * t) + 0.5 * np.sin(1.6 * 2 * np.pi * t)
    y -= np.mean(y)
    y /= np.max(np.abs(y))

    t_even = np.arange(0, n_secs, 1/fs)
    y_interp = np.interp(t_even, t, y)


    with open(os.path.join(Path(__file__).parent, "../data/lerp_test_fake_timestamps.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(t)

    with open(os.path.join(Path(__file__).parent, "../data/lerp_test_fake_out_data.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y)

    with open(os.path.join(Path(__file__).parent, "../data/lerp_test_fake_interp_data.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y_interp)
