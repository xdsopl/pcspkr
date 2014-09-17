/*
pcspkr - listen to pcm sound over the internal pc speaker
Written in 2014 by <Ahmet Inan> <xdsopl@googlemail.com>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>
#include "pcspkr.h"

void print_measured_frequency()
{
	static struct timespec last;
	if (!last.tv_sec)
		clock_gettime(CLOCK_MONOTONIC, &last);
	static long count, max = 1 << 20;
	if (++count > max) {
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		long nsec = 1000000000 * (now.tv_sec - last.tv_sec) + now.tv_nsec - last.tv_nsec;
		if (nsec < 1000000000)
			max <<= 1;
		if (nsec > 2000000000)
			max >>= 1;
		long freq = count * 1000000000 / (1000000000 * (now.tv_sec - last.tv_sec) + now.tv_nsec - last.tv_nsec);
		fprintf(stderr, "measured frequency: %ld Hz\n", freq);
		last = now;
		count = 0;
	}
}

int main()
{
	set_io_permissions();
	install_signal_handlers();
	prepare_hardware();

	for (int i = 0; i < 10000000; i++) {
		move_speaker(0);
		print_measured_frequency();
	}

	disable_speaker_and_counter();

	return 0;
}

