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

BarrVerb::BarrVerb() : Plugin(kParameterCount, 1, 0) { // two parameters, one program, no states
	// dummy
	deactivate();
}

// Initialisation functions

void BarrVerb::initAudioPort(bool input, uint32_t index, AudioPort &port) {
	port.groupId = kPortGroupStereo;
	Plugin::initAudioPort(input, index, port);
}

void BarrVerb::initProgramName(uint32_t index, String &programName) {
	programName="Default Reverb";
}

// Processing functions

void BarrVerb::activate() {
	// calculate filter coefficients
}

void BarrVerb::deactivate() {
	// zero out the outputs, maybe
}

void BarrVerb::run(const float **inputs, float **outputs, uint32_t frames) {
}

// create the plugin
Plugin* createPlugin() { return new BarrVerb(); }
END_NAMESPACE_DISTRHO

