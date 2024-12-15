#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <fstream>

#include "program.h"
#include "core/catalog.h"

#define PRESS_ENTER ">>> Pressione <Enter> para continuar..."

#pragma region UTILS

void tokenizer(std::vector<std::string> &tokens, std::string line, char divider){
  std::stringstream check1(line); 
  std::string intermediate;
    
  while(std::getline(check1, intermediate, divider))
    tokens.push_back(intermediate);   
}

template<typename T>
std::vector<std::string> get_tokenized_line(T &in){
  std::string line;
  std::getline(in, line);

  std::vector<std::string> tokens;
  tokenizer(tokens, line, ';');

  return tokens;
}

void process_enter(){
  std::string line;
  std::getline(std::cin, line);
}

#pragma endregion // UTILs

#pragma region RENDERS

void Program::print_products() const {
  m_catalog.print_catalog();

  std::cout << "\n\n" << RESET << PRESS_ENTER;
}

void Program::print_orders() const {
  int current_order = 1, size= m_orders.size();
  for(const auto& order : m_orders){
    order->print();

    if (current_order != size) {
      std::cout << "\n\n Pressione <Enter> para ver o próximo pedido ou <X> "
                    "para sair ("
                << current_order << "/" << size << ") > ";

      std::string line;
      std::getline(std::cin, line);

      if (line == "x" || line == "X")
        break;
    }

    current_order++;
  }

  std::cout << "\n\n" << RESET << PRESS_ENTER;
}

void Program::print_menu() const {
  std::cout << "\n\n"
    << BLUE << "-------------------- Gestão de catálogo de produtos ----------------------\n"
    << "| "
    << pad_start(std::to_string(m_orders.size()), 4, '0')
    << " Pedidos cadastrados                                               |\n"
        << "| "
    << pad_start(std::to_string(m_catalog.size()), 4, '0')
    << " Produtos cadastrados                                              |\n"
    << "| Selecione uma opção:                                                   |\n"
    << "|                                                                        |\n"
    << "| 1: Adicionar produto                                                   |\n"
    << "| 2: Listar produtos                                                     |\n"
    << "| 3: Cadastrar pedido                                                    |\n"
    << "| 4: Listar pedidos                                                      |\n"
    << "| 5: Buscar pedido                                                       |\n"
    << "| 6: Buscar produto                                                      |\n"
    << "| 7: Salvar registros                                                    |\n"
    << "| 8: Sair                                                                |\n"
    << "--------------------------------------------------------------------------\n\n"
    << RED << "ERROR: [" << m_error_msg << "]\n" << RESET
    << YELLOW << "MSG: [" << m_curr_msg << "]\n\n" << RESET
    << ">>> ";
}

void print_usage(){
  std::cout << "AJUDA\n";
  exit(0);
}

void print_welcome(){
  std::cout << "---------------------------------------------\n"
            << "|            Catálogo de produtos           |\n"
            << "---------------------------------------------\n\n"
            << PRESS_ENTER;
}

void print_add_product(){
  std::cout << "\nInformações do produto (<id>;<nome>;<preço>) > ";
}

#pragma endregion // RENDERS

void Program::initialize(int argc, char* argv[]) {
  m_state = e_program_state::STARTING;
  m_curr_msg = "";
  m_file_path = "";
  m_error_msg = "";

  for (int i = 1; i < argc; i++) {
    std::string current_arg{ argv[i] };

    if (current_arg[0] == '-') {
      if (current_arg == "-f") {
        std::string file_path = argv[i + 1];
        m_file_path = file_path;
      }
      else if (current_arg == "-h" || current_arg == "--help") {
        print_usage();
      }
    }
  }
}

#pragma region LEITURAS_E_ESCRITAS

Product parse_product(std::vector<std::string>& tokens) {
  auto id = std::stoi(tokens[0]);
  std::string product_name = tokens[1];
  float price = std::stof(tokens[2]);

  Product new_product(id, product_name, price);
  return new_product;
}

Order Program::parse_order(std::string customer, std::vector<std::string>& products) {
  std::string invalid_token;
  product_list order_products;

  try{
    for(const auto& product_token: products){
      invalid_token = product_token;
      auto product_id = std::stoi(product_token);
      auto product = m_catalog.search_product(product_id);
      if(product == nullptr) throw std::runtime_error("Produto não encontrado: ");

      order_products.push_back(product);
    }
  } catch(const std::exception& e){
    throw std::runtime_error("A lista de produtos possui IDs inválidos: " + invalid_token);
  }

  Order new_order(customer, order_products);

  return new_order;
}

void Program::write_file() const {
  std::cout << m_file_path << "\n";
  std::ofstream ofs(m_file_path, std::ios::out);
  if (!ofs)
    return;

  auto products = m_catalog.list_products();

  for (const auto &product : products) {
    ofs << product->write();
  }

  ofs << "@\n";

  for (const auto &order : m_orders) {
    ofs << order->m_customer << ";";
    std::ostringstream oss;

    for (const auto &product : order->m_products) {
      oss << product->m_id << ";";
    }

    auto str = oss.str();
    str.pop_back();
    ofs << str << "\n";
  }

  ofs.close();
}

void Program::read_file(){
  std::ifstream ifs(m_file_path);
  std::ostringstream oss;

  if (ifs.is_open()) {
    oss << BLUE << "\n>>> Abrindo arquivo [" << m_file_path << "]\n"
      << ">>> Processando dados...\n";

    bool is_parsing_orders = false;
    while (ifs) {
      std::vector<std::string> tokens = get_tokenized_line(ifs);
      if(tokens.size() == 0) continue;

      if (tokens.size() != 3 && !is_parsing_orders){
        if(tokens.size() == 1 && tokens[0] == "@" && !is_parsing_orders){
          is_parsing_orders = true;
        } 
        
        continue;
      }

      if(is_parsing_orders){
        try {
          auto customer = tokens[0];
          tokens.erase(tokens.begin());
          Order order = parse_order(customer, tokens);
          auto order_ptr = std::make_shared<Order>(order);
          m_orders.push_back(order_ptr);
        } catch (const std::exception &e) { }

        continue;
      }

      try{
        Product product = parse_product(tokens);
        auto product_ptr = std::make_shared<Product>(product);
        m_catalog.add_product(product_ptr);
      } catch(const std::exception& e) {
        continue;
      }
    }

    ifs.close();
  }

  oss << ">>> Arquivo lido com sucesso. " << m_catalog.size() << " produtos encontrados. " << m_orders.size() << " pedidos encontrados.\n"
  << PRESS_ENTER;
  std::cout << oss.str() << "\n\n";
}

void Program::read_product(){
  std::vector<std::string> tokens = get_tokenized_line(std::cin);

  if(tokens.size() != 3) {
    m_error_msg = "Informações de produto inválidas";
    return;
  }

  try
  {
    Product new_product = parse_product(tokens);
    auto product_ptr = std::make_shared<Product>(new_product);
    m_catalog.add_product(product_ptr);
    m_curr_msg = "Produto adicionado.";
  }
  catch(const std::runtime_error& e)
  {
    m_error_msg = e.what();
  }
}

void Program::read_order(){
  std::cout << "\nNome do cliente > ";
  std::string customer;
  std::getline(std::cin, customer);

  std::cout << "\nProdutos (<id>;...;<id>) > ";

  std::vector<std::string> products = get_tokenized_line(std::cin);

  try
  {
    Order new_order = parse_order(customer, products);
    auto order_ptr = std::make_shared<Order>(new_order);
    m_orders.push_back(order_ptr);
    m_curr_msg = "Pedido adicionado.";
  }
  catch(const std::runtime_error& e)
  {
    m_error_msg = e.what();
  }
}


void Program::read_query(){
  std::string line;
  std::getline(std::cin, line);

  try
  {
    m_search_query = std::stoi(line);
    // process_enter();
  }
  catch(const std::exception& e)
  {
    m_error_msg = "ID inválido.";
  }
}

#pragma endregion // LEITURAS_E_ESCRITAS

#pragma region GAME_LOOP

void Program::process_events(){
  if(m_state == e_program_state::WELCOME){ 
    process_enter();
  } else if (m_state == e_program_state::LISTING_PRODUCTS ||
             m_state == e_program_state::LISTING_ORDERS) {
    process_enter();
  } else if (m_state == e_program_state::ADDING_PRODUCT) {
    read_product();
  } else if (m_state == e_program_state::CREATING_ORDER) {
    read_order();
  } else if (m_state == e_program_state::READING_FILE) {
    read_file();
    process_enter();
  } else if (m_state == e_program_state::SEARCHING_PRODUCTS) {
    read_query();
    search_product();
  } else if (m_state == e_program_state::SEARCHING_ORDERS) {
    read_query();
    search_order();
  } else if(m_state == e_program_state::SAVING) {
    std::getline(std::cin, m_file_path);

    write_file();
    m_curr_msg = "Arquivo salvo!";
  }  else if (m_state == e_program_state::READING_MENU_OPT) {
    std::string line;
    std::getline(std::cin, line);

    m_curr_msg = "";
    try
    {
      auto option_selected = std::stoi(line);

      if(option_selected > 8 || option_selected < 1) throw std::runtime_error("error");

      m_curr_menu_opt = (e_menu_opt)option_selected;
    }
    catch(const std::exception& e)
    {
      m_error_msg = "Opção inválida.";
      m_curr_menu_opt = e_menu_opt::INVALID;
    }
  }
}

void Program::update_state_from_menu() {
  if (m_curr_menu_opt == e_menu_opt::ADD_PRODUCT) {
    m_state = e_program_state::ADDING_PRODUCT;
  } else if (m_curr_menu_opt == e_menu_opt::LIST_PRODUCTS) {
    m_state = e_program_state::LISTING_PRODUCTS;
  } else if (m_curr_menu_opt == e_menu_opt::CREATE_ORDER) {
    m_state = e_program_state::CREATING_ORDER;
  } else if (m_curr_menu_opt == e_menu_opt::LIST_ORDERS) {
    m_state = e_program_state::LISTING_ORDERS;
  } else if (m_curr_menu_opt == e_menu_opt::SEARCH_ORDER) {
    m_state = e_program_state::SEARCHING_ORDERS;
  } else if (m_curr_menu_opt == e_menu_opt::SEARCH_PRODUCT) {
    m_state = e_program_state::SEARCHING_PRODUCTS;
  } else if (m_curr_menu_opt == e_menu_opt::SAVE) {
    m_state = e_program_state::SAVING;
  } else if (m_curr_menu_opt == e_menu_opt::EXIT) {
    m_state = e_program_state::QUITTING;
  }
}

void Program::update() {
  if(m_state == e_program_state::STARTING){
    if (!m_file_path.empty())
      m_state = e_program_state::READING_FILE;
    else
      m_state = e_program_state::WELCOME;
  } else if(m_state == e_program_state::WELCOME 
    || m_state == e_program_state::READING_FILE){
    m_state = e_program_state::READING_MENU_OPT;
  } else if(m_state == e_program_state::READING_MENU_OPT){
    update_state_from_menu();
  } else {
    m_state = e_program_state::READING_MENU_OPT;
  }
}

void Program::render() const {
  if(m_state == e_program_state::WELCOME){
    print_welcome();
  } else if(m_state == e_program_state::READING_MENU_OPT){
    print_menu();
  } else if(m_state == e_program_state::ADDING_PRODUCT){
    print_add_product();
  } else if(m_state == e_program_state::CREATING_ORDER){
    // Direto do método read_order
  } else if (m_state == e_program_state::LISTING_PRODUCTS) {
    print_products();
  } else if (m_state == e_program_state::LISTING_ORDERS) {
    print_orders();
  } else if (m_state == e_program_state::SEARCHING_PRODUCTS) {
    std::cout << "ID do produto > ";
  } else if (m_state == e_program_state::SEARCHING_ORDERS) {
    std::cout << "ID do pedido > ";
  } else if(m_state == e_program_state::SAVING){
    std::cout << "\nCaminho do arquivo de saída > ";
  }
}

#pragma endregion // GAME_LOOP

void Program::search_order(){
  std::cout << "searching orders";
}

void Program::search_product(){
  std::cout << "\n";
  
  print_catalog_header();
  
  auto product = m_catalog.search_product(m_search_query);
  product->print();
  
  print_catalog_footer();

  std::cout << "\n\n" << RESET << PRESS_ENTER;
  process_enter();
}
