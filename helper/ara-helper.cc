/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ara-helper.h"
#include "ns3/ara-routing-protocol.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-list-routing.h"

namespace ns3 
{

AraHelper::AraHelper() : 
  Ipv4RoutingHelper ()
{
  m_agentFactory.SetTypeId ("ns3::ara::RoutingProtocol");
}

AraHelper* 
AraHelper::Copy (void) const 
{
  return new AraHelper (*this); 
}

Ptr<Ipv4RoutingProtocol> 
AraHelper::Create (Ptr<Node> node) const
{
  Ptr<ara::RoutingProtocol> agent = m_agentFactory.Create<ara::RoutingProtocol> ();
  node->AggregateObject (agent);
  return agent;
}

void 
AraHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

int64_t
AraHelper::AssignStreams (NodeContainer c, int64_t stream)
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
      Ptr<ara::RoutingProtocol> ara = DynamicCast<ara::RoutingProtocol> (proto);
      if (ara)
        {
          currentStream += ara->AssignStreams (currentStream);
          continue;
        }
      // Ara may also be in a list
      Ptr<Ipv4ListRouting> list = DynamicCast<Ipv4ListRouting> (proto);
      if (list)
        {
          int16_t priority;
          Ptr<Ipv4RoutingProtocol> listProto;
          Ptr<ara::RoutingProtocol> listAra;
          for (uint32_t i = 0; i < list->GetNRoutingProtocols (); i++)
            {
              listProto = list->GetRoutingProtocol (i, priority);
              listAra = DynamicCast<ara::RoutingProtocol> (listProto);
              if (listAra)
                {
                  currentStream += listAra->AssignStreams (currentStream);
                  break;
                }
            }
        }
    }
  return (currentStream - stream);
}



}

