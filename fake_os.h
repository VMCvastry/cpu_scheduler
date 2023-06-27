#include "fake_process.h"
#include "linked_list.h"
#pragma once

typedef struct
{
    ListItem list;
    int pid;
    ListHead events;
    int predicted_burst_duration; // used for prediction in SJF
} FakePCB;

struct FakeOS;
typedef FakePCB *(*ScheduleFn)(struct FakeOS *os, void *args);

typedef struct FakeOS
{
    ListHead running;
    ListHead ready;
    ListHead waiting;
    int timer;
    ScheduleFn schedule_fn;
    void *schedule_args;

    ListHead processes;
    int n_cores; // number of cores
} FakeOS;

void FakeOS_init(FakeOS *os);
void FakeOS_simStep(FakeOS *os);
