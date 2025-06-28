################################################################################

CC		= gcc
CXX		= g++
CPPFLAGS	= -I/home/alu/rpi-rgb-led-matrix/include
CFLAGS		= -march=native -O3 -funroll-loops -fpic -fstack-protector-all \
		  -Wall -Wextra -Werror -pipe -Wno-unused-parameter
LDFLAGS		= -L. -lrgbmatrix -li2c -lrt -lm -lpthread -s
REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

.c.o:
	$(REASON)
	$(COMPILE.c) $< $(OUTPUT_OPTION)

################################################################################

all: demo-matrix max print calib-val calib-hue demo-orient calib-orient demo-particle \
	ppms2matrix

demo-matrix: demo-matrix.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

max: max.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

print: print.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

calib-val: calib-val.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

calib-hue: calib-hue.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

demo-orient: demo-orient.o orient.o mpu6050.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

calib-orient: calib-orient.o mpu6050.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

demo-particle: demo-particle.o orient.o mpu6050.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

################################################################################

ppms2matrix: ppms2matrix.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lnetpbm

################################################################################

rgbmatrix.h librgbmatrix.o: build
	$(REASON)
	./build

deps depend: *.h *.c
	$(REASON)
	$(CC) -MM $(CPPFLAGS) *.c > deps

clean:
	$(REASON)
	$(RM) *.o *~ deps

-include deps
 
################################################################################
