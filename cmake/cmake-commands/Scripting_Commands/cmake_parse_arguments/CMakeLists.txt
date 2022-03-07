macro(my_install)
    set(options OPTIONAL FAST)
    set(oneValueArgs DESTINATION RENAME)
    set(multiValueArgs TARGETS CONFIGURATIONS)
    cmake_parse_arguments(PREFIX_TEST "${options}" "${oneValueArgs}"
	    "${multiValueArgs}" ${ARGN})
endmacro()

my_install(TARGETS foo bar DESTINATION bin OPTIONAL test FAST lhq CONFIGURATIONS lhx ai yll RENAME)

message(STATUS "${PREFIX_TEST_TARGETS}")
message(STATUS "${PREFIX_TEST_OPTIONAL}")
message(STATUS "${PREFIX_TEST_FAST}")
message(STATUS "${PREFIX_TEST_DESTINATION}")
message(STATUS "${PREFIX_TEST_RENAME}")
message(STATUS "${PREFIX_TEST_CONFIGURATIONS}")
message(STATUS "${PREFIX_TEST_KEYWORDS_MISSING_VALUES}")
