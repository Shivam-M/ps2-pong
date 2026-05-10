#include "audio.h"
#include "common.h"
#include "audsrv_irx.h"

#include <audsrv.h>
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void audio_initialise(void)  {
    SifLoadModule("rom0:LIBSD", 0, NULL);

    SifExecModuleBuffer(audsrv_irx, audsrv_irx_length, 0, NULL, NULL);

    audsrv_init();
}

struct Sound* audio_load(const char* path) {
    if (path == NULL) return NULL;

    FILE* wav_file = fopen(path, "rb");

    if (wav_file == NULL) return NULL;

    struct Sound* sound = calloc(1, sizeof(struct Sound));

    if (sound == NULL) {
        fclose(wav_file);
        return NULL;
    }

    fseek(wav_file, 0, SEEK_END);
    sound->size = ftell(wav_file);
    rewind(wav_file);

    sound->data = malloc(sound->size);

    if (sound->data == NULL) {
        fclose(wav_file);
        free(sound);
        return NULL;
    }

    fread(sound->data, 1, sound->size, wav_file);
    fclose(wav_file);

    if (memcmp(sound->data, "RIFF", 4) != 0) {
        free(sound->data);
        free(sound);
        return NULL;
    }

    sound->format.channels = *(u16*)(sound->data + 22);
    sound->format.freq = *(u32*)(sound->data + 24);
    sound->format.bits = *(u16*)(sound->data + 34);

    return sound;
}

void audio_play(struct Sound* sound) {
    audsrv_stop_audio();

    sound->playing = true;
    sound->offset = 0;

    audsrv_set_format(&sound->format);
}

void audio_stop(struct Sound* sound) {
    sound->playing = false;

    audsrv_stop_audio();
}

void audio_free(struct Sound* sound) {
    if (sound->data) {
        free(sound->data);
    }

    free(sound);
}

void audio_update(struct Sound* sound) {
    if (!sound->playing) return;

    int remaining = (sound->size - 44) - sound->offset;

    if (remaining <= 0) {
        sound->playing = false;
        audsrv_stop_audio();
        return;
    }

    int available = audsrv_available();

    if (available <= 0) return;

    int chunk = available;

    if (chunk > remaining) {
        chunk = remaining;
    }

    audsrv_play_audio((char*)(sound->data + 44 + sound->offset), chunk);
    sound->offset += chunk;
}
