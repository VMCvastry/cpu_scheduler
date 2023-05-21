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

    e->burst_time = args->quantum;
};
static const float a = 0.5;
void schedSJF_PRED(FakeOS *os, void *args_)
{
    SchedRRArgs *args = (SchedRRArgs *)args_;
    if (!os->ready.first)
        return;

    FakePCB *best_pcb = (FakePCB *)os->ready.first;
    for (ListItem *aux = os->ready.first; aux; aux = aux->next)
    {
        FakePCB *pcb = (FakePCB *)aux;
        if (pcb->predicted_burst_duration < best_pcb->predicted_burst_duration)
        {
            fprintf(stderr, "swi\n");
            best_pcb = pcb;
        }
    }

    List_detach(&os->ready, (ListItem *)best_pcb);
    os->running = best_pcb;
    assert(best_pcb->events.first);
    ProcessEvent *e = (ProcessEvent *)best_pcb->events.first;
    assert(e->type == CPU);
    e->burst_time = args->quantum;
    if (e->duration <= args->quantum) // Last time this event will run
    {
        best_pcb->predicted_burst_duration = a * e->total_duration + (1 - a) * best_pcb->predicted_burst_duration; // Calculate next event prediction
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
    case SJF_PRED:
        fprintf(stderr, "SJF_PRED\n");
        return schedSJF_PRED;
    default:
        assert(0);
    }
}