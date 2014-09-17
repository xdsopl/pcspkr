/*
pcspkr - listen to pcm sound over the internal pc speaker
Written in 2014 by <Ahmet Inan> <xdsopl@googlemail.com>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>
#include "pcspkr.h"

int sigma_delta_modulation(int x)
{
	static int qe;
	int y = x >= qe;
	qe += 255 * y - x;
	return y;
}

int main()
{
	set_io_permissions();
	install_signal_handlers();
	set_realtime_scheduling();
	prepare_hardware();

	int c;
	while (EOF != (c = getchar_unlocked()))
		move_speaker(sigma_delta_modulation(c));

	disable_speaker_and_counter();

	return 0;
}

