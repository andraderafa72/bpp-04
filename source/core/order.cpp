#include "order.h"
#include "common.h"
#include "lib/messages.h"
#include <string>

#define PRODUCT_SEPARATOR "------------------------------------------------------------------------------------"
#define CUSTOMER_SEPARATOR "------------------------------------------------------\n"

void Order::add_product(product_t product){
  m_products.push_back(product);
}

void Order::print() const {
  std::cout << "\n"
            << YELLOW << "# " << PRODUCT_SEPARATOR 
            << GREEN << "\nCliente:\n"
            << CUSTOMER_SEPARATOR << "| " << std::setw(50) << std::left
            << m_customer << "R$ " << calculate_total() <<  " |\n"
            << CUSTOMER_SEPARATOR;

  print_catalog_header(true);
  for(const auto& product : m_products){
    product->print();
  }
  print_catalog_footer();

  std::cout << YELLOW << PRODUCT_SEPARATOR << " #";
}

float Order::calculate_total() const {
  auto ptr = m_products.begin(), 
    end = m_products.end();
  float total = 0.0;

  while(ptr < end){
    auto product = *ptr;
    total += product->m_price;
    ++ptr;
  }

  coms::Message("Total calculado para o pedido de " + m_customer + ": " +
                std::to_string(total));

  return total;
}