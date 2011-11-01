simple HardwareClock {
	parameters:
		double interval @unit(s) = 0.001s;
		int update = 100;
		@display("i=device/clock");

//		volatile double drift_distribution = uniform(-10e-6, 10e-6);
}



network TestNetwork {
	submodules:
		clock: HardwareClock;
}
