AnalogTape : UGen {
	*ar { |input, bias=0.5, saturation=0.5, drive=0.5, oversample=1, mode=0|
		^this.multiNew('audio', input, bias, saturation, drive, oversample, mode);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

