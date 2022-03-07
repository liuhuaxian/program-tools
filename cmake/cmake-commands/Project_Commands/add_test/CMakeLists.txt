enable_testing() #可以开启项目的测试功能
add_executable(main main.c)
add_test(NAME test1 COMMAND main)
set_tests_properties(test1 PROPERTIES PASS_REGULAR_EXPRESSION "in main")#验证结果是否正确
