ROOT=../..
SINGLEHEADER=$(ROOT)/singleheader
JSONEXAMPLES=$(ROOT)/jsonexamples

test: quickstart twitter.json
	./quickstart
quickstart: quickstart.cpp simdjson2.cpp simdjson2.h
	c++ -o ./quickstart quickstart.cpp simdjson2.cpp
clean:
	rm -f simdjson2.cpp simdjson2.h twitter.json quickstart quickstart11 quickstart14
simdjson2.cpp: $(SINGLEHEADER)/simdjson2.cpp
	cp $(SINGLEHEADER)/simdjson2.cpp .
simdjson2.h: $(SINGLEHEADER)/simdjson2.h
	cp $(SINGLEHEADER)/simdjson2.h .
twitter.json: $(JSONEXAMPLES)/twitter.json
	cp $(JSONEXAMPLES)/twitter.json .

quickstart11: $(ROOT)/src/**.h $(ROOT)/src/**.cpp $(ROOT)/include/**.h $(ROOT)/src/**.cpp
	rm -f simdjson2.h simdjson2.cpp
	c++ -o ./quickstart11 quickstart.cpp $(ROOT)/src/simdjson2.cpp -I$(ROOT)/src -I$(ROOT)/include -std=c++11
test11: quickstart11 twitter.json
	./quickstart11
quickstart14: $(ROOT)/src/**.h $(ROOT)/src/**.cpp $(ROOT)/include/**.h $(ROOT)/src/**.cpp
	rm -f simdjson2.h simdjson2.cpp
	c++ -o ./quickstart14 quickstart.cpp $(ROOT)/src/simdjson2.cpp -I$(ROOT)/src -I$(ROOT)/include -std=c++14
test14: quickstart14 twitter.json
	./quickstart14
quickstart17: $(ROOT)/src/**.h $(ROOT)/src/**.cpp $(ROOT)/include/**.h $(ROOT)/src/**.cpp
	rm -f simdjson2.h simdjson2.cpp
	c++ -o ./quickstart17 quickstart.cpp $(ROOT)/src/simdjson2.cpp -I$(ROOT)/src -I$(ROOT)/include -std=c++17
test17: quickstart17 twitter.json
	./quickstart17
