#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"
#include "TClonesArray.h"
#include <TStopwatch.h>
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
int main(int argc, char **argv){

	if ( argc < 2 ) {
		cout << "Wrong usuage : it nees infile and outfile name" << endl;
		exit(1);
	}

	char *fname = argv[1];
	char *outputfile = argv[2];

	ifstream in;
	in.open(fname,ios::in);
	if(!in){ return 1;}

	TFile *fOutRoot = new TFile(outputfile, "RECREATE");
	TClonesArray *event = new TClonesArray("JBaseEventHeader",250000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack",100000);
	TTree *jTree = new TTree("jTree","Tree from DCCI2 files");
	jTree->Branch("JEventHeaderList",&event);
	jTree->Branch("JTrackList",tracks);

	int ieout = 12000;
	if (ieout<1) ieout=1;
	TStopwatch timer;
	timer.Start();
	int  eventID;
	int nevents = 0;
	int ip = 0;
	std::string templine;
	event->Clear();

	//We want to get the second line starting from # at the top of each event files.
	//I would like to set EventInfoHead = true when it detects the second #, 
	//and avoid reading the first one.
	//Yuuka
	//==================================================================
	bool EventInfoHead=false;
	int CountHush=0;
	while(getline(in,templine)) {
		if ( templine.find('%')!=std::string::npos) {
			istringstream isss(templine);
			int tevent;
			string com;
			isss >> com >> tevent;
			if (tevent>0) {jTree->Fill();};
			EventInfoHead=false;
			CountHush=0;
		} else if(templine.find('#')!=std::string::npos){
			CountHush++;
			if(CountHush==2) EventInfoHead=true;
			istringstream iss(templine);
			std::string pct;
			int iev, nv;
			string com;
			double weight_in, tau;
			iss >> com >> iev >> nv >> tau >> weight_in;
			if (EventInfoHead) {
			//if (iev<49999) {
				JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
				eventID = iev-1;
				hdr->SetEventID(eventID);
				//hdr->SetEventPlane(xx)
				if(eventID % ieout == 0) cout << "eventID : " << eventID << "\t# tracks: " << nv << endl;
				tracks->Clear();
				ip = 0;
				nevents++;				
			}
	
		} else {
			istringstream is(templine);
			int data1, data2, ID, col, acol;
			double m,e,px,py,pz,x,y,z,t,ft, rap;
			std::string TAG;
			is >> data1 >> data2 >> col >> acol >> ID >> m >> e >> px >> py >> pz >> rap >> x >> y >> z >> t >> ft >> TAG;
			new ( (*tracks)[ip++] )JBaseTrack(px, py, pz, e, ID, ID, 0);
			//if(eventID % ieout == 0) cout <<"\t"<< eventID<<" "<< ip<< "\t"<<px<<"\t"<<py<<"\t"<<pz<<"\t"<< ID << endl;
		}
	}
	cout <<"Total events = "<<event->GetEntriesFast() << endl;

	//fin.close();

	cout <<"Total # of events = "<< nevents << endl;
	jTree->Write();
	fOutRoot->Close();
	cout <<"Successfully finished."<<endl;
	timer.Print();

	return 0;
}
