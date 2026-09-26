import numpy as np
import csv
import os
from pathlib import Path


def gen_blackman_window_truth():
    window = np.blackman(200)

    with open(os.path.join(Path(__file__).parent, "../data/blackman_window_test.csv"), "w") as f:
            writer = csv.writer(f)
            writer.writerow(window)


if __name__ == "__main__":
    gen_blackman_window_truth()