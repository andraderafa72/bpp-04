#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>

#include "core/catalog.h"
#include "core/order.h"

using order_t = std::shared_ptr<Order>;
using order_list = std::vector<order_t>;

class Program {
  enum e_program_state {
    STARTING,
    WELCOME,
    READING_FILE,
    READING_MENU_OPT,
    ADDING_PRODUCT,
    LISTING_PRODUCTS,
    CREATING_ORDER,
    LISTING_ORDERS,
    SEARCHING_ORDERS,
    SEARCHING_PRODUCTS,
    SAVING,
    QUITTING,
  };

  enum e_menu_opt {
    ADD_PRODUCT = 1,
    LIST_PRODUCTS,
    CREATE_ORDER,
    LIST_ORDERS,
    SEARCH_ORDER,
    SEARCH_PRODUCT,
    SAVE,
    EXIT,
    INVALID,
  };

  e_program_state m_state;
  e_menu_opt m_curr_menu_opt;
  order_list m_orders;
  Catalog m_catalog;
  std::string m_curr_msg;
  std::string m_error_msg; 
  std::string m_file_path;
  int m_search_query;

  void print_menu() const;
  void print_products() const;
  void print_orders() const;

  void read_product();
  void read_order();
  Order parse_order(std::string customer, std::vector<std::string>& products);  
  void read_file();
  void read_query();

  void search_order();
  void search_product();

  void write_file() const;

public:
  bool is_quitting() { 
    return m_state == e_program_state::QUITTING;
  };

  void initialize(int argc, char* argv[]);
  void process_events();
  void update();
  void render() const;
};

#endif