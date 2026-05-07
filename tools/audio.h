#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <audsrv.h>
#include <stdbool.h>

struct Sound {
    unsigned char *data;
    audsrv_fmt_t format;
    int size;
    int offset;
    bool playing;
};

void audio_initialise();

struct Sound* audio_load(const char* path);

void audio_play(struct Sound* sound);

void audio_stop(struct Sound* sound);

void audio_free(struct Sound* sound);

void audio_update(struct Sound* sound);

#endif
