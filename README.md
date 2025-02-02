# ns3_with_Wireshark
This project is adept at modifying example scripts, simulating diverse network architectures, and evaluating communication protocols that explore and appreciate the ns-3 simulator for its networking concepts, complemented by Wireshark with packet-capturing for further analysis.


The objective of this task is to become familiar with the ns-3 working environment and the simulation workflow. To complete it, we need a working environment with ns-3 and Wireshark properly set up.

We deal with UDP Echo, an application in which the client sends some information to the server, who replies back with the same packet. We will set up the client to send just one packet at time t=2 sec. For detailed information go to the ns-3 Tutorial, section 5.2 “A First ns-3 Script". Refer the pdf document above to get better understanding and visualization.

## Part 1: Modify the example 'first.cc' to simulate the following network:
- 2 nodes: 1 network interface at each node
- Point-to-point link: 
   o Data Rate: 10 Mbps
   o Delay: 2 ms
- IP address assignment:
   o 192.168.2.0/24
- Application:
   o UDP Echo Server on port 63
   o Packet size: 256 bytes

## Part 2: Starting from second.cc in the Tutorials folder within your ns-3 installation, let's create and simulate a network with the following architecture:
- The network contains:
     o 3 nodes in the first shared bus operating under CSMA
     o 3 nodes in the second shared bus operating under CSMA
     o 2 nodes in the point-to-point link (Note that Nodes 2 and 3 in Figure 1 have two network interfaces, one for each link to which they are connected)
- The applications running in the network are:
     o UDP Echo Server at Node 1 (Listening on port 21)
     o UDP Echo Client at Node 5 (Sends 2 UDP Echo packets to the server at times 4s and 7s)
- Enable packet tracing with promiscuous mode only in Nodes 2 in the point-to-point interface and Node 4


### References:
- ns-3 Tutorial: https://www.nsnam.org/docs/tutorial/html/index.html
