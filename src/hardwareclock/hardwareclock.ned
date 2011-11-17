package ptp.hardwareclock;

simple HardwareClock {
	parameters:
		double interval @unit(s) = default(1s);
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
		volatile double drift_distribution = default(uniform(-100e-6, 100e-6));
}

simple BoundedDriftVariation extends HardwareClock {
	parameters:
		volatile double drift_distribution = default(uniform(-100e-6, 100e-6));
		double max_drift_variation = default(10e-8);
}
