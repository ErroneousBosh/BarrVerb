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

#ifndef BARRVERB_HPP
#define BARRVERB_HPP

#include "DistrhoPlugin.hpp"

class SVF {
   public:
    //SVF(float cutoff, float q, float samplerate);
    void setFreq(float cutoff, float q, float samplerate);
    float lpStep(float in);

   private:
    float w, a, b;
    float c1, c2, d0;
    float z1, z2, x;
};

START_NAMESPACE_DISTRHO

class BarrVerb : public Plugin {
   public:
    enum Parameters {
        paramProgram,
        kParameterCount
    };

    BarrVerb();

   protected:
    const char *getLabel() const override { return "BarrVerb"; }
    const char *getDescription() const override {
        return "MIDIVerb emulation, a tribute to Keith Barr";
    }
    const char *getMaker() const override { return "Gordonjcp"; }
    const char *getLicense() const override { return "ISC"; }
    uint32_t getVersion() const override { return d_version(1, 0, 0); }
    int64_t getUniqueId() const override { return d_cconst('B', 'A', 'R', 'R'); }

    // Initialisation
    void initAudioPort(bool input, uint32_t index, AudioPort &port) override;
    void initParameter(uint32_t index, Parameter &parameter) override;

    void setParameterValue(uint32_t index, float value) override;
    float getParameterValue(uint32_t index) const override;

    void initProgramName(uint32_t index, String &programName) override;
    void loadProgram(uint32_t index) override;

    // Processing
    void activate() override;
    void deactivate() override;
    void run(const float **inputs, float **outputs, uint32_t frames) override;

   private:
    // float c1_1, c2_1, d0_1, c1_2, c2_2, d0_2, in_z1, in_z2, in_z12,in_z22, out_z1, out_z2;
    SVF f1, f2;

    int16_t ai = 0, li = 0, acc = 0;
    uint16_t ptr = 0;
    uint16_t prog_offset;

    int16_t *ram;
    float *lowpass;

    uint8_t program;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BarrVerb);
};

END_NAMESPACE_DISTRHO

#endif  // BARRVERB_HPP
