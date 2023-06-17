import sys
import os
import numpy as np
import matplotlib.pyplot as plt
from enum import Enum


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


class Status(Enum):
    OFF = 0
    WAITING = 2
    READY = 1
    RUNNING = 3


def get_segments(x, p, status, pid):
    xx = np.array(x)
    y = np.array([s.value for s in p])
    mask = y == status.value
    x_segment = xx[mask]
    y_segment = [pid for _ in x_segment]
    return x_segment, y_segment


def analyze(data, processes):
    total_time = len(data)
    waste_time = 0
    ready_time = {int(p): 0 for p in processes}
    life_time = {int(p): 0 for p in processes}
    process_status = {int(p): [] for p in processes}

    for i, t in enumerate(data):
        waiting, running, ready = t
        for p in process_status:
            process_status[p].append(Status.OFF)
        for p in waiting:
            process_status[p][i] = Status.WAITING
            life_time[p] += 1
        for p in ready:
            process_status[p][i] = Status.READY
            ready_time[p] += 1
            life_time[p] += 1
        if len(running) == 1 and running[0] == -1:
            waste_time += 1
            continue
        for p in running:
            process_status[p][i] = Status.RUNNING
            life_time[p] += 1
    print("Total time: {}".format(total_time))
    print(
        f"CPU utilization: {total_time-waste_time} ({round((1-waste_time / total_time) * 100)}%)"
    )
    print(
        f"Tournaround time: {sum(life_time.values())} (avg: {np.mean(list(life_time.values()))})"
    )
    print(
        f"Waiting time: {sum(ready_time.values())} ({round(sum(ready_time.values()) / total_time * 100)}%), avg: {np.mean(list(ready_time.values()))}"
    )
    x = list(range(total_time))
    # plot
    for p in process_status:
        x_segment, y_segment = get_segments(x, process_status[p], Status.READY, p)
        plt.plot(x_segment, y_segment, ".", label=p, color="red")
        x_segment, y_segment = get_segments(x, process_status[p], Status.WAITING, p)
        plt.plot(x_segment, y_segment, ".", label=p, color="gray")
        x_segment, y_segment = get_segments(x, process_status[p], Status.RUNNING, p)
        plt.plot(x_segment, y_segment, ".", label=p, color="green")

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
