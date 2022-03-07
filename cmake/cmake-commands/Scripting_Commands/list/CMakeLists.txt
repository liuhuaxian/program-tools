set (testlist
	lhx
	yll
	lhq
	)
list(APPEND testlist lr)
list(REMOVE_AT testlist lhx)
list(SORT testlist)
foreach(var IN LISTS testlist)
	message("====${var}====")
endforeach()

