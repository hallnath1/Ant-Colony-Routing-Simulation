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
 * Based on
 *      NS-2 AODV model developed by the CMU/MONARCH group and optimized and
 *      tuned by Samir Das and Mahesh Marina, University of Cincinnati;
 *
 *      AODV-UU implementation by Erik Nordström of Uppsala University
 *      http://core.it.uu.se/core/index.php/AODV-UU
 *
 * Authors: Elena Buchatskaia <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#include "ant-packet.h"
#include "ns3/address-utils.h"
#include "ns3/packet.h"

namespace ns3 {
namespace ant {

NS_OBJECT_ENSURE_REGISTERED (TypeHeader);

TypeHeader::TypeHeader (MessageType t)
  : m_type (t),
    m_valid (true)
{
}

TypeId
TypeHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ant::TypeHeader")
    .SetParent<Header> ()
    .SetGroupName ("Ant")
    .AddConstructor<TypeHeader> ()
  ;
  return tid;
}

TypeId
TypeHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
TypeHeader::GetSerializedSize () const
{
  return 1;
}

void
TypeHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 ((uint8_t) m_type);
}

uint32_t
TypeHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t type = i.ReadU8 ();
  m_valid = true;
  switch (type)
    {
    case ARATYPE_FANT:
    case ARATYPE_BANT:
    case ARATYPE_FANT_ACK:
    case ARATYPE_RERR:
      {
        m_type = (MessageType) type;
        break;
      }
    default:
      m_valid = false;
    }
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
TypeHeader::Print (std::ostream &os) const
{
  switch (m_type)
    {
    case ARATYPE_FANT:
      {
        os << "FANT";
        break;
      }
    case ARATYPE_BANT:
      {
        os << "BANT";
        break;
      }
    case ARATYPE_FANT_ACK:
      {
        os << "ROUTE_FANT_ACL";
        break;
      }
    case ARATYPE_RERR:
      {
        os << "ROUTE_RERR";
        break;
      }
    default:
      os << "UNKNOWN_TYPE";
    }
}

bool
TypeHeader::operator== (TypeHeader const & o) const
{
  return (m_type == o.m_type && m_valid == o.m_valid);
}

std::ostream &
operator<< (std::ostream & os, TypeHeader const & h)
{
  h.Print (os);
  return os;
}

//-----------------------------------------------------------------------------
// FANT
//-----------------------------------------------------------------------------
FANTHeader::FANTHeader (uint8_t flags, uint8_t hopCount, uint32_t sequenceNo, Ipv4Address dst,
                        Ipv4Address origin)
  : m_flags (flags),
    m_hopCount (hopCount),
    m_sequenceNo (sequenceNo),
    m_dst (dst),
    m_origin (origin)
{
}

NS_OBJECT_ENSURE_REGISTERED (FANTHeader);

TypeId
FANTHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ant::RreqHeader")
    .SetParent<Header> ()
    .SetGroupName ("Ant")
    .AddConstructor<FANTHeader> ()
  ;
  return tid;
}

TypeId
FANTHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
FANTHeader::GetSerializedSize () const
{
  return 23;
}

void
FANTHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (m_hopCount);
  i.WriteHtonU32 (m_sequenceNo);
  WriteTo (i, m_dst);
  WriteTo (i, m_origin);
}

uint32_t
FANTHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_hopCount = i.ReadU8 ();
  m_sequenceNo = i.ReadNtohU32 ();
  ReadFrom (i, m_dst);
  ReadFrom (i, m_origin);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
FANTHeader::Print (std::ostream &os) const
{
  os << "FANT Sequence No " << m_sequenceNo << " destination: ipv4 " << m_dst
     << " source: ipv4 " << m_origin;
}

std::ostream &
operator<< (std::ostream & os, FANTHeader const & h)
{
  h.Print (os);
  return os;
}

void
FANTHeader::SetDuplicateErr (bool f)
{
  if (f)
    {
      m_flags |= (1 << 5);
    }
  else
    {
      m_flags &= ~(1 << 5);
    }
}

bool
FANTHeader::GetDuplicateErr () const
{
  return (m_flags & (1 << 5));
}

bool
FANTHeader::operator== (FANTHeader const & o) const
{
  return (m_hopCount == o.m_hopCount && m_sequenceNo == o.m_sequenceNo
          && m_dst == o.m_dst && m_origin == o.m_origin);
}

//-----------------------------------------------------------------------------
// BANT
//-----------------------------------------------------------------------------
BANTHeader::BANTHeader (uint8_t flags, uint8_t hopCount, uint32_t sequenceNo, Ipv4Address dst,
                        Ipv4Address origin)
  : m_flags (flags),
    m_hopCount (hopCount),
    m_sequenceNo (sequenceNo),
    m_dst (dst),
    m_origin (origin)
{
}

NS_OBJECT_ENSURE_REGISTERED (BANTHeader);

TypeId
BANTHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ant::RreqHeader")
    .SetParent<Header> ()
    .SetGroupName ("Ant")
    .AddConstructor<BANTHeader> ()
  ;
  return tid;
}

TypeId
BANTHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
BANTHeader::GetSerializedSize () const
{
  return 23;
}

void
BANTHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (m_hopCount);
  i.WriteHtonU32 (m_sequenceNo);
  WriteTo (i, m_dst);
  WriteTo (i, m_origin);
}

uint32_t
BANTHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_hopCount = i.ReadU8 ();
  m_sequenceNo = i.ReadNtohU32 ();
  ReadFrom (i, m_dst);
  ReadFrom (i, m_origin);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
BANTHeader::Print (std::ostream &os) const
{
  os << "BANT Sequence No " << m_sequenceNo << " destination: ipv4 " << m_dst
     << " source: ipv4 " << m_origin;
}

std::ostream &
operator<< (std::ostream & os, BANTHeader const & h)
{
  h.Print (os);
  return os;
}

void
BANTHeader::SetDuplicateErr (bool f)
{
  if (f)
    {
      m_flags |= (1 << 5);
    }
  else
    {
      m_flags &= ~(1 << 5);
    }
}

bool
BANTHeader::GetDuplicateErr () const
{
  return (m_flags & (1 << 5));
}

bool
BANTHeader::operator== (BANTHeader const & o) const
{
  return (m_hopCount == o.m_hopCount && m_sequenceNo == o.m_sequenceNo
          && m_dst == o.m_dst && m_origin == o.m_origin);
}


//-----------------------------------------------------------------------------
// RREP-ACK
//-----------------------------------------------------------------------------

FANTAckHeader::FANTAckHeader ()
  : m_reserved (0)
{
}

NS_OBJECT_ENSURE_REGISTERED (FANTAckHeader);

TypeId
FANTAckHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::aodv::FANTAckHeader")
    .SetParent<Header> ()
    .SetGroupName ("Ant")
    .AddConstructor<FANTAckHeader> ()
  ;
  return tid;
}

TypeId
FANTAckHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
FANTAckHeader::GetSerializedSize () const
{
  return 1;
}

void
FANTAckHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_reserved);
}

uint32_t
FANTAckHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;
  m_reserved = i.ReadU8 ();
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
FANTAckHeader::Print (std::ostream &os ) const
{
}

bool
FANTAckHeader::operator== (FANTAckHeader const & o ) const
{
  return m_reserved == o.m_reserved;
}

std::ostream &
operator<< (std::ostream & os, FANTAckHeader const & h )
{
  h.Print (os);
  return os;
}


//-----------------------------------------------------------------------------
// RERR
//-----------------------------------------------------------------------------
RerrHeader::RerrHeader ()
  : m_flag (0),
    m_reserved (0)
{
}

NS_OBJECT_ENSURE_REGISTERED (RerrHeader);

TypeId
RerrHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ant::RerrHeader")
    .SetParent<Header> ()
    .SetGroupName ("Ant")
    .AddConstructor<RerrHeader> ()
  ;
  return tid;
}

TypeId
RerrHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
RerrHeader::GetSerializedSize () const
{
  return (11);
}

void
RerrHeader::Serialize (Buffer::Iterator i ) const
{
  i.WriteU8 (m_flag);
  i.WriteU8 (m_reserved);
  WriteTo (i, m_unreachableDst);
}

uint32_t
RerrHeader::Deserialize (Buffer::Iterator start )
{
  Buffer::Iterator i = start;
  m_flag = i.ReadU8 ();
  m_reserved = i.ReadU8 ();
  uint8_t dest = i.ReadU8 ();
  ReadFrom (i, m_unreachableDst);
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
RerrHeader::Print (std::ostream &os ) const
{
  os << "Unreachable destination (ipv4 address, seq. number):";
  std::map<Ipv4Address, uint32_t>::const_iterator j;
  os << m_unreachableDst;
  os << "No delete flag " << (*this).GetNoDelete ();
}

void
RerrHeader::SetNoDelete (bool f )
{
  if (f)
    {
      m_flag |= (1 << 0);
    }
  else
    {
      m_flag &= ~(1 << 0);
    }
}

bool
RerrHeader::GetNoDelete () const
{
  return (m_flag & (1 << 0));
}


bool
RerrHeader::operator== (RerrHeader const & o ) const
{
  if (m_flag != o.m_flag || m_reserved != o.m_reserved || m_unreachableDst!= o.m_unreachableDst)
    {
      return false;
    }
  return true;
}

std::ostream &
operator<< (std::ostream & os, RerrHeader const & h )
{
  h.Print (os);
  return os;
}
}
}
