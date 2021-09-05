LockhartWavefolder : UGen {
	*ar { |input, gain=1|
		/* TODO */
		^this.multiNew('audio', input, gain);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
