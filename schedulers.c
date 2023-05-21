#include "fake_os.h"
#include "sched_sim.h"
#include <assert.h>
#include <stdlib.h>
#include "schedulers.h"
#include <stdio.h>
void schedRR(FakeOS *os, void *args_)
{
    SchedRRArgs *args = (SchedRRArgs *)args_;

    // look for the first process in ready
    // if none, return
    if (!os->ready.first)
        return;

    FakePCB *pcb = (FakePCB *)List_popFront(&os->ready);
    os->running = pcb;

    assert(pcb->events.first);
    ProcessEvent *e = (ProcessEvent *)pcb->events.first;
    assert(e->type == CPU);
    e->burst_time = args->quantum;

    // look at the first event
    // if duration>quantum
    // push front in the list of event a CPU event of duration quantum
    // alter the duration of the old event subtracting quantum
    if (e->duration > args->quantum)
    {
        ProcessEvent *qe = (ProcessEvent *)malloc(sizeof(ProcessEvent));
        qe->list.prev = qe->list.next = 0;
        qe->type = CPU;
        qe->duration = args->quantum;
        e->duration -= args->quantum;
        List_pushFront(&pcb->events, (ListItem *)qe);
    }
};
void schedSJF(FakeOS *os, void *args_)

{
    SchedRRArgs *args = (SchedRRArgs *)args_;
    // look for the first process in ready
    // if none, return
    if (!os->ready.first)
        return;

    FakePCB *best_pcb = (FakePCB *)os->ready.first;
    ProcessEvent *best_e = (ProcessEvent *)best_pcb->events.first;
    for (ListItem *aux = os->ready.first; aux; aux = aux->next)
    {
        FakePCB *pcb = (FakePCB *)aux;
        ProcessEvent *e = (ProcessEvent *)pcb->events.first;
        if (e->duration < best_e->duration)
        {
            fprintf(stderr, "swi\n");

            best_pcb = pcb;
            best_e = (ProcessEvent *)best_pcb->events.first;
        }
    }
    List_detach(&os->ready, (ListItem *)best_pcb);
    os->running = best_pcb;
    assert(best_pcb->events.first);
    ProcessEvent *e = (ProcessEvent *)best_pcb->events.first;
    assert(e->type == CPU);

    // look at the first event
    // if duration>quantum
    // push front in the list of event a CPU event of duration quantum
    // alter the duration of the old event subtracting quantum
    if (e->duration > args->quantum)
    {
        ProcessEvent *qe = (ProcessEvent *)malloc(sizeof(ProcessEvent));
        qe->list.prev = qe->list.next = 0;
        qe->type = CPU;
        qe->duration = args->quantum;
        e->duration -= args->quantum;
        List_pushFront(&best_pcb->events, (ListItem *)qe);
    }
};

void (*getSched(Sched s))(FakeOS *os, void *args_)
{
    switch (s)
    {
    case DEFAULT:
        fprintf(stderr, "DEFAULT\n");
        return schedRR;
    case SJF:
        fprintf(stderr, "SJF\n");
        return schedSJF;
    default:
        assert(0);
    }
}