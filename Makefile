#
# Makefile
# Ilja Kartašov, 2019-11-26 13:39
#
#
.PHONY: dirs clean

default: minion

minion: aisl/build/libaisl.a dirs
	gcc -I./aisl/include src/main.c aisl/build/libaisl.a -o build/$@

aisl/build/libaisl.a:
	cd aisl && make AISL_WITH_SSL=0 libaisl.a

dirs:
	mkdir -p build

clean:
	rm -Rf build

# vim:ft=make
#
