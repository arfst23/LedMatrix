################################################################################

CC		= gcc
CXX		= g++
CPPFLAGS	= -I/home/alu/rpi-rgb-led-matrix/include
CFLAGS		= -march=native -O3 -funroll-loops -fpic -fstack-protector-all \
		  -Wall -Wextra -Werror -pipe -Wno-unused-parameter
LDFLAGS		= -L. -lrgbmatrix -lrt -lpthread -s
REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

.c.o:
	$(REASON)
	$(COMPILE.c) $< $(OUTPUT_OPTION)

################################################################################

all: matrix-min matrix-rgb matrix-max matrix-rot matrix-txt matrix-rain playppm

matrix-min: matrix-min.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-rgb: matrix-rgb.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-max: matrix-max.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-rot: matrix-rot.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-txt: matrix-txt.o text.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-rain: matrix-rain.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

matrix-calib: matrix-calib.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

gamma.h:
	gamma 1.45 100 > gamma.h

playppm.o: gamma.h

playppm: playppm.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lnetpbm

################################################################################

# rgbmatrix.h librgbmatrix.a: build
# 	$(REASON)
# 	./build

deps depend: *.h *.c
	$(REASON)
	$(CC) -MM $(CPPFLAGS) *.h *.c > deps

clean:
	$(REASON)
	$(RM) *.o *~ deps gamma.h

distclean: clean
	$(REASON)
	$(RM) matrix-min matrix-rgb matrix-max matrix-rot matrix-txt matrix-rain playppm

-include deps
 
################################################################################
