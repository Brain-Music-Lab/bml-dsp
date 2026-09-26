import os
import csv
from pathlib import Path

import numpy as np
import scipy as sp


def _test0():
    # toy example -- upsample from 100 to 200
    fs1 = 100
    fs2 = 200
    n_secs = 1

    t = np.arange(0, 1, 1/fs1)
    y = np.sin(2 * np.pi * t)
    y_res = sp.signal.resample(y, n_secs * fs2)
    
    with open(os.path.join(Path(__file__).parent, "../data/resample_test0_y.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test0_y_truth.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y_res)


def _test1():
    # upsample: 512 to 2048
    fs1 = 512
    fs2 = 2048
    n_secs = 2

    f0 = 1
    f1 = 0
    f2 = 0

    t = np.arange(0, n_secs, 1/fs1)
    y = np.sin(f0 * 2 * np.pi * t) + np.sin(f1 * 2 * np.pi * t) + np.sin(f2 * 2 * np.pi * t)

    t2 = np.arange(0, n_secs, 1/fs2)
    y_res = sp.signal.resample(y, n_secs * fs2)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test1_y.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test1_y_truth.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y_res)


def _test2():
    # downsample: 22050 to 11025
    fs1 = 22050
    fs2 = 11025
    n_secs = 1

    f0 = 121
    f1 = 158
    f2 = 568

    t = np.arange(0, n_secs, 1/fs1)
    y = np.sin(f0 * 2 * np.pi * t) + np.sin(f1 * 2 * np.pi * t) + np.sin(f2 * 2 * np.pi * t)
    y_res = sp.signal.resample(y, n_secs * fs2)
    assert len(y) != len(y_res)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test2_t.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(t)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test2_y.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test2_y_truth.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y_res)


def _test3():
    # resample: 512 to 44100
    fs1 = 512
    fs2 = 44100
    n_secs = 1

    f0 = 20
    f1 = 90
    f2 = 200

    t = np.arange(0, n_secs, 1/fs1)
    y = np.sin(f0 * 2 * np.pi * t) + np.sin(f1 * 2 * np.pi * t) + np.sin(f2 * 2 * np.pi * t)
    y_res = sp.signal.resample(y, n_secs * fs2)
    assert len(y) != len(y_res)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test3_t.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(t)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test3_y.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y)

    with open(os.path.join(Path(__file__).parent, "../data/resample_test3_y_truth.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow(y_res)


def gen_resample_truth():
    _test0()
    _test1()
    # _test2()
    # _test3()

if __name__ == "__main__":
    gen_resample_truth()
