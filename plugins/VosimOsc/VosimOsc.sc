VosimOsc : UGen {
	*ar { |freq=100, form1freq=951, form2freq=919, shape=0|
		^this.multiNew('audio', freq, form1freq, form2freq, shape);
	}

	*kr { |freq=100, form1freq=951, form2freq=919, shape=0|
		^this.multiNew('control', freq, form1freq, form2freq, shape);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

