add_link_options(-lmain)
link_directories(${CMAKE_CURRENT_SOURCE_DIR})##存在问题?? 将add_library去掉则报错
add_library(main main.c)
add_executable(main main.c)
