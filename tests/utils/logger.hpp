#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

template <typename T> void print_data(T data) {
  std::cout << "[LOG] " << data << std::endl;
}

void print_header(std::string header);

#endif
