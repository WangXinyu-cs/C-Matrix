VPATH = .:../../include:../../bin
name := datatype matrix_impl vector_impl
src := $(addsuffix .cpp, $(name))
head := $(addsuffix .h, $(name))
api_head := matrix.h vector.h

all: libmatrix.so

libmatrix.so: $(src) $(head) $(api_head)
	echo $(src) $(head)
	$(CXX) -g  -shared -fPIC $^ -I ../../include -o ../../bin/$@

clean:
	rm -f ../../bin/*.so
