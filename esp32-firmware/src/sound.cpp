#include "sound.hpp"

void soundmessage()
{
    unsigned melody[] = {
        262, 196, 196, 740, 740};
    unsigned noteDurations[] = {
        4, 8, 8, 8, 7};

    for (unsigned NOTE = 0; NOTE < sizeof(melody) / sizeof(melody[0]); NOTE++)
    {
        int noteDuration = 1000 / noteDurations[NOTE];
        tone(BUZZZER_PIN, melody[NOTE], noteDuration);

        int pause = noteDuration * 1.30;
        delay(pause);
        noTone(BUZZZER_PIN);
    }
}