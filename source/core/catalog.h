#ifndef CATALOG_H
#define CATALOG_H

#include <vector>
#include <memory>
#include <map>
#include "product.h"

using product_t = std::shared_ptr<Product>;
using product_list = std::vector<product_t>;
using product_map = std::map<int, product_t>;

class Catalog {
  product_map m_products;

public:
  Catalog() {
    m_products = std::map<int, product_t>();
  };

  void add_product(product_t product);
  product_list list_products() const;
  int size() const;
  product_t search_product(int product_id) const;
  void print_catalog() const;
};

#endif