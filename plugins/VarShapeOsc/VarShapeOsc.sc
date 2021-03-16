VarShapeOsc : MultiOutUGen {
	*ar { |input, gain|
		/* TODO */
		^this.multiNew('audio', input, gain);
	}

	init { arg ... theInputs;
		var numChannelsOut = 2;
		inputs = theInputs;
		^this.initOutputs(numChannelsOut, rate)
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

