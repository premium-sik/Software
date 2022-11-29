OBJS = main.o fx_s4_3.o
main: $(OBJS)
	cc $(OBJS) -o $@
main.o: main.c fx_head.h fx_s4_3.h
	cc -c $<
fx_s4_3.o: fx_s4_3.c fx_s4_3.h
	cc -c $<
clean:
	rm main $(OBJS)

