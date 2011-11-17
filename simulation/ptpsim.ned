import inet.linklayer.ethernet.EthernetInterface;
import inet.nodes.ethernet.EtherSwitch;
import ptp.IPtpClock;
import ptp.TimestampingPhy;
import ptp.PtpMaster;

module PtpNode
{
	parameters:
		string ptpImplementation;

	gates:
		inout eth;

	submodules:
		p: <ptpImplementation> like IPtpClock { parameters: @display("p=100,100"); }
		ethif: EthernetInterface              { parameters: @display("p=100,200"); }
		phy: TimestampingPhy                  { parameters: @display("p=200,200"); }

	connections:
		p.port$o --> ethif.netwIn;
		p.port$i <-- ethif.netwOut;

		ethif.phys <--> phy.internal;
		phy.external <--> eth;
}

module PtpMasterNode extends PtpNode
{
	parameters:
		ptpImplementation = "PtpMaster";
}

module PtpSlaveNode extends PtpNode
{
	parameters:
		ptpImplementation = "PtpSlave";
}

network PtpSimNetwork
{
	submodules:
		m: PtpMasterNode;

		s1: PtpSlaveNode;
		s2: PtpSlaveNode;

		switch: EtherSwitch;

	connections:
		m.eth  <--> switch.ethg++;
		s1.eth <--> switch.ethg++;
		s2.eth <--> switch.ethg++;
}
