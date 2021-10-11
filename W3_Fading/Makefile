target=main
src=$(wildcard ./src/*.c)
inc=$(wildcard ./inc/*.h)

all:$(target)
$(target): $(src) $(inc)
	gcc $^ -o $@ -lm -g

.PHONY:clean
clean:
	-rm $(target)