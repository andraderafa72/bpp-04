cmake_minimum_required(VERSION 3.14)
project(DateTime)

# Define o padrão de C++ (por exemplo, C++17)
set(CMAKE_CXX_STANDARD 17)

# Adiciona o diretório "source" ao projeto
add_subdirectory(source)

# Adiciona o Google Test como subdiretório
add_subdirectory(external/googletest)

# Adiciona o diretório "test" ao projeto
add_subdirectory(tests)
