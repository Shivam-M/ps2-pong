#include "utils.h"

#include <stdio.h>

const char* get_time_from_seconds(int seconds) {
    static char buffer[16];

    snprintf(buffer, sizeof(buffer), "%02d:%02d", seconds / 60, seconds % 60);
    return buffer;
}

int get_seconds_from_time(const char* time) {
    int minutes, seconds;

    if (sscanf(time, "%d:%d", &minutes, &seconds) != 2) {
        return -1;
    }

    if (seconds < 0 || seconds >= 60 || minutes < 0) {
        return -1;
    }

    return minutes * 60 + seconds;
}
