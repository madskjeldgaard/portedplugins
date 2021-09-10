VALadder : UGen {
	*ar { |input, freq=500, res=0.1, overdrive=0.0, type=0|
		/* TODO */
		^this.multiNew('audio', input, freq, res, overdrive, type);
	}

    *kr { |input, freq=500, res=0.1, overdrive=0.0, type=0|
		/* TODO */
		^this.multiNew('control', input, freq, res, overdrive, type);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
