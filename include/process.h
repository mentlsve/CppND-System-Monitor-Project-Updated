#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "process_cpu_statistics.h"

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, string user, ProcessCpuStatistics processCpuStatistics,
          string cmd)
      : pid_{pid},
        user_{user},
        process_cpu_statistics_{processCpuStatistics},
        cmd_{cmd},
        cpu_utilization_{processCpuStatistics.ProcessCpuUtilization()} {}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_{0};
  string user_{};
  ProcessCpuStatistics process_cpu_statistics_{};
  string cmd_{};
  float cpu_utilization_{0};
};

#endif