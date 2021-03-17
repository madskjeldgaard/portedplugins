AnalogBassDrum : UGen {
	*ar { |trig=0, infsustain=0.0, accent=0.5, freq=50, tone=0.5, decay=0.5, attackfm=0.5, selffm=0.25|
		^this.multiNew('audio', trig, infsustain, accent, freq, tone, decay, attackfm, selffm);
	}

	checkInputs {
		^this.checkValidInputs;
	}
}

