import inet.linklayer.ethernet.EthernetInterface;
import inet.nodes.ethernet.EtherSwitch;
import ptp.hardwareclock.IHardwareClock;
import ptp.Jitter;
import ptp.IPtpClock;
import ptp.TimestampingPhy;
import ptp.PtpMaster;
import ptp.SoftwareClock;

module PtpNode
{
	parameters:
		string ptpImplementationType;
		string hwClockType;

	gates:
		inout eth;

	submodules:
		swClock: SoftwareClock
			{ parameters: @display("p=100,100"); }
		hwClock: <hwClockType> like IHardwareClock
			{ parameters: @display("p=100,200"); }
		ptpImplementation: <ptpImplementationType> like IPtpClock
			{ parameters: @display("p=200,100"); }
		osJitter: Jitter
			{ parameters: @display("p=200,200"); }
		ethif: EthernetInterface
			{ parameters: @display("p=200,300"); }
		phy: TimestampingPhy
			{ parameters: @display("p=200,400"); }

	connections:
		ptpImplementation.port <--> osJitter.upperLayer;

		osJitter.lowerLayer$o --> ethif.netwIn;
		osJitter.lowerLayer$i <-- ethif.netwOut;

		ethif.phys <--> phy.internal;
		phy.external <--> eth;
}

module PtpMasterNode extends PtpNode
{
	parameters:
		hwClockType = "ConstantDrift";
		hwClock.constant_drift = 0;
		ptpImplementationType = "PtpMaster";
}

module PtpSlaveNode extends PtpNode
{
	parameters:
		hwClockType = default("BoundedDriftVariation");
		ptpImplementationType = "PtpSlave";
}

network PtpSimNetwork
{
	parameters:
		int slaves = default(2);

	submodules:
		m: PtpMasterNode
			{ parameters: @display("p=40,50"); }
		s[slaves]: PtpSlaveNode
			{ parameters: @display("p=40,250,r,100"); }
		switch: EtherSwitch
			{ parameters: @display("p=40,150"); }

	connections:
		m.eth  <--> switch.ethg++;

		for i = 0 .. sizeof(s) - 1 {
			s[i].eth <--> switch.ethg++;
		}
}
