#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return process_cpu_statistics_.ProcessCpuUtilization();
}

// TODO: Return the command that generated this process
string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  long system_uptime_in_seconds = LinuxParser::UpTime();

  long starttime_in_clock_ticks_after_system_start =
      process_cpu_statistics_.Starttime();
  long starttime_in_seconds_ticks_after_system_start =
      starttime_in_clock_ticks_after_system_start / sysconf(_SC_CLK_TCK);

  return system_uptime_in_seconds -
         starttime_in_seconds_ticks_after_system_start;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return cpu_utilization_ > a.cpu_utilization_;
}