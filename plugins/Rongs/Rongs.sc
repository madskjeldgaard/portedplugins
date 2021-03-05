Rongs : UGen {
	*ar { |trigger=0, sustain=1, f0=0.01, structure=0.5, brightness=0.5, damping=0.75, accent=0.9, stretch=0.5, position=0.15, loss=0.15, modeNum=2, cosFreq=0.25|
		^this.multiNew('audio', trigger, sustain, f0, structure, brightness, damping, accent, stretch, position, loss, modeNum, cosFreq);
	}

	checkInputs {

		// This dictionary maps what rates are allowed for each parameter of the UGen
		var allowedRates = IdentityDictionary[
			\modeNum -> [\scalar],
			\cosFreq -> [\scalar],

			\trigger -> [\control, \scalar],
			\sustain -> [\control, \scalar],
			\f0 -> [\control, \scalar],
			\structure -> [\control, \scalar],
			\brightness -> [\control, \scalar],
			\damping -> [\control, \scalar],
			\accent -> [\control, \scalar],
			\stretch -> [\control, \scalar],
			\position -> [\control, \scalar],
			\loss -> [\control, \scalar],
		];

		// Iterate over all inputs and check if they comply
		inputs.do{|input, inputNum| 
			var name = this.argNameForInputAt(inputNum);
			var inrate = input.rate;
			var expected = allowedRates[name];
			var rateIsExpected = expected.indexOfEqual(inrate).notNil;

			if(rateIsExpected.not, {
				^"%'s input % not % (it is %)".format(this.name, name, expected, inrate).error
			})

		};

		^this.checkValidInputs;
	}
}

