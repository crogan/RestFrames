/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   RFCharge.hh
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

#ifndef RFCharge_HH
#define RFCharge_HH

namespace RestFrames {

  ///////////////////////////////////////////////
  // RFCharge class
  ///////////////////////////////////////////////
  class RFCharge {

  public:
    RFCharge(const RFCharge& charge);
    RFCharge(int charge);
    RFCharge(int charge_num, int charge_den);
    ~RFCharge();

    // void operator=(const RFCharge& charge);
    // bool operator==(int key) const;
    // bool operator==(const RFKey& key) const;
    // int GetKey() const;
    // bool IsSame(const RFKey& key) const;

  private:
    int m_ChargeNum;
    int m_ChargeDen;

  };

}

#endif
