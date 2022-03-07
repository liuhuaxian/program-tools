add_executable(main main.c) #使用有问题
define_property(TARGET PROPERTY test BRIEF_DOCS "this is test" FULL_DOCS "this my test")
set_property(TARGET main PROPERTY test hello)
get_property(result TARGET main PROPERTY test)
message("----------------${reslut}==========")
