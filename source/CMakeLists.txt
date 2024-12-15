project (Catalog VERSION 1.0.0 LANGUAGES CXX )

# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall" )
# set( PREPROCESSING_FLAGS  "-D PRINT -D DEBUG -D CASE="WORST" -D ALGO="QUAD"")
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS} ${PREPROCESSING_FLAGS}" )

#=== Main App ===

add_library(CatalogLib
    core/catalog.cpp
    core/order.cpp
)

target_include_directories(CatalogLib PUBLIC ${CMAKE_SOURCE_DIR}/source/core)

add_executable(catalog
    "program.cpp"
    "main.cpp"
)

target_link_libraries(catalog PRIVATE CatalogLib)

# target_compile_features( DateTimeLib PUBLIC cxx_std_17 )