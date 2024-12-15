#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>
#include <memory>
#include "product.h" 

using product_t = std::shared_ptr<Product>;
using product_list = std::vector<product_t>;

class Order {
public:
  product_list m_products;
  std::string m_customer;
  
  Order(std::string customer, product_list products) {
    m_customer = customer;
    m_products = products;
  };

  void add_product(product_t product);
  void print() const;
  float calculate_total() const;
};

#endif