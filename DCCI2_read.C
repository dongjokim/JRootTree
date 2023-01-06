#include "src/JBaseTrack.h"
#include "src/JBaseEventHeader.h"
#include "TClonesArray.h"
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
    TClonesArray *event = new TClonesArray("JBaseEventHeader",1000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack",100000);
	TTree *jTree = new TTree("jTree","Tree from DCCI2 files");
    jTree->Branch("JEventHeaderList",&event);
	jTree->Branch("tracks",tracks);

	int  eventID;
	int nevents = 0;
	int NTracks = 10;
	int ip = 0;
	std::string templine;
	event->Clear();
	tracks->Clear();
	while(getline(in,templine)) {
		if(templine.find('#')!=std::string::npos) {
			cout <<"Just comments" << endl;
		} else if(templine.find('%')!=std::string::npos){
			
				istringstream iss(templine);
				std::string pct;
				int iev, nv;
				string com;
				double weight_in, tau;
				iss >> com >> iev >> nv >> tau >> weight_in;
				JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
				eventID = iev;
				hdr->SetEventID(eventID);
				//hdr->SetEventPlane(xx)
				cout << "eventID : " << eventID <<  endl;
				nevents++;	
		} else {
				istringstream is(templine);
				int data1, data2, ID, col, acol;
				double m,e,px,py,pz,x,y,z,t,ft, rap;
				std::string TAG;
				is >> data1 >> data2 >> col >> acol >> ID >> m >> e >> px >> py >> pz >> rap >> x >> y >> z >> t >> ft >> TAG;
			    new ( (*tracks)[ip++] )JBaseTrack(px, py, pz, e, ID, ID, 0);
			    cout <<eventID<<" "<< ip<< "\t"<<px<<"\t"<<py<<"\t"<<pz<<"\t"<< ID << endl;
		}
		jTree->Fill(); // fill last event
	}
	cout <<eventID<<"\t"<< tracks->GetEntriesFast() << endl;
	
	//fin.close();

	cout <<"Total # of events = "<< nevents << endl;
	jTree->Write();
	fOutRoot->Close();
	cout <<"Successfully finished."<<endl;

	return 0;
}
