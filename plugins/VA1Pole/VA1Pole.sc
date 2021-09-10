VA1Pole : UGen {
	*ar { |input, freq=500, feedback=0.5, type=0|
		^this.multiNew('audio', input, freq, feedback, type);
	}

    *kr { |input, freq=500, feedback=0.5, type=0|
      ^this.multiNew('control', input, freq, feedback, type);
    }

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
