import numpy as np
import csv
import os
from pathlib import Path


def gen_sinc_truth():
    sinc = np.sinc(np.arange(-50, 50, 0.1))

    with open(os.path.join(Path(__file__).parent, "../data/sinc_test.csv"), "w") as f:
            writer = csv.writer(f)
            writer.writerow(sinc)


if __name__ == "__main__":
    gen_sinc_truth()