# Build the common derived files

DERIVED = lex.yy.c y.tab.c y.tab.h

all: $(DERIVED)

lex.yy.c: ../../src-common/cfdg.l
	flex ../../src-common/cfdg.l

y.tab.c y.tab.h: ../../src-common/cfdg.y
	yacc -d ../../src-common/cfdg.y

clean:
	rm $(DERIVED)
