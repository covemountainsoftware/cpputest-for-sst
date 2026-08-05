#include "sst.hpp"
#include "dbc_assert.h"
#include <cstdio>
#include <cassert>

void DBC_fault_handler(char const * module, const int label)
{
    fprintf(stderr, "%s : %s %d\n", __FUNCTION__, module, label);
    assert(true == false);
}

namespace SST {
void onStart()
{

}

void onIdleCond()
{
}

}