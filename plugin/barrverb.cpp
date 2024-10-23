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

#include "rom.h"

SVF::SVF(float cutoff = 0, float q = 0, float samplerate = 0) {
    z1 = z2 = 0;
    setFreq(cutoff, q, samplerate);
}

void SVF::setFreq(float cutoff, float q, float samplerate) {
    z1 = z2 = 0;

    //printf("called with %f %f %f\n", cutoff, q, samplerate);
    w = 2 * tan(3.14159 * (cutoff / samplerate));
    a = w / q;
    b = w * w;

    // corrected SVF params, per Fons Adriaensen
    c1 = (a + b) / (1 + a / 2 + b / 4);
    c2 = b / (a + b);

    d0 = c1 * c2 / 4;

    //printf("c1 %f c2 %f d0 %f\n", c1, c2, d0);
}

inline float SVF::lpStep(float in) {
    x = in - z1 - z2;
    z2 += c2 * z1;
    z1 += c1 * x;
    return d0 * x + z2;
}

START_NAMESPACE_DISTRHO

BarrVerb::BarrVerb() : Plugin(kParameterCount, 64, 0) {  // one parameter, 64 programs, no states
    lowpass = new float[getBufferSize()];
    ram = new int16_t[16384];

    memset(lowpass, 0, sizeof(float) * getBufferSize());
    memset(ram, 0, sizeof(int16_t) * 16384);

    f1.setFreq(5916, .6572, getSampleRate());
    f2.setFreq(9458, 2.536, getSampleRate());
}

// Initialisation functions

void BarrVerb::initParameter(uint32_t index, Parameter &parameter) {
    if (index == paramProgram) {
        parameter.hints = kParameterIsAutomatable | kParameterIsInteger;
        parameter.name = "Program";
        parameter.symbol = "program";
        parameter.ranges.def = 20.0f;
        parameter.ranges.min = 1.0f;
        parameter.ranges.max = 64.0f;
    }
}

void BarrVerb::setParameterValue(uint32_t index, float value) {
    if (index == paramProgram) {
        program = value;
        prog_offset = (((int)value - 1) & 0x3f) << 7;
    }
}

float BarrVerb::getParameterValue(uint32_t index) const {
    if (index == paramProgram) {
        return program;
    }
    return 0;
}

void BarrVerb::initAudioPort(bool input, uint32_t index, AudioPort &port) {
    port.groupId = kPortGroupStereo;
    Plugin::initAudioPort(input, index, port);

    if (input && index == 0) port.name = "Left In";
    if (input && index == 1) port.name = "Right In";
    if (!input && index == 0) port.name = "Left Out";
    if (!input && index == 1) port.name = "Right Out";
}

void BarrVerb::initProgramName(uint32_t index, String &programName) {
    programName = prog_name[index & 0x3f].c_str();
}

void BarrVerb::loadProgram(uint32_t index) {
    prog_offset = (index & 0x3f) << 7;
    program = index + 1;
}

// Processing functions

void BarrVerb::activate() {
    // calculate filter coefficients
    //printf("called activate()\n");
}

void BarrVerb::deactivate() {
    // zero out the outputs, maybe
   // printf("called deactivate()\n");
}

void BarrVerb::run(const float **inputs, float **outputs, uint32_t frames) {
    // actual effects here

    uint16_t opcode = 0;

    for (uint32_t i = 0; i < frames; i++) {
        // smash to mono
        lowpass[i] = f2.lpStep(f1.lpStep((inputs[0][i] + inputs[1][i]) / 2));
    }

    // now run the DSP
    for (uint32_t i = 0; i < frames; i += 2) {
        // run the actual DSP engine for each sample
        for (uint8_t step = 0; step < 128; step++) {
            opcode = rom[prog_offset + step];
            switch (opcode & 0xc000) {
                case 0x0000:
                    ai = ram[ptr];
                    li = acc + (ai >> 1);
                    break;
                case 0x4000:
                    ai = ram[ptr];
                    li = (ai >> 1);
                    break;
                case 0x8000:
                    ai = acc;
                    ram[ptr] = ai;
                    li = acc + (ai >> 1);
                    break;
                case 0xc000:
                    ai = acc;
                    ram[ptr] = -ai;
                    li = -(ai >> 1);
                    break;
            }

            // clamp
            if (ai > 2047) ai = 2047;
            if (ai < -2047) ai = -2047;

            if (step == 0x00) {
                // load RAM from ADC
                ram[ptr] = (int)(lowpass[i] * 2048);
            } else if (step == 0x60) {
                // output right channel
                // ai=0;
                outputs[1][i] = (float)ai / 2048;
                outputs[1][i + 1] = (float)ai / 2048;

            } else if (step == 0x70) {
                // output left channel
                // ai=0;
                outputs[0][i] = (float)ai / 2048;
                outputs[0][i + 1] = (float)ai / 2048;
            } else {
                // everything else
                // ADC and DAC operations don't affect the accumulator
                // every other step ends with the accumulator latched from the Latch Input reg
                acc = li;
            }

            // 16kW of RAM
            ptr += opcode & 0x3fff;
            ptr &= 0x3fff;
        }
    }
}

// create the plugin
Plugin *createPlugin() { return new BarrVerb(); }

END_NAMESPACE_DISTRHO
