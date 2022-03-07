set(TEST_FILE "log.txt")
add_custom_command(OUTPUT  ${TEST_FILE}
  COMMAND echo "Generating log.txt file..."
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_FILE} ${TEST_FILE}
  COMMENT  "This is a test"
 )
add_custom_target(Test1 ALL DEPENDS ${TEST_FILE})
#ALL选项，那就表明该目标会被添加到默认的构建目标，使得它每次都被运行

add_custom_command(TARGET Test1
  PRE_BUILD 
  COMMAND echo "executing a fake command"
  COMMENT "This command will be executed before building target Test1"
 )
