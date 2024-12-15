# Adiciona o executável de teste
add_executable(catalog_tests
    integration_tests.cpp
    unit_tests.cpp
)

# Liga o Google Test e a biblioteca a ser testada ao executável
target_link_libraries(catalog_tests
    gtest
    gtest_main
    CatalogLib
)

# Inclui diretórios de headers
target_include_directories(catalog_tests PRIVATE ${CMAKE_SOURCE_DIR}/source)

# Adiciona os testes para serem executados com "ctest"
enable_testing()
add_test(NAME CatalogTests COMMAND catalog_tests)
