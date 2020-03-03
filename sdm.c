/*
pcspkr - listen to pcm sound over the internal pc speaker
Written in 2014 by <Ahmet Inan> <xdsopl@googlemail.com>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>
#include "pcspkr.h"

int sigma_delta_modulation(int x, int order)
{
	static int sum;
	int y = sum >= 128;
	int e = y << 8;
	if (order >= 2) {
		static int sum2;
		sum2 += x - e;
		x = sum2;
	}
	sum += x - e;
	return y;
}

int main(int argc, char **argv)
{
	int order = 1;
	if (argc == 2)
		order = atoi(argv[1]);

	set_io_permissions();
	install_signal_handlers();
	set_realtime_scheduling();
	prepare_hardware();

	int c;
	while (EOF != (c = getchar_unlocked()))
		move_speaker(sigma_delta_modulation(c, order));

	disable_speaker_and_counter();

	return 0;
}

