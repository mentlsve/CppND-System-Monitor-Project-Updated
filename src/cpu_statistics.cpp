#include <cpu_statistics.h>

int CpuStatistics::getIdleTime() { 
  return idle_ + iowait_;
}

int CpuStatistics::getNonIdleTime() { 
  return user_ + nice_ + system_ + irq_ + softirq_ + steal_;
}

int CpuStatistics::getTotalTime() { 
  return getIdleTime() + getNonIdleTime();
}
