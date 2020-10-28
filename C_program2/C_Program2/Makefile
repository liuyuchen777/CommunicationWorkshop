target=coherent
src=$(wildcard ./src/*.c)
inc=$(wildcard ./inc/*.h)

all:$(target)
$(target): $(src) $(inc)
	gcc $^ -o $@ -lm

.PHONY:clean
clean:
	-rm awgn