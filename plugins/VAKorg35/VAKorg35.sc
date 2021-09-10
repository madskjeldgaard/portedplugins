VAKorg35 : UGen {
	*ar { |input, freq=500, res=0.5, overdrive=0|
		^this.multiNew('audio', input, freq, res, overdrive);
	}

    *kr { |input, freq=500, res=0.5, overdrive=0|
      ^this.multiNew('control', input, freq, res, overdrive);
    }

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
