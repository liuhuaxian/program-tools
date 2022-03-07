option(FOO_ENABLE "Enable Foo" ON)
message(STATUS "${FOO_ENABLE}")
message(STATUS "${FOO_STRING}")
if(FOO_ENABLE)
  set(FOO_STRING "test")
endif()
message(STATUS "${FOO_STRING}")
configure_file(foo.h.in foo.h )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

