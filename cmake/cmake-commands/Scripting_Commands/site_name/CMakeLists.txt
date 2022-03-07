site_name("lhx-cp") ##没有效果???
cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
message(STATUS "lhx test ${hostname}")
