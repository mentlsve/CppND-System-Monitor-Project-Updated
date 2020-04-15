#ifndef CPU_STATISTICS_H
#define CPU_STATISTICS_H

class CpuStatistics {
 public:
  CpuStatistics() = default;
  CpuStatistics(int user, int nice, int system, int idle, int iowait, int irq, int softirq, int steal, int guest, int guest_nice)
      : user_{user},
        nice_{nice},
        system_{system},
        idle_{idle},
        iowait_{iowait},
        irq_{irq},
        softirq_{softirq},
        steal_{steal},
        guest_{guest},
        guest_nice_{guest_nice} {}
  int getIdleTime();
  int getNonIdleTime();
  int getTotalTime();

 private:
  // Assumptions:
  // * int is 32 bits so the maximum positve value is 2,147,483,647.
  // * USER_HZ is 100 ticks per second, so we can deal with 21474836 seconds,
  // 357913 minutes, 5965 hours, 248 days

  // Description taken from https://linux.die.net/man/5/proc
  // Time spent in user mode.
  int user_{0};
  // Time spent in user mode with low priority (nice).
  int nice_{0};
  // Time spent in system mode.
  int system_{0};
  // Time spent in the idle task.
  int idle_{0};
  // Time waiting for I/O to complete.
  int iowait_{0};
  // Time servicing interrupts.
  int irq_{0};
  // Time servicing softirqs.
  int softirq_{0};
  // Stolen time, which is the time spent in other operating systems when
  // running in a virtualized environment.
  int steal_{0};
  // Time spent running a virtual CPU for guest operating systems under the
  // control of the Linux kernel.
  int guest_{0};
  // Time spent running a niced guest (virtual CPU for guest operating systems
  // under the control of the Linux kernel).
  int guest_nice_{0};
};

#endif