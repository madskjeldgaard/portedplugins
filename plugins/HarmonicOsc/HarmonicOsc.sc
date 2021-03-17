HarmonicOsc : UGen {
	*ar { |freq=100, firstharmonic=1, amplitudes|
		^this.multiNewList(['audio', freq, firstharmonic] ++ amplitudes.asArray);
	}

	*kr { |freq=100, firstharmonic=1, amplitudes|
		^this.multiNewList(['control', freq, firstharmonic]++ amplitudes.asArray);
	}

	checkInputs {
		^this.checkValidInputs;
	}
}

