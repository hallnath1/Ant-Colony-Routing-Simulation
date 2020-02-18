/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ant-helper.h"
#include "ns3/ant-routing-protocol.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-list-routing.h"

namespace ns3 
{

AntHelper::AntHelper() : 
  Ipv4RoutingHelper ()
{
  m_agentFactory.SetTypeId ("ns3::ant::RoutingProtocol");
}

AntHelper* 
AntHelper::Copy (void) const 
{
  return new AntHelper (*this); 
}

Ptr<Ipv4RoutingProtocol> 
AntHelper::Create (Ptr<Node> node) const
{
  Ptr<ant::RoutingProtocol> agent = m_agentFactory.Create<ant::RoutingProtocol> ();
  node->AggregateObject (agent);
  return agent;
}

void 
AntHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

int64_t
AntHelper::AssignStreams (NodeContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<Node> node;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      node = (*i);
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      NS_ASSERT_MSG (ipv4, "Ipv4 not installed on node");
      Ptr<Ipv4RoutingProtocol> proto = ipv4->GetRoutingProtocol ();
      NS_ASSERT_MSG (proto, "Ipv4 routing not installed on node");
      Ptr<ant::RoutingProtocol> ant = DynamicCast<ant::RoutingProtocol> (proto);
      if (ant)
        {
          currentStream += ant->AssignStreams (currentStream);
          continue;
        }
      // Ant may also be in a list
      Ptr<Ipv4ListRouting> list = DynamicCast<Ipv4ListRouting> (proto);
      if (list)
        {
          int16_t priority;
          Ptr<Ipv4RoutingProtocol> listProto;
          Ptr<ant::RoutingProtocol> listAnt;
          for (uint32_t i = 0; i < list->GetNRoutingProtocols (); i++)
            {
              listProto = list->GetRoutingProtocol (i, priority);
              listAnt = DynamicCast<ant::RoutingProtocol> (listProto);
              if (listAnt)
                {
                  currentStream += listAnt->AssignStreams (currentStream);
                  break;
                }
            }
        }
    }
  return (currentStream - stream);
}
}