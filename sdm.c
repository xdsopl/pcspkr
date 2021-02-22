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
	static short sum;
	if (!order)
		sum = x;
	int y = sum >= 0;
	int e = y << 8;
	if (order >= 2) {
		static short sum2;
		sum2 += x - e;
		x = sum2;
	}
	sum += x - e;
	return y;
}

int integrator_cascade(int x)
{
	static int sum0, sum1, sum2;
	return sum2 += (sum1 += (sum0 += x));
}

int comb_cascade(int x)
{
	static int prv2, prv1, prv0;
	int tmp = x;
	tmp -= prv0; prv0 = x; x = tmp;
	tmp -= prv1; prv1 = x; x = tmp;
	tmp -= prv2; prv2 = x; x = tmp;
	return x;
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

	for (short s; fread_unlocked(&s, 2, 1, stdin) == 1;) {
		int comb = comb_cascade(s);
		for (int i = 0; i < 64; ++i) {
			int intp = integrator_cascade(comb) >> 20;
			comb = 0;
			move_speaker(sigma_delta_modulation(intp+128, order));
		}
	}

	disable_speaker_and_counter();

	return 0;
}

