#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
#include <iomanip>
#include "common.h"


class Product {
public:
  int m_id;
  std::string m_name;
  float m_price;

  Product(int id, std::string name, float price){
    m_id = id;
    m_name = name;
    m_price = price;
  };

  void print() const {
    std::cout << "| "
    << pad_start(std::to_string(m_id), 4, '0') << " | "
    << std::setw(56) << std::left << m_name << " | "
    << std::setw(14) << std::left << m_price << " |\n";
  };

  std::string write() const {
    std::ostringstream oss;
    oss << m_id << "; " << m_name << "; " << m_price;
    return oss.str();
  };
};

#endif
