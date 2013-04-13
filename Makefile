# 1b5e2204cd2c47982353d38a705f3b4d
CFLAGS=-O$(O) -std=c++11
O=2
LFLAGS=-lboost_system -lboost_thread
OBJS=objs/main.o objs/filereader.o objs/chunkinfo.o


.PHONY: all
all: objs boost-wc

./boost-wc: $(OBJS)
	@ echo "    LINK ./boost-wc"
	@ $(CXX) $(OBJS) -o "./boost-wc" $(LFLAGS)

objs/main.o: main.cpp filereader.h chunkinfo.h
	@ echo "    CXX  ./main.cpp"
	@ $(CXX) $(CFLAGS) -c "./main.cpp" -o $@
objs/filereader.o: filereader.cpp filereader.h chunkinfo.h
	@ echo "    CXX  ./filereader.cpp"
	@ $(CXX) $(CFLAGS) -c "./filereader.cpp" -o $@
objs/chunkinfo.o: chunkinfo.cpp chunkinfo.h
	@ echo "    CXX  ./chunkinfo.cpp"
	@ $(CXX) $(CFLAGS) -c "./chunkinfo.cpp" -o $@

objs:
	@ mkdir "objs"
.PHONY: c clean
c: clean
clean:
	@ if [ -d "objs" ]; then rm -r "objs"; fi
	@ rm -f "./boost-wc"
	@ echo "    CLEAN"
.PHONY: f fresh
f: fresh
fresh: clean
	@ make all --no-print-directory
.PHONY: r run
r: run
run: all
	@ ././boost-wc

.PHONY: d debug
d: debug
debug: CFLAGS += -DDEBUG -g3 -Wall -Wextra
debug: O=0
debug: all

.PHONY: check-syntax
check-syntax:
	$(CXX) $(CFLAGS) -fsyntax-only -Wall -o /dev/null -S $(CHK_SOURCES)
