/*
pcspkr - listen to pcm sound over the internal pc speaker
Written in 2014 by <Ahmet Inan> <xdsopl@googlemail.com>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>
#include "pcspkr.h"

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	const int counter_freq = 1193182;
	const int sample_rate = atoi(argv[1]);

	set_io_permissions();
	install_signal_handlers();
	set_realtime_scheduling();
	prepare_hardware();
	enable_speaker_and_counter();

	int c;
	while (EOF != (c = getchar_unlocked())) {
		reset_counter((c * counter_freq) / (sample_rate * 256));
		abs_nano_sleep(1000000000 / sample_rate);
	}

	disable_speaker_and_counter();

	return 0;
}

