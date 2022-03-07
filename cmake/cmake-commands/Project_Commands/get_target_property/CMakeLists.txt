add_executable(main main.c)
set_target_properties(main PROPERTIES COMPILE_DEFINITIONS -Dtest)
get_target_property(result main COMPILE_DEFINITIONS)
message("================${result}==============")
