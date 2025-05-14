################################################################################

CC		= gcc
CXX		= g++
CPPFLAGS	= -I/home/alu/rpi-rgb-led-matrix/include
CFLAGS		= -march=native -O3 -funroll-loops -fpic -fstack-protector-all \
		  -Wall -Wextra -Werror -pipe -Wno-unused-parameter
LDFLAGS		= -L/home/alu/rpi-rgb-led-matrix/lib -lrgbmatrix \
		  -lrt -li2c -lm -lpthread
REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

.c.o:
	$(REASON)
	$(COMPILE.c) $< $(OUTPUT_OPTION)

################################################################################

matrix-demo: matrix-demo.o matrix.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

mpu6050-demo: mpu6050-demo.o mpu6050.o
	$(REASON)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

################################################################################

deps depend: *.h *.c
	$(REASON)
	$(CC) -MM $(CPPFLAGS) *.c > deps

clean:
	$(REASON)
	$(RM) *.o *~ deps

-include deps
 
################################################################################
