
all: cfdg


#
# Dirs
#

OBJ_DIR = objs

COMMON_DIR = src-common
UNIX_DIR = src-unix
DERIVED_DIR = $(OBJ_DIR)
AGG_DIR = src-agg

SRC_DIRS = $(COMMON_DIR) $(UNIX_DIR) $(DERIVED_DIR) $(AGG_DIR)/src
vpath %.cpp $(SRC_DIRS)
vpath %.c	$(SRC_DIRS)

INC_DIRS = $(COMMON_DIR) $(UNIX_DIR) $(DERIVED_DIR) $(AGG_DIR)/include $(COMMON_DIR)/agg-extras

#
# Sources and Objects
#

COMMON_SRCS = cfdg.cpp Rand48.cpp makeCFfilename.cpp \
	cfdgimpl.cpp renderimpl.cpp builder.cpp shape.cpp \
	yglue.cpp variation.cpp countable.cpp tempfile.cpp \
	aggCanvas.cpp HSBColor.cpp SVGCanvas.cpp \
	primShape.cpp bounds.cpp shape.cpp shapeSTL.cpp tiledCanvas.cpp

UNIX_SRCS = pngCanvas.cpp posixSystem.cpp main.cpp posixTimer.cpp \
    posixVersion.cpp

DERIVED_SRCS = lex.yy.c y.tab.c

AGG_SRCS = agg_trans_affine.cpp agg_curves.cpp agg_vcgen_contour.cpp \
    agg_vcgen_stroke.cpp agg_bezier_arc.cpp

SRCS = $(COMMON_SRCS) $(UNIX_SRCS) $(DERIVED_SRCS) $(AGG_SRCS)
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o, \
	$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS)))
DEPS = $(patsubst %.o,%.d,$(OBJS))

deps: $(OBJ_DIR) $(DEPS)
include $(DEPS)

$(OBJS): $(OBJ_DIR)/Sentry

#
# Executable
#
# Under Mac OS X replace $(LINK.o) with g++.
# Under Cygwin replace strip $@ with strip $@.exe

cfdg: $(OBJS)
	$(LINK.o) $^ -L/usr/local/lib -lstdc++ -lpng -lz -fexceptions -o $@
	strip $@


#
# Derived
#

$(DERIVED_DIR)/lex.yy.c: $(COMMON_DIR)/cfdg.l
	flex -t $^ > $@

$(DERIVED_DIR)/y.tab.h: $(DERIVED_DIR)/y.tab.c
$(DERIVED_DIR)/y.tab.c: $(COMMON_DIR)/cfdg.y
	cd $(DERIVED_DIR); yacc -d ../$(COMMON_DIR)/cfdg.y

$(OBJ_DIR)/lex.yy.o: $(DERIVED_DIR)/y.tab.h

#
# Utility
#

clean :
	rm -f $(OBJ_DIR)/*
	rm -f cfdg

distclean: clean
	rmdir $(OBJ_DIR)

$(OBJ_DIR)/Sentry :
	mkdir -p $(OBJ_DIR) 2> /dev/null || true
	touch $@

#
# Tests
#

RTEST_CFDG = input/rendering-tests.cfdg
OUTPUT_DIR = output

rtests: $(OUTPUT_DIR)/rtest-700.png $(OUTPUT_DIR)/rtest-2k.png

$(OUTPUT_DIR)/rtest-700.png: cfdg $(RTEST_CFDG)
	./cfdg -s 700 $(RTEST_CFDG) $@
$(OUTPUT_DIR)/rtest-2k.png: cfdg $(RTEST_CFDG)
	./cfdg -s 2000 $(RTEST_CFDG) $@

#
# Rules
#

CPPFLAGS += $(patsubst %,-I%,$(INC_DIRS))
CPPFLAGS += -O3 -Wall
#CPPFLAGS += -ggdb

$(OBJ_DIR)/%.o : %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.o : %.c
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJ_DIR)/%.d : %.cpp
	mkdir -p $(OBJ_DIR) 2> /dev/null || true
	set -e; $(COMPILE.cpp) -MM $< \
	| sed 's,\(.*\.o\)\( *:\),$(OBJ_DIR)/\1 $@\2,g' > $@; \
	[ -s $@ ] || rm -f $@

$(OBJ_DIR)/%.d : %.c
	mkdir -p $(OBJ_DIR) 2> /dev/null || true
	set -e; $(COMPILE.c) -MM $< \
	| sed 's,\(.*\.o\)\( *:\),$(OBJ_DIR)/\1 $@\2,g' > $@; \
	[ -s $@ ] || rm -f $@

$(OBJ_DIR)/y.tab.d:

$(OBJ_DIR)/lex.yy.d:

