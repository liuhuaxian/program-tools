add_executable(main main.c)
set_source_files_properties(main.c  PROPERTIES COMPILE_DEFINITIONS DEFINE_A)
get_source_file_property(result main.c COMPILE_DEFINITIONS)
message("===========${result}==========")
