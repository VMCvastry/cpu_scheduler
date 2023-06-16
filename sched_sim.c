#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fake_os.h"
#include "schedulers.h"
#include "sched_sim.h"

FakeOS os;
Sched sched = DEFAULT;
int n_cores = 10;

int main(int argc, char **argv)
{
    FakeOS_init(&os);
    SchedRRArgs srr_args;
    srr_args.quantum = 50;
    os.schedule_args = &srr_args;
    os.schedule_fn = getSched(sched);
    os.n_cores = n_cores;
    for (int i = 1; i < argc; ++i)
    {
        FakeProcess new_process;
        int num_events = FakeProcess_load(&new_process, argv[i]);
        // printf("loading [%s], pid: %d, events:%d",
        //        argv[i], new_process.pid, num_events);
        printf("%d, ", new_process.pid);
        if (num_events)
        {
            FakeProcess *new_process_ptr = (FakeProcess *)malloc(sizeof(FakeProcess));
            *new_process_ptr = new_process;
            List_pushBack(&os.processes, (ListItem *)new_process_ptr);
        }
    }
    printf("num processes in queue %d\n", os.processes.size);
    while (os.running.first || os.ready.first || os.waiting.first || os.processes.first)
    {
        FakeOS_simStep(&os);
    }
}
