LPG : UGen {
	*ar { |input, controlinput, controloffset=0, controlscale=1, vca=1, resonance=1.5, lowpassmode=1, linearity=1|
		^this.multiNew('audio', input, controlinput, controloffset, controlscale, vca, resonance, lowpassmode, linearity);
	}

	checkInputs {

		// This dictionary maps what rates are allowed for each parameter of the UGen
		var allowedRates = IdentityDictionary[
			\input -> [\audio],
			\controlinput -> [\control, \audio],

			// Allowed to be either control or scalar
			\controloffset -> [\control, \scalar], 
			\controlscale ->[\control, \scalar], 
			\vca -> [\control, \scalar], 
			\resonance -> [\control, \scalar],
			\lowpassmode -> [\control, \scalar],
			\linearity -> [\control, \scalar],
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

