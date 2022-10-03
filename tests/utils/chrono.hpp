#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <string>
#include <sys/time.h>
#include <vector>

class Chrono {
  std::string m_name;
  struct rec {
    struct timespec time;
    std::string name;
  };
  std::vector<rec> m_records;

  double get_time_diff(struct timespec const &end) const;

  static double get_time_diff(struct timespec const &first,
                              struct timespec const &second);

public:
  Chrono(std::string const &name);

  ~Chrono();

  void reset();
  void begin();
  void stop(std::string const &name);
  void print() const;
};

#endif
