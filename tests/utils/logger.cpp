#include <algorithm>
#include <cctype>
#include <iostream>

void print_header(std::string header) {
  std::transform(header.begin(), header.end(), header.begin(), ::toupper);
  std::cout << std::endl
            << "########## " << header << " ##########" << std::endl;
}