#ifndef __EDA_H__
#define __EDA_H__

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbEDA                                                               //
//                                                                      //
// Mother of EDA (Event Display and Analyser)                           //
//                                                                      //
// it consists of 5 EdbTrackSet (TS,SB,SF,BT,MN) and 1 EdbVertexSet.    //
//                                                                      //
// It inherits following.                                               //
//  EdbEDACamera     : View, Pictures and Animation                     //
//  EdbEDAIO         : I/O of feedback file, filtered text, cs_info.txt, Basetracks. //
//  EdbEDABrickData  : Manage Brick info and store EdbDataProc. PID <-> IPL conversion. //
//  EdbEDASelection  : Manage Selected segments and tracks.             //
//////////////////////////////////////////////////////////////////////////


#include "EdbEDAUtil.h"
#include "EdbEDATrackSet.h"
#include "EdbEDASets.h"
#include "EdbEDAMainTab.h"
#include "EdbEDATabs.h"
#include "EdbEDAPlotTab.h"
#include "EdbEDADecaySearch.h"
#include "EdbEDAShowerTab.h"

//-------------------------------------------------------------------

using namespace EdbEDAUtil;

class EdbEDAObject : public TObject {
	private:
	int eDraw;
	void *eRef;
	public:
	EdbEDAObject():eDraw(kTRUE),eRef(NULL){}
	~EdbEDAObject(){}
	virtual void Draw(Option_t *option=""){ printf("override this");}
	virtual void SetColorMode(int mode){ printf("override this.\n");}
	void SetReference(void *ref) { eRef=ref;}
	void *GetReference(){ return eRef;}
	bool GetDraw(){ return eDraw;}
	void SetDraw(bool b=kTRUE){ eDraw=b;}
	ClassDef(EdbEDAObject,0) // test
};

class EdbEDAText : public EdbEDAObject{
	private:
	float eX, eY, eZ;
	int    eTextColor;
	int    eTextSize;
	int    eTextFont;
	TString eText;
	void *eRef;
	
	public:
	EdbEDAText(float x, float y, float z, TString str, int color=kWhite) : 
		eX(x), eY(y), eZ(z), eText(str){SetAttribute(14,color);}
	~EdbEDAText(){}

	void SetAttribute(int size=14, int color=kWhite, int fontfile=4){
		eTextSize  = size;
		eTextColor = color;
		eTextFont  = fontfile;
	}
	void SetColorMode(int mode){
		if(mode==kBLACKWHITE) SetAttribute(eTextSize, kBlack, eTextFont);
		else SetAttribute(eTextSize, kWhite, eTextFont);
	}
	int GetTextColor() { return eTextColor;}
	int GetTextSize () { return eTextSize;}
	int GetTextFont () { return eTextFont;}
	void SetXYZ(float x, float y, float z){ eX=x; eY=y; eZ=z;}
	void SetText(TString str){ eText=str;}
	void AddText(TString str){ eText+=str;}
	int N(){return eText.Length();}
	void Draw(Option_t *option="");
	
	ClassDef(EdbEDAText, 0) // a comment class
	
};

class EdbEDALine : public EdbEDAObject{
	private:
	float X1,Y1,Z1, X2,Y2,Z2;
	int col, wid;
	TString text;
	
	public:
	EdbEDALine( float x1, float y1, float z1, float x2, float y2, float z2, int color, int width, char *comment = "") : X1(x1), Y1(y1), Z1(z1), X2(x2), Y2(y2), Z2(z2), col(color), wid(width), text(comment){}
	EdbEDALine( int color, int width, char *comment = "") : col(color), wid(width), text(comment){}
	~EdbEDALine(){}
	
	void SetPoint1(float x1, float y1, float z1){ X1=x1; Y1=y1; Z1=z1;}
	void SetPoint2(float x2, float y2, float z2){ X2=x2; Y2=y2; Z2=z2;}
	
	void Draw(Option_t *option="");
	ClassDef(EdbEDALine, 0) // Line as EdbEDAObject
};

//-------------------------------------------------------------------
#include<TGLOverlay.h>
class EdbEDAOverlay : public TGLOverlayElement{
	
	private:
	bool eDraw;
	int eXa,eYa, eLength;
	int eXb,eYb;
	int eBarDirection;
	public:
	EdbEDAOverlay();
	~EdbEDAOverlay(){}
	void Render(TGLRnrCtx& ctx);
	void RenderBar(TGLRnrCtx& ctx);
	void RenderAxes(TGLRnrCtx& ctx);
	void SetDraw(bool b) {eDraw=b;}
	void SetXY(int pixel_x=50, int pixel_y=30);
	void SetLength(int pixel_length=120);
	ClassDef(EdbEDAOverlay, 0) // a class for draw overlays, i.e. scale bars.
};


class EdbEDACamera {
	
	private:
	TTimer * eTimer;
	
	int eAnimationMode;
	float eTheta;
	float eAnimationAmplitude;
	EdbEDAOverlay *eOverlay;
	
	public:
	TGLViewer * eViewer;
	
	enum {X,Y,Z,B};
	
	EdbEDACamera():eTimer(NULL),eAnimationMode(0), eTheta(3.14/2), eAnimationAmplitude(0.002), eViewer(NULL){}
//	~EdbEDACamera(){if(NULL==eTimer) {eTimer->Stop(); delete eTimer;}}
	virtual ~EdbEDACamera(){}
	//TGLViewer * GetGLViewer(){ return eViewer;}
	void SetCamera(int projection = Y);
	void Animation();
	void SetAnimationMode();
	void StartAnimation();
	void StopAnimation();
	void SaveAnimation(char *filename = NULL, int n=100, int interval=10);
	void SavePictures();
	void Snapshot(char *filename=NULL);
	TGLViewer * GetGLViewer();
	EdbEDAOverlay *SetOverlay() {eOverlay=new EdbEDAOverlay; eViewer->AddOverlayElement(eOverlay); return eOverlay;}
	EdbEDAOverlay *GetOverlay() {return eOverlay;}
	
	
	ClassDef(EdbEDACamera,0) // part of EdbEDA. Manage camera control and pictures.
};


class EdbEDAIO{
	public:
	int eOutputFileMode;
	enum OutputFileMode{
		kBern,
		kOtherLabs
	};
	
	TCut eCutCP;
	
	EdbEDAIO():eOutputFileMode(kBern), eCutCP("eN1==1&&eN2==1&&eCHI2P<s.eW*0.12-1"){}
	virtual ~EdbEDAIO(){}
	EdbPattern * ReadCouples(int ipl, EdbPattern *pat=NULL);
	EdbPattern * ReadCouplesPID(int pid, EdbPattern *pat=NULL);
	EdbPattern * GetPatternIPL(int pid);
	void WriteFeedbackFile(char *filename = NULL);
	void WriteFeedbackFile2008(EdbVertex *v = NULL, char *filename = NULL);
	void ReadFeedbackFile(char *filename = NULL);
	EdbVertex * ReadFeedbackFile2008(char *filename = NULL);
	int IdMuon(char *filename = "../cs_info.txt");
	void OpenTextEditor(char *filename);
	void WriteFilteredText(char *filename = NULL);
	EdbPVRec * ReadFilteredText(char *filename = NULL);
	
	//void PrintTrackFeedback(EdbTrackP *t=NULL, EdbVertex *v1=NULL, EdbVertex *v2=NULL, EdbSegP *s1=NULL, FILE *io=stdout, bool print_segs=kTRUE);
	
	
	ClassDef(EdbEDAIO,0) // part of EdbEDA. Manage I/O.
};

class EdbEDABrickData{
	// Data management

	private:
	double eBeamTX, eBeamTY;
	public:

	EdbID eIDSF;
	EdbID eIDSB;
	EdbID eID;
	
	EdbDataSet  *eDataSet;
	EdbScanProc *eScanProc;
	EdbScanSet  *eScanSet;
	EdbPVRec    *ePVR;
	
	int eDataType;
	
	int eFirstPlate; // first plate of brick, 1
	int eLastPlate;  // last plate of brick, 57
	
	EdbEDABrickData() : eBeamTX(0), eBeamTY(0.058), eDataSet(0), eScanSet(0), ePVR(0), eDataType(-1), eFirstPlate(1), eLastPlate(57){
	}
	virtual ~EdbEDABrickData(){}
	void SetIDSF(EdbID id) { eIDSF=id;}
	void SetIDSB(EdbID id) { eIDSB=id;}
	void SetID(EdbID id) { eID=id;}
	EdbID GetID() { return eID;}
	
	void SetScanSet(EdbScanSet *ss) { eScanSet = ss;}
	EdbScanSet *GetScanSet() { return eScanSet;}
	
	void SetPVR(EdbPVRec *pvr){ ePVR=pvr;}
	EdbPVRec *GetPVR(){if(ePVR==NULL) printf("EdbEDA::GetPVR() is only to obtain global Z coordinate.\n"); return ePVR;}
	
	void SetBrick(int ibrick){
		eID.eBrick   = ibrick;
		eIDSF.eBrick = ibrick;
		eIDSF.eBrick = ibrick;
	}
	int GetBrick() { return eID.eBrick;}
	
	int  GetDataType(){ return eDataType;}
	void SetDataType(int datatype){ eDataType=datatype;}
	
	int GetPlateUp()  {return GetIPL(0);} // upstream of data volume
	int GetPlateDown(){return GetIPL(eDataSet->N()-1);} // downstream of data volume
	
	int GetFirstPlate(){ return eFirstPlate;} // first plate of brick
	int GetLastPlate(){ return eLastPlate;}  // last plate of brick
	
	
	EdbDataSet * GetDataSet(){ return eDataSet;}
	
	int GetNPL(){
		if      (eScanSet) return eScanSet->eB.Npl();
		else if (eDataSet) return eDataSet->N();
		else return 0;
	}
	
	int GetIPL(int PID){
		if(eScanSet) { // using ScanSet for calibration.
			EdbPlateP *pl = eScanSet->eB.GetPlate(PID);
			return pl->ID(); 
		}
		else if( eDataSet ){
			if(PID>=eDataSet->N()) return PID;
			EdbDataPiece *piece = eDataSet->GetPiece(PID);
			if(NULL==piece) return PID;
			return piece->Plate();
		}
		
		return PID;
	}
	
	int GetIPLZ(float z){
		// return ipl just downstream of given z.
		for(int ipl=GetFirstPlate(); ipl<=GetLastPlate(); ipl++){
			float zz= GetZ(ipl)+0.1; // 0.1 is for the case that given z is exactly at the plate.
			if(zz>=z) return ipl;
		}
		return GetLastPlate();
	}
	
	int GetPID(int ipl){
		if(eScanSet) { // return the ordinal number in eB.
			EdbBrickP *b = &eScanSet->eB;
			for(int i=0;i<b->Npl();i++){
				EdbPlateP *pl = b->GetPlate(i);
				if(pl->ID()==ipl) return i;
			}
			return ipl; // if not correspondence found.
		}
		else if( eDataSet ){
			// Get PID from iPlate.
			// if iPlate is out of volume scan, return -1;
			for(int i=0;i<eDataSet->N();i++){
				EdbDataPiece *piece = eDataSet->GetPiece(i);
				if(NULL==piece) continue;
				if(ipl==piece->Plate()){
					return i;
				}
			}
			// printf("error at GetPID(%d)\n", ipl);
		}
		return ipl;
	}
	
	double GetZ(int ipl){
		// Get Z value for give plate number.
		if(eScanSet) {
			EdbPlateP *pl = eScanSet->GetPlate(ipl);
			if(pl!=NULL) return pl->Z();
			return (ipl-57)*1300.0;
		}
		else if ( eDataSet ){
			for(int i=0;i<eDataSet->N();i++){
				if( GetIPL(i) == ipl ) return GetZPID(i);
			}
		}
		else if( ePVR ){
			for(int i=0;i<ePVR->Npatterns();i++){
				EdbPattern *pat = ePVR->GetPattern(i);
				if(pat->Plate()==ipl) return pat->Z();
			}
		}
	 	return (ipl-57)*1300.0;
	}
	
	double GetZPID(int pid){
		// Get Z from PID
		// if pid is out of range, set pid within the range.

		if(eScanSet) {
			EdbBrickP& b = eScanSet->Brick();
			EdbPlateP *pl = b.GetPlate(pid);
			return pl->Z();
		}
		else if (eDataSet){
		
			if(pid<0) pid=0;
			if(pid>eDataSet->N()-1) pid=eDataSet->N()-1;
			EdbDataPiece *piece = eDataSet->GetPiece(pid);
			EdbLayer* l = piece->GetLayer(0);
			return l->Z();
		}
		
		return GetZ(pid);
	}

	void SetBeamAngle(double tx, double ty) { eBeamTX=tx; eBeamTY=ty;}
	double GetBeamTX() { return eBeamTX;}
	double GetBeamTY() { return eBeamTY;}
	
	EdbAffine2D * GetAffine(int ipl);

	ClassDef(EdbEDABrickData, 0) // part of EdbEDA. Brick Data Management for EDA
};


class EdbEDASelection{
	private:
	TEveSelection *eSel;
	int eDisable;
	EdbVertex *eSelectedVertex;  // current(last) selected vertex
	TObjArray *eSelectedVertices; // selected vertices
	TObjArray *eSelectedTracks;   // selected tracks
	TObjArray *eSelectedSegments; // selected segments
	TObjArray *eSelected;


	public:

	EdbEDASelection(){
		eSelectedVertices = new TObjArray;
		eSelectedTracks   = new TObjArray;
		eSelectedSegments = new TObjArray;
		eSelected         = new TObjArray;
		eSelectedVertex = NULL;
		eSel = NULL;
		eDisable = 0;
	}

	virtual ~EdbEDASelection(){
		if(eSel!=NULL){
			eSel->Disconnect("SelectionAdded(TEveElement*)", this, "Added(TEveElement*)");
			eSel->Disconnect("SelectionRemoved(TEveElement*)", this, "Removed(TEveElement*)");
			eSel->Disconnect("SelectionCleared()", this, "Cleared()");
		}
	}

	// init Selection, connect to the TEveManager::eSelection = gSel
	void InitSelection(TEveSelection* sel){
		if(eSel!=NULL){
			eSel->Disconnect("SelectionAdded(TEveElement*)", this, "Added(TEveElement*)");
			eSel->Disconnect("SelectionRemoved(TEveElement*)", this, "Removed(TEveElement*)");
			eSel->Disconnect("SelectionCleared()", this, "Cleared()");
		}

		eSel = sel; // supposed to be sel = gEDA->GetSelection()
		eSel->Connect("SelectionAdded(TEveElement*)",   "EdbEDASelection", this, "Added(TEveElement*)");
		eSel->Connect("SelectionRemoved(TEveElement*)", "EdbEDASelection", this, "Removed(TEveElement*)");
		eSel->Connect("SelectionCleared()",             "EdbEDASelection", this, "Cleared()");
	}
	
	void ClearSelected(){
		eSelected->Clear();
		eSelectedTracks->Clear();
	}
	
	EdbTrackP * GetTrack( TEveElement *e);

	TObjArray * SetSelected(TObjArray *selected = NULL);
	TObjArray * GetSelected(void) {return eSelected;}
	EdbSegP * GetSelected(int i) {return (EdbSegP *)eSelected->At(i);}

	void SetSelectedTracks(TObjArray *arr) {
		if(arr) {
			*eSelectedTracks = *arr; 
			*eSelected       = *arr;
		}
	}

	
	TEveElement * GetSelectedElement () { TEveElement::List_i it= gEve->GetSelection()->EndChildren(); return *(--it);}
	TObjArray * GetSelectedTracks(void)  { return eSelectedTracks; }
	
	int NSelectedTracks() { return eSelectedTracks->GetEntries();}
	int NSelected()       { return eSelected->      GetEntries();}
	EdbTrackP * GetSelectedTrack(int i = -1){
		if(NSelectedTracks()==0) return NULL;
		if(i==-1) i=NSelectedTracks()-1; 
		return (EdbTrackP *) eSelectedTracks->At(i); 
	}
	EdbTrackP * GetSelectedTrack(EdbSegP *s){
		for( int i=0; i<NSelectedTracks(); i++){
			EdbTrackP *t = GetSelectedTrack(i);
			for(int j=0; j<t->N(); j++) if(s==t->GetSegment(j)) return t;
		}
		return NULL;
	}
	
	EdbSegP   * GetSelectedSegment(int i=-1){if(i==-1) i=NSelected()-1;         return (EdbSegP *)   eSelected->At(i);}

	void SelectAll();
	void SetSelection ( TObjArray * selected = NULL);

	EdbVertex *GetSelectedVertex(void) { return eSelectedVertex;}
	void SetSelectedVertex(EdbVertex *v) { eSelectedVertex = v; }
	void ClearSelectedVertex() {eSelectedVertex = NULL;}

	void Added(TEveElement* el);
	void Removed(TEveElement* el);
	void Cleared();
	
	int Disable(int force = kFALSE) { return force ? eDisable = 1 : eDisable++;}
	int Enable (int force = kFALSE) { return force ? eDisable = 0 : eDisable--;}

	ClassDef(EdbEDASelection,0)  // part of EdbEDA. Manage Selection.
};

/*	// Set PID as PlateNumber.
	EdbDataSet *dset = dproc.GetDataSet();
	
	printf("hoge %d %d\n", __LINE__, dset);
	for(int j=0;j<ali.Ntracks();j++){
		EdbTrackP *t = ali.GetTrack(j);
		
		for(int k=0;k<t->N();k++){
			EdbSegP *s = t->GetSegment(k);
			int pid = s->PID();
			
			EdbDataPiece *piece = dset->GetPiece(pid);
			int ipl = piece->Plate();
			s->SetPID(ipl);
		}
		t->SetPID(t->GetSegmentFirst()->PID());
	}
	printf("hoge %d\n", __LINE__);
*/

//-------------------------------------------------------------------
class EdbEDA :
	public EdbEDACamera,
	public EdbEDAIO,
	public EdbEDABrickData,
	public EdbEDASelection,
	public TNamed {
	
	private:
	
	TObjArray *eDrawObjects;
	TObjArray *eTrackSets;
	
	EdbEDATrackSet     *eSetTracks;
	EdbEDAVertexSet    *eVertexSet;
	EdbEDAExtentionSet *eExt;
	TEveEventManager   *eEvent;

	EdbEDAMainTab      *eMainTab;
	EdbEDATrackSetTab  *eTrackSetTab;
	EdbEDAPlotTab      *ePlotTab;
	EdbEDAVertexTab    *eVertexTab;
	EdbEDAOperationTab    *eOperationTab;
	EdbEDASelection *eSel;
	EdbEDADecaySearchTab *eDSTab;
	
	double eScaleZ;
	bool eJapanese;
	
	public:
	
	EdbEDA(char *filename = NULL, int datatype = 100, TCut rcut = "1", bool auto_run=kTRUE) : 
				TNamed("EDA","Event Display and Analyser"){
		// Constructor for EDA
		//  filename : filename for "LinkDef" or "*.set.root(ScanSet)" or "*.root(linked_track.root format)"
		//  datatype : only for "LinkDef". Data type for EdbDataProc::InitVolume(). put -1 for no InitVolume
		//             if datatype= 100 or 1000. linked tracks will be registred to "TS".
		//  rcut     : Track selection when read.
		//  auto_run : Run automatically.
		OpenFile(filename, datatype, rcut);
		if(auto_run) Run();
	}

	EdbEDA(EdbID IDCalib, char *ProcDirClient=NULL) {
		Init();
		if(ProcDirClient!=NULL) eScanProc->eProcDirClient=ProcDirClient;
		eID=IDCalib;
		eScanSet = eScanProc->ReadScanSet(eID);
	}
	
	EdbEDA(EdbScanSet *ss, char *ProcDirClient=NULL) : TNamed("EDA","Event Display and Analyser"){
		Init();
		if(ProcDirClient!=NULL) eScanProc->eProcDirClient=ProcDirClient;
		eScanSet = ss;
		eID = *((EdbID *)ss->eIDS.At(0));
		eID.ePlate = 0;
	}
	
	EdbEDA(EdbDataProc *dproc) : TNamed("EDA","Event Display and Analyser") {
		Init();
		eDataSet  = dproc->GetDataSet();
		GetTrackSet("TS")->AddTracksPVR(dproc->PVR());
		GetTrackSet("TS")->SetNsegCut(3);
		GetTrackSet("TS")->DoSelection();
	}

	void OpenFile(char *filename = NULL, int datatype = 100, TCut rcut = "1");	

	EdbEDA(EdbPVRec *pvr, bool autorun=1){
		Init();
		ePVR = pvr;
		
		if(pvr->Ntracks()==0){
			printf("*******************************************\n");
			printf(" Worning: No tracks in EdbPVRec object! \n");
			printf("*******************************************\n");
			printf("EDA is made to show \"Tracks\". if you want to show segments in PVR, make tracks from the segments. or you may try EdbEDAUtil::FillTracksFromPatterns(EdbPVRec *)\n");
		}
		
		// if plate number is not set. set plate number as PID.
		int flag_plset=0;
		if(ePVR->eTracks==NULL) ePVR->eTracks = new TObjArray;
		for(int i=0;i<ePVR->Ntracks();i++){
			EdbTrackP *t = ePVR->GetTrack(i);
			EdbSegP *s = t->GetSegmentLast();
			if(s->Plate()!=0) flag_plset++; // if plate number is set.
		}
		
		if(flag_plset==0){ // in case plate number is not set, set PID as plate number
			for(int i=0;i<ePVR->Npatterns();i++){
				EdbPattern *p = ePVR->GetPattern(i);
				if(p==NULL) continue;
				p->SetSegmentsPlate(p->ID());
			}
		}
		
		GetTrackSet("TS")->AddTracksPVR(pvr);
		GetVertexSet()->AddVertices(pvr->eVTX);
		if(autorun) Run();
	}
	
	EdbDataProc * PVRec2DataProc(EdbPVRec *pvr);
	
	void Init(void);
	void Run();	
	EdbScanProc *ScanProc() { return eScanProc;}
	
	EdbEDATrackSet *AddTrackSet(char *name) { EdbEDATrackSet *set = new EdbEDATrackSet(name); eTrackSets->Add(set); return set;}
	
	void AddDrawObject(EdbEDAObject *o) { eDrawObjects->Add(o);}
	void RemoveDrawObject(TObject *o){ eDrawObjects->Remove(o); eDrawObjects->Sort();}
	void ClearDrawObjects(){ eDrawObjects->Clear();}
	int  NDrawObjects(){ return eDrawObjects->GetEntries();}
	EdbEDAObject *GetDrawObject(int i){ return (EdbEDAObject*) eDrawObjects->At(i);}

	TObjArray * GetCurrentTracks(void){
		TEveEventManager *ev = gEve->GetCurrentEvent();
		TObjArray *tracks = new TObjArray;
		for(TEveElement::List_i it = ev->BeginChildren(); it!=ev->EndChildren(); it++){
			TEveElement *e = *it;
			if(IsTrack(e)) tracks->Add((EdbTrackP *)e->GetUserData());
		}
		return tracks;
	}
	
	EdbTrackP * GetTrack( EdbSegP *s){
		for(int i=0;i<NTrackSets();i++){
			EdbTrackP *t = GetTrackSet(i)->GetTrack(s);
			if(t) return t;
		}
		return NULL;
	}
	
	TEveElement * GetEveElement(TObject *t){
		// return TEveElement of the track, vertex. if element is not in the Event, return NULL.
		TEveEventManager *ev = gEve->GetCurrentEvent();
		for(TEveElement::List_i it = ev->BeginChildren(); it!=ev->EndChildren(); it++){
			TEveElement *e = *it;
			if( t == e->GetUserData() ) return e;
		}
		return NULL;
	}
	
	int NTrackSets() { return eTrackSets->GetEntries();}
	EdbEDATrackSet * GetTrackSet(int i){ return (EdbEDATrackSet *) eTrackSets->At(i);}
	EdbEDATrackSet * GetTrackSet(char *name){ return (EdbEDATrackSet *) eTrackSets->FindObject(name);}
	EdbEDATrackSet * GetTrackSet(EdbTrackP *t){ 
		for(int i=0;i<NTrackSets();i++) 
		if ( GetTrackSet(i)->IsMember(t) ) 
		return (EdbEDATrackSet *) eTrackSets->At(i);
		return NULL;
	}
	EdbEDATrackSet * GetTrackSet(EdbID& ID){ 
		for(int i=0;i<NTrackSets();i++) {
			EdbID& ID2 = GetTrackSet(i)->GetID();
			if(ID.eMajor==ID2.eMajor&&ID.eMinor==ID2.eMinor) return GetTrackSet(i);
		}
		return NULL;
	}
	
	EdbEDATrackSet * CreateTrackSet(char *name) {
		EdbEDATrackSet *set=new EdbEDATrackSet(name);
		eTrackSets->Add(set);
		return set;
	}
	TObjArray * GetTrackSets(){ return eTrackSets;}
	
	void RemoveTrackBase(EdbTrackP *t) {for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->RemoveTrackBase(t);}
	void RemoveTrack    (EdbTrackP *t) {for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->RemoveTrack(t);}
	
	void RemoveSelected(){ 
		// remove selected tracks or vertices.
		// to remove vertex, select vertex but not select tracks.
		int nselected = NSelected();
		for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->RemoveTracks(GetSelectedTracks());
		if(nselected==0) eVertexSet->RemoveVertex(GetSelectedVertex());
	}

	void SetOnlySelected(){
		for(int i=0;i<NTrackSets();i++) {
			GetTrackSet(i)->ClearTracks();
			GetTrackSet(i)->SetTracks(GetSelectedTracks());
		}
		eVertexSet->ClearVertices();
		eVertexSet->SetVertex(GetSelectedVertex());
	}
	
	EdbEDAVertexSet * GetVertexSet() { return eVertexSet;}
	void AddVertex(EdbVertex *v) { eVertexSet->AddVertex(v);}
	TObjArray *GetVertices() {return eVertexSet->GetVertices();}
	void ClearVertices() { eVertexSet->ClearVertices();}
	void DrawVertices(int redraw = 0) { eVertexSet->Draw(redraw);}
	
	void SetBrick(int ibrick) { 
		// Set brick ID. it sets also for all TrackSets.
		EdbEDABrickData::SetBrick(ibrick);
		for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->GetID().eBrick=ibrick;
		if(gEve) gEve->GetMainWindow()->SetWindowName(
			Form("OPERA Event Display and Analyser. Brick %d", EdbEDABrickData::GetBrick()));		
	}
	
	void SetDrawTrackID(bool draw)    { for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->SetDrawTrackIDG(draw);}
	void SetDrawTrackAngle(bool draw) { for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->SetDrawTrackAngleG(draw);}
	void SetDrawAreaSet(bool draw)    { for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->GetAreaSet()->SetDrawG(draw);}

	void Draw(int redraw = kFALSE);
	
	void Redraw() { if(gEve)Draw(kTRUE);}
	void UpdateScene() { if(gEve) gEve->GetDefaultGLViewer()->UpdateScene();}
	
	void Reset(){ 
		for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->Clear();
		ClearDrawObjects();
		ClearVertices();
	}
	
	void StorePrevious() {
		for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->StorePrevious();
		GetVertexSet()->StorePrevious();
	}
	
	void RestorePrevious() {
		for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->RestorePrevious();
		GetVertexSet()->RestorePrevious();
	}
	
	EdbEDAExtentionSet * GetExtentionSet(){ return eExt;}
	
	void SetColorMode(int mode){
		// Set color mode,
		//	enum EdbEDAUtil::EDACOLOR{
		//		kCOLOR_BY_PLATE,
		//		kCOLOR_BY_PH,
		//		kCOLOR_BY_ID,
		//		kCOLOR_BY_PARTICLE, // to be done
		//		kBLACKWHITE
		//	};

		printf("color mode %d\n", mode);
		if(mode==kBLACKWHITE) eViewer->SetClearColor(kWhite);
		else eViewer->SetClearColor(kBlack);
		for(int i=0;i<NTrackSets();i++)   GetTrackSet(i)  ->SetColorMode(mode);
		GetVertexSet()->SetColorMode(mode);
		for(int i=0;i<NDrawObjects();i++) GetDrawObject(i)->SetColorMode(mode);
		Redraw();
	}
	void SetLayerLength(double layer_length) {
		// Set LayerLength for all TrackSet.
		for(int i=0;i<NTrackSets();i++){ GetTrackSet(i)->SetLayerLength(layer_length);}
	}
	
	EdbEDAMainTab     *GetMainTab()     {return eMainTab;}
	EdbEDATrackSetTab *GetTrackSetTab() {return eTrackSetTab;}
	EdbEDAPlotTab     *GetPlotTab()     {return ePlotTab;}
	EdbEDAVertexTab   *GetVertexTab()   {return eVertexTab;}
	EdbEDAOperationTab   *GetOperationTab()   {return eOperationTab;}
	EdbEDADecaySearchTab *GetDSTab()    {return eDSTab;}
	EdbTrackP * CheckScanback(EdbTrackP *t) {return GetTrackSet("SB")->FindTrackLast(t->GetSegmentLast());}
	
	double GetScaleZ() { return eScaleZ;}
	void SetScaleZ(double scale) { eScaleZ=scale;}
	bool Japanese(){return eJapanese;}
	void SetJapanese(bool b=kTRUE) {eJapanese = b;}
	
	ClassDef(EdbEDA,0); // Base of Display
};

R__EXTERN EdbEDA* gEDA;


#endif // __EDA_H__
 EdbEDA.h:1
 EdbEDA.h:2
 EdbEDA.h:3
 EdbEDA.h:4
 EdbEDA.h:5
 EdbEDA.h:6
 EdbEDA.h:7
 EdbEDA.h:8
 EdbEDA.h:9
 EdbEDA.h:10
 EdbEDA.h:11
 EdbEDA.h:12
 EdbEDA.h:13
 EdbEDA.h:14
 EdbEDA.h:15
 EdbEDA.h:16
 EdbEDA.h:17
 EdbEDA.h:18
 EdbEDA.h:19
 EdbEDA.h:20
 EdbEDA.h:21
 EdbEDA.h:22
 EdbEDA.h:23
 EdbEDA.h:24
 EdbEDA.h:25
 EdbEDA.h:26
 EdbEDA.h:27
 EdbEDA.h:28
 EdbEDA.h:29
 EdbEDA.h:30
 EdbEDA.h:31
 EdbEDA.h:32
 EdbEDA.h:33
 EdbEDA.h:34
 EdbEDA.h:35
 EdbEDA.h:36
 EdbEDA.h:37
 EdbEDA.h:38
 EdbEDA.h:39
 EdbEDA.h:40
 EdbEDA.h:41
 EdbEDA.h:42
 EdbEDA.h:43
 EdbEDA.h:44
 EdbEDA.h:45
 EdbEDA.h:46
 EdbEDA.h:47
 EdbEDA.h:48
 EdbEDA.h:49
 EdbEDA.h:50
 EdbEDA.h:51
 EdbEDA.h:52
 EdbEDA.h:53
 EdbEDA.h:54
 EdbEDA.h:55
 EdbEDA.h:56
 EdbEDA.h:57
 EdbEDA.h:58
 EdbEDA.h:59
 EdbEDA.h:60
 EdbEDA.h:61
 EdbEDA.h:62
 EdbEDA.h:63
 EdbEDA.h:64
 EdbEDA.h:65
 EdbEDA.h:66
 EdbEDA.h:67
 EdbEDA.h:68
 EdbEDA.h:69
 EdbEDA.h:70
 EdbEDA.h:71
 EdbEDA.h:72
 EdbEDA.h:73
 EdbEDA.h:74
 EdbEDA.h:75
 EdbEDA.h:76
 EdbEDA.h:77
 EdbEDA.h:78
 EdbEDA.h:79
 EdbEDA.h:80
 EdbEDA.h:81
 EdbEDA.h:82
 EdbEDA.h:83
 EdbEDA.h:84
 EdbEDA.h:85
 EdbEDA.h:86
 EdbEDA.h:87
 EdbEDA.h:88
 EdbEDA.h:89
 EdbEDA.h:90
 EdbEDA.h:91
 EdbEDA.h:92
 EdbEDA.h:93
 EdbEDA.h:94
 EdbEDA.h:95
 EdbEDA.h:96
 EdbEDA.h:97
 EdbEDA.h:98
 EdbEDA.h:99
 EdbEDA.h:100
 EdbEDA.h:101
 EdbEDA.h:102
 EdbEDA.h:103
 EdbEDA.h:104
 EdbEDA.h:105
 EdbEDA.h:106
 EdbEDA.h:107
 EdbEDA.h:108
 EdbEDA.h:109
 EdbEDA.h:110
 EdbEDA.h:111
 EdbEDA.h:112
 EdbEDA.h:113
 EdbEDA.h:114
 EdbEDA.h:115
 EdbEDA.h:116
 EdbEDA.h:117
 EdbEDA.h:118
 EdbEDA.h:119
 EdbEDA.h:120
 EdbEDA.h:121
 EdbEDA.h:122
 EdbEDA.h:123
 EdbEDA.h:124
 EdbEDA.h:125
 EdbEDA.h:126
 EdbEDA.h:127
 EdbEDA.h:128
 EdbEDA.h:129
 EdbEDA.h:130
 EdbEDA.h:131
 EdbEDA.h:132
 EdbEDA.h:133
 EdbEDA.h:134
 EdbEDA.h:135
 EdbEDA.h:136
 EdbEDA.h:137
 EdbEDA.h:138
 EdbEDA.h:139
 EdbEDA.h:140
 EdbEDA.h:141
 EdbEDA.h:142
 EdbEDA.h:143
 EdbEDA.h:144
 EdbEDA.h:145
 EdbEDA.h:146
 EdbEDA.h:147
 EdbEDA.h:148
 EdbEDA.h:149
 EdbEDA.h:150
 EdbEDA.h:151
 EdbEDA.h:152
 EdbEDA.h:153
 EdbEDA.h:154
 EdbEDA.h:155
 EdbEDA.h:156
 EdbEDA.h:157
 EdbEDA.h:158
 EdbEDA.h:159
 EdbEDA.h:160
 EdbEDA.h:161
 EdbEDA.h:162
 EdbEDA.h:163
 EdbEDA.h:164
 EdbEDA.h:165
 EdbEDA.h:166
 EdbEDA.h:167
 EdbEDA.h:168
 EdbEDA.h:169
 EdbEDA.h:170
 EdbEDA.h:171
 EdbEDA.h:172
 EdbEDA.h:173
 EdbEDA.h:174
 EdbEDA.h:175
 EdbEDA.h:176
 EdbEDA.h:177
 EdbEDA.h:178
 EdbEDA.h:179
 EdbEDA.h:180
 EdbEDA.h:181
 EdbEDA.h:182
 EdbEDA.h:183
 EdbEDA.h:184
 EdbEDA.h:185
 EdbEDA.h:186
 EdbEDA.h:187
 EdbEDA.h:188
 EdbEDA.h:189
 EdbEDA.h:190
 EdbEDA.h:191
 EdbEDA.h:192
 EdbEDA.h:193
 EdbEDA.h:194
 EdbEDA.h:195
 EdbEDA.h:196
 EdbEDA.h:197
 EdbEDA.h:198
 EdbEDA.h:199
 EdbEDA.h:200
 EdbEDA.h:201
 EdbEDA.h:202
 EdbEDA.h:203
 EdbEDA.h:204
 EdbEDA.h:205
 EdbEDA.h:206
 EdbEDA.h:207
 EdbEDA.h:208
 EdbEDA.h:209
 EdbEDA.h:210
 EdbEDA.h:211
 EdbEDA.h:212
 EdbEDA.h:213
 EdbEDA.h:214
 EdbEDA.h:215
 EdbEDA.h:216
 EdbEDA.h:217
 EdbEDA.h:218
 EdbEDA.h:219
 EdbEDA.h:220
 EdbEDA.h:221
 EdbEDA.h:222
 EdbEDA.h:223
 EdbEDA.h:224
 EdbEDA.h:225
 EdbEDA.h:226
 EdbEDA.h:227
 EdbEDA.h:228
 EdbEDA.h:229
 EdbEDA.h:230
 EdbEDA.h:231
 EdbEDA.h:232
 EdbEDA.h:233
 EdbEDA.h:234
 EdbEDA.h:235
 EdbEDA.h:236
 EdbEDA.h:237
 EdbEDA.h:238
 EdbEDA.h:239
 EdbEDA.h:240
 EdbEDA.h:241
 EdbEDA.h:242
 EdbEDA.h:243
 EdbEDA.h:244
 EdbEDA.h:245
 EdbEDA.h:246
 EdbEDA.h:247
 EdbEDA.h:248
 EdbEDA.h:249
 EdbEDA.h:250
 EdbEDA.h:251
 EdbEDA.h:252
 EdbEDA.h:253
 EdbEDA.h:254
 EdbEDA.h:255
 EdbEDA.h:256
 EdbEDA.h:257
 EdbEDA.h:258
 EdbEDA.h:259
 EdbEDA.h:260
 EdbEDA.h:261
 EdbEDA.h:262
 EdbEDA.h:263
 EdbEDA.h:264
 EdbEDA.h:265
 EdbEDA.h:266
 EdbEDA.h:267
 EdbEDA.h:268
 EdbEDA.h:269
 EdbEDA.h:270
 EdbEDA.h:271
 EdbEDA.h:272
 EdbEDA.h:273
 EdbEDA.h:274
 EdbEDA.h:275
 EdbEDA.h:276
 EdbEDA.h:277
 EdbEDA.h:278
 EdbEDA.h:279
 EdbEDA.h:280
 EdbEDA.h:281
 EdbEDA.h:282
 EdbEDA.h:283
 EdbEDA.h:284
 EdbEDA.h:285
 EdbEDA.h:286
 EdbEDA.h:287
 EdbEDA.h:288
 EdbEDA.h:289
 EdbEDA.h:290
 EdbEDA.h:291
 EdbEDA.h:292
 EdbEDA.h:293
 EdbEDA.h:294
 EdbEDA.h:295
 EdbEDA.h:296
 EdbEDA.h:297
 EdbEDA.h:298
 EdbEDA.h:299
 EdbEDA.h:300
 EdbEDA.h:301
 EdbEDA.h:302
 EdbEDA.h:303
 EdbEDA.h:304
 EdbEDA.h:305
 EdbEDA.h:306
 EdbEDA.h:307
 EdbEDA.h:308
 EdbEDA.h:309
 EdbEDA.h:310
 EdbEDA.h:311
 EdbEDA.h:312
 EdbEDA.h:313
 EdbEDA.h:314
 EdbEDA.h:315
 EdbEDA.h:316
 EdbEDA.h:317
 EdbEDA.h:318
 EdbEDA.h:319
 EdbEDA.h:320
 EdbEDA.h:321
 EdbEDA.h:322
 EdbEDA.h:323
 EdbEDA.h:324
 EdbEDA.h:325
 EdbEDA.h:326
 EdbEDA.h:327
 EdbEDA.h:328
 EdbEDA.h:329
 EdbEDA.h:330
 EdbEDA.h:331
 EdbEDA.h:332
 EdbEDA.h:333
 EdbEDA.h:334
 EdbEDA.h:335
 EdbEDA.h:336
 EdbEDA.h:337
 EdbEDA.h:338
 EdbEDA.h:339
 EdbEDA.h:340
 EdbEDA.h:341
 EdbEDA.h:342
 EdbEDA.h:343
 EdbEDA.h:344
 EdbEDA.h:345
 EdbEDA.h:346
 EdbEDA.h:347
 EdbEDA.h:348
 EdbEDA.h:349
 EdbEDA.h:350
 EdbEDA.h:351
 EdbEDA.h:352
 EdbEDA.h:353
 EdbEDA.h:354
 EdbEDA.h:355
 EdbEDA.h:356
 EdbEDA.h:357
 EdbEDA.h:358
 EdbEDA.h:359
 EdbEDA.h:360
 EdbEDA.h:361
 EdbEDA.h:362
 EdbEDA.h:363
 EdbEDA.h:364
 EdbEDA.h:365
 EdbEDA.h:366
 EdbEDA.h:367
 EdbEDA.h:368
 EdbEDA.h:369
 EdbEDA.h:370
 EdbEDA.h:371
 EdbEDA.h:372
 EdbEDA.h:373
 EdbEDA.h:374
 EdbEDA.h:375
 EdbEDA.h:376
 EdbEDA.h:377
 EdbEDA.h:378
 EdbEDA.h:379
 EdbEDA.h:380
 EdbEDA.h:381
 EdbEDA.h:382
 EdbEDA.h:383
 EdbEDA.h:384
 EdbEDA.h:385
 EdbEDA.h:386
 EdbEDA.h:387
 EdbEDA.h:388
 EdbEDA.h:389
 EdbEDA.h:390
 EdbEDA.h:391
 EdbEDA.h:392
 EdbEDA.h:393
 EdbEDA.h:394
 EdbEDA.h:395
 EdbEDA.h:396
 EdbEDA.h:397
 EdbEDA.h:398
 EdbEDA.h:399
 EdbEDA.h:400
 EdbEDA.h:401
 EdbEDA.h:402
 EdbEDA.h:403
 EdbEDA.h:404
 EdbEDA.h:405
 EdbEDA.h:406
 EdbEDA.h:407
 EdbEDA.h:408
 EdbEDA.h:409
 EdbEDA.h:410
 EdbEDA.h:411
 EdbEDA.h:412
 EdbEDA.h:413
 EdbEDA.h:414
 EdbEDA.h:415
 EdbEDA.h:416
 EdbEDA.h:417
 EdbEDA.h:418
 EdbEDA.h:419
 EdbEDA.h:420
 EdbEDA.h:421
 EdbEDA.h:422
 EdbEDA.h:423
 EdbEDA.h:424
 EdbEDA.h:425
 EdbEDA.h:426
 EdbEDA.h:427
 EdbEDA.h:428
 EdbEDA.h:429
 EdbEDA.h:430
 EdbEDA.h:431
 EdbEDA.h:432
 EdbEDA.h:433
 EdbEDA.h:434
 EdbEDA.h:435
 EdbEDA.h:436
 EdbEDA.h:437
 EdbEDA.h:438
 EdbEDA.h:439
 EdbEDA.h:440
 EdbEDA.h:441
 EdbEDA.h:442
 EdbEDA.h:443
 EdbEDA.h:444
 EdbEDA.h:445
 EdbEDA.h:446
 EdbEDA.h:447
 EdbEDA.h:448
 EdbEDA.h:449
 EdbEDA.h:450
 EdbEDA.h:451
 EdbEDA.h:452
 EdbEDA.h:453
 EdbEDA.h:454
 EdbEDA.h:455
 EdbEDA.h:456
 EdbEDA.h:457
 EdbEDA.h:458
 EdbEDA.h:459
 EdbEDA.h:460
 EdbEDA.h:461
 EdbEDA.h:462
 EdbEDA.h:463
 EdbEDA.h:464
 EdbEDA.h:465
 EdbEDA.h:466
 EdbEDA.h:467
 EdbEDA.h:468
 EdbEDA.h:469
 EdbEDA.h:470
 EdbEDA.h:471
 EdbEDA.h:472
 EdbEDA.h:473
 EdbEDA.h:474
 EdbEDA.h:475
 EdbEDA.h:476
 EdbEDA.h:477
 EdbEDA.h:478
 EdbEDA.h:479
 EdbEDA.h:480
 EdbEDA.h:481
 EdbEDA.h:482
 EdbEDA.h:483
 EdbEDA.h:484
 EdbEDA.h:485
 EdbEDA.h:486
 EdbEDA.h:487
 EdbEDA.h:488
 EdbEDA.h:489
 EdbEDA.h:490
 EdbEDA.h:491
 EdbEDA.h:492
 EdbEDA.h:493
 EdbEDA.h:494
 EdbEDA.h:495
 EdbEDA.h:496
 EdbEDA.h:497
 EdbEDA.h:498
 EdbEDA.h:499
 EdbEDA.h:500
 EdbEDA.h:501
 EdbEDA.h:502
 EdbEDA.h:503
 EdbEDA.h:504
 EdbEDA.h:505
 EdbEDA.h:506
 EdbEDA.h:507
 EdbEDA.h:508
 EdbEDA.h:509
 EdbEDA.h:510
 EdbEDA.h:511
 EdbEDA.h:512
 EdbEDA.h:513
 EdbEDA.h:514
 EdbEDA.h:515
 EdbEDA.h:516
 EdbEDA.h:517
 EdbEDA.h:518
 EdbEDA.h:519
 EdbEDA.h:520
 EdbEDA.h:521
 EdbEDA.h:522
 EdbEDA.h:523
 EdbEDA.h:524
 EdbEDA.h:525
 EdbEDA.h:526
 EdbEDA.h:527
 EdbEDA.h:528
 EdbEDA.h:529
 EdbEDA.h:530
 EdbEDA.h:531
 EdbEDA.h:532
 EdbEDA.h:533
 EdbEDA.h:534
 EdbEDA.h:535
 EdbEDA.h:536
 EdbEDA.h:537
 EdbEDA.h:538
 EdbEDA.h:539
 EdbEDA.h:540
 EdbEDA.h:541
 EdbEDA.h:542
 EdbEDA.h:543
 EdbEDA.h:544
 EdbEDA.h:545
 EdbEDA.h:546
 EdbEDA.h:547
 EdbEDA.h:548
 EdbEDA.h:549
 EdbEDA.h:550
 EdbEDA.h:551
 EdbEDA.h:552
 EdbEDA.h:553
 EdbEDA.h:554
 EdbEDA.h:555
 EdbEDA.h:556
 EdbEDA.h:557
 EdbEDA.h:558
 EdbEDA.h:559
 EdbEDA.h:560
 EdbEDA.h:561
 EdbEDA.h:562
 EdbEDA.h:563
 EdbEDA.h:564
 EdbEDA.h:565
 EdbEDA.h:566
 EdbEDA.h:567
 EdbEDA.h:568
 EdbEDA.h:569
 EdbEDA.h:570
 EdbEDA.h:571
 EdbEDA.h:572
 EdbEDA.h:573
 EdbEDA.h:574
 EdbEDA.h:575
 EdbEDA.h:576
 EdbEDA.h:577
 EdbEDA.h:578
 EdbEDA.h:579
 EdbEDA.h:580
 EdbEDA.h:581
 EdbEDA.h:582
 EdbEDA.h:583
 EdbEDA.h:584
 EdbEDA.h:585
 EdbEDA.h:586
 EdbEDA.h:587
 EdbEDA.h:588
 EdbEDA.h:589
 EdbEDA.h:590
 EdbEDA.h:591
 EdbEDA.h:592
 EdbEDA.h:593
 EdbEDA.h:594
 EdbEDA.h:595
 EdbEDA.h:596
 EdbEDA.h:597
 EdbEDA.h:598
 EdbEDA.h:599
 EdbEDA.h:600
 EdbEDA.h:601
 EdbEDA.h:602
 EdbEDA.h:603
 EdbEDA.h:604
 EdbEDA.h:605
 EdbEDA.h:606
 EdbEDA.h:607
 EdbEDA.h:608
 EdbEDA.h:609
 EdbEDA.h:610
 EdbEDA.h:611
 EdbEDA.h:612
 EdbEDA.h:613
 EdbEDA.h:614
 EdbEDA.h:615
 EdbEDA.h:616
 EdbEDA.h:617
 EdbEDA.h:618
 EdbEDA.h:619
 EdbEDA.h:620
 EdbEDA.h:621
 EdbEDA.h:622
 EdbEDA.h:623
 EdbEDA.h:624
 EdbEDA.h:625
 EdbEDA.h:626
 EdbEDA.h:627
 EdbEDA.h:628
 EdbEDA.h:629
 EdbEDA.h:630
 EdbEDA.h:631
 EdbEDA.h:632
 EdbEDA.h:633
 EdbEDA.h:634
 EdbEDA.h:635
 EdbEDA.h:636
 EdbEDA.h:637
 EdbEDA.h:638
 EdbEDA.h:639
 EdbEDA.h:640
 EdbEDA.h:641
 EdbEDA.h:642
 EdbEDA.h:643
 EdbEDA.h:644
 EdbEDA.h:645
 EdbEDA.h:646
 EdbEDA.h:647
 EdbEDA.h:648
 EdbEDA.h:649
 EdbEDA.h:650
 EdbEDA.h:651
 EdbEDA.h:652
 EdbEDA.h:653
 EdbEDA.h:654
 EdbEDA.h:655
 EdbEDA.h:656
 EdbEDA.h:657
 EdbEDA.h:658
 EdbEDA.h:659
 EdbEDA.h:660
 EdbEDA.h:661
 EdbEDA.h:662
 EdbEDA.h:663
 EdbEDA.h:664
 EdbEDA.h:665
 EdbEDA.h:666
 EdbEDA.h:667
 EdbEDA.h:668
 EdbEDA.h:669
 EdbEDA.h:670
 EdbEDA.h:671
 EdbEDA.h:672
 EdbEDA.h:673
 EdbEDA.h:674
 EdbEDA.h:675
 EdbEDA.h:676
 EdbEDA.h:677
 EdbEDA.h:678
 EdbEDA.h:679
 EdbEDA.h:680
 EdbEDA.h:681
 EdbEDA.h:682
 EdbEDA.h:683
 EdbEDA.h:684
 EdbEDA.h:685
 EdbEDA.h:686
 EdbEDA.h:687
 EdbEDA.h:688
 EdbEDA.h:689
 EdbEDA.h:690
 EdbEDA.h:691
 EdbEDA.h:692
 EdbEDA.h:693
 EdbEDA.h:694
 EdbEDA.h:695
 EdbEDA.h:696
 EdbEDA.h:697
 EdbEDA.h:698
 EdbEDA.h:699
 EdbEDA.h:700
 EdbEDA.h:701
 EdbEDA.h:702
 EdbEDA.h:703
 EdbEDA.h:704
 EdbEDA.h:705
 EdbEDA.h:706
 EdbEDA.h:707
 EdbEDA.h:708
 EdbEDA.h:709
 EdbEDA.h:710
 EdbEDA.h:711
 EdbEDA.h:712
 EdbEDA.h:713
 EdbEDA.h:714
 EdbEDA.h:715
 EdbEDA.h:716
 EdbEDA.h:717
 EdbEDA.h:718
 EdbEDA.h:719
 EdbEDA.h:720
 EdbEDA.h:721
 EdbEDA.h:722
 EdbEDA.h:723
 EdbEDA.h:724
 EdbEDA.h:725
 EdbEDA.h:726
 EdbEDA.h:727
 EdbEDA.h:728
 EdbEDA.h:729
 EdbEDA.h:730
 EdbEDA.h:731
 EdbEDA.h:732
 EdbEDA.h:733
 EdbEDA.h:734
 EdbEDA.h:735
 EdbEDA.h:736
 EdbEDA.h:737
 EdbEDA.h:738
 EdbEDA.h:739
 EdbEDA.h:740
 EdbEDA.h:741
 EdbEDA.h:742
 EdbEDA.h:743

» Last changed: 2017-07-03 09:59 » Last generated: 2017-07-03 09:59
This page has been automatically generated. For comments or suggestions regarding the documentation or ROOT in general please send a mail to ROOT support.