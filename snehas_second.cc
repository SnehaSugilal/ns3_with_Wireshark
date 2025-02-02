//Sneha's second.cc

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Network Topolgy: 
// 3 nodes, namely Node 0 through Node 2, connected via a CSMA link in the 1st LAN (192.168.1.0/24)
// 3 nodes, namely Node 3 through Node 5, connected via a CSMA link in the 2nd LAN (192.168.2.0/24)
// 2 nodes, Nodes 2 and 3 are in the point-to-point link, connecting both LANs
// The applications running in the network are: 
// UDP Echo Server at Node 1 (192.168.1.1): Listening on port 21
// UDP Echo Client at Node 5 (192.168.2.3): Sends 2 UDP Echo packets to the server at times 4s and 7s

//                            TOPOLOGY
//                   
//                         192.168.3.0/24 
//              n0  n1 n2  -------------   n3  n4  n5  
//               |   |   |                 |   |   |
//               =========                 ==========
//             192.168.1.0/24            192.168.2.0/24
//              (CSMA link1)              (CSMA link2)

 
using namespace ns3;
 
NS_LOG_COMPONENT_DEFINE("NetworkSimTask");  //Referenced from second.cc example
 
int main(int argc, char *argv[]) {
   
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);   // Here, we do this to enable Logging
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

     // Defining Node containers
    NodeContainer csmaNodes1, csmaNodes2, p2pNodes;
    csmaNodes1.Create(3); // CSMA Nodes 0, 1, 2           
    csmaNodes2.Create(3); // CSMA Nodes 3, 4, 5

    p2pNodes.Add(csmaNodes1.Get(2)); // p2p Node 2
    p2pNodes.Add(csmaNodes2.Get(0)); // p2p Node 3

    // Creatin P2P i.e. point-to-point link between Node 2 and Node 3

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));   //setting data rate = 10Mbps
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));       // delay as 2 milliseconds 
    NetDeviceContainer p2pDevices = pointToPoint.Install(p2pNodes);

    // CSMA setup for first LAN (192.168.1.0/24)
    CsmaHelper csma1;
    csma1.SetChannelAttribute("DataRate", StringValue("100Mbps"));    //setting data rate = 100Mbps
    csma1.SetChannelAttribute("Delay", TimeValue(MicroSeconds(10))); // delay as 10 microseconds 
    NetDeviceContainer csmaDevices1 = csma1.Install(csmaNodes1);
 

    // CSMA setup for second LAN (192.168.2.0/24)
    CsmaHelper csma2;
    csma2.SetChannelAttribute("DataRate", StringValue("100Mbps")); //same settings as above
    csma2.SetChannelAttribute("Delay", TimeValue(MicroSeconds(10)));
    NetDeviceContainer csmaDevices2 = csma2.Install(csmaNodes2);

    // Installing network stacks for the 2 csma nodes

    InternetStackHelper stack;
    stack.Install(csmaNodes1);
    stack.Install(csmaNodes2);
 
    Ipv4AddressHelper address;                          // Assigning IP addresses
    address.SetBase("192.168.1.0", "255.255.255.0");   // Assigning addresses to CSMA1 (192.168.1.0/24)
    Ipv4InterfaceContainer csmaInterfaces1 = address.Assign(csmaDevices1);

    address.SetBase("192.168.2.0", "255.255.255.0");   // Assigning addresses to CSMA2 (192.168.2.0/24)
    Ipv4InterfaceContainer csmaInterfaces2 = address.Assign(csmaDevices2);

    address.SetBase("192.168.3.0", "255.255.255.0"); // Assigning addresses to the Point-to-Point link (192.168.3.0/24)
    Ipv4InterfaceContainer p2pInterfaces = address.Assign(p2pDevices);

    // Setting up the UDP Echo Server on Node 1
    UdpEchoServerHelper echoServer(21);                                       // also set the port to 21
    ApplicationContainer serverApps = echoServer.Install(csmaNodes1.Get(1)); // for Node 1
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Now setting up the UDP Echo Client on Node 5
    UdpEchoClientHelper echoClient(csmaInterfaces1.GetAddress(1), 21);   //port 21 on the client side as well
    echoClient.SetAttribute("MaxPackets", UintegerValue(2));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(3.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));
 
    ApplicationContainer clientApps = echoClient.Install(csmaNodes2.Get(2)); // for Node 5
    clientApps.Start(Seconds(4.0));
    clientApps.Stop(Seconds(10.0));

    // To generate the pcap files, enable packet capture via Wireshark 
    pointToPoint.EnablePcap("task2-p2p", p2pDevices.Get(0), true);
    csma2.EnablePcap("task2-csma", csmaDevices2.Get(1), true);

    // Enable routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
