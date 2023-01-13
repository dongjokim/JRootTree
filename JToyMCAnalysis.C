#include "TRandom.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include <iostream>
#include <fstream>
#include "src/JBaseTrack.h"
#include "src/JTreeDataManager.h"
#include "src/JBaseEventHeader.h"
#include "src/JHistos.h"


using namespace std;
int main(int argc, char **argv){

	if ( argc < 2 ) {
		cout << "Usuage : " << endl;
		cout<<"+  "<<argv[0]<<" [outputFileList] [outputfile]"<<endl;
		exit(1);
	}

	char *inputfile = argv[1];
	char *outputfile = argv[2];

	TFile *fout = new TFile(outputfile,"recreate","Final analysis");
	TClonesArray *trackList = new TClonesArray("JBaseTrack",1000);
	TClonesArray *inputList = NULL;
	JTreeDataManager* dmg = new JTreeDataManager();

	JHistos *histos = new JHistos();
	histos->CreateQAHistos();

	JBaseEventHeader *eventHeader;

	//==== Read the Data files =====
	dmg->ChainInputStream(inputfile);
	//AliJBaseEventHeader *eventHeader;

	int numberEvents = dmg->GetNEvents();
	int ieout = numberEvents/20;
	int EventCounter = 0;

	if (ieout<1) ieout=1;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Loop over all events
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	TStopwatch timer;
	timer.Start();

	for(Int_t evt=0; evt<numberEvents; evt++) {
		if(evt % ieout == 0) cout << evt << "\t" << int(float(evt)/numberEvents*100) << "%" << endl ;
		dmg->LoadEvent(evt);
		eventHeader  = dmg->GetEventHeader();
		//cout << eventHeader->GetEventID() << endl;
		if(!dmg->IsGoodEvent()) continue;  // Vertex cut applied in IsGoodEvent and histo saved there too

		trackList->Clear();
		dmg->RegisterList(trackList, NULL);
		inputList = trackList;
		//---- assign input list ----
		int noAllTracks = inputList->GetEntries();
		for(int itrack=0; itrack<noAllTracks; itrack++){
			JBaseTrack *trk = (JBaseTrack*)inputList->At(itrack);
			histos->fhPt->Fill(trk->Pt());
			histos->fheta->Fill(trk->Eta());
			//trk->Print("dmg");
		}
		EventCounter++;
	}
	fout->cd();
	fout->Write();
	fout->Close();
	timer.Print();

	cout << "Total Number of Event scanned from input = " << numberEvents << endl;
	cout << "Total Number of Event used for analysis  = " << EventCounter << endl;
	cout<<"All files properly closed. Good Bye!"<<endl;

}

