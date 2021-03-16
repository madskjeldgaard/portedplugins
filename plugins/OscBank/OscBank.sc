OscBank : UGen {
	*ar { |freq=100, gain=1, saw8=0.5, square8=0.5, saw4=0.5, square4=0.5, saw2=0.5, square2=0.5, saw1=0.5|
		^this.multiNew('audio', freq, gain, saw8, square8, saw4, square4, saw2, square2, saw1);
	}

	*kr { |freq=100, gain=1, saw8=0.5, square8=0.5, saw4=0.5, square4=0.5, saw2=0.5, square2=0.5, saw1=0.5|
		^this.multiNew('control', freq, gain, saw8, square8, saw4, square4, saw2, square2, saw1);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

