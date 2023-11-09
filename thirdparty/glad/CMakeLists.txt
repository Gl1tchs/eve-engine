cmake_minimum_required(VERSION 3.25)

project(glad)

set(SOURCES
  include/glad/glad.h
  include/KHR/khrplatform.h
  src/glad.c
)

add_library(glad STATIC ${SOURCES})

target_include_directories(glad PUBLIC include/)
