LockhartWavefolder : UGen {
	*ar { |input, numCells=4|
		^this.multiNew('audio', input, numCells);
	}

    *kr { |input, numCells=4|
		^this.multiNew('control', input, numCells);
	}

	checkInputs {
		^this.checkValidInputs;
	}
}
