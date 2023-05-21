import sys
import os
import numpy as np
import matplotlib.pyplot as plt


def parse(raw):
    timesteps = raw.split("************** TIME:")
    res = []
    for timestep in timesteps:
        data = timestep.split("\n")[1:]
        waiting = []
        running = []
        ready = []
        for l in data:
            if l.startswith("\t\t"):
                continue
            if l.startswith("\twaiting"):
                waiting.append(int(l.split(":")[1]))
            if l.startswith("\trunning"):
                running.append(int(l.split(":")[1]))
            if l.startswith("\tready"):
                ready.append(int(l.split(":")[1]))
        res.append((waiting, running, ready))
    return res


def analyze(data, processes):
    total_time = len(data)
    waste_time = 0
    process_status = {int(p): [] for p in processes}

    for i, t in enumerate(data):
        waiting, running, ready = t
        for p in process_status:
            process_status[p].append(0)
        for p in waiting:
            process_status[p][i] = 2
        for p in ready:
            process_status[p][i] = 1
        if len(running) == 1 and running[0] == -1:
            waste_time += 1
            continue
        for p in running:
            process_status[p][i] = 3
    print("Total time: {}".format(total_time))
    print("Waste time: {}".format(waste_time))
    print("Waste percentage: {}".format(waste_time / total_time))
    # plot
    for p in process_status:
        plt.plot(process_status[p], ".", label=p)
    plt.legend()
    plt.show()


def main():
    args = sys.argv[1:]

    if len(args) != 1:
        print("Usage: python analyze_output.py <output_file>")
        sys.exit(1)

    output_file = args[0]
    if not os.path.exists(output_file):
        print("File {} does not exist".format(output_file))
        sys.exit(1)

    with open(output_file, "r") as f:
        raw = f.read()
    first, raw = raw.split("\n", 1)
    processes = first.split(",")[:-1]
    print("Processes: {}".format(processes))
    data = parse(raw)
    analyze(data, processes)


if __name__ == "__main__":
    main()
