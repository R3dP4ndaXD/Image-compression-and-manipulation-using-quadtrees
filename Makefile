CC = gcc
CFLAGS = -Wall -Werror -std=c11
DEPS = quadtree_comp.h quadtree_decomp.h queue.h quadtree_struct.h image_processing.h
OBJ = quadtree.o quadtree_comp.o quadtree_decomp.o queue.o image_processing.o

build: quadtree

quadtree: $(OBJ) $(DEPS)
	gcc $(CFLAGS) $(OBJ) -o $@ -lm
quadtree.o: quadtree.c $(DEPS)

quadtree_comp.o: quadtree_comp.c quadtree_comp.h queue.h quadtree_struct.h
 
quadtree_decomp.o: quadtree_decomp.c quadtree_decomp.h queue.h quadtree_struct.h

queue.o: queue.c queue.h quadtree_struct.h

image_processing.o: image_processing.c image_processing.h

run:
	./quadtree

.PHONY: clean
clean:
	rm *.o quadtree