
CFLAGS = -O3 -std=gnu99 -W -Wall
LDLIBS = -lrt

all: pwm sdm measure

test: test_sdm test_pwm test_measure

test_pwm: pwm
	sox bigben.ogg -t ub -c 1 -r 48000 - | sudo ./pwm 48000

test_sdm: sdm
	sox bigben.ogg -t s16 -c 1 -r $$((1000000/16)) - | sudo ./sdm 1

test_measure: measure
	sudo ./measure

clean:
	rm -f pwm sdm measure

