DCompressor : UGen {
	*ar { |input, sidechainIn=0, sidechain=1, ratio=2, threshold=(-40), attack=0.1, release=100.1, makeup=0.5, automakeup=1|
		^this.multiNew('audio', input, sidechainIn, sidechain, ratio, threshold, attack, release, makeup, automakeup);
	}

	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}

