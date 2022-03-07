add_custom_target(test COMMAND touch lhx COMMENT "this is a test") #test必须为一个target 使用add_custom_commad则不行
add_executable(main main.c)
add_dependencies(main test)
