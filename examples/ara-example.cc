/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This is an example script for AODV manet routing protocol.
 *
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#include "ns3/ara-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/v4ping-helper.h"
#include "ns3/netanim-module.h"
#include <iostream>
#include <cmath>

using namespace ns3;

/**
 * \brief Stationary 4x4 Matrix using AODV script.
 *
 * This script creates 2-dimensional grid topology and then ping last node from the first one:
 *
 * [10.0.0.1] <-- step --> [10.0.0.2] <-- step --> [10.0.0.3] <-- step --> [10.0.0.4]
 *
 * ping 10.0.0.4
 */
class AraExample
{
  public:
    AraExample ();
    /// Configure script parameters, \return true on successful configuration
    bool Configure (int argc, char **argv);
    /// Run simulation
    void Run ();
    /// Report results
    void Report (std::ostream & os);

  private:
    ///\name parameters
    //\{
    /// Number of nodes
    uint32_t size;
    /// Distance between nodes, meters
    double step;
    /// Simulation time, seconds
    double totalTime;
    /// Write per-device PCAP traces if true
    bool pcap;
    /// Print routes if true
    bool printRoutes;
    //\}

    ///\name network
    //\{
    NodeContainer nodes;
    NetDeviceContainer devices;
    Ipv4InterfaceContainer interfaces;
    //\}

  private:
    void CreateNodes ();
    void CreateDevices ();
    void InstallInternetStack ();
    void InstallApplications ();
};

int main (int argc, char **argv)
{
  AraExample sim;
  if (!sim.Configure (argc, argv))
    NS_FATAL_ERROR ("Configuration failed. Aborted.");

  sim.Run ();
  sim.Report (std::cout);
  return 0;
}

//-----------------------------------------------------------------------------
AraExample::AraExample () :
  size (16),
  step (100),
  totalTime (10),
  pcap (true),
  printRoutes (true)
{
}

bool AraExample::Configure (int argc, char **argv)
{

  SeedManager::SetSeed (12345);
  CommandLine cmd;

  cmd.AddValue ("pcap", "Write PCAP traces.", pcap);
  cmd.AddValue ("printRoutes", "Print routing table dumps.", printRoutes);
  cmd.AddValue ("size", "Number of nodes.", size);
  cmd.AddValue ("time", "Simulation time, s.", totalTime);
  cmd.AddValue ("step", "Grid step, m", step);
 
  cmd.Parse (argc, argv);
  return true;
}

void AraExample::Run ()
{
  CreateNodes ();
  CreateDevices ();
  InstallInternetStack ();
  InstallApplications ();

  std::cout << "Starting simulation for " << totalTime << " s ...\n";

  /* NetAnim XML output
  AnimationInterface anim ("animation.xml");
  anim.SetMobilityPollInterval(Seconds(0.25));
  anim.EnablePacketMetadata(true);
  */

  Simulator::Stop (Seconds (totalTime));
  Simulator::Run ();
  Simulator::Destroy ();
}

void AraExample::Report (std::ostream &)
{
}

void AraExample::CreateNodes ()
{
  std::cout << "Creating " << (unsigned)size << " nodes " << step << " m apart.\n";
  nodes.Create (size);
  // Name nodes
  for (uint32_t i = 0; i < size; ++i)
    {
      std::ostringstream os;
      os << "node-" << i;
      Names::Add (os.str (), nodes.Get (i));
    }
  // Create static grid
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (step),
    "DeltaY", DoubleValue (step),
    "GridWidth", UintegerValue (4),
    "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);
}

void AraExample::CreateDevices ()
{
  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"), "RtsCtsThreshold", UintegerValue (0));
  devices = wifi.Install (wifiPhy, wifiMac, nodes); 

  if (pcap)
    {
      wifiPhy.EnablePcapAll (std::string ("ara"));
    }
}

void AraExample::InstallInternetStack ()
{
  AraHelper ara;
  InternetStackHelper stack;
  stack.SetRoutingHelper (ara);
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.0.0.0");
  interfaces = address.Assign (devices);

  if (printRoutes)
    {
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("ara.routes", std::ios::out);
      ara.PrintRoutingTableAllAt (Seconds (8), routingStream);
    }
}

void
AraExample::InstallApplications ()
{
  V4PingHelper ping (interfaces.GetAddress (size - 1));
  ping.SetAttribute ("Verbose", BooleanValue (true));

  ApplicationContainer p = ping.Install (nodes.Get (0));
  p.Start (Seconds (0));
  p.Stop (Seconds (totalTime) - Seconds (0.001));
}

