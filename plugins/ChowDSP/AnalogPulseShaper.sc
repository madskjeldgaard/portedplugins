AnalogPulseShaper : UGen {
	*ar { |pulseinput, width=0.5, decay=0.5, double=0.5|
		^this.multiNew('audio', pulseinput, width, decay, double);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

