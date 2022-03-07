macro(foo)
	message(STATUS "lhx test")
endmacro()

foo()
Foo()
FOO()
cmake_language(CALL foo)
