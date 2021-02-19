Chen : MultiOutUGen {
	*ar { | speed=0.5, a=0.5, b=0.3, c=0.28|
		^this.multiNew('audio', speed, a, b, c);
	}

	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(3, rate)
	}

// checkInputs {

// 		// This dictionary maps what rates are allowed for each parameter of the UGen
// 		var allowedRates = IdentityDictionary[
// 			// Allowed to be either control or scalar
// 			\drywet -> [\control, \scalar], 
// 			\delayTime -> [\control, \scalar], 
// 			\ratio -> [\control, \scalar], 
// 			\feedback -> [\control, \scalar], 
// 			\modFreq -> [\control, \scalar], 
// 			\modDepth -> [\control, \scalar], 
// 			\resonance1 -> [\control, \scalar], 
// 			\resonance2 -> [\control, \scalar], 
// 			\filterCutoff1 -> [\control, \scalar], 
// 			\filterCutoff2 -> [\control, \scalar], 
// 			\diffuse -> [\control, \scalar],

// 			// Only allowed to be scalar
// 			\modPhase -> [\scalar], 
// 			\maxDelay -> [\scalar], 
// 			\stereoProcessing -> [\scalar], 
// 			\lfotype -> [\scalar], 
// 			\filterType1 -> [\scalar], 
// 			\filterType2 -> [\scalar],

// 			// Only audio
// 			\input -> [\audio]
// 		];

// 		// Iterate over all inputs and check if they comply
// 		inputs.do{|input, inputNum| 
// 		var name = this.argNameForInputAt(inputNum);
// 		var inrate = input.rate;
// 		var expected = allowedRates[name];
// 		var rateIsExpected = expected.indexOfEqual(inrate).notNil;

// 		if(rateIsExpected.not, {
// 			^"%'s input % not % (it is %)".format(this.name, name, expected, inrate).error
// 		})

// 		};

// 		^this.checkValidInputs;
// 	}

	checkInputs {
		^this.checkValidInputs;
	}
}
