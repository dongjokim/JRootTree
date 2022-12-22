#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using namespace std;
int main(int argc, char **argv){

	if ( argc < 2 ) {
		cout << "Wrong usuage : it nees infile and outfile name" << endl;
		exit(1);
	}

	char *inputfile = argv[1];
	char *outputfile = argv[2];

	//ifstream in;
	//in.open(inputfile);
	FILE *pf = fopen(inputfile,"r");

	TFile *fOutRoot = new TFile(outputfile, "RECREATE");
    TClonesArray *event = new TClonesArray("JBaseEventHeader",1000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack",100000);
	TTree *jTree = new TTree("jTree","Tree from DCCI2 files");
    jTree->Branch("JEventHeaderList",&event);
	jTree->Branch("tracks",tracks);

	int  eventID;
	int dummy1, dummy2, colortag, anticolortag, ParticleID; 
    double mass, energy, px, py,pz, rapidity, x, y, z, t, dummy3,origin;

	int nevents = 0;
	int NTracks = 10;
	while(!feof(pf)){
	//int c;
	//while ((c = getc(pf)) != '#' && c != EOF){
		event->Clear();
		tracks->Clear();
		JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
		eventID = nevents;
		hdr->SetEventID(eventID);
			//hdr->SetEventPlane(xx)
		cout << "eventID : " << eventID <<"\t Number of tracks ="<< NTracks<<  endl;
		for ( int ip=0; ip<NTracks; ip++ ){
			fscanf(pf,"%d %d %d %d %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %*[^\n]\n",
					&dummy1, &dummy2, &colortag, &anticolortag, &ParticleID, &mass, &energy, &px, &py, &pz, &rapidity, &x, &y, &z, &t, &dummy3,&origin);
			new ( (*tracks)[ip] )JBaseTrack(px, py, pz, energy, ParticleID, ParticleID, 0);
			cout <<eventID<<"\t"<<px<<"\t"<<py<<"\t"<<pz<<"\t"<< ParticleID << endl;
		}
		jTree->Fill(); // fill last event
		nevents++;
	}

	fclose(pf);

	cout <<"Total # of events = "<< nevents << endl;
	jTree->Write();
	fOutRoot->Close();
	cout <<"Successfully finished."<<endl;

	return 0;
}
