set_directory_properties(PROPERTIES  ADDITIONAL_CLEAN_FILES "foo")
get_directory_property(result ADDITIONAL_CLEAN_FILES) ##属性名详见cmake-properties手册
message(STATUS "-----------${result}------")
