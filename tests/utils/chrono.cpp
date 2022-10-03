#include "chrono.hpp"
#include <iostream>

Chrono::Chrono(std::string const &name) : m_name(name) {}

Chrono::~Chrono() {}

void Chrono::reset() { m_records.clear(); }

void Chrono::begin() {
  if (m_records.empty()) {
    rec r;
    clock_gettime(CLOCK_MONOTONIC, &r.time);
    std::ios_base::sync_with_stdio(false);
    m_records.push_back(r);
  }
}

void Chrono::stop(std::string const &name) {
  rec r;
  r.name = name;
  clock_gettime(CLOCK_MONOTONIC, &r.time);
  m_records.push_back(r);
}

void Chrono::print() const {
  std::cout << "[CHRONO] ";
  if (m_records.size() > 1) {
    std::cout << m_name << std::endl;
    for (std::vector<rec>::const_iterator it = m_records.begin() + 1;
         it != m_records.end(); ++it) {
      std::cout << "  - " << it->name << ": "
                << get_time_diff((it - 1)->time, it->time) << std::endl;
    }
  } else {
    std::cout << m_name << ": does not have enough records" << std::endl;
  }
}

double Chrono::get_time_diff(struct timespec const &first,
                             struct timespec const &second) {
  double time_taken;
  time_taken = (second.tv_sec - first.tv_sec) * 1e9;
  time_taken = (time_taken + (second.tv_nsec - first.tv_nsec)) * 1e-9;
  return time_taken;
}