#ifndef COMMON_H
#define COMMON_H

#pragma once

#include <string>
#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[31m"     
#define GREEN   "\033[32m"  
#define YELLOW  "\033[33m" 
#define BLUE    "\033[34m"
#define BOLD    "\033[1m" 

#define LOGGING true

inline std::string pad_start(const std::string& input, size_t targetLength, char padChar = ' ') {
  if (input.length() >= targetLength) {
    return input;
  }
  return std::string(targetLength - input.length(), padChar) + input;
}

inline void print_catalog_header(bool is_order = false) {  
  std::cout << BLUE << BOLD << (is_order ? "Produtos" : "Catálogo") << ":\n\n" << RESET;

  std::cout << BLUE << BOLD
    << "------------------------------------------------------------------------------------\n"
    << "| ID   | Produto                                                  | Preço          |\n"
    << "------------------------------------------------------------------------------------\n"
    << RESET;
}

inline void print_catalog_footer() {
  std::cout << BLUE << BOLD
    << "------------------------------------------------------------------------------------\n";
}

#endif