find_package(Boost REQUIRED)
#find_package(lhx REQUIRED) #若没找到lhx 终止cmake的执行
find_package(lhx) #只是报错不终止cmake的执行
find_package(ZLIB REQUIRED)
#需要/usr/share/cmake/Modules下有对应FindBoost.cmake find_package(Boost)才能正常运行
