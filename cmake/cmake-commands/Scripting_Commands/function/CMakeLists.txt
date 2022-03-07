function(foo)
	message(STATUS "lhx test")
endfunction()

foo()
Foo()
FOO()
cmake_language(CALL foo)
