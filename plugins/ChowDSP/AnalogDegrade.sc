AnalogDegrade : UGen {
	*ar { |input, depth=0.5, amount=0.5, variance=0.5, envelope=0.5|
		^this.multiNew('audio', input, depth, amount, variance, envelope);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

