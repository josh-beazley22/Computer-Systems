
struct timeval {
    unsigned int tv_sec;         // seconds
    unsigned int tv_usec;        // microseconds
};

struct timeval time;
gettime(&time);      // get current time

float seconds = time.tv_sec + time.tv_usec / 1e6;

printf("Seconds=%d, usecs=%d\n", (int) seconds, (int) (seconds - (int) seconds) * 1e6);