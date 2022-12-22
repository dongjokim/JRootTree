#include <TRandom3.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <iostream>
#include <fstream>
#include <TStopwatch.h>
#include <TLorentzVector.h>
#include "src/JBaseEventHeader.h"
#include "src/JBaseTrack.h"

const double kJPi           = 3.14159265358979; //TMath::Pi();
const double kEtaMax = 0.8;

using namespace std;
int main(int argc, char **argv){

	if ( argc < 3 ) {
		cout << "Usuage : " << endl;
		cout<<"+  "<<argv[0]<<" [outputFile] [Nevents] [Seed Number]"<<endl;
		exit(1);
	}

	char *outputfile = argv[1];
	Int_t numberEvents= atoi(argv[2]);
	int random_seed = atoi(argv[3]);

    int ieout = numberEvents/20;
    if (ieout<1) ieout=1;

	TFile *fOutRoot = new TFile(outputfile, "RECREATE");
	TClonesArray *event = new TClonesArray("JBaseEventHeader",1000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack",1000);
    TTree *jTree = new TTree("jTree","Tree from ToyMC");
    jTree->Branch("JTrackList",&tracks);
    jTree->Branch("JEventHeaderList",&event);
	double px=-1;
	double py=-1;
	double pz=-1;
	double E=-1;
	double mass = 0.139; // pizero mass
	int NTracks=2000;
	int nevents = 0;

	// pizero spectra from pythia event generator->Levy Para
	double lpt = 2, upt = 70;
	double par_Lpi14tev_pythia[3] = {2.62912e+04, 6.17808e+00, 5.26791e+00};
	TF1 *Lpi14tev_pythia = new TF1("Lpi14tev_pythia","[0]*exp(-[2]/x)/pow(x,[1])",lpt, upt);
	for(int i=0;i<3;i++) Lpi14tev_pythia->SetParameter(i,par_Lpi14tev_pythia[i]);

	TStopwatch timer;
	timer.Start();

	TRandom3 *myRandom = new TRandom3(random_seed);

	for(int evt = 0; evt < numberEvents; evt++){//event loop
        event->Clear();
        tracks->Clear();
        JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
        hdr->SetEventID(evt);
		if(evt % ieout == 0) cout << evt << "\t" << int(float(evt)/numberEvents*100) << "%" << endl ;
		for ( int ip=0; ip<NTracks; ip++ ){
			double etaBg  = myRandom->Uniform(-kEtaMax, kEtaMax);//generate bg flat in eta
			double phiBg = kJPi*myRandom->Uniform(-1,1); //rad
			double pt = Lpi14tev_pythia->GetRandom();;
			px = pt*cos(phiBg);
			py = pt*sin(phiBg);
			pz = pt*sinh(etaBg);
			E = sqrt(mass*mass+px*px+py*py+pz*pz);
			new ( (*tracks)[ip] )TLorentzVector(px, py, pz, E);
		}
		jTree->Fill(); // fill last event		
		nevents++;
	}
	timer.Print();
	cout <<"Total # of events = "<< nevents << endl;
	fOutRoot->Write();
	fOutRoot->Close();
	cout <<"Successfully finished."<<endl;
}

