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

engine.addFixturePreset({
	name: "Simple 7 channel fixture",
	slots: [
		{ name: "Brightness" },
		{ name: "Red", tint: [1, 0.5, 0.5, 1.0] },
		{ name: "Green", tint: [0.5, 1, 0.5, 1.0] },
		{ name: "Blue", tint: [0.5, 0.5, 1, 1.0] },
		{ name: "Strobe" },
		{ name: "Mode" },
		{ name: "Parameter" },
	],
	constrols: [
		{ type: "Color", order: "rgb", slots: [1, 2, 3] },
		{ type: "Select", slots: [5], values: [
			{ from: 0, to: 10, label: "RGB" },
			{ from: 11, to: 60, label: "Color select" },
		]},
	]
});


engine.addFixturePreset({
	name: "Simple 3 channel fixture",
	slots: [
		{ name: "Red", tint: [1, 0.5, 0.5, 1.0] },
		{ name: "Green", tint: [0.5, 1, 0.5, 1.0] },
		{ name: "Blue", tint: [0.5, 0.5, 1, 1.0] },
	],
	constrols: []
});

engine.addUniverse("Universe 1", 0);
