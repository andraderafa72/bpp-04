#include <string>
#include "catalog.h"
#include "lib/messages.h"

void Catalog::add_product(product_t product){
  auto already_on_list = search_product(product->m_id);

  if(already_on_list != nullptr){ 
    coms::Warning("O produto " + std::to_string(product->m_id) + " já está presente no catálogo");

    std::string error_message = "Producto com Id " + std::to_string(product->m_id) + " já existe no catálogo: " + already_on_list->m_name;
    throw std::runtime_error(error_message);
  }

  coms::Message("O produto " + std::to_string(product->m_id) + " foi adicionado ao catálogo");
  m_products[product->m_id] = product;
}  

product_list Catalog::list_products() const {
  product_list products;
  for (const auto& [key, product] : m_products) {
    products.push_back(product);
  }
  return products;
}

int Catalog::size() const {
  return m_products.size();
}

product_t Catalog::search_product(int product_id) const {
  auto product_itr = m_products.find(product_id); 
  if (product_itr != m_products.end()) {
    coms::Message("O produto " + std::to_string(product_id) + " foi encontrado");

    return product_itr->second;
  }

  coms::Warning("O produto " + std::to_string(product_id) + " não foi encontrado");
  return nullptr;
}

void Catalog::print_catalog() const {
  print_catalog_header();

  for (const auto& [key, product] : m_products) {
    product->print();
  }

  print_catalog_footer();
}