LPG : UGen {
	*ar { |input, controlinput, controloffset=0, controlscale=1, vca=1, resonance=1.5, lowpassmode=1, linearity=1, outnum=0|
		^this.multiNew('audio', input, controlinput, controloffset, controlscale, vca, resonance, lowpassmode, linearity, outnum);
	}

	// init { arg ... theInputs;
	// 	var numChannelsOut = 1;
	// 	inputs = theInputs;
	// 	^this.initOutputs(numChannelsOut, rate)
	// }

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

