BarrVerb - a DPF Reverb plugin
==============================

BarrVerb is a reverb plugin based on the legendary Alesis MIDIVerb from the
mid-1980s. It implements an emulation of Keith Barr's custom DSP, originally
implemented in LS-family logic ICs. It could also run the MIDIFex DSP code,
if a suitable ROM image could be found and "deinterleaved" to suit BarrVerb's
decoder.

I have to acknowledge the work of /u/thement_ on Reddit, who nerd-sniped me
into doing this in the first place, Eric Brombaugh on the synth-diy mailing
list who reverse-engineered the MIDIVerb, and Paul Schreiber who made a
a couple of videos of Eric's reverse-engineering of it from which I was able
to get the circuit diagram and a bit of explanation of the opcodes.

Some liberties were taken with the arithmetic in the unit in regards to how
it handles twos'-complement arithmetic. Eric Brombaugh explained that it
appears to have an off-by-one error that gets corrected over two instructions
but to simplify the process loop I just use normal arithmetic. This can cause
an error of up to +/- 2 DAC values in the output, which in practice is
audibly indistinguishable from doing it "100% accurately". On test it can be
measured by compiling the plugin with "accurate" and with "simple" maths,
subtracting the output of one from the other, and boosting the gain by around
50dB or so - you're never hearing that difference in practice.

A brief technical guide
-----------------------

The Alesis MIDIVerb has a simple DSP made out of discrete logic which can
carry out four operations. Unlike a general-purpose DSP it can only multiply
by 0.5, which is implemented by shifting one input to the adder right one place
and feeding the leftmost bit to both bit 15 and bit 14 to extend the sign. Bit
15 is also fed to the carry input of the adder chain, for the twos'-complement
add.

The "program counter" is an 8-bit counter clocked at 6MHz, which feeds the
lower 8 bits of the DSP EPROM address. This is latched into the DSP low byte
first. Each instruction consists of a two-bit opcode and 14-bit offset, which
is added to the memory pointer on each step to address 16-bit words within the
16kB DSP RAM. The instruction steps are latched in stages so that for a given
counter position, the offset comes from the instruction before, and the opcode
comes from the opcode before that, presumably to allow time for the latches to
settle. In this implementation, the ROM has been pre-"prefetched" so that the
value in `rom.h` at word 0 of any program actually contains the opcode from
word 126 and the offset from word 127 and so on, which simplifies the DSP loop.

There are no branches possible so unused code must be filled with a "dummy"
write to RAM. Three of the addresses are "magic" - at step 0 the ADC is loaded
into RAM at the address in the pointer register, at step 96 (60 hex) the
right channel output DAC is loaded with the "adder input" bus, and at step
112 (70 hex) the left channel output DAC is loaded. For each of these "magic"
addresses the ALU works as normal but the accumulator register is not loaded
with the output of the adder. In general the DSP code used seems to run an
instruction to load the adder input bus with the contents of RAM, pointing to
a "temporary" address where the effect outputs are stored.

Known limitations
-----------------

This is the first thing I've written from scratch using DPF, and as such
may not actually be very good.

This plugin will work at any sample rate but will only produce approximately
correct results at 48kHz because it does not downsample and upsample very
well. The input filter does not have the right response and there is no
reconstruction filter on the output, which you are unlikely to notice in use.

The DSP engine runs at 24kHz (or really, half the sample rate) rather than
the correct 6MHz/256 = 23.4375kHz, which you are unlikely to notice in use.


Building BarrVerb
-----------------

1. clone the repository

2. `git submodule update --init` to pull in Distrho Plugin Framework

3. `make`

You should now have a `./bin/` directory with `BarrVerb` as a standalone
Jack client, `BarrVerb.lv2` as an LV2 plugin, `BarrVerb.vst3` as a VST3
plugin, and `BarrVerb-vst.so` as a VST2 plugin. These have been tested on
Linux using Carla 2.4.2, but very little else. Further testing and patches
would be welcome.

This software is provided under the ISC licence as documented in the file
LICENCE which is fairly permissive. The file `rom.h` contains a permuted
version of the MIDIVerb ROM which has already been shared and distributed
widely, but must be considered to be copyrighted by the late Keith Barr.

