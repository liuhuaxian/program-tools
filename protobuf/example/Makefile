all:addressbook read write  

write : wirte.cc 
	g++ wirte.cc addressbook.pb.cc `pkg-config --libs --cflags protobuf` -o write
read : read.cc
	g++ read.cc addressbook.pb.cc `pkg-config --libs --cflags protobuf` -o read
addressbook : addressbook.proto
	protoc --cpp_out=. addressbook.proto
clean :
	rm read write addressbook.pb.cc addressbook.pb.h
