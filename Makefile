all: example_dict example_json utest

example_dict: ndict.cpp ndict.h example_dict.cpp
	g++ -o example_dict ndict.cpp example_dict.cpp

example_json: ndict.cpp ndict.h njson.cpp njson.h example_json.cpp
	g++ -o example_json ndict.cpp njson.cpp example_json.cpp


utest: ndict.cpp ndict.h njson.cpp njson.h utest.cpp
	g++ -Wall -o utest ndict.cpp njson.cpp utest.cpp

dist: clean
	tar czvf ndict.tar.gz --transform "s+^+ndict/+" \
	    LICENSE README.md example_json.cpp ndict.doxy njson.cpp utest.cpp \
	    Makefile example_dict.cpp ndict.cpp ndict.h njson.h
doxygen:
	doxygen ndict.doxy

clean:
	rm -rf utest example_dict example_json doxy/ ndict.tar.gz
