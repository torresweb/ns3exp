/*Simular no NS2 (ou OMNeT++, ou outro simulador) uma fonte que envia pacotes de
tamanho fixo com intervalo aleatório de distribuição exponencial. Trata-se de
um regime estacionário, onde taxa média de envio (Rin) é menor que taxa do enlace (Rout).
Fator de Utilização (u) = Rin / Rout.

Calcular e plotar em função do fator de utilização o atraso médio na fila (M/D/1), utilizando
simbolos, juntamente com a curva teórica, utilizando linha contínua.
(ex. nas simulações usar u=[0.5 0.8 0.9 0.95], para curva teorica usar u=0:.01:0.99) */


// alteracao 1: fazer funcionar com MD1 (pacotes tam. igua) e MD1 (pacotes tam var).
// alteracao 2: fazer warm-up - ficou ruim, mas deve funcionar...
// alteracao 3: mtu max 65535

// todo
// incluir numero de pacotes como argumento
// incluir warmup como argumento
//

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
uint32_t numPackets = 100; // total de pacotes para a simulacao
int warmup = 10; // pacotes que serao desconsiderados dos calculos
int mode = 0 ; // mode de operaca padrao eh md1 = 0
int u = 95; // fator de utilizacao

// 0: only show mean
// 1: show each interval
// 2: show each packet size
int show = 0;
double start_time = 0.0;
uint32_t PacketSize = 970; // +30B de cabecalhos dara 1000Bytes
uint32_t HeadersSize = 30; // 2B PPP + 20B IP + 8B UDP

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, Ptr<Node> n, uint32_t pktCount,  double mean_interval)
{
	Time pktInterval;

	Ptr<ExponentialRandomVariable> ExpInterval;
	Ptr<ParetoRandomVariable> ParInterval;
	Ptr<ExponentialRandomVariable> ExpPktSize;

	double CurrentMeanInterval;

	uint32_t CurrentPktSize = pktSize;

	// sobre o envio:
	// md1 e mm1 o intervalo de envio eh exponencial
	if (mode == 0 or mode == 1) {
		ExpInterval = CreateObject<ExponentialRandomVariable> ();
		ExpInterval->SetAttribute ("Mean", DoubleValue (mean_interval));
		CurrentMeanInterval = ExpInterval->GetValue();
		//std::cout << CurrentMeanInterval << "\n";
		pktInterval =  Seconds(CurrentMeanInterval);
	}

	// gd1 o intervalo de envio eh pareto
	double shape = 50.0;
	if (mode == 2) {
		ParInterval = CreateObject<ParetoRandomVariable> ();
		ParInterval->SetAttribute ("Mean", DoubleValue (mean_interval));
		ParInterval->SetAttribute ("Shape",DoubleValue (shape));
		CurrentMeanInterval = ParInterval->GetValue();
		//std::cout << CurrentMeanInterval << "\n";
		pktInterval =  Seconds(CurrentMeanInterval);
	}

	// sobre o tamanho do pacote:
	// mm1 o pacote eh de tam variavel
	if (mode == 1) {
		ExpPktSize = CreateObject<ExponentialRandomVariable> ();
		ExpPktSize->SetAttribute ("Mean", DoubleValue (pktSize));
		CurrentPktSize = (uint32_t) ExpPktSize->GetValue();
		//std::cout << CurrentPktSize << "\n";
	}

	if (show == 1)
		printf("%f\n",CurrentMeanInterval);

	if (show == 2)
		printf("%i\n",CurrentPktSize + HeadersSize);


	if (pktCount >= 1)
	{
		socket->Send (Create<Packet> (CurrentPktSize));
		Simulator::Schedule (pktInterval, &GenerateTraffic, socket, pktSize, n,
				pktCount - 1, mean_interval);
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
	//std::cout << "Number of packets handled " << n << "\n";
	double total_waiting_time = 0.0;
	double mean_waiting_time;
	double waiting_time;
	int x = 0;
	for(int i=warmup; i < n; i++){
		waiting_time = Dequeue_time[i] - Enqueue_time[i];
		//std::cout << i << " " << waiting_time << " waiting_time\n";
		total_waiting_time += waiting_time;
		x++;

	}
	mean_waiting_time = total_waiting_time / (n - warmup);
	//std::cout << "Packets: " << (n - warmup) << " Mean_waiting_time: " << mean_waiting_time << "\n";
	if (show == 0)
		std::cout << u << " " << mean_waiting_time << "\n";
}

int main (int argc, char *argv[])
{

	int r = 1; //Run Seed

	CommandLine cmd;
	cmd.Usage ("ns3exp [options]\n"
			"options: --u=[1-...] Fator de utilizacao.\n"
			"         --r=[1-...] Run Seed\n"
			"		  --m=[0 ou ] MD1=0, MM1=1,"
			"		  --p packets"
			"         --w warmup packets"
			"         --s what to show"
	           "\n");
	cmd.AddValue ("u",  "Fator de Utilizacao", u);
	cmd.AddValue ("r",  "Run Seed", r);
	cmd.AddValue ("m",  "Mode: MD1=0, MM1=1, or...", mode);
	cmd.AddValue ("p",  "Packets simulation", numPackets);
	cmd.AddValue ("w",  "Packets simulation", warmup);
	cmd.AddValue ("s",  "What to show", show);
	cmd.Parse (argc, argv);


	uint32_t queueSize = 4294967295; //maior tamanho de fila possivel
	DataRate Rout("10Mbps");
	double PacketRate = (double) Rout.GetBitRate () / ((PacketSize + HeadersSize) * 8.0);
	double MinIntervalPackets = 1/double(PacketRate);
	//printf ("PacketSize: %iB, BitRate: %fbps, PacketRate: %.2fpps, IntervalPackets: %.3fms \n", PacketSize + HeadersSize, float(Rout.GetBitRate ()), float(PacketRate), MinIntervalPackets*1000 );
	char LinkDelay[10] = "1ms";
	double mean = 0.0;


	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	NodeContainer nodes;
	nodes.Create(2);

	InternetStackHelper internet;
	internet.Install (nodes);

	PointToPointHelper ptp;
	//ptp.SetQueue ("ns3::DropTailQueue");
	ptp.SetQueue("ns3::DropTailQueue", "MaxPackets",UintegerValue(queueSize));
	ptp.SetDeviceAttribute ("DataRate", DataRateValue(Rout));
	ptp.SetChannelAttribute ("Delay", StringValue (LinkDelay));
	ptp.SetDeviceAttribute("Mtu", UintegerValue(65535));


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

	double u_fact = u / 100.0;
	mean = MinIntervalPackets / u_fact;

	// isso vai funcionar? sera o usado o mesmo seed para cada chamada recursiva!
	RngSeedManager::SetSeed (1);
	RngSeedManager::SetRun (r);

	// std::cout << ' ' << *it << "mean:" << mean << "\n";
	Simulator::Schedule (Seconds (start_time), &GenerateTraffic, source, PacketSize, nodes.Get (1), numPackets, mean);

	Config::Connect ("/NodeList/*/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Enqueue", MakeCallback (&Enqueue));
	Config::Connect ("/NodeList/*/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Dequeue", MakeCallback (&Dequeue));

	//AsciiTraceHelper ascii;
	//ptp.EnableAsciiAll (ascii.CreateFileStream ("udp-echo.tr"));
	//ptp.EnablePcapAll ("mydump");

	Simulator::Run ();
	Simulator::Destroy ();
	ComputeStats();
	return 0;
}
