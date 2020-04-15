#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cpu_statistics.h"

class Processor {
 public:
  float Utilization();

 private:
  CpuStatistics prevCpuStatistics;
};

#endif