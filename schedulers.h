#pragma once
#include "fake_os.h"
typedef enum
{
    DEFAULT = 0,
    SJF = 1,
    SJF_PRED = 2,
} Sched;

void (*getSched(Sched s))(FakeOS *os, void *args_);