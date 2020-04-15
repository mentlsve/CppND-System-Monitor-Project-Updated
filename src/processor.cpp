#include "processor.h"

#include <vector>

#include "cpu_statistics.h"
#include "linux_parser.h"

using std::string;
using std::vector;

// Returns the Utilization for the window between now and when this function was
// called last time (which is ~1 second)
float Processor::Utilization() {
  vector<string> raw = LinuxParser::CpuUtilization();

  CpuStatistics cpuStatistics(std::stoi(raw.at(LinuxParser::kUser_)),
                              std::stoi(raw.at(LinuxParser::kNice_)),
                              std::stoi(raw.at(LinuxParser::kSystem_)),
                              std::stoi(raw.at(LinuxParser::kIdle_)),
                              std::stoi(raw.at(LinuxParser::kIOwait_)),
                              std::stoi(raw.at(LinuxParser::kIRQ_)),
                              std::stoi(raw.at(LinuxParser::kSoftIRQ_)),
                              std::stoi(raw.at(LinuxParser::kSteal_)),
                              std::stoi(raw.at(LinuxParser::kGuest_)),
                              std::stoi(raw.at(LinuxParser::kGuestNice_)));

  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  int totald =
      cpuStatistics.getTotalTime() - this->prevCpuStatistics.getTotalTime();
  int idled =
      cpuStatistics.getIdleTime() - this->prevCpuStatistics.getIdleTime();

  float utilization = static_cast<float>(totald - idled) / totald;

  this->prevCpuStatistics = cpuStatistics;

  return utilization;
}