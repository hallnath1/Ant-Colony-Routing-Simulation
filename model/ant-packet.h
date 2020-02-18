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
 * Authors: Nathan Hall <n.hall.4@warwick.ac.uk>
 *          Elena Buchatskaia <borovkovaes@iitp.ru>
 *          Pavel Boyko <boyko@iitp.ru>
 */
#ifndef ANTPACKET_H
#define ANTPACKET_H

#include <iostream>
#include "ns3/header.h"
#include "ns3/enum.h"
#include "ns3/ipv4-address.h"
#include <map>
#include "ns3/nstime.h"

namespace ns3 {
namespace ant {

/**
* \ingroup ant
* \brief MessageType enumeration
*/
enum MessageType
{
  ARATYPE_FANT  = 1,   //!< ARATYPE_FANT
  ARATYPE_BANT  = 2,   //!< ARATYPE_BANT
  ARATYPE_FANT_ACK = 3, //!< ARATYPE_FANT_ACK
  ARATYPE_RERR = 4 //!< ARATYPE_ROUTE_ERR
};

/**
* \ingroup ant
* \brief ANT types
*/
class TypeHeader : public Header
{
public:
  /**
   * constructor
   * \param t the AODV RREQ type
   */
  TypeHeader (MessageType t = ARATYPE_FANT);

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /**
   * \returns the type
   */
  MessageType Get () const
  {
    return m_type;
  }
  /**
   * Check that type if valid
   * \returns true if the type is valid
   */
  bool IsValid () const
  {
    return m_valid;
  }
  /**
   * \brief Comparison operator
   * \param o header to compare
   * \return true if the headers are equal
   */
  bool operator== (TypeHeader const & o) const;
private:
  MessageType m_type; ///< type of the message
  bool m_valid; ///< Indicates if the message is valid
};

/**
  * \brief Stream output operator
  * \param os output stream
  * \return updated stream
  */
std::ostream & operator<< (std::ostream & os, TypeHeader const & h);

/**
* \ingroup ant
* \brief   Forward Ant (FANT) Message Format
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |D|          Reserved           |   Hop Count   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                     FANT Sequence Number                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Destination IP Address                     |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Originator IP Address                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class FANTHeader : public Header
{
public:
  /**
   * constructor
   *
   * \param flags the message flags (0)
   * \param reserved the reserved bits (0)
   * \param hopCount the hop count
   * \param sequenceNo the sequence number
   * \param dst the destination IP address
   * \param origin the origin IP address
   */
   FANTHeader (uint8_t flags = 0, uint8_t hopCount = 0, 
              uint32_t sequenceNo = 0, Ipv4Address dst = Ipv4Address (), 
              Ipv4Address origin = Ipv4Address ());

  /**
   * \brief Get the sequenceNo.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  // Fields
  /**
   * \brief Set the hop count
   * \param count the hop count
   */
  void SetHopCount (uint8_t count)
  {
    m_hopCount = count;
  }
  /**
   * \brief Get the hop count
   * \return the hop count
   */
  uint8_t GetHopCount () const
  {
    return m_hopCount;
  }
  /**
   * \brief Set the sequence number
   * \param id the sequence number
   */
  void SetSequenceNo (uint32_t id)
  {
    m_sequenceNo = id;
  }
  /**
   * \brief Get the sequence number
   * \return the sequence number
   */
  uint32_t GetSequenceNo () const
  {
    return m_sequenceNo;
  }
  /**
   * \brief Set the destination address
   * \param a the destination address
   */
  void SetDst (Ipv4Address a)
  {
    m_dst = a;
  }
  /**
   * \brief Get the destination address
   * \return the destination address
   */
  Ipv4Address GetDst () const
  {
    return m_dst;
  }
  void SetOrigin (Ipv4Address a)
  {
    m_origin = a;
  }
  /**
   * \brief Get the origin address
   * \return the origin address
   */
  Ipv4Address GetOrigin () const
  {
    return m_origin;
  }
  // Flags
  /**
   * \brief Set the Duplicate Error flag
   * \param f the Duplicate Error flag
   */
  void SetDuplicateErr (bool f);
  /**
   * \brief Get the gratuitous RREP flag
   * \return the gratuitous RREP flag
   */
  bool GetDuplicateErr () const;
  /**
   * \brief Set the Duplicate Error flag
   * \param f the Duplicate Error flag
   */

  bool operator== (FANTHeader const & o) const;
private:
  uint8_t        m_hopCount;       ///< Hop Count
  uint32_t       m_sequenceNo;     ///< Sequence Number
  Ipv4Address    m_dst;            ///< Destination IP Address
  Ipv4Address    m_origin;         ///< Originator IP Address
  uint8_t        m_flags;          ///< |D| bit flags
};

/**
  * \brief Stream output operator
  * \param os output stream
  * \return updated stream
  */
std::ostream & operator<< (std::ostream & os, BANTHeader const &);

/**
* \ingroup ant
* \brief   Forward Ant (BANT) Message Format
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |D|           Reserved          |   Hop Count   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                     BANT Sequence Number                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Destination IP Address                     |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Originator IP Address                      |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class BANTHeader : public Header
{
public:
  /**
   * constructor
   *
   * \param flags the message flags (0)
   * \param reserved the reserved bits (0)
   * \param hopCount the hop count
   * \param sequenceNo the sequence number
   * \param dst the destination IP address
   * \param origin the origin IP address
   */
   BANTHeader (uint8_t flags = 0, uint8_t hopCount = 0, 
              uint32_t sequenceNo = 0, Ipv4Address dst = Ipv4Address (), 
              Ipv4Address origin = Ipv4Address ());

  /**
   * \brief Get the sequenceNo.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  // Fields
  /**
   * \brief Set the hop count
   * \param count the hop count
   */
  void SetHopCount (uint8_t count)
  {
    m_hopCount = count;
  }
  /**
   * \brief Get the hop count
   * \return the hop count
   */
  uint8_t GetHopCount () const
  {
    return m_hopCount;
  }
  /**
   * \brief Set the sequence number
   * \param id the sequence number
   */
  void SetSequenceNo (uint32_t id)
  {
    m_sequenceNo = id;
  }
  /**
   * \brief Get the sequence number
   * \return the sequence number
   */
  uint32_t GetSequenceNo () const
  {
    return m_sequenceNo;
  }
  /**
   * \brief Set the destination address
   * \param a the destination address
   */
  void SetDst (Ipv4Address a)
  {
    m_dst = a;
  }
  /**
   * \brief Get the destination address
   * \return the destination address
   */
  Ipv4Address GetDst () const
  {
    return m_dst;
  }
  void SetOrigin (Ipv4Address a)
  {
    m_origin = a;
  }
  /**
   * \brief Get the origin address
   * \return the origin address
   */
  Ipv4Address GetOrigin () const
  {
    return m_origin;
  }
  // Flags
  /**
   * \brief Set the Duplicate Error flag
   * \param f the Duplicate Error flag
   */
  void SetDuplicateErr (bool f);
  /**
   * \brief Get the gratuitous RREP flag
   * \return the gratuitous RREP flag
   */
  bool GetDuplicateErr () const;
  /**
   * \brief Set the Duplicate Error flag
   * \param f the Duplicate Error flag
   */

  bool operator== (BANTHeader const & o) const;
private:
  uint8_t        m_hopCount;       ///< Hop Count
  uint32_t       m_sequenceNo;     ///< Sequence Number
  Ipv4Address    m_dst;            ///< Destination IP Address
  Ipv4Address    m_origin;         ///< Originator IP Address
  uint8_t        m_flags;          ///< |D| bit flags
};

/**
  * \brief Stream output operator
  * \param os output stream
  * \return updated stream
  */
std::ostream & operator<< (std::ostream & os, BANTHeader const &);

/**
* \ingroup ant
* \brief FANT Acknowledgment (FANT-ACK) Message Format
  \verbatim
  0                   1
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |   Reserved    |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class FANTAckHeader : public Header
{
public:
  /// constructor
  FANTAckHeader ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /**
   * \brief Comparison operator
   * \param o RREP header to compare
   * \return true if the RREQ headers are equal
   */
  bool operator== (FANTAckHeader const & o) const;
private:
  uint8_t       m_reserved; ///< Not used (must be 0)
};

/**
  * \brief Stream output operator
  * \param os output stream
  * \return updated stream
  */
std::ostream & operator<< (std::ostream & os, FANTAckHeader const &);

/**
* \ingroup ant
* \brief Route Error (RERR) Message Format
  \verbatim
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |     Type      |N|          Reserved           |   DestCount   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |            Unreachable Destination IP Address (1)             |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  \endverbatim
*/
class RerrHeader : public Header
{
public:
  /// constructor
  RerrHeader ();

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator i) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  // No delete flag
  /**
   * \brief Set the no delete flag
   * \param f the no delete flag
   */
  void SetNoDelete (bool f);
  /**
   * \brief Get the no delete flag
   * \return the no delete flag
   */
  bool GetNoDelete () const;

  /**
   * \brief Comparison operator
   * \param o RERR header to compare
   * \return true if the RERR headers are equal
   */
  bool operator== (RerrHeader const & o) const;
private:
  uint8_t m_flag;            ///< No delete flag
  uint8_t m_reserved;        ///< Not used (must be 0)
  Ipv4Address m_unreachableDst; ///< Unreachable IP address
};

/**
  * \brief Stream output operator
  * \param os output stream
  * \return updated stream
  */
std::ostream & operator<< (std::ostream & os, RerrHeader const &);


}  // namespace aodv
}  // namespace ns3

#endif /* ANTPACKET_H */
