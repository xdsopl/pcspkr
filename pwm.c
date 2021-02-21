/*
pcspkr - listen to pcm sound over the internal pc speaker
Written in 2014 by <Ahmet Inan> <xdsopl@googlemail.com>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>
#include <time.h>
#include "pcspkr.h"

void abs_nano_sleep(long nsec)
{
	static struct timespec ts;
	if (!ts.tv_sec)
		clock_gettime(CLOCK_MONOTONIC, &ts);
	ts.tv_nsec += nsec;
	if (ts.tv_nsec >= 1000000000) {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, 0);
}

int sigma_delta_modulation(int x, int order)
{
	static int sum;
	if (!order)
		sum = x;
	int y = sum >> 10;
	int e = y << 10;
	if (order >= 2) {
		static int sum2;
		sum2 += x - e;
		x = sum2;
	}
	sum += x - e;
	return y + 32;
}

int main(int argc, char **argv)
{
	if (argc > 2)
		return 1;
	int order = 1;
	if (argc == 2)
		order = atoi(argv[1]);

	set_io_permissions();
	install_signal_handlers();
	set_realtime_scheduling();
	prepare_hardware();
	enable_speaker_and_counter();

	short input;
	while (fread_unlocked(&input, 2, 1, stdin) == 1) {
		reset_counter(sigma_delta_modulation(input, order));
		abs_nano_sleep(1000000000/(1193182/64));
	}

	disable_speaker_and_counter();

	return 0;
}

