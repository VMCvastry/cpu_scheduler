#pragma once
#include "fake_os.h"
typedef enum
{
    DEFAULT = 0,
    SJF = 1
} Sched;

void (*getSched(Sched s))(FakeOS *os, void *args_);