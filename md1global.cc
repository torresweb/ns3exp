/*Simular no NS2 (ou OMNeT++, ou outro simulador) uma fonte que envia pacotes de
tamanho fixo com intervalo aleatório de distribuição exponencial. Trata-se de
um regime estacionário, onde taxa média de envio (Rin) é menor que taxa do enlace (Rout).
Fator de Utilização (u) = Rin / Rout.

Calcular e plotar em função do fator de utilização o atraso médio na fila (M/D/1), utilizando
simbolos, juntamente com a curva teórica, utilizando linha contínua.
(ex. nas simulações usar u=[0.5 0.8 0.9 0.95], para curva teorica usar u=0:.01:0.99) */

#include <iostream>
#include <vector>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/random-variable-stream.h"
#include "ns3/data-rate.h"
#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MD1 queue");

std::vector<double> Dequeue_time;
std::vector<double> Enqueue_time;
int n = 0;

double start_time = 0.0;
static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, Ptr<Node> n, uint32_t pktCount, float mean)
{
	Ptr<ExponentialRandomVariable> x = CreateObject<ExponentialRandomVariable> ();
	x->SetAttribute ("Mean", DoubleValue (mean));
	double interval = x->GetValue();
	Time pktInterval =  Seconds(interval);
	//std::cout << interval << "\n";
	if (pktCount >= 1)
	{
		socket->Send (Create<Packet> (pktSize));
		Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize, n,
				pktCount - 1, mean);
	}
}

void Enqueue (std::string context, Ptr< const Packet > p ) {
	double time = Simulator::Now().GetSeconds();
	Enqueue_time.push_back( time );
	n++;
	//std::cout << time << " Enqueue\n";
}

void Dequeue (std::string context, Ptr< const Packet > p ) {
	double time = Simulator::Now().GetSeconds();
	Dequeue_time.push_back( time );
	//std::cout << time << " Dequeue\n";
}

void ComputeStats(){
	//std::cout << "Number of packets handled " << n;
	double total_waiting_time = 0.0;
	double mean_waiting_time;
	double waiting_time;
	for(int i=0; i < n; i++){
		waiting_time = Dequeue_time[i] - Enqueue_time[i];
		// std::cout << i << " " << waiting_time << " waiting_time\n";
		total_waiting_time += waiting_time;
	}
	mean_waiting_time = total_waiting_time / n;
	std::cout << "Packets: " << n << " Mean_waiting_time: " << mean_waiting_time << "\n";
}

//static void CourseChangeCallback (void,std::string,ns3::Ptr<ns3::Packet const> p)
//{
// int x = 1;
// std::cout << "passei aqui\n";
// x++;
//}

int main (int argc, char *argv[])
{
	uint32_t PacketSize = 1000; // em bytes
	DataRate Rout("10Mbps");
	uint64_t PacketRate = Rout.GetBitRate () / (PacketSize * 8);
	double MinIntervalPackets = 1/float(PacketRate);
	printf ("PacketSize: %iB, BitRate: %ibps, PacketRate: %.2fpps, IntervalPackets: %.3fms \n", int(PacketSize), int(Rout.GetBitRate ()), float(PacketRate), MinIntervalPackets*1000 );
	char LinkDelay[10] = "1ms";
	std:: vector<float> u;
	//u.push_back( 0.5 );
	u.push_back( 0.8 );
	u.push_back( 0.9 );
	//u.push_back( 0.95 );
	Time interPacketInterval = Seconds (1);
	uint32_t numPackets = 3000;
	double mean = 0.0;

	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	NodeContainer nodes;
	nodes.Create(2);

	/** Internet stack **/
	InternetStackHelper internet;
	internet.Install (nodes);

	PointToPointHelper ptp;
	ptp.SetQueue ("ns3::DropTailQueue");
	ptp.SetDeviceAttribute ("DataRate", DataRateValue(Rout));
	ptp.SetChannelAttribute ("Delay", StringValue (LinkDelay));

	NetDeviceContainer devices;
	devices = ptp.Install (nodes);

	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign (devices);

	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

	Ptr<Socket> recvSink = Socket::CreateSocket (nodes.Get (0), tid);  // node 0, receiver
	InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 9);
	recvSink->Bind (local);
//	recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

	Ptr<Socket> source = Socket::CreateSocket (nodes.Get (1), tid);    // node 1, sender
	InetSocketAddress remote = InetSocketAddress (interfaces.GetAddress(0), 9);
	source->SetAllowBroadcast (true);
	source->Connect (remote);

	for (std::vector<float>::iterator it = u.begin() ; it != u.end(); ++it) {
		mean = MinIntervalPackets / *it;
		// std::cout << ' ' << *it << "mean:" << mean << "\n";
		Simulator::Schedule (Seconds (start_time), &GenerateTraffic, source, PacketSize,
		                       nodes.Get (1), numPackets, mean);
	}

	Config::Connect ("/NodeList/*/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Enqueue", MakeCallback (&Enqueue));
	Config::Connect ("/NodeList/*/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Dequeue", MakeCallback (&Dequeue));

	//Ptr<Object> theptp = nodes.Get (0);
	//theptp->TraceConnectWithoutContext ("Enqueue", MakeCallback (&seila2));

	AsciiTraceHelper ascii;
	ptp.EnableAsciiAll (ascii.CreateFileStream ("udp-echo.tr"));


	Simulator::Run ();
	Simulator::Destroy ();
	ComputeStats();
	return 0;
}