all: xinyuw

xinyuw: syntax.lex.c syntax.tab.c xinyuw.cpp dstruct.cpp
	@c++ -g $^ -I ../../include -L ../../bin -lmatrix -o ../../bin/$@

syntax.lex.c: syntax.l
	@flex $^

syntax.tab.c: syntax.y
	@bison -rall -d $^

clean:
	@rm -f syntax.lex.* syntax.tab.* *.output ../../bin/xinyuw
