#include <linux_parser.h>
#include <process_cpu_statistics.h>
#include <unistd.h>

// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float ProcessCpuStatistics::ProcessCpuUtilization() {
  int hertz = sysconf(_SC_CLK_TCK);
  long clockTicksCpuUsed =
      utime_ + stime_ + cutime_ + cstime_;  // in clock ticks
  float secondsCpuUsed =
      static_cast<float>(clockTicksCpuUsed) / hertz;  // in seconds
  long systemUptime = LinuxParser::UpTime();          // in seconds

  long secondsSinceProcessWasStarted = systemUptime - (starttime_ / hertz);

  return secondsCpuUsed / secondsSinceProcessWasStarted;
}

long ProcessCpuStatistics::Starttime() { return starttime_; }
