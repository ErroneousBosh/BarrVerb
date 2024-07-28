/*
   BarrVerb reverb plugin

   Copyright 2024 Gordon JC Pearce <gordonjcp@gjcp.net>

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
   SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
   OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "barrverb.hpp"

START_NAMESPACE_DISTRHO

BarrVerb::BarrVerb() : Plugin(kParameterCount, 1, 0) {  // two parameters, one program, no states
    lowpass = new float[getBufferSize()];

/*
    // calculate SVF params
    // hardcoded values for now
    float fc = 5019;
    float F = fc / 48000;  // assume 48kHz
    float w = 2 * tan(3.14159 * F);
    float a = w / 0.7845;  // 1dB Chebyshev, 2-pole
    float b = w * w;

    // "corrected" SVF params, per Fons Adriaensen
    c1_1 = (a + b) / (1 + a / 2 + b / 4);
    c2_1 = b / (a + b);
    d0_1 = c1_1 * c2_1 / 4;

    fc = 9433;
    F = fc / 48000;  // assume 48kHz
    w = 2 * tan(3.14159 * F);
    a = w / 3.5594;  // 1dB Chebyshev, 2-pole
    b = w * w;

    c1_2 = (a + b) / (1 + a / 2 + b / 4);
    c2_2 = b / (a + b);
    d0_2 = c1_2 * c2_2 / 4;*/
        // calculate SVF params
    // hardcoded values for now

    float fc = 10000;
    float F = fc / 48000;  // assume 48kHz
    float w = 2 * tan(3.14159 * F);
    float a = w / 0.5412;  // 1dB Chebyshev, 2-pole
    float b = w * w;

    // "corrected" SVF params, per Fons Adriaensen
    c1_1 = (a + b) / (1 + a / 2 + b / 4);
    c2_1 = b / (a + b);
    d0_1 = c1_1 * c2_1 / 4;

    fc = 10000;
    F = fc / 48000;  // assume 48kHz
    w = 2 * tan(3.14159 * F);
    a = w / 1.3065;  // 1dB Chebyshev, 2-pole
    b = w * w;

    c1_2 = (a + b) / (1 + a / 2 + b / 4);
    c2_2 = b / (a + b);
    d0_2 = c1_2 * c2_2 / 4;
}

// Initialisation functions

void BarrVerb::initAudioPort(bool input, uint32_t index, AudioPort &port) {
    port.groupId = kPortGroupStereo;
    Plugin::initAudioPort(input, index, port);
}

void BarrVerb::initProgramName(uint32_t index, String &programName) {
    programName = "Default Reverb";
}

// Processing functions

void BarrVerb::activate() {
    // calculate filter coefficients
    printf("called activate()\n");
}

void BarrVerb::deactivate() {
    // zero out the outputs, maybe
    printf("called deactivate()\n");
}

void BarrVerb::run(const float **inputs, float **outputs, uint32_t frames) {
    // actual effects here

    float x, o;

    for (uint32_t i = 0; i < frames; i++) {
        // smash to mono
        lowpass[i] = (inputs[0][i] + inputs[1][i]) / 2;

        // 10kHz lowpass filter, 2x oversampling
        x = lowpass[i] - in_z1 - in_z2;
        in_z2 += c2_1 * in_z1;
        in_z1 += c1_1 * x;

        x = (d0_1 * x + in_z2) - in_z12 - in_z22;
        in_z22 += c2_2 * in_z12;
        in_z12 += c1_2 * x;
        lowpass[i] = d0_2 * x + in_z22;

        outputs[0][i] = lowpass[i];
        outputs[1][i] = lowpass[i];
    }
}

// create the plugin
Plugin *createPlugin() { return new BarrVerb(); }
END_NAMESPACE_DISTRHO
