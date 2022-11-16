main:
	gcc main.c -o main -lm -lisal -I isa-l-master/include -L isa-l-master/bin helper/absolutePath.c helper/fullSize.c helper/merge.c helper/nameext.c helper/split.c