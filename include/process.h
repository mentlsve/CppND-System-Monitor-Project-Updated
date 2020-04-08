#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  //https://linux.die.net/man/5/proc
  int user; // Time spent in user mode.
  int nice; // Time spent in user mode with low priority (nice).
  int system; // Time spent in system mode.
  int idle; // Time spent in the idle task. 
  int iowait; // Time waiting for I/O to complete. 
  int irq; // Time servicing interrupts. 
  int softirq; // Time servicing softirqs. 
  int steal; // Stolen time, which is the time spent in other operating systems when running in a virtualized environment.
  int guest; // Time spent running a virtual CPU for guest operating systems under the control of the Linux kernel. 
  int guest_nice; // Time spent running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel). 

};

#endif