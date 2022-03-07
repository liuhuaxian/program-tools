add_executable(main main.c)
target_compile_features(main PUBLIC cxx_constexpr) ##参考cmake-compile-features文档
