#!/bin/bash
## install faust
# wget https://github.com/grame-cncm/faust/releases/download/2.54.9/faust-2.54.9.tar.gz
# tar -xvzf faust-2.54.9.tar.gz
# cd faust-2.54.9
# make
# sudo make install

## convert fverb dsp to sc
wget https://raw.githubusercontent.com/jpcima/fverb/master/fverb.dsp
faust2sc.py fverb.dsp
