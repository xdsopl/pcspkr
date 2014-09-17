/*
pcspkr - listen to pcm sound over the internal pc speaker
Written in 2014 by <Ahmet Inan> <xdsopl@googlemail.com>
To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <signal.h>
#include <sys/io.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>

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

void disable_speaker_and_counter()
{
	outb(inb(0x61) & 12, 0x61);
}

void enable_speaker_and_counter()
{
	outb((inb(0x61) & 12) | 3, 0x61);
}

void reset_counter(unsigned char c)
{
	outb(c, 0x42);
}

void move_speaker(int v)
{
	static int init;
	static unsigned char settings;
	if (!init) {
		init = 1;
		settings = (inb(0x61) & 12) | 1;
	}
	outb(settings | (v << 1), 0x61);
}

void prepare_hardware()
{
	disable_speaker_and_counter();
	outb(160, 0x43); // 16bit counter, mode 0, MSB only, timer 2
	reset_counter(0);
	outb(144, 0x43); // 16bit counter, mode 0, LSB only, timer 2
	reset_counter(0);
}

void cleanup(int signum)
{
	(void)signum;
	disable_speaker_and_counter();
	exit(1);
}

void install_signal_handlers()
{
	signal(SIGTERM, cleanup);
	signal(SIGINT, cleanup);
}

void set_io_permissions()
{
	ioperm(0x42, 2, 1);
	ioperm(0x61, 1, 1);
}

void set_realtime_scheduling()
{
	const int policy = SCHED_FIFO;
	struct sched_param param;
	sched_getparam(0, &param);
	param.sched_priority = sched_get_priority_max(policy);
	sched_setscheduler(0, policy, &param);
}

