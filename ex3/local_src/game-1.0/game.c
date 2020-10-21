#include "game.h"

static FILE* dev_gamepad;

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n");

	exit(EXIT_SUCCESS);
}

int gamepad_init()
{
    dev_gamepad = fopen("/dev/gamepad", "rb");
    if (!dev_gamepad) {
        printf("Unable to open driver dev_gamepad, maybe you didn't load the module?\n");
        return EXIT_FAILURE;
    }
    if (signal(SIGIO, &sigio_handler) == SIG_ERR) {
        printf("An error occurred while register a signal handler.\n");
        return EXIT_FAILURE;
    }
    if (fcntl(fileno(dev_gamepad), F_SETOWN, getpid()) == -1) {
        printf("Error setting pid as owner.\n");
        return EXIT_FAILURE;
    }
    long oflags = fcntl(fileno(dev_gamepad), F_GETFL);
    if (fcntl(fileno(dev_gamepad), F_SETFL, oflags | FASYNC) == -1) {
        printf("Error setting FASYNC flag.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void sigio_handler(int signo)
{
	/* Put input handling here */
}