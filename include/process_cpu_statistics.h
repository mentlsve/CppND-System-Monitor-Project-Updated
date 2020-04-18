#ifndef PROCESS_CPU_STATISTICS_H
#define PROCESS_CPU_STATISTICS_H

class ProcessCpuStatistics {
 public:
  float ProcessCpuUtilization();
  ProcessCpuStatistics() = default;
  ProcessCpuStatistics(int utime, int stime, int cutime, int cstime,
                       int starttime)
      : utime_{utime},
        stime_{stime},
        cutime_{cutime},
        cstime_{cstime},
        starttime_{starttime} {}

 private:
  // Description taken from https://linux.die.net/man/5/proc

  // Amount of time that this process has been scheduled in user mode,
  // measured in clock ticks (divide by sysconf(_SC_CLK_TCK)). This includes
  // guest time, guest_time (time spent running a virtual CPU, see below),
  // so that applications that are not aware of the guest time field do not
  // lose that time from their calculations.
  long utime_{0};
  // Amount of time that this process has been scheduled in kernel mode,
  // measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  long stime_{0};
  // Amount of time that this process's waited-for children have been scheduled
  // in user mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  // (See also times(2).)  This includes guest time, cguest_time (time spent
  // running a virtual CPU, see below).
  long cutime_{0};
  // Amount of time that this process's waited-for children have been scheduled
  // in kernel mode, measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  long cstime_{0};
  // The time the process started after system boot. In kernels before
  // Linux 2.6, this value was expressed in jiffies.  Since Linux 2.6, the value
  // is expressed in clock ticks (divide by sysconf(_SC_CLK_TCK)).
  long starttime_{0};
};

#endif