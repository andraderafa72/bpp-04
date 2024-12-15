# Catálogo de produtos

## Compilar e rodar o projeto (Linux)
```bash
cmake -S . -B build
cmake --build build
```

### Executar o programa
```bash
./build/source/catalog
```

### Executar testes
```bash
./build/tests/catalog_tests
```

## Habilitar Logging
* Logging pode bagunçar a interface quando estiver rodando a aplicação via `build/source/catalog`


```c++
// source/core/common.h

#define LOGGING true // false
```
