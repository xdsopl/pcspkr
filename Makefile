
CFLAGS = -O3 -std=gnu99 -W -Wall
LDFLAGS = -lrt

all: pwm sdm

test: test_sdm test_pwm

test_pwm: pwm
	sox bigben.ogg -t ub -c 1 -r 48000 - | sudo ./pwm 48000

test_sdm: sdm
	sox bigben.ogg -t ub -c 1 -r 1000000 - | sudo ./sdm

clean:
	rm -f pwm sdm

