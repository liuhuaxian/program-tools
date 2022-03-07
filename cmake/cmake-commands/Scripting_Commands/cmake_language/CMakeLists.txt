set(message_command "message")
cmake_language(CALL ${message_command} STATUS "Hello World!")

message(STATUS "Hello World!")


set(A TRUE)
set(B TRUE)
set(C TRUE)
set(condition "(A AND B) OR C")

cmake_language(EVAL CODE "
  if (${condition})
    message(STATUS TRUE)
  else()
    message(STATUS FALSE)
  endif()"
)

set(A TRUE)
set(B TRUE)
set(C TRUE)
set(condition "(A AND B) OR C")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/eval.cmake "
  if (${condition})
    message(STATUS TRUE)
  else()
    message(STATUS FALSE)
  endif()"
)

include(${CMAKE_CURRENT_BINARY_DIR}/eval.cmake)
