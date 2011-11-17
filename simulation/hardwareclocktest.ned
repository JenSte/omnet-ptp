import ptp.hardwareclock.*;

network HardwareClockTestNetwork {
	submodules:
		testApp: ScheduleTestApp                          { parameters: @display("p=600,200"); }

		constantDriftClock: ConstantDrift                 { parameters: @display("p=200,200"); }
		boundedDriftClock: BoundedDrift                   { parameters: @display("p=200,400"); }
		boundedDriftDeviationClock: BoundedDriftVariation { parameters: @display("p=200,600"); }
}
