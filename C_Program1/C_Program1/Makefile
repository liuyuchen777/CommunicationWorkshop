target=awgn
src=$(wildcard ./src/*.c)
inc=$(wildcard ./inc/*.h)

all:awgn
$(target): $(src) $(inc)
	gcc $^ -o $@ -lm

.PHONY:clean
clean:
	-rm awgn