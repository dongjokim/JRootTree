#include <TVector3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TFormula.h>


#include  "JHistos.h"


//______________________________________________________________________________
JHistos::JHistos(){   // constructor

}

//__________________________________________________________________________________________________
void JHistos::CreateQAHistos() {
	const int nBins =  74 ;
	double ptt[nBins] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
		1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 60, 70, 80, 90, 100};
	fhPt = new TH1D("hPt", "pT spectra", nBins-1, ptt );
	fhPt->Sumw2();
	fheta = new TH1D("heta", "eta", 500, -10,10);
	fheta->Sumw2();
}
