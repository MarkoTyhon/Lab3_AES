#pragma once
#include "memory.h"

#define STACK_LEN 8
#define RAM 8

#define HEX_LEN 32/8

#define MANTISSA 30
#define EXP 9
#define BIT (MANTISSA + EXP + 1)
#define BIAS pow(2, EXP-1)-1
#define MAX_BIAS pow(2,EXP)-1 

Memory mem;