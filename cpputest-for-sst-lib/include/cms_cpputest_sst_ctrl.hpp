#ifndef CPPUTEST_FOR_SST_LIB_CMS_CPPUTEST_SST_CTRL_HPP
#define CPPUTEST_FOR_SST_LIB_CMS_CPPUTEST_SST_CTRL_HPP

#include "sst.hpp"
#include <chrono>
#include <cstdint>

namespace cms::test::sst_ctrl {

enum : uint8_t {
  DUMMY_AO_A_PRIORITY,
  DUMMY_AO_B_PRIORITY,
  DUMMY_AO_C_PRIORITY,
  DUMMY_AO_D_PRIORITY,
  DUMMY_AO_E_PRIORITY,
  UNIT_UNDER_TEST_PRIORITY
};

static_assert(UNIT_UNDER_TEST_PRIORITY < SST_PORT_MAX_TASK,
              "too many priorities defined");

/**
 * Setup for unit testing of an SST Task, using the faux SST host PC test.
 * environment. Intended to be called from the cpputest setup() method.
 * @param ticksPerSecond : ticks per second. Match your target device's setting.
 * Default is 1000.
 */
void Setup(uint32_t ticksPerSecond = 1000);

/**
 *  Reset, teardown.
 *  Intended to be called from the cpputest teardown() method.
 */
void Teardown();

/**
 * Process all events available for processing, in the same manner
 * as the SST0 kernel.
 *
 * Returns when no more events are found.
 */
void ProcessEvents();

/**
 * Faux movement of time. Will execute the SST timer tick and
 * also call ProcessEvents() with each tick.
 * @param duration milliseconds of faux time to tick the kernel forward.
 */
void MoveTimeForward(const std::chrono::milliseconds &duration);

inline void PostAndProcess(SST::Evt const *e, SST::Task *dest) {
  dest->post(e);
  ProcessEvents();
}

const char *GetVersion();

} // namespace cms::test::sst_ctrl

#endif // CPPUTEST_FOR_SST_LIB_CMS_CPPUTEST_SST_CTRL_HPP
