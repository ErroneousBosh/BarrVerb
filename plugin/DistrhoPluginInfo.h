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


#ifndef DISTRHO_PLUGIN_INFO_H
#define DISTRHO_PLUGIN_INFO_H

#define DISTRHO_PLUGIN_NAME "BarrVerb"
#define DISTRHO_PLUGIN_URI "https://gjcp.net/plugins/barrverb"

#define DISTRHO_PLUGIN_NUM_INPUTS 2
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2

#define DISTRHO_PLUGIN_IS_RT_SAFE 1

#define DISTRHO_PLUGIN_WANT_PROGRAMS 1

#define DISTRHO_PLUGIN_UNIQUE_ID BARR
#define DISTRHO_PLUGIN_BRAND_ID GJCP


enum Parameters {
	paramProg,
	kParameterCount
};

#endif
