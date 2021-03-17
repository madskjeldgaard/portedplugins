AnalogSnareDrum : UGen {
	*ar { |trig=0, infsustain=0, accent=0.1, freq=200, tone=0.5, decay=0.5, snappy=0.5|
		^this.multiNew('audio', trig, infsustain, accent, freq, tone, decay, snappy);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

