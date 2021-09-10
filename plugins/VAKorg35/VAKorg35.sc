VAKorg35 : UGen {
	*ar { |input, freq=500, res=0.5, overdrive=0, type=0|
		^this.multiNew('audio', input, freq, res, overdrive, type);
	}

    *kr { |input, freq=500, res=0.5, overdrive=0, type=0|
      ^this.multiNew('control', input, freq, res, overdrive, type);
    }

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
