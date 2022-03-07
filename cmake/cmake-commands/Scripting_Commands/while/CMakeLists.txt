set(foo 1)
while(${foo} EQUAL 1)
	message(STATUS "lhx test")
	set(foo 2)
endwhile()

