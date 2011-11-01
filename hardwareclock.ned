simple HardwareClock {
	parameters:
		double interval @unit(s) = default(0.001s);
		int update = default(100);
		@display("i=device/clock");
}

simple ConstantDrift extends HardwareClock {
	parameters:
		interval = 1s;
		double constant_drift = default(20e-6);
}

simple BoundedDrift extends HardwareClock {
	parameters:
		volatile double drift_distribution = default(uniform(-10e-6, 10e-6));
}

network TestNetwork {
	submodules:
		constant: ConstantDrift;
		bounded: BoundedDrift;
}
