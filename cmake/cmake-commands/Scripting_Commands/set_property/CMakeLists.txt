set_property(DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES "foo.c")
#set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "foo.c")
get_property(result DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES)
message("-------------------${result}========")
