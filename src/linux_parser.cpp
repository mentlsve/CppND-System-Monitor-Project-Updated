#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "parsing_utils.h"
#include "process_cpu_statistics.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

int extract_memory_value(vector<string> line, string identifier) {
  static const int identifier_idx = 0;
  static const int value_idx = 1;

  assert(line.at(identifier_idx).find(identifier) == 0);
  return std::stoi(line.at(value_idx));
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // documentation from http://man7.org/linux/man-pages/man5/proc.5.html
  // mem_available is not used
  // https://superuser.com/questions/980820/what-is-the-difference-between-memfree-and-memavailable-in-proc-meminfo
  long int mem_total;  // Total usable RAM (i.e., physical RAM minus a few
                       // reserved bits and the kernel binary code).
  long int mem_free;   // The sum of LowFree+HighFree. (User space == high
                       // memory, Kernel space == low memory)
  long int buffers;    // Relatively temporary storage for raw disk blocks that
                       // shouldn't get tremendously large (20MB or so).
  long int cached;     // In-memory cache for files read from the disk (the page
                       // cache). Doesn't include SwapCached.

  vector<vector<string>> lines =
      file_to_tokenized_lines(kProcDirectory + kMeminfoFilename);

  vector<string> mem_total_line = lines.at(0);
  mem_total = extract_memory_value(mem_total_line, "MemTotal");

  vector<string> mem_free_line = lines.at(1);
  mem_free = extract_memory_value(mem_free_line, "MemFree");

  vector<string> mem_buffers_line = lines.at(3);
  buffers = extract_memory_value(mem_buffers_line, "Buffers");

  vector<string> mem_cached_line = lines.at(4);
  cached = extract_memory_value(mem_cached_line, "Cached");

  long int total_used_memory =
      mem_total - mem_free + buffers +
      cached;  // actually memory for chaches is missing here

  return static_cast<float>(total_used_memory) / mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  /*
    /proc/uptime
      This file contains two numbers (values in seconds): the uptime
      of the system (including time spent in suspend) and the amount
      of time spent in the idle process.
  */

  vector<vector<string>> lines =
      file_to_tokenized_lines(kProcDirectory + kUptimeFilename);

  long number_of_seconds_system_has_been_up = std::stol(lines.at(0).at(0));
  return number_of_seconds_system_has_been_up;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<vector<string>> lines =
      file_to_tokenized_lines(kProcDirectory + kStatFilename);
  for (auto line : lines) {
    if (line.at(0) == "cpu") {
      line.erase(line.begin());
      return line;
    }
  }
  return {};
}

// TODO: Read and return CPU utilization
ProcessCpuStatistics LinuxParser::ProcessCpuUtilization(int pid) {
  string proc_pid_stat_path =
      kProcDirectory + std::to_string(pid) + kStatFilename;
  vector<vector<string>> lines = file_to_tokenized_lines(proc_pid_stat_path);

  if (lines.size() == 0 || lines.at(0).size() < LinuxParser::kStarttime_)
    return ProcessCpuStatistics(0, 0, 0, 0, 0);

  ProcessCpuStatistics processCpuStatistics(
      std::stol(lines.at(0).at(LinuxParser::kUtime_)),
      std::stol(lines.at(0).at(LinuxParser::kStime_)),
      std::stol(lines.at(0).at(LinuxParser::kCutime_)),
      std::stol(lines.at(0).at(LinuxParser::kCstime_)),
      std::stol(lines.at(0).at(LinuxParser::kStarttime_)));
  return processCpuStatistics;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  vector<vector<string>> lines =
      file_to_tokenized_lines(kProcDirectory + kStatFilename);
  for (auto line : lines) {
    if (line.at(0) == "processes") {
      return std::stoi(line.at(1));
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  vector<vector<string>> lines =
      file_to_tokenized_lines(kProcDirectory + kStatFilename);
  for (auto line : lines) {
    if (line.at(0) == "procs_running") {
      return std::stoi(line.at(1));
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string proc_pid_cmdline_path =
      kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  std::ifstream stream(proc_pid_cmdline_path);
  string line{};
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string proc_pid_status_path =
      kProcDirectory + std::to_string(pid) + kStatusFilename;
  vector<vector<string>> lines = file_to_tokenized_lines(proc_pid_status_path);

  int vmsize_in_kilobytes{0};  // Virtual memory size

  for (auto line : lines) {
    if (line.at(0) == "VmSize:") {
      vmsize_in_kilobytes = std::stoi(line.at(1));  // value in kB
    }
  }

  return std::to_string(vmsize_in_kilobytes / 1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // get uid
  string uid{};
  string username{};

  string proc_pid_status_path =
      kProcDirectory + std::to_string(pid) + kStatusFilename;
  vector<vector<string>> lines = file_to_tokenized_lines(proc_pid_status_path);
  for (auto line : lines) {
    if (line.at(0).find("Uid:") == 0) {
      uid = line.at(1);
      break;
    }
  }

  vector<vector<string>> users = file_to_tokenized_lines(kPasswordPath, true);
  for (auto line : users) {
    if (line.at(2) == uid) {
      username = line.at(0);
      break;
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

// utility functions for file parsing
vector<string> split_on_space(string line) {
  std::istringstream linestream(line);
  // https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
  vector<string> results(std::istream_iterator<std::string>{linestream},
                         std::istream_iterator<std::string>());
  return results;
}

vector<string> split_on_colon(string line) {
  std::istringstream linestream(line);
  vector<string> results{};
  for (string token; std::getline(linestream, token, ':');) {
    results.push_back(token);
  }
  return results;
}

vector<vector<string>> file_to_tokenized_lines(string absolutePath,
                                               bool colon) {
  vector<vector<string>> lines;
  std::ifstream stream(absolutePath);
  for (string line; std::getline(stream, line);) {
    if (colon) {
      lines.push_back(split_on_colon(line));
    } else {
      lines.push_back(split_on_space(line));
    }
  }
  return lines;
}
