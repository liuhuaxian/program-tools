all: rfvmreglic rfhareglic rfaxreglic rflicchk checksn libcheckrfsn.so liblicense.so rfreglic liblicense.a

rflicchk: rflicchk.c libcheckrfsn.so
	gcc  rflicchk.c -L. -lssl -lcrypt -lcheckrfsn -o rflicchk
	strip rflicchk

rfvmreglic: rfvmreglic.c  libcheckrfsn.so
	gcc rfvmreglic.c -L. -lssl -lcrypt -lcheckrfsn -o rfvmreglic
	strip rfvmreglic

rfhareglic: rfhareglic.c  libcheckrfsn.so
	gcc rfhareglic.c -L. -lssl -lcrypt -lcheckrfsn -o rfhareglic
	strip rfhareglic

rfaxreglic: rfaxreglic.c  libcheckrfsn.so
	gcc rfaxreglic.c -L. -lssl -lcrypt -lcheckrfsn -o rfaxreglic
	strip rfaxreglic

rfreglic: rfreglic.c libcheckrfsn.so
	gcc rfreglic.c -L. -lssl -lcrypt -lcheckrfsn -o rfreglic
	strip rfreglic

checksn: checksn.cpp libcheckrfsn.so
	g++ procini.cpp checksn.cpp -fpic -L. -lssl -lcrypt -lcheckrfsn -I/include -I/usr/include/qt5/ -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtWidgets -L/lib `pkg-config --libs --cflags Qt5Core` -o checksn
	strip checksn

libcheckrfsn.so: check.c crypt.c crypt.h regchk.c regchk.h oplicense.h debug.h lic_error.h prod_config.h oplicense.c  
	gcc check.c prod_config.h crypt.c crypt.h regchk.c regchk.h lic_error.h debug.h oplicense.c oplicense.h -O2 -lssl -shared -fPIC -o libcheckrfsn.so
	strip libcheckrfsn.so

liblicense.so: license.c license.h libcheckrfsn.so
	gcc license.c license.h -L. -lssl -lcrypt -lcheckrfsn -O2 -shared -fPIC -o liblicense.so
	strip liblicense.so

#gensn: procini.cpp genui.cpp gen.c genui.h macchk.c macchk.h
#	moc genui.h -o genui.moc
#	g++ gen.c genui.cpp procini.cpp macchk.c macchk.h -O2 -I$(QTDIR)/include -I/usr/include/qt5/ -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtWidgets  -L$(QTDIR)/lib -L. -lssl -lcrypto -lqt-mt -fPIC -o gensn
#	strip gensn

liblicense.a: license.c license.h check.c crypt.c crypt.h regchk.c regchk.h oplicense.h debug.h lic_error.h prod_config.h oplicense.c
	gcc -c license.c check.c crypt.c regchk.c oplicense.c
	ar cr liblicense.a license.o check.o crypt.o regchk.o oplicense.o

clean:
	rm -rf rflicchk rfvmreglic rfhareglic rfaxreglic checksn libcheckrfsn.so genui.moc liblicense.so rfreglic liblicense.a *.o
