VASEM12 : UGen {
	*ar { |input, freq=500, res=0.1, transition=0|
		^this.multiNew('audio', input, freq, res, transition);
	}

    *kr { |input, freq=500, res=0.1, transition=0|
      ^this.multiNew('control', input, freq, res, transition);
    }

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
