add_custom_command(OUTPUT test COMMAND echo lhx test)

add_executable(main main.c test)
