import inet.linklayer.ethernet.EthernetInterface;
import inet.nodes.ethernet.EtherSwitch;
import ptp.hardwareclock.IHardwareClock;
import ptp.IPtpClock;
import ptp.TimestampingPhy;
import ptp.PtpMaster;

module PtpNode
{
	parameters:
		string ptpImplementation;
		string clockType;

	gates:
		inout eth;

	submodules:
		clock: <clockType> like IHardwareClock
			{ parameters: @display("p=200,100"); }
		p: <ptpImplementation> like IPtpClock
			{ parameters: @display("p=100,100"); }
		ethif: EthernetInterface
			{ parameters: @display("p=100,200"); }
		phy: TimestampingPhy
			{ parameters: @display("p=200,200"); }

	connections:
		p.port$o --> ethif.netwIn;
		p.port$i <-- ethif.netwOut;

		ethif.phys <--> phy.internal;
		phy.external <--> eth;
}

module PtpMasterNode extends PtpNode
{
	parameters:
		clockType = "ConstantDrift";
		clock.constant_drift = 0;
		ptpImplementation = "PtpMaster";
}

module PtpSlaveNode extends PtpNode
{
	parameters:
		clockType = default("BoundedDriftVariation");
		ptpImplementation = "PtpSlave";
}

network PtpSimNetwork
{
	submodules:
		m: PtpMasterNode    { parameters: @display("p=40,50");        }
		s[2]: PtpSlaveNode  { parameters: @display("p=40,250,r,100"); }
		switch: EtherSwitch { parameters: @display("p=40,150");       }

	connections:
		m.eth  <--> switch.ethg++;

		for i = 0 .. sizeof(s) - 1 {
			s[i].eth <--> switch.ethg++;
		}
}
