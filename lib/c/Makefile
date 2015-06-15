CCFLAGS=-I"."


teacrypt : tests.o pystring/pystring.o pystring/pystring_join.o
		cc -o tests tests.o pystring/pystring.o pystring/pystring_join.o \
			pystring/pystring_split.o $(CCFLAGS)

tests.o : tests.c tea.h pystring.h
	cc -c tests.c $(CCFLAGS)

pystring/pystring.o : pystring/pystring.c pystring/private.h
	cc -c pystring/pystring.c -o pystring/pystring.o $(CCFLAGS)

pystring/pystring_join.o : pystring/pystring_join.c pystring/private.h
	cc -c pystring/pystring_join.c  -o pystring/pystring_join.o $(CCFLAGS)

pystring/pystring_split.o : pystring.h pystring/pystring_split.o pystring/private.h
	cc -c pystring/pystring_split.c  -o pystring/pystring_split.o $(CCFLAGS)

clean :
	rm tests tests.o pystring/pystring.o pystring/pystring_split.o pystring/pystring_join.o  $(CCFLAGS)
