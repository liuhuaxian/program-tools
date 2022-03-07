separate_arguments(result_list UNIX_COMMAND "touch lhx yll lr")
foreach(var IN LISTS result_list)
	message("================${var}=============")
endforeach()
