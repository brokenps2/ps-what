SRCS = $(wildcard src/*.c)
TARGET = bin/prog

include common.mk

cd:
	cp bin/prog.ps-exe cddir/prog.ps-exe
	cp src/*.TIM cddir/
	cp src/*.VAG cddir/
	mkpsxiso -y cddir/isoconfig.xml
	mv cd.bin cddir/
	mv cd.cue cddir/
