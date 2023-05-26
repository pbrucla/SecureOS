#include "timer.h"

unsigned long long get_cpu_time() {
  unsigned long long time_stamp;
  // get time_stamp from cpu register
  asm volatile("rdtsc" :  "=A" (time_stamp));

  return time_stamp;
}

unsigned long long get_cpu_frequency() {
  
}