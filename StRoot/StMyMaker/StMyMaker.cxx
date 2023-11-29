#include "StMyMaker.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoEpdHit.h"
#include "StThreeVectorF.hh"
#include "TFile.h"
#include "Stiostream.h"
#include <TMath.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "phys_constants.h"
#include "StBTofUtil/tofPathLength.hh"
#include <fstream>
#include <vector>
#include <algorithm>
#include "TProfile.h"
#include "TTree.h"
#include "StRoot/MeanDcaTool/MeanDcaTool.h"
#include "StRoot/TriggerTool/TriggerTool.h"

ClassImp(StMyMaker)

StMyMaker::StMyMaker(const char *name, StPicoDstMaker *picoMaker, const char *outName) : StMaker(name){
	mOutputName = outName;
	mPicoDstMaker = picoMaker;
	mPicoDst = 0;
}

StMyMaker::~StMyMaker() {}

// ------
Int_t StMyMaker::Init(){
	mFileOut = new TFile(mOutputName, "recreate");

	cker = new MeanDcaTool();
	tgtool = new TriggerTool();
	hDCAz = new TH1F(
		"hDCAz", ";<DCA_{z}> [cm];",
		300, -1.5, 1.5
	);
	hDCAxy = new TH1F(
		"hDCAxy", ";<sDCA_{xy}> [cm];",
		300, -1.5, 1.5
	);

	h2DCAzRef = new TH2F(
		"h2DCAzRef", ";RefMult;<DCA_{z}> [cm]",
		700, -0.5, 699.5,
		300, -1.5, 1.5
	);
	h2DCAxyRef = new TH2F(
		"h2DCAxyRef", ";RefMult;<DCA_{xy}> [cm]",
		700, -0.5, 699.5,
		300, -1.5, 1.5
	);

	tpDCAzRef = new TProfile(
		"tpDCAzRef", ";RefMult;<DCA_{z}> [cm]",
		700, -0.5, 699.5
	);
	tpDCAxyRef = new TProfile(
		"tpDCAxyRef", ";RefMult;<DCA_{xy}> [cm]",
		700, -0.5, 699.5
	);


	return kStOK;
}

//---------------------------------------------------------
Int_t StMyMaker::Finish(){
	mFileOut->cd();

	hDCAz->Write();
	h2DCAzRef->Write();
	tpDCAzRef->Write();

	hDCAxy->Write();
	h2DCAxyRef->Write();
	tpDCAxyRef->Write();

	mFileOut->Close();
	return kStOK;
}

void StMyMaker::Clear(Option_t *opt) {}

//---------------------------------------------------------------
Int_t StMyMaker::Make(){
	if (!mPicoDstMaker){
		LOG_WARN << " No PicoDstMaker! Skip! " << endm;
		return kStWarn;
	}

	mPicoDst = mPicoDstMaker->picoDst();
	if (!mPicoDst){
		LOG_WARN << " No PicoDst! Skip! " << endm;
		return kStWarn;
	}

	// Load event
	mPicoEvent = (StPicoEvent *)mPicoDst->event();
	if (!mPicoEvent){
		cerr << "Error opening picoDst Event, skip!" << endl;
		return kStOk;
	}

	TVector3 pVtx = mPicoEvent->primaryVertex();
	Double_t vx = pVtx.X();
	Double_t vy = pVtx.Y();
	Double_t vz = pVtx.Z();

	if (fabs(vx) < 1e-5 && fabs(vy) < 1e-5 && fabs(vz) < 1e-5){
		return kStOk;
	}
	if (sqrt(vx * vx + vy * vy) >= 2.0){
		return kStOk;
	}
	if (fabs(vz) > 50.){
		return kStOk;
	}

	if (tgtool->GetTriggerID(mPicoEvent) < 0) {
		return kStOK;
	}

	if (!cker->Make(mPicoDst)) {
		return kStOK;
	}
	int refMult = mPicoEvent->refMult();

	double dcaz = cker->mDCAz;
	double dcaxy = cker->mDCAxy;

	hDCAz->Fill(dcaz);
	hDCAxy->Fill(dcaxy);

	h2DCAzRef->Fill(refMult, dcaz);
	h2DCAxyRef->Fill(refMult, dcaxy);

	tpDCAzRef->Fill(refMult, dcaz);
	tpDCAxyRef->Fill(refMult, dcaxy);

	return kStOK;
}
