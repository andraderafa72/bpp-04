#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "../source/core/product.h"
#include "../source/core/catalog.h"
#include "../source/core/order.h"

#pragma region ProductTests

TEST(Product, Constructor) {
  Product p(1, "test", 15.9);

  EXPECT_EQ(p.m_id, 1);
  EXPECT_EQ(p.m_name, "test");
  EXPECT_EQ(p.m_price, (float)15.9);
}

#pragma endregion

#pragma region CatalogTests

TEST(Catalog, AddProduct) {
  Product p(1, "test", 15.9);
  Catalog cat;

  cat.add_product(std::make_shared<Product>(p));

  auto product_ptr =cat.search_product(1);
  EXPECT_NE(product_ptr,nullptr);
}

TEST(Catalog, SearchProduct) {
  Product p(1, "test", 15.9);
  Catalog cat;

  cat.add_product(std::make_shared<Product>(p));
  auto product_ptr =cat.search_product(1);

  EXPECT_NE(product_ptr,nullptr);
}

TEST(Catalog, ListProducts) {
  Product p(1, "test", 15.9);
  Catalog cat;

  cat.add_product(std::make_shared<Product>(p));

  auto products = cat.list_products();
  auto product_ptr = cat.search_product(1);
  
  EXPECT_EQ(products.size(), 1);
  EXPECT_NE(product_ptr,nullptr);
}

TEST(Catalog, Size) {
  Product p(1, "test", 15.9);
  Catalog cat;

  EXPECT_EQ(cat.size(), 0);

  cat.add_product(std::make_shared<Product>(p));
 
  EXPECT_EQ(cat.size(), 1);
}

#pragma endregion

#pragma region OrderTests

TEST(Order, Constructor) {
  Order order("Name", {});

  EXPECT_EQ(order.m_products.size(), 0);
  EXPECT_EQ(order.m_customer, "Name");
}

TEST(Order, AddProduct) {
  Order order("Name", {});
  EXPECT_EQ(order.m_products.size(), 0);

  Product product(1, "test", 15.9);
  order.add_product(std::make_shared<Product>(product));

  EXPECT_EQ(order.m_products.size(), 1);
}

TEST(Order, CalculateTotal) {
  Order order("Name", {});
  EXPECT_EQ(order.m_products.size(), 0);

  Product product(1, "test", 15.9);
  order.add_product(std::make_shared<Product>(product));
  order.add_product(std::make_shared<Product>(product));

  EXPECT_EQ(order.m_products.size(), 2);

  EXPECT_EQ(order.calculate_total(), (float)31.8);
}

#pragma endregion
