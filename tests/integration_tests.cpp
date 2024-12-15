#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "../source/core/product.h"
#include "../source/core/catalog.h"
#include "../source/core/order.h"


TEST(Integration, CatalogProducts) {
  Product p1(1, "test", 15.9);
  Product p2(2, "test 2", 25.9);
  Catalog cat;

  EXPECT_EQ(cat.size(), 0);

  cat.add_product(std::make_shared<Product>(p1));
  cat.add_product(std::make_shared<Product>(p2));

  EXPECT_EQ(cat.size(), 2);

  auto product = cat.search_product(2);
  auto not_found_product = cat.search_product(3);

  EXPECT_NE(product, nullptr);    
  EXPECT_EQ(not_found_product, nullptr);

  auto products = cat.list_products();
  EXPECT_EQ(products.size(), 2);
}

TEST(Integration, OrderProducts) {
  Product p1(1, "test", 15.9);
  Product p2(2, "test 2", 25.9);
  Order order("Name", {});

  EXPECT_EQ(order.m_products.size(), 0);

  order.add_product(std::make_shared<Product>(p1));
  order.add_product(std::make_shared<Product>(p2));

  EXPECT_EQ(order.m_products.size(), 2);
 
  auto sum = order.calculate_total();
  EXPECT_EQ(sum, (float)(15.9 + 25.9));
}