cmake_minimum_required(VERSION 3.23)
project(variant_impl CXX)

add_library(${PROJECT_NAME} INTERFACE include/variant/detail/node_interleave.hpp)
target_include_directories(${PROJECT_NAME} INTERFACE include)

add_library(variant::variant ALIAS variant_impl)