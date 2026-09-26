import csv
import os
from pathlib import Path


def gen_csv_truth():
    with open(os.path.join(Path(__file__).parent, "../data/read_csv_test.csv"), "w") as f:
        writer = csv.writer(f)
        writer.writerow([4.1,105.2,3.8,105.14,20659.1,20.2,-92.1])
        