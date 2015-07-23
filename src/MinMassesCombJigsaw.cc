/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   MinMassesCombJigsaw.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
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

#include "RestFrames/MinMassesCombJigsaw.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // MinMassesCombJigsaw class methods
  ///////////////////////////////////////////////
  MinMassesCombJigsaw::MinMassesCombJigsaw(const string& sname, const string& stitle) : 
    CombinatoricJigsaw(sname, stitle) {}

  MinMassesCombJigsaw::MinMassesCombJigsaw() : CombinatoricJigsaw() {}
  
  MinMassesCombJigsaw::~MinMassesCombJigsaw() {}

  void MinMassesCombJigsaw::Clear(){
    CombinatoricJigsaw::Clear();
  }

  MinMassesCombJigsaw& MinMassesCombJigsaw::Empty(){
    return MinMassesCombJigsaw::m_Empty;
  }

  bool MinMassesCombJigsaw::AnalyzeEvent(){
    if(!IsSoundMind() || !GetGroup())
      return SetSpirit(false);

    if(!InitializeEvent()){
      m_Log << LogWarning;
      m_Log << "Problem initializing event info" << m_End;
      return SetSpirit(false);
    }

    // have only implemented this case so far
    if(GetNChildren() != 2){
      m_Log << LogWarning;
      m_Log << "output size != 2 not implemented" << m_End;
      return SetSpirit(false);
    }

    int Ninput = m_InputStates.GetN();
    int Ndeps = m_DependancyStates.size();
    
    vector<TLorentzVector> inputs;
    for(int i = 0; i < Ninput; i++)
      inputs.push_back(m_InputStates[i].GetFourVector());	

    bool DO_HEM = (m_NForChild[&GetChildState(0)] == 1) && 
      (m_NForChild[&GetChildState(1)] == 1) && 
      !m_NExclusive[&GetChildState(0)] && 
      !m_NExclusive[&GetChildState(1)] &&
      (int(m_DependancyStates.size()) <= 0);

    //////////////////////////////////////
    // N^3 hemispheres
    //////////////////////////////////////
    if(DO_HEM){ 
      // boost input vectors to CM frame
      TLorentzVector TOT(0.,0.,0.,0.);
      for(int i = 0; i < Ninput; i++) TOT += inputs[i];
      TVector3 boost = TOT.BoostVector();
      for(int i = 0; i < Ninput; i++) inputs[i].Boost(-boost);

      int ip_max[2];
      int jp_max[2];
      for(int i = 0; i < 2; i++) ip_max[i] = -1;
      for(int i = 0; i < 2; i++) jp_max[i] = -1;
      double val_max = -1.;
      // Loop over all 2-jet seed probes
      int ip[2], jp[2];
      for(ip[0] = 0; ip[0] < Ninput-1; ip[0]++){
	for(ip[1] = ip[0]+1; ip[1] < Ninput; ip[1]++){
	  TVector3 nRef = inputs[ip[0]].Vect().Cross(inputs[ip[1]].Vect());
	  int Nhem[2];
	  TLorentzVector hem[2];
	  for(int i = 0; i < 2; i++){
	    Nhem[i] = 0;
	    hem[i].SetPxPyPzE(0.,0.,0.,0.);
	  }
	  // Loop over all jets
	  for(int i = 0; i < Ninput; i++){
	    if((i == ip[0]) || (i ==ip[1])) continue;
	    int ihem = int(inputs[i].Vect().Dot(nRef) > 0.);
	    Nhem[ihem]++;
	    hem[ihem] += inputs[i];
	  }
	  // assign 2 probes
	  for(jp[0] = 0; jp[0] < 2; jp[0]++){
	    for(jp[1] = 0; jp[1] < 2; jp[1]++){
	      if(jp[0] == jp[1] && Nhem[!jp[0]] == 0) continue;
	      TLorentzVector hem_probes[2];
	      for(int i = 0; i < 2; i++) hem_probes[i] = hem[i];
	      for(int i = 0; i < 2; i++) hem_probes[jp[i]] += inputs[ip[i]];
	      double val = hem_probes[0].P() + hem_probes[1].P();
	      if(val > val_max){
		val_max = val;
		for(int i = 0; i < 2; i++) ip_max[i] = ip[i];
		for(int i = 0; i < 2; i++) jp_max[i] = jp[i];
	      }
	    }
	  }
	}
      }
      if(val_max < 0)
	return false;
     
      // initialize output states
      for(int i = 0; i < 2; i++) GetChildState(i).ClearElements();
      for(int i = 0; i < 2; i++) GetChildState(jp_max[i]).AddElement(m_InputStates[ip_max[i]]);
      TVector3 nRef = inputs[ip_max[0]].Vect().Cross(inputs[ip_max[1]].Vect());
      for(int i = 0; i < Ninput; i++){
	if((i == ip_max[0]) || (i == ip_max[1])) continue;
	int ihem = int(inputs[i].Vect().Dot(nRef) > 0.);
	GetChildState(ihem).AddElement(m_InputStates[i]);
      }
      if(GetChildState(1).GetFourVector().M() > GetChildState(1).GetFourVector().M()){
	vector<RFList<VisibleState> > flip;
	for(int i = 0; i < 2; i++) flip.push_back(GetChildState(i).GetElements());
	for(int i = 0; i < 2; i++) GetChildState(i).ClearElements();
	for(int i = 0; i < 2; i++) GetChildState(i).AddElements(flip[!i]);
      }
    } 
    //////////////////////////////////////
    // NlogN 2^N brute force 
    //////////////////////////////////////
    if(!DO_HEM){
      int N_comb = 1;
      for(int i = 0; i < Ninput; i++) N_comb *= 2;
      
      int c_max = -1;
      double val_max = -1; 
      // Loop through combinatorics
      for(int c = 0; c < N_comb; c++){
	int key = c;
	int Nhem[2];
	TLorentzVector hem[2];
	for(int i = 0; i < 2; i++){
	  Nhem[i] = 0;
	  GetChildState(i).ClearElements();
	  hem[i].SetPxPyPzE(0.,0.,0.,0.);
	} 
	// set output states for combinatoric;
	for(int i = 0; i < Ninput; i++){
	  int ihem = key%2;
	  key /= 2;
	  Nhem[ihem]++;
	  hem[ihem] += inputs[i];
	  GetChildState(ihem).AddElement(m_InputStates[i]);
	}
	// check validity of combinatoric
	bool valid = true;
	for(int i = 0; i < 2; i++){
	  if(m_NExclusive[&GetChildState(i)]){
	    if(Nhem[i] != m_NForChild[&GetChildState(i)]) valid = false;
	  } else {
	    if(Nhem[i] < m_NForChild[&GetChildState(i)]) valid = false;
	  }
	}
	if(!valid) continue;

	// Execute depedancy Jigsaws
	ExecuteDependancyJigsaws();

	// Evaluate metric for this cominatoric
	for(int i = 0; i < Ndeps;  i++)
	  hem[i] += m_DependancyStates[i].GetFourVector();
       
	TVector3 boost = (hem[0]+hem[1]).BoostVector();
	hem[0].Boost(-boost);
	hem[1].Boost(-boost);
	double val = hem[0].P()+hem[1].P();
	if(Nhem[0] == 0 || Nhem[1] == 0){
	  val = 0.; // cout << "zero in Nhem " << val << endl;
	}
	if(val >= val_max){
	  val_max = val;
	  c_max = c;
	}
      }
      if(c_max < 0) return false;
     
      // Set outputs to best combinatoric
      for(int i = 0; i < 2; i++) GetChildState(i).ClearElements();
      int key = c_max;
      for(int i = 0; i < Ninput; i++){
	int ihem = key%2;
	key /= 2;
	GetChildState(ihem).AddElement(m_InputStates[i]);
      }
    }
    // Execute depedancy Jigsaws
    ExecuteDependancyJigsaws();

    return SetSpirit(true);
  }

  MinMassesCombJigsaw MinMassesCombJigsaw::m_Empty;

}

