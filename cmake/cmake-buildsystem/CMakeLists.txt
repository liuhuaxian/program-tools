#连接默认的库文件
add_library(math add.c sub.c)
add_executable(main main.c)
target_link_libraries(main math)

#指定生成库文件的类型
add_library(math_share SHARED add.c sub.c)
add_library(math_module MODULE add.c sub.c)

#架构相关的库
add_library(MyFramework SHARED add.c sub.c)
set_target_properties(MyFramework PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  MACOSX_FRAMEWORK_IDENTIFIER org.cmake.MyFramework
)

#对象库类型的创建
add_library(math_object OBJECT add.c)

add_library(math_use_object STATIC $<TARGET_OBJECTS:math_object> sub.c)

add_executable(main_object $<TARGET_OBJECTS:math_object> main.c)

add_executable(main_link_object main.c)
target_link_libraries(main_link_object math_object)


#定义对象的属性(编译器宏的定义-D )
set(srcs add.c sub.c div.c)
add_library(math_have_div SHARED ${srcs})
target_compile_definitions(math_have_div PRIVATE HAVE_A_ADD_TEN) #-DHAVE_A_ADD_TEN for math_have_div不进行传递
target_compile_definitions(math_have_div INTERFACE USING_ARCHIVE_LIB) # -DUSING_ARCHIVE_LIB 传递给本库的使用者,本库不使用该宏 

add_executable(main_define main.c)
target_link_libraries(main_define math_have_div)#main_define is compiled with -DUSING_ARCHIVE_LIB



#传递使用需求
add_library(add_transitive add.c)
target_compile_definitions(add_transitive INTERFACE HAVE_ADD_MODIFY)

add_library(div_transitive div.c)
target_compile_definitions(div_transitive INTERFACE HAVE_A_ADD_TEN)

add_library(math_transitive sub.c)
target_link_libraries(math_transitive PUBLIC add_transitive)
target_link_libraries(math_transitive PRIVATE div_transitive)
# math_transitiveis compiled with -DHAVE_ADD_MODIFY and -DHAVE_A_ADD_TEN

add_executable(main_transitive main.c)
# main_transitive compiled with -DHAVE_ADD_MODIFY and -DHAVE_A_ADD_TEN ?????
target_link_libraries(main_transitive math_transitive)


add_executable(main_change_order main.c)
target_link_libraries(main_change_order div_transitive add_transitive math_transitive)
target_include_directories(main_change_order
  PRIVATE $<TARGET_PROPERTY:math_transitive,INTERFACE_INCLUDE_DIRECTORIES>)

#Compatible Interface Properties
add_executable(main_position_independent main.c add.c) #-fPIC
set_property(TARGET main_position_independent PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(math_interface_properties SHARED add.c sub.c div.c)#该动态库使用-fPIC编译选项
#INTERFACE_POSITION_INDEPENDENT_CODE属性使使用该库的程序也会继承编译选项-fPIE
set_property(TARGET math_interface_properties PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

add_executable(main_interface_properties main.c) #-fPIE 继承自math_interface_properties
target_link_libraries(main_interface_properties math_interface_properties)

#--------------------------------------------------------------------------------------------------#


#以下命令行会得到以下报错但能调整编译选项编译出可执行的结果
#Configuring done
#CMake Error: Property POSITION_INDEPENDENT_CODE on target "exe1" does
#not match the INTERFACE_POSITION_INDEPENDENT_CODE property requirement
#of dependency "lib1".
#
#CMake Error: Property POSITION_INDEPENDENT_CODE on target "exe1" does
#not match the INTERFACE_POSITION_INDEPENDENT_CODE property requirement
#of dependency "lib1".
#
#CMake Error: The INTERFACE_POSITION_INDEPENDENT_CODE property of "lib2" does
#not agree with the value of POSITION_INDEPENDENT_CODE already determined
#for "exe2".
#
#CMake Error: The INTERFACE_POSITION_INDEPENDENT_CODE property of "lib2" does
#not agree with the value of POSITION_INDEPENDENT_CODE already determined
#for "exe2".

add_library(lib1 SHARED add.c)
set_property(TARGET lib1 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE ON)

add_library(lib2 SHARED sub.c)
set_property(TARGET lib2 PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE OFF)

add_executable(exe1 main.c)
target_link_libraries(exe1 lib1)#编译不带-fPIC
set_property(TARGET exe1 PROPERTY POSITION_INDEPENDENT_CODE OFF)

add_executable(exe2 main.c)#编译带-fPIC
target_link_libraries(exe2 lib1 lib2)

#---------------------------------------------------------------------------------#


add_library(lib1Version2 SHARED add.c)
set_property(TARGET lib1Version2 PROPERTY INTERFACE_CUSTOM_PROP ON)
set_property(TARGET lib1Version2 APPEND PROPERTY
  COMPATIBLE_INTERFACE_BOOL CUSTOM_PROP
)


add_library(lib1Version3 SHARED sub.c)
set_property(TARGET lib1Version3 PROPERTY INTERFACE_CUSTOM_PROP OFF)

add_executable(compatible_main main.c)
target_link_libraries(compatible_main lib1Version2) # CUSTOM_PROP will be ON 

add_executable(uncompatible_main main.c)
target_link_libraries(uncompatible_main lib1Version2 lib1Version3) # Diagnostic
#CMake Error: The INTERFACE_CUSTOM_PROP property of "lib1Version3" does
#not agree with the value of CUSTOM_PROP already determined for "uncompatible_main".

#eg2
add_library(addVersion2 SHARED add.c)
set_property(TARGET addVersion2 PROPERTY INTERFACE_LIB_VERSION 2)
set_property(TARGET addVersion2 APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING LIB_VERSION
)

add_library(subVersion3 SHARED sub.c)
set_property(TARGET subVersion3 PROPERTY INTERFACE_LIB_VERSION 3)

add_executable(compatible_string_main main.c)
target_link_libraries(compatible_string_main addVersion2) # LIB_VERSION will be "2"

add_executable(uncompatible_string_main main.c)
target_link_libraries(uncompatible_string_main addVersion2 subVersion3) # Diagnostic


#---------------Property Origin Debugging ----------------------------#
set(CMAKE_DEBUG_TARGET_PROPERTIES
  INCLUDE_DIRECTORIES
  COMPILE_DEFINITIONS
  POSITION_INDEPENDENT_CODE
  CONTAINER_SIZE_REQUIRED
  LIB_VERSION
)

#编译过程中将输出这些属性用到了那些执行文件上.

#CMake Debug Log at CMakeLists.txt:49 (target_link_libraries):
#  Used compile definitions for target math_transitive:

#  * HAVE_A_ADD_TEN




#--------------Build Specification with Generator Expressions--------------------------#
add_library(add_expressions SHARED add.c sub.c div.c)
set_property(TARGET add_expressions PROPERTY
  INTERFACE_CONTAINER_SIZE_REQUIRED 200)
set_property(TARGET add_expressions APPEND PROPERTY
  COMPATIBLE_INTERFACE_NUMBER_MAX CONTAINER_SIZE_REQUIRED
)

add_executable(main_define_val main.c)
target_link_libraries(main_define_val add_expressions) #-DCONTAINER_SIZE=200
target_compile_definitions(main_define_val PRIVATE
    CONTAINER_SIZE=$<TARGET_PROPERTY:CONTAINER_SIZE_REQUIRED>
)



add_library(math_expressions1 add.c sub.c)
target_compile_definitions(math_expressions1 INTERFACE ##供使用该库的对象继承
  $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:LIB1_WITH_EXE>
  $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:LIB1_WITH_SHARED_LIB>
  $<$<TARGET_POLICY:CMP0041>:CONSUMER_CMP0041_NEW>
)

add_executable(main_executable main.c) #-DLIB1_WITH_EXE 
target_link_libraries(main_executable math_expressions1)

cmake_policy(SET CMP0041 NEW)

add_library(math_expressions2 SHARED  div.c) ##-DLIB1_WITH_SHARED_LIB -DCONSUMER_CMP0041_NEW 
target_link_libraries(math_expressions2 math_expressions1)


#----------Include Directories and Usage Requirements------------------------------#







