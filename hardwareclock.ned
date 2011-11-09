simple HardwareClock {
	parameters:
		double interval @unit(s) = default(0.001s);
		int update = default(100);
		@display("i=device/clock");
}

simple ConstantDrift extends HardwareClock {
	parameters:
		interval = 1s;
		double constant_drift = default(50e-6);
}

simple BoundedDrift extends HardwareClock {
	parameters:
		volatile double drift_distribution = default(uniform(-10e-4, 10e-4));
}

simple BoundedDriftVariation extends HardwareClock {
	parameters:
		volatile double drift_distribution = default(uniform(-10e-4, 10e-4));
		double max_drift_variation = default(10e-8);
}

simple ScheduleTestApp {
	parameters:
		@display("i=block/app");
}

network TestNetwork {
	submodules:
		testApp: ScheduleTestApp;

		constantDriftClock: ConstantDrift;
		boundedDriftClock: BoundedDrift;
		boundedDriftDeviationClock: BoundedDriftVariation;

}

