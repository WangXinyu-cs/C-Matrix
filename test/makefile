VPATH = .:../include:../bin
ALL_BIN=test
api_head := vector.h matrix.h

all: $(ALL_BIN)

test: test.cpp $(api_head)
	$(CXX) -g $^ -I ../include -L ../bin -lmatrix -o $@

clean:
	rm -f $(ALL_BIN)
