engine.addModulator("sine", {
	name: "Sine",
	parameters: {
		frequency: 1.0,
		amplitude: 1.0,
		phase: 0
	},
	operator: (t, { frequency, amplitude, phase }) => {
		return ((Math.sin((phase + t) * frequency * 2 * Math.PI) + 1) / 2) * amplitude;
	}
});