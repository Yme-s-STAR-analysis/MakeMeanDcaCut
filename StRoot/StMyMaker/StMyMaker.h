#ifndef STAR_StMyMaker
#define STAR_StMyMaker
#include "StMaker.h"
#include "TTree.h"
#include "StThreeVectorF.hh"
#include "TString.h"
#include "StarClassLibrary/StThreeVectorF.hh"
#include "StarClassLibrary/StThreeVectorD.hh"

class StPicoDst;
class StPicoEvent;
class StPicoTrack;
class StPicoDstMaker;
class TH1F;
class TH2F;
class TProfile;
class TTree;
class MeanDcaTool;
class TriggerTool;
#include "TVector3.h"


class StMyMaker : public StMaker{
	public:
		StMyMaker(const char *name, StPicoDstMaker *picoMaker, const char *outName = ".root");
		virtual ~StMyMaker();

		virtual Int_t Init();
		virtual Int_t Make();
		virtual void Clear(Option_t *opt = "");
		virtual Int_t Finish();

	private:
		StPicoDstMaker *mPicoDstMaker;
		StPicoDst *mPicoDst;
		StPicoEvent *mPicoEvent;
		StPicoTrack *picoTrack;

		TString mOutputName;
		TFile *mFileOut;

		MeanDcaTool* cker;
		TriggerTool* tgtool;


		TH1F* hDCAz;
		TH1F* hDCAxy;
		TH2F* h2DCAzRef;
		TH2F* h2DCAxyRef;
		TProfile* tpDCAzRef;
		TProfile* tpDCAxyRef;

		ClassDef(StMyMaker, 1)
};

#endif
