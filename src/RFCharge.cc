/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFCharge.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2016 May
///
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "RestFrames/RFCharge.hh"

namespace RestFrames {

  RFCharge::RFCharge(const RFCharge& charge) {}
  RFCharge::RFCharge(int charge) {}
  RFCharge::RFCharge(int charge_num, int charge_den) {}
  RFCharge::~RFCharge() {}

  // void RFKey::operator=(const RFKey& key){ m_Key = key.GetKey(); }

  // bool RFKey::operator==(int key) const { return (m_Key == key); }

  // bool RFKey::operator==(const RFKey& key) const { return (m_Key == key.GetKey()); }

  // int RFKey::GetKey() const { return m_Key; }

  // bool RFKey::IsSame(const RFKey& key) const {
  //   return m_Key == key.GetKey();
  // }

}
