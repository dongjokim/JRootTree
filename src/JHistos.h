//===========================================================
// JHistos.h
//===========================================================

#ifndef JHISTOS_H
#define JHISTOS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TFile.h>

#include "JHistos.h"
#include <TClonesArray.h>
#include <TFormula.h>

class JHistos {

	public:
		JHistos(); 
		virtual ~JHistos(){;}	  //destructor

		// ALICE methods =====================================================
		void CreateQAHistos();

	public:
		char  hname[140], htit[140];
		float b1, b2, pb1, pb2;
		TH1D *fhPt;


	protected:

};

#endif

