#include"EdbEDA.h"

EdbEDA *gEDA=0;

ClassImp(EdbEDAIO)
ClassImp(EdbEDACamera)
ClassImp(EdbEDABrickData)
ClassImp(EdbEDASelection)
ClassImp(EdbEDA)
ClassImp(EdbEDAText)
ClassImp(EdbEDALine)
ClassImp(EdbEDAOverlay)


#include<TEveViewer.h>

using namespace EdbEDAUtil;




TGLViewer * EdbEDACamera::GetGLViewer(){
	// Get GLViewer which is infront.
	// when you make a new GLViewer, need to define same name for GLViewer and Tab.
	
	
	TGTabElement *tab=gEve->GetBrowser()->GetTabRight()->GetCurrentTab();
	
	TEveViewerList *viewers = gEve->GetViewers();

	TGLViewer *v = NULL;
	for(TEveElement::List_i it = viewers->BeginChildren(); it!=viewers->EndChildren(); it++){
		printf("%s\n", ((TEveViewer *)(*it))->GetName());
		if( strcmp(((TEveViewer *)(*it))->GetName(), tab->GetName())==0 ) {
			// compare the name of viewer and name of tab.
			//printf("GLViewer %s\n", tab->GetName());
			v = ((TEveViewer *)(*it))->GetGLViewer();
			printf("GLViewer in current Tab = %s\n", ((TEveViewer *)(*it))->GetName());
			break;
		}
	}
	if(v==NULL) {
		printf("No viewer found in current Tab. Set Default GLViewer.\n");
		v=gEve->GetDefaultGLViewer();
	}
	eViewer = v;
	
	return v;
}
void EdbEDACamera::SetCamera(int projection){
	switch(projection){
		case X: // X projection
			{
				eViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
				TGLOrthoCamera& cam = (TGLOrthoCamera &)(eViewer->CurrentCamera());
				cam.Reset();
				cam.RotateRad( -3.14159*0.5, 0.0);
				cam.SetEnableRotate(1);
				printf("set camera : X projection\n");
			}
			break;
		case Y: // Y projection
			{	
				eViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
				TGLOrthoCamera& cam = (TGLOrthoCamera &)eViewer->CurrentCamera();
				cam.Reset();
				cam.SetEnableRotate(1);
				printf("set camera : Y projection\n");
			}
			break;
		case Z: // Z
			{
				eViewer->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
				TGLOrthoCamera& cam = (TGLOrthoCamera &)eViewer->CurrentCamera();
				cam.Reset();
				cam.SetEnableRotate(1);
				printf("set camera : Y projection\n");
			}
			break;
		case B: // Beam view
			{
				eViewer->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
				TGLOrthoCamera& cam = (TGLOrthoCamera &)eViewer->CurrentCamera();
				cam.Reset();
				double tx = atan(gEDA->GetBeamTY()/gEDA->GetScaleZ());
				double ty = atan(gEDA->GetBeamTX()/gEDA->GetScaleZ());
				cam.RotateRad(-tx,-ty);
				cam.SetEnableRotate(1);
				printf("set camera : Beam projection. 0.058 rad tilted\n");
			}
			break;
		default : break;
	}
	(eViewer)->DoDraw(); // ???? doesn't work...
}

void EdbEDACamera::SetAnimationMode(){
	
	eAnimationMode = InputNumberInteger("Animation Mode? 0: rotation(default), 1: hand move like, 2: left-right.", eAnimationMode);
	eAnimationAmplitude = InputNumberReal("Amplitude?", eAnimationAmplitude);
	
}
void EdbEDACamera::Animation(){
	
	
	TGLViewer *v=eViewer;
	TGLCamera& cam=v->CurrentCamera();
	if(eAnimationMode==0) cam.RotateRad(0.0,0.02);
	if(eAnimationMode==1) cam.RotateRad( eAnimationAmplitude*sin(eTheta), eAnimationAmplitude*cos(eTheta+=0.04));
	if(eAnimationMode==2) cam.RotateRad(0, eAnimationAmplitude*sin(eTheta+=0.04));
	v->DoDraw();
}

void EdbEDACamera::StartAnimation(){
	if(NULL==eTimer){
		eTimer = new TTimer;
		eTimer->Connect("Timeout()","EdbEDA",this,"Animation()");
		eViewer->Connect("Destroyed()", "EdbEDA", this, "StopAnimation()");
	}
	eTimer->Start(50,0);
}
void EdbEDACamera::StopAnimation() { eTimer->TurnOff();}


void EdbEDACamera::SaveAnimation(char *filename, int n, int interval){
	printf("gEDA->SaveAnimation(char *filename, int nview=100, int interval=10)\n");
	if(filename==NULL){ // File dialog
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "Animated Gif", "*.gif", 0,0};
		fi->fFileTypes = filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDSave, fi);
		filename = fi->fFilename;
	}
	if(filename==NULL) return;

	n=InputNumberInteger("Input N frame per rotation.", n);
	
	TGLViewer *v=eViewer;
	TGLCamera& c=v->CurrentCamera();
	v->SavePicture(Form("%s",filename));
	for(int i=0;i<n;i++){
		printf("save animation %d/%d\r",i,n);
		if(eAnimationMode==0) c.RotateRad(0.0,2*3.14/n);
		if(eAnimationMode==1) c.RotateRad(eAnimationAmplitude*sin(eTheta+=2*3.14/n), 
		                                  eAnimationAmplitude*cos(eTheta));
		if(eAnimationMode==2) c.RotateRad(0,
		                                  eAnimationAmplitude*cos(eTheta+=2*3.14/n));
		v->DoDraw();
		v->SavePicture(Form("%s+%d",filename,interval));
	
	}
	v->SavePicture(Form("%s++",filename));
	printf("save animation done!!.\n");
}

void EdbEDACamera::SavePictures(){
	printf("Save Pictures : x.gif, y.gif, z.gif, b.gif\n");
	FILE *fp = fopen("x.gif","rb");
	if(fp) {
		fclose(fp);
		printf("file already exist. delete and re-try.\n");
		return;
	}
	
	TGLViewer *v = eViewer;
	gEDA->SetCamera(EdbEDACamera::X);
	v->SavePicture("x.gif");
	gEDA->SetCamera(EdbEDACamera::Y);
	v->SavePicture("y.gif");
	gEDA->SetCamera(EdbEDACamera::Z);
	v->SavePicture("z.gif");
	gEDA->SetCamera(EdbEDACamera::B);
	v->SavePicture("b.gif");
}




void EdbEDACamera::Snapshot(char *filename){
	if(filename==NULL) {
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = {	"GIF", "*.gif", "JPG","*.jpg", "PNG","*.png", 0,0};
		fi->fFileTypes=filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDSave, fi);
		printf("Save current view : %s\n", fi->fFilename);
		filename=fi->fFilename;
	}
	if(filename==NULL) return;
	TGLViewer *v = eViewer;
	TString name=filename;
	v->SavePicture(filename);
}


EdbAffine2D * EdbEDABrickData::GetAffine(int ipl){
	int i;
	
	if(eScanSet){
		EdbPlateP *p = eScanSet->GetPlate(ipl);
		if(p!=NULL) return p->GetAffineXY();
		else return NULL;
	}
	else if (eDataSet){
		EdbDataSet *dset = eDataSet;
		for(i=0;i<dset->N();i++){
			EdbDataPiece *piece = dset->GetPiece(i);
			if(piece->Plate()==ipl){
				EdbLayer *l = piece->GetLayer(0);
				return l->GetAffineXY();
			}
		}
	}
	return NULL;
}

EdbPattern * EdbEDAIO::ReadCouples(int ipl, EdbPattern *pat0){
	printf("ReadCouples DS ipl=%d\n", ipl);
	return ReadCouplesPID(gEDA->GetPID(ipl), pat0);
}



EdbPattern * EdbEDAIO::ReadCouplesPID(int pid, EdbPattern *pat0){
			
	EdbDataSet *dset = gEDA->GetDataSet();
	EdbDataPiece *piece = dset->GetPiece(pid);
	EdbLayer *l = piece->GetLayer(0);
	EdbPattern *pat = new EdbPattern( 0.,0., l->Z());
	pat->SetPID(pid);
	piece->InitCouplesTree("READ");
	piece->GetCPData_new(pat);
	piece->CloseCPData();
	pat->SetSegmentsZ();
	pat->SetID(pid);
	pat->SetSegmentsPID();
	pat->Transform(    l->GetAffineXY()   );
	pat->TransformA(   l->GetAffineTXTY() );
	pat->TransformShr( l->Shr()  );
	
	if(pat0==0) return pat;
	
	EdbScanCond cond;
	
	// copy segments.
	for(int i=0;i<pat->N();i++){
		EdbSegP *s = pat->GetSegment(i);
		if(s->Plate()==0) s->SetPlate(gEDA->GetIPL(pid));
	
		// fill COV for vertexing
		s->SetErrors();
		cond.FillErrorsCov(s->TX(), s->TY(), s->COV());
	
		pat0->AddSegment(*s);
	}
	delete pat;
	return pat0;
}


EdbPattern * EdbEDAIO::GetPatternIPL(int ipl){
	// return Pattern with Couples.
	// if Couples are not loaded yet, load it in PVRec for TS.
	
	EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
	EdbPVRec *pvr = set->GetPVRec();

	EdbPattern *pat=NULL;
	
	for(int i=0;i<pvr->Npatterns();i++){
		EdbPattern *p = pvr->GetPattern(i);
		if(p==NULL) continue;
		if(p->N()>0){
			if(p->GetSegment(0)->Plate()==ipl) {
				pat=p;
				break;
			}
		}
	}
	
	if(pat==NULL) {
		printf("No pattern in TS, pl=%d.\n", ipl);
		return NULL;
	}
	
	// Check if couples data were already loaded.
	if( IsIncludeCouples(pat)==0 ) {
		
		if     (set->GetDataSet()!=NULL) gEDA->ReadCouples(ipl,pat);
		else if(set->GetScanSet()!=NULL) {
			printf("ReadCouples SS ipl=%d\n", ipl);
			EdbID id = *(EdbID *)set->GetScanSet()->eIDS.At(0);
			id.ePlate=ipl;
			id.Print();
			float z = pat->Z();
			gEDA->ScanProc()->ReadPatCP(*pat,id,eCutCP);
			pat->SetZ(z);
			pat->SetSegmentsZ();
			printf("z = %lf\n", pat->Z());
			pat->SetSegmentsPlate(ipl);
		}
	}
	return pat;
}


void EdbEDAIO::WriteFeedbackFile(char *filename){
	// write feedback file
	// if eOutputFileMode == 0 (kBern), output comments with "//" escape sequence (default).
	//                                  also track id is not 1,2,3,4, but id of track.
	// if eOutputFileMode == 1 (kOtherLabs). doesn't output comments.
	//
	// to convert Bern format, use
	// gawk -f ConvertFeedback.awk tmp.feedback
	//
	// -- ConvertFeedback.awk --
	// BEGIN{ itrk=1;}
	// !/\/\//&&NF==27{print itrk++,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27;}
	// !/\/\//&&NF==9{print $1,$2,$3,$4,$5,$6,$7,$8,$9;}
	// !/\/\//&&NF==10{print $1,$2,$3,$4,$5,$6,$7,$8,$9,$10;}
	
	
	if( gEDA->GetVertexSet()->N() == 0 ){
		ErrorMessage("No vertex is found. stop.");
		return;
	}
	
	if(filename==NULL){ // File dialog
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "Feedback file", "*.feedback", "All files","*",0,0};
		fi->fFileTypes = filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDSave, fi);
		if(fi->fFilename!=NULL) filename = fi->fFilename;
	}
	
	FILE *fp;
	if(NULL==filename||strlen(filename)==0) {
		return;
	}
	else {
		fp=fopen(filename,"wt");
		if(NULL==fp){
			printf("Couldn't open file : %s\n", filename);
			return;
		}
	}

	printf("--------- Feedback format---------------\n");
	printf("VID        X        Y        Z 1ry charm tau Ndw Nup OutOfBrick\n");
	printf("trkid v1 v2        x        y        z      tx      ty   ip1   ip2 ");
	printf("    p  pmin  pmax mn pa sb dk ou ps ");
	printf(" n RmaxT RmaxL   rmsT   rmsL pl1 pl2 res\n");
	printf("        ipl        x        y        z      tx      ty type irec ngrains\n");
	
	if(eOutputFileMode==kBern){
		fprintf(fp,"// --------- Feedback format---------------\n");
		fprintf(fp,"// VID X Y Z 1ry charm tau Ndw Nup OutOfBrick\n");
		fprintf(fp,"//   OutOfBrick   : 1= vertex in dead material, 0= others\n");
		fprintf(fp,"// \n");
		fprintf(fp,"// Trkid v1 v2 x y z tx ty ip1 ip2 p pmin pmax mn pa sb dk ou ps n RmaxT RmaxL rmsT rmsL pl1 pl2 res\n");
		fprintf(fp,"//   Manual       : 0= auto, 1= manually added\n");
		fprintf(fp,"//   Particle     : 1= muon, 2= charm, 3= electron, 4= e+e- pair\n");
		fprintf(fp,"//   Scanback     : 1= scanback, 0=not scanback\n");
		fprintf(fp,"//   Darkness     : 0= MIP, 1= BLACK, 2= GRAY\n");
		fprintf(fp,"//   OutOfBrick   : 1= pass through up, 2= edge out, 0 others\n");
		fprintf(fp,"//   Last plate   : 1= pass through down, n= edge out last plate number, 0 others\n");
		fprintf(fp,"//   Out_flag(res): 1= 1ry vertex track, 2= e+e- pair, 3= Low momentum (DTheta_RMS >20 mrad),\n");
		fprintf(fp,"//                  4= Scan Forth to be done, 5= Scan Forth  done\n");
		fprintf(fp,"// \n");
		fprintf(fp,"// Ipl x y z tx ty type irec ngrains\n");
		fprintf(fp,"//   Type         : 0 BT, 1 microtrack top, 2 microtrack bottom\n");
		fprintf(fp,"//   Irec         : 0 Automatic, 1 SB, 2 Manual\n");
		fprintf(fp,"// \n");
		fprintf(fp,"///////////////////////////////////////////// \n");
		fprintf(fp,"// ---------- Vertex Data ---------------- // \n");
		fprintf(fp,"///////////////////////////////////////////// \n");
	}
	// Vertices
	
	TObjArray *vertices = (gEDA->GetVertexSet())->GetVertices();
	
	printf("%d vertices\n", vertices->GetEntries());
	EdbVertex *v_1ry=NULL;
	double z=1e9;
	for(int i=0;i<vertices->GetEntries();i++){
		EdbVertex *v = (EdbVertex *) vertices->At(i);
		if(v->Z()<z) {
			z = v->Z();
			v_1ry=v;
		}
	}
	int vtxpl = gEDA->GetIPLZ(v_1ry->Z());
	printf("Vertex plate = %d. z=%7.1lf\n", vtxpl, gEDA->GetZ(vtxpl));
	if(eOutputFileMode==kBern) fprintf(fp, "// primary vertex plate = %d. dz = %f\n", vtxpl, gEDA->GetZ(vtxpl)-v_1ry->Z());
	
	if(eOutputFileMode==kBern) fprintf(fp, "// VID     X        Y        Z 1ry charm tau Ndw Nup OutOfBrick\n");
	
	for(int i=0; i<vertices->GetEntries(); i++){
		EdbVertex *v = (EdbVertex *) vertices->At(i);
		v->SetID(i+1);
		int ISPRIMARY = v==v_1ry ? 1 : 0;
		int ISCHARM = 0;
		int ISTAU = 0;
		int Nup=0, Ndown=0;
		for(int j=0;j<v->N();j++){
			EdbTrackP *t = v->GetTrack(j);
			if(t->GetSegmentFirst()->Z()>v->Z()) Ndown++;
			if(t->GetSegmentLast()->Z()<v->Z()) Nup++;
		}
		int OutOfBrick = v->Z()<gEDA->GetZ(1)? 1:0; // 1: if vertex is outside of the brick. 0: others.
		
		if(eOutputFileMode==kBern){
			fprintf(fp,"%3d %8.1lf %8.1lf %8.1lf %3d %5d %3d %3d %3d %3d\n", 
					v->ID(), v->X(), v->Y(), v->Z(),ISPRIMARY,ISCHARM,ISTAU, Ndown, Nup, OutOfBrick);
		} else {
			fprintf(fp,"%d %8.1lf %8.1lf %8.1lf %3d %5d %3d %3d %3d %3d\n", 
					v->ID(), v->X(), v->Y(), v->Z(),ISPRIMARY,ISCHARM,ISTAU, Ndown, Nup, OutOfBrick);
		}
	}
	
	// Tracks + Segments
	int itrk=1;
	for(int i=0;i<gEDA->NTrackSets();i++){
		EdbEDATrackSet *set = gEDA->GetTrackSet(i);
		if(set->N()==0) continue;
		if(set->GetDraw()==0) continue;
		printf("TrackSet %s : %3d tracks\n", set->GetName(), set->N()); 
		if(eOutputFileMode==kBern){
			EdbID& id=set->GetID();
			fprintf(fp,"////////////////////////////////////////////////////////// \n");
			fprintf(fp,"// ---------- TrackSet : %s %6d 0 %d %4d ----------- //\n", set->GetName(), id.eBrick, id.eMajor, id.eMinor);
			fprintf(fp,"////////////////////////////////////////////////////////// \n");
		}
		for(int j=0;j<set->N();j++){
			EdbTrackP *t = set->GetTrack(j);
			EdbSegP *sf = t->GetSegmentFirst();
			EdbSegP *sl = t->GetSegmentLast();
			EdbVertex * v1 = NULL;
			EdbVertex * v2 = NULL;
			for(int k=0;k<vertices->GetEntries();k++){
				EdbVertex *v = (EdbVertex *) vertices->At(k);
				for(int l=0;l<v->N();l++){
					EdbTrackP *tv = v->GetTrack(l);
					if(t==tv){
						if(NULL==v1) v1 = v;
						else {
							if(v1->Z()>v->Z()) { // v1 should 1ry.
								v2 = v1;
								v1 = v;
							}
							else v2 = v;
			}	}	}	}
			if(v1==NULL) v1=v_1ry; // all tracks are attached to 1ry vertex at this moment.
			
			// t  : pointer to the track
			// v1 : upstream vertex
			// v2 : downstream vertex
			// sf : first segment to be used for vertex calculation
			// fp : file pointer to print data.
			
			int ivtx1 = v1 ? v1->ID() : -1;
			int ivtx2 = v2 ? v2->ID() : -1;
			
			float ip1= v1? CalcIP(sf,v1) : -1.0; // v1->DistSeg(sf): -1.0;
			float ip2= v2? CalcIP(sl,v2) : -1.0; // v2->DistSeg(sl): -1.0;
			
			double p,pmin,pmax;
			CalcP(t, p, pmin, pmax); // pmin = pmin, pmax = pmax
			
			// Flags.... same as Feedback file.
			int MAN = 0;
			int SCANBACK=0, idscanback=-1;
			int MUON    =0;
			int DARKNESS=0; // no function to define.
			
			if(gEDA->GetTrackSet("MN")->FindTrack(t)) MAN=1;
			
			EdbTrackP * sbt = set==gEDA->GetTrackSet("SB")? t : gEDA->CheckScanback(t); // check if the track is a scanback track
			if(sbt!=NULL) idscanback = sbt->ID();
			if (idscanback!=-1) SCANBACK = 1;
			if (idscanback==gEDA->IdMuon()&&gEDA->IdMuon()!=-1) MUON =1;
			
			int OutOfBrick=0; // OutOfBrick: 1= pass through, 2=edge out, 0 others
			int LastPlate=0;  // Last plate: 1= pass though, n=edge out track last plate, 0 others 
			
			int nseg = t->N();
			
			// Small Kink search

			// remove Micro-tracks.
			double RmaxT=0.0, RmaxL=0.0, Rmax=0.0, dtmaxt=0.0, dtmaxl=0.0, rmstmax=-1.0, rmslmax=-1.0;
			int iplRmax1=0, iplRmax2=0;
			
			EdbTrackP *tt = new EdbTrackP;
			tt->Copy(*t);
			tt->SetCounters();
			
			for(int k=0;k<tt->N()&&tt->N()>1;k++){
				EdbSegP *s = tt->GetSegment(k);
				if(s->Side()!=0) tt->RemoveSegment(s); // if it's microtrack, remove.
			}
			
			int n = tt->N();
			
			if(n<=1) {
				// cannot calculate rms.
				RmaxT=RmaxL=-1.;
				iplRmax1=iplRmax2=-1;
			}
			else if( n<4 ){
				// if nseg <=3, cannot calculate kink, just calculate rms but not R.
				RmaxT=RmaxL=-1.;
				iplRmax1=iplRmax2=-1;
			}
			else {
				// Calculate rms and R.
				for(int k=0; k<n-1;k++){
					EdbSegP *ss1 = tt->GetSegment(k);
					EdbSegP *ss2 = tt->GetSegment(k+1);
					
					int ipl1 = ss1->Plate();
					int ipl2 = ss2->Plate();

					// if ss2 is on pl 57, skip.
					if(ipl2>=gEDA->GetLastPlate()) continue;
					// if ss1 is more downstream than 3 plates from vertex, skip. search only kink between 1-x,2-x,3-x.
					if(ipl1>vtxpl+2) continue;
					
					double dtt,dtl;
					CalcDTTransLongi(ss1, ss2, &dtt, &dtl);
					dtt=fabs(dtt);
					dtl=fabs(dtl);
					
					double r,rt,rl;
					int ndata;
					DTRMSTLGiven1Kink(tt, k, &r, &rt, &rl, &ndata);

					double RT =dtt/rt/sqrt((double)ipl2-ipl1);
					double RL =dtl/rl/sqrt((double)ipl2-ipl1);
					double R=sqrt(RT*RT+RL*RL);
					printf("ipl %2d-%2d dtt=%7.4lf dtl=%7.4lf rmst=%7.4lf rmsl=%7.4lf RT=%5.2lf RL=%5.2lf n=%d\n", 
							ipl1, ipl2, dtt, dtl, rt, rl, RT,RL,ndata);
					if(Rmax<R){
						Rmax  = R;
						RmaxT = RT;
						RmaxL = RL;
						iplRmax1=ipl1;
						iplRmax2=ipl2;
						dtmaxt = dtt;
						dtmaxl = dtl;
						rmstmax = rt;
						rmslmax = rl;
					}
				}
			}
			// nseg in 5 plate.
			int nin5pl = 0;
			for(int k=0;k<n;k++){
				EdbSegP *s = tt->GetSegment(k);
				if(s->Plate()>=vtxpl && s->Plate()<vtxpl+5) nin5pl++;
			}
			if( nin5pl<=2 ){
				// nseg <=2 in first 5 plate is out of criteria. just calculate RMS but not R.
				RmaxT=RmaxL=-1.;
				iplRmax1=iplRmax2=-1;
			}
			
			double rms, rmst, rmsl;
			if(n<=1) rms=rmst=rmsl=-1.;
			else DTRMSTL(tt, &rms, &rmst, &rmsl);
			
			delete tt;
			
			int result=1;
			
			const char *comment = set->GetComment(t);
			if(eOutputFileMode==kBern){
				// Bern format
				if(eOutputFileMode==kBern){
					fprintf(fp,"// tid v1 v2        x        y        z      tx      ty    ip1    ip2 ");
					fprintf(fp,"    p  pmin  pmax mn pa sb dk of ps ");
					fprintf(fp," n RmaxT RmaxL    rmsT    rmsL pl1 pl2 res\n");
				}
				fprintf(fp, "%6d %2d %2d %8.1f %8.1f %8.1f %7.4f %7.4f %6.1f %6.1f ",
					t->ID(), ivtx1, ivtx2, sf->X(), sf->Y(), sf->Z(), sf->TX(), sf->TY(), ip1, ip2);
				
			} else {
				// Official
				fprintf(fp, "%d %2d %2d %8.1f %8.1f %8.1f %7.4f %7.4f %6.1f %6.1f ",
					itrk++, ivtx1, ivtx2, sf->X(), sf->Y(), sf->Z(), sf->TX(), sf->TY(), ip1, ip2);
			}
			fprintf(fp, "%5.1lf %5.1lf %5.1lf %2d %2d %2d %2d %2d %2d ", 
				p,pmin,pmax, MAN, MUON, SCANBACK, DARKNESS, OutOfBrick, LastPlate);
			fprintf(fp, "%2d %5.2lf %5.2lf %7.4lf %7.4lf %3d %3d %3d\n",
				nseg, RmaxT, RmaxL, rmst, rmsl, iplRmax1, iplRmax2, result);

			// print on the shell.
			fprintf(stdout, "%5d %2d %2d %8.1f %8.1f %8.1f %7.4f %7.4f %6.1f %6.1f ",
				t->ID(), ivtx1, ivtx2, sf->X(), sf->Y(), sf->Z(), sf->TX(), sf->TY(), ip1, ip2);
			fprintf(stdout, "%5.1lf %5.1lf %5.1lf %2d %2d %2d %2d %2d %2d ", 
				p,pmin,pmax, MAN, MUON, SCANBACK, DARKNESS, OutOfBrick, LastPlate);
			fprintf(stdout, "%2d %5.2lf %5.2lf %7.4lf %7.4lf %3d %3d %3d\n",
				nseg, RmaxT, RmaxL, rmst, rmsl, iplRmax1, iplRmax2, result);
			
			// Check if the rms is smaller than angular resolution or not.
			// if it's smaller, then print a note in the feedback file.
			
			// minimum value of rms (angular resolution).
			double angle = sqrt(t->TX()*t->TX()+t->TY()*t->TY());
			double resT  = 0.0015*sqrt(2.0);
			double resL  = 0.0015*(1+angle*5)*sqrt(2.0);
			double RresT = RmaxT>0 ? dtmaxt/resT : -1.;
			double RresL = RmaxL>0 ? dtmaxl/resL : -1.;

			if( eOutputFileMode==kBern ){
				if( ( rmst<resT || rmsl<resL ) && rmst>0)
				     fprintf(fp, "//     rms for R = %7.4lf %7.4lf , note : rms is smaller than angular resolution.  "
							    "RresT RresL resT resL = %5.2lf %5.2lf %7.4lf %7.4lf\n", 
							    rmstmax, rmslmax, RresT, RresL, resT, resL);
				else fprintf(fp, "//     rms for R = %7.4lf %7.4lf                                                   "
							    "RresT RresL resT resL = %5.2lf %5.2lf %7.4lf %7.4lf\n", 
							    rmstmax, rmslmax, RresT, RresL, resT, resL);
				
				if( nin5pl<3 ) fprintf(fp, "//     nBT = %d. nBT in first five plate = %d.\n", n, nin5pl);
				fprintf(fp,      "//     track comment = \"%s\"\n", comment);
			}
			
			// Fill microtrack information
			
			int irec = 0;    // 0:TS, 1:PredScan, 2: Manual
			TString name = set->GetName();
			if( name.BeginsWith("TS") ) irec=0;
			if( name.BeginsWith("SB") ) irec=1;
			if( name.BeginsWith("SF") ) irec=1;
			if( name.BeginsWith("MN") ) irec=2;
			
			for(int k=0;k<t->N();k++){
				EdbSegP *s = t->GetSegment(k);

				int type = s->Side(); // 0:BT, 1:MTtop, 2:MTbottom
				EdbID idseg=s->ScanID();
				EdbEDATrackSet *setseg = gEDA->GetTrackSet( idseg);
				if(setseg){
					TString name = setseg->GetName();
					if( name.BeginsWith("TS") ) irec=0;
					if( name.BeginsWith("SB") ) irec=1;
					if( name.BeginsWith("SF") ) irec=1;
					if( name.BeginsWith("MN") ) irec=2;
				}
				
				if(eOutputFileMode==kBern){
					// Bern format
					if(k==0) fprintf(fp,"//       ipl        x        y        z      tx      ty type irec ngrains\n");
					fprintf(fp,"%12d %8.1f %8.1f %8.1f %7.4f %7.4f %4d %4d %4d\n",
						s->Plate(), s->X(), s->Y(), s->Z(), s->TX(), s->TY(), type, irec, (int) s->W());
				} else {
					// Official
					fprintf(fp,"%d %8.1f %8.1f %8.1f %7.4f %7.4f %4d %4d %4d\n",
						s->Plate(), s->X(), s->Y(), s->Z(), s->TX(), s->TY(), type, irec, (int) s->W());
				}
			}
			
		}
	}
	
	// Scanning area
	
	EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
	EdbEDAAreaSet *aset = set->GetAreaSet();
	if(aset->N()!=0){
		fprintf(fp,"//////////////////////////////////////////\n");
		fprintf(fp,"// ------ Area information : %s ------- //\n", set->GetName());
		fprintf(fp,"//////////////////////////////////////////\n");
		for(int i=0;i<aset->N();i++){
			EdbEDAArea *a = aset->GetArea(i);
			fprintf(fp,"// area : TS %2d %lf %lf %lf %lf %lf\n", 
				a->Plate(), a->Xmin(), a->Xmax(),
				a->Ymin(), a->Ymax(), a->Z());
		}
	}
	
	fclose(fp);

	printf("file %s was written.\n", filename);

}

void EdbEDAIO::WriteFeedbackFile2008(EdbVertex *v, char *filename){

// write feedback file. original code from Frederic.
// the flags are correct if you work in the correct directory.

	// extract vertex position and multiplicity
	// extract vertex tracks infos
	if(v==NULL) v = gEDA->GetSelectedVertex();
	if(filename==NULL){ // File dialog
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "Feedback file", "*.feedback", "All files","*",0,0};
		fi->fFileTypes = filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDSave, fi);
		if(fi->fFilename!=NULL) filename = fi->fFilename;
	}
	
	if(NULL==filename||strlen(filename)==0) { printf("Cancel to write feedback\n"); return;}
	FILE *fp=fopen(filename,"wt");
	if(NULL==fp){
		printf("Couldn't open file : %s\n", filename);
		return;
	}


	// put first the primary vertex

	fprintf(fp,"%i %.1f %.1f %.1f %i %i %i \n",1,v->X(),v->Y(),v->Z(),1,0,0);

	// tracks from primary vertex
	for(int j=0; j<v->N(); j++){
		int MANUAL=0, MUON=0, SCANBACK=0, DARKNESS=0;
		EdbTrackP *t = v->GetTrack(j);
		EdbSegP *s = t->GetSegmentFirst();

		// check if the track is a scanback track
		int idscanback = -1;
		EdbTrackP *tsb = gEDA->CheckScanback(t);
		if(tsb) idscanback = tsb->ID();

		int idmuon     = IdMuon();
		if (idscanback!=-1) SCANBACK = 1;
		if (idscanback==idmuon&&idmuon!=-1) {
			MUON =1;
			printf("      Track: %d is muon\n", t->ID());
		}

		double p, pmin, pmax;
		CalcP(t, p, pmin, pmax); // pmin = pmin, pmax = pmax

		fprintf(fp,"%4i %i %i %8.1f %8.1f %8.1f %7.4f %7.4f %6.2f %3.1f %6.2lf %5.2lf %8.1lf %i %i %i %i \n",
		t->ID(),1,-1,s->X(),s->Y(),s->Z(),s->TX(), s->TY(), v->Impact(j),-1.,p,pmin,pmax,MANUAL,MUON,SCANBACK,DARKNESS);

	}
	printf("-- FeedbackFile : %s--\n", filename);
	fclose(fp);

#ifdef _WINDOWS
	char buf[256];
	sprintf(buf, "type %s",filename);
	for(int i=0;i<strlen(buf);i++) if(buf[i]=='/') buf[i]='\\';
	gSystem->Exec(buf);
#else
	gSystem->Exec(Form("cat %s",filename));
#endif
}

void EdbEDAIO::ReadFeedbackFile(char *filename){
	// Read feedback file format (ver 2009 Oct).
	
	gEDA->Reset();
	
	
	char FlagPart[][10]={"", "mu", "charm", "e", "e-pair"};
	char FlagCS[][10]   ={"", "cs"};
	
	// if filename is not given, open file browser.
	if(filename==NULL){
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "Feedback File", "*.feedback", "All files","*",0,0};
		fi->fFileTypes=filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve?gEve->GetMainWindow():0, kFDOpen, fi);
		filename = fi->fFilename;
	}
	
	if(NULL==filename) return;
	
	FILE *fp = fopen(filename,"rt");
	if(fp==NULL) {
		ErrorMessage(Form("File open error : %s . stop.\n", filename));
		return;
	}
	
	EdbPVRec *pvr = new EdbPVRec;
	EdbScanCond cond;
	EdbTrackP *t=0;
	EdbEDAText *tx = 0;
	
	char buf[256];
	
	char asetname[10];
	EdbEDAAreaSet *aset=NULL;
	
	while(fgets(buf,sizeof(buf),fp)){
		TString line=buf;
		printf("%s", line.Data());

		// track comment = ""
		if(line.Contains("track comment")){
			int istart = line.Index("\"");
			int iend   = line.Index("\"",1,istart+1, TString::kExact);
			if(istart!=kNPOS && iend!=kNPOS && istart+1!=iend){
				char buf[256];
				strncpy(buf, line.Data()+istart+1, iend-istart-1);
				buf[iend-istart-1]='\0';
				tx->AddText(buf);
			}
		}
		
		if(line.Contains("Area information :")){
			aset = new EdbEDAAreaSet;
			aset->SetDraw(kTRUE);
			sscanf( line.Data()+line.Index(":")+1, "%s", asetname);
			printf("Area infor %s\n", asetname);
		}
		if(aset){
			if(line.Contains(Form("area : %s", asetname))){
				int ipl;
				double xmin, xmax, ymin, ymax, z;
				sscanf(line.Data() + line.Index(Form("area : %s", asetname)) + strlen(Form("area : %s", asetname)),
					"%d %lf %lf %lf %lf %lf", &ipl, &xmin, &xmax, &ymin, &ymax, &z);
				aset->AddArea(ipl, xmin, xmax, ymin, ymax, z);
			}
		}
		
		// Remove comments
		line.ReplaceAll("\t"," ");
		int iposcmt = line.Index("//");
		if(iposcmt!=kNPOS) line.Remove(iposcmt, line.Length()-iposcmt);
		
		// Check number of columns.
		TObjArray *tokens = line.Tokenize(" ");
		int ntokens = tokens->GetEntries();
		delete tokens;
		
		
		if(ntokens==0) continue;
		
		else if( ntokens == 10 ){
			// Vertices
			float x,y,z; int id,isprimary,istau, ischarm;
			sscanf(line.Data(),"%d %f %f %f %d %d %d", &id, &x, &y, &z, &isprimary, &ischarm, &istau);
			EdbVertex *v = new EdbVertex();
			v->SetXYZ(x,y,z); v->SetID(id);
			v->SetFlag(isprimary);
			pvr->AddVertex(v);
			printf("Vertex %d %f %f %f\n",v->ID(), v->X(), v->Y(), v->Z());
		}

		else if( ntokens == 27 ){
			// Tracks
			float x,y,z,ax,ay, ip_upvtx, ip_downvtx,  p,pmin,pmax;
			int id_track, id_upvtx, id_downvtx,  manual, particle_id, scanback, darkness;
			int OutOfBrick, LastPlate, nseg, iplRmax1, iplRmax2, result;
			float RmaxT, RmaxL, rmst, rmsl;
			
			int n,nn;
			sscanf(line.Data(),         "%d %d %d %f %f %f %f %f %f%n", &id_track, &id_upvtx, &id_downvtx, &x, &y, &z, &ax, &ay, &ip_upvtx, &nn);
			sscanf(line.Data()+(n=nn),  "%f %f %f %f %d %d %d %d %d%n", &ip_downvtx, &p,&pmin,&pmax, &manual, &particle_id, &scanback, &darkness, &OutOfBrick, &nn);
			sscanf(line.Data()+(n+=nn), "%d %d %f %f %f %f %d %d %d",   &LastPlate, &nseg, &RmaxT, &RmaxL, &rmst, &rmsl, &iplRmax1, &iplRmax2, &result);
			
			// Create Track. "t" is defined out of main loop.
			t = new EdbTrackP;
			t->Set(id_track, x, y, ax, ay, 0, 0);
			t->SetZ(z);
			t->SetTrack(id_track);
			pvr->AddTrack(t);
			
			tx = new EdbEDAText(t->X(), t->Y(), t->Z(), "[");
			if(0<=particle_id && particle_id<=4) { if(tx->N()>1)tx->AddText(", "); tx->AddText(FlagPart[particle_id]);}
			if(scanback==1)                      { if(tx->N()>1)tx->AddText(", "); tx->AddText(FlagCS[scanback]);}
			tx->AddText("]");
			if(tx->N()==2) tx->SetText("");
			tx->SetReference(tx);
			gEDA->AddDrawObject(tx);
			
			// fill COV for vertexing
			t->SetErrors();
			cond.FillErrorsCov(t->TX(), t->TY(), t->COV());
			
			// associate to vertex.
			for(int i=0; i<pvr->Nvtx(); i++){
				EdbVertex *v = pvr->GetVertex(i);
				if(id_upvtx==v->ID()||id_downvtx==v->ID()){
					EdbVTA *vta = new EdbVTA(t, v);
					vta->SetZpos( t->Z()>v->Z() ? 1 : 0);
					vta->SetFlag(2);
					vta->SetImp( id_upvtx==v->ID()? ip_upvtx : ip_downvtx);
					v->AddVTA(vta);
				}
			}
		}
		
		else if( ntokens ==  9 ){
			// Segments
			int ipl, type, irec, ph;
			float x,y,z,ax,ay;
			sscanf(line.Data(),"%d %f %f %f %f %f %d %d %d", &ipl, &x, &y, &z, &ax, &ay, &type, &irec, &ph);
			
			EdbSegP *s = new EdbSegP(t->ID(),x,y,ax,ay,0,0);
			s->SetZ(z);
			s->SetPID(ipl);
			s->SetPlate(ipl);
			s->SetW(ph);
			s->SetTrack(t->ID());
			s->SetSide(type);

			// fill COV for vertexing
			s->SetErrors();
			cond.FillErrorsCov(s->TX(), s->TY(), s->COV());
			
			// Add segment in PVRec and Track, keeping consistency of pointer in them.
			EdbSegP *s_in_pattern = pvr->AddSegment(*s);
			t->AddSegment( s_in_pattern);
			
			if(NULL!=tx){
				EdbSegP seg(*s);
				seg.PropagateTo(seg.Z()+1000);
				tx->SetXYZ(seg.X(),seg.Y(),seg.Z());
			}
		}
		
		
	}
	
	for(int i=0;i<pvr->Npatterns(); i++) pvr->GetPattern(i)->SetPID(i);
	for(int i=0;i<pvr->Npatterns(); i++) pvr->GetPattern(i)->SetSegmentsPID();
	for(int i=0;i<pvr->Ntracks(); i++) pvr->GetTrack(i)->SetCounters();
	
	printf("\nEdbPVRec summary. %d vertices, %d tracks.\n", pvr->Nvtx(), pvr->Ntracks());
	pvr->Print();
	
	fclose(fp);

	if(pvr==NULL) return;
	for(int i=0;i<pvr->Npatterns(); i++){
		EdbPattern *pat = pvr->GetPattern(i);
		pat->SetPID(gEDA->GetPID(pat->ID()));
	}

	gEDA->SetPVR(pvr);
	gEDA->GetTrackSet("TS")->AddTracksPVR(pvr);
	gEDA->GetVertexSet()->AddVertices(pvr->eVTX);
	
	if(aset){
		EdbEDATrackSet *set = gEDA->GetTrackSet(asetname);
		if(set) set->SetAreaSet(aset);
	}
	gEDA->Redraw();
	gEDA->UpdateScene();
}


EdbVertex * EdbEDAIO::ReadFeedbackFile2008(char *filename){
	using namespace std;

	// if filename is not given, open file browser.
	if(filename==NULL){
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "Feedback File", "*.feedback", "All files","*",0,0};
		fi->fFileTypes=filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDOpen, fi);
		filename = fi->fFilename;
	}
	
	if(NULL==filename) return NULL;
		
	// check the file.
	ifstream fin(filename);
	if(!fin) {
		printf("can not open file : %s\n", filename);
		return NULL;
	} printf("read feedback %s\n", filename);
	

	gEDA->ClearVertices();
	
	EdbEDATrackSet *set = gEDA->GetTrackSet("TS");
	set->ClearTracks();
	
	// read file line by line.
	char buf[512];
	while( fin && fin.getline(buf,512) ){
		string line(buf);
		
		// ignore comment
		if(line.find("//")!=line.npos) line[line.find("//")]='\0';
		
		// check the number of columnline.
		istringstream ss(line);
		string s2;
		int ncolumn;
		for(ncolumn=0;ss>>s2;ncolumn++){
			//cout <<s2 << endl;
		}
		
		// ncolumn == 7 --> vertex info.  ncolumn == 17 --> track info
		if(ncolumn==7){
			double x,y,z;
			int id,isprimary,istau, ischarm;
			sscanf(line.c_str(),"%d %lf %lf %lf %d %d %d", &id, &x, &y, &z, &isprimary, &ischarm, &istau);
			EdbVertex *v = new EdbVertex();
			v->SetXYZ(x,y,z);
			v->SetID(id);
			gEDA->AddVertex(v);
		}
		else if (ncolumn==17){
			double x,y,z,ax,ay, ip_upvtx, ip_downvtx;
			double p,pmin,pmax;
			int id_track, id_upvtx, id_downvtx;
			int manual, particle_id, scanback, darkness;
			
			istringstream ss(line);
			
			ss	>> id_track >> id_upvtx >> id_downvtx >> x >> y >> z >> ax >> ay >> ip_upvtx >> ip_downvtx 
				>> p >> pmin >> pmax >> manual >> particle_id >> scanback >> darkness;
			
			EdbSegP s(id_track, x, y, ax, ay);
			s.SetZ(z);
			
			EdbTrackP *t = set->FindTrack(&s);
			
			if(t) {
				printf("%s track %d\n", line.c_str(), t->ID());
				set->SetTrack(t);
				for(int i=0; i<gEDA->GetVertexSet()->N(); i++){
					EdbVertex *v = gEDA->GetVertexSet()->GetVertex(i);
					if(id_upvtx!=v->ID()) continue;
					EdbVTA *vta = new EdbVTA(t, v);
					vta->SetZpos(1);
					vta->SetFlag(2);
					vta->SetImp(EdbEDAUtil::CalcIP(&s,v));
					v->AddVTA(vta);
				}
			}
			else  printf("track not found. id = %d\n", id_track);
		}
	}
	gEDA->Redraw();
	if( gEDA->GetVertexSet()->N()) return gEDA->GetVertexSet()->GetVertex(0);
	else return NULL;
}

int EdbEDAIO::IdMuon(char *filename){
	// code by GL
  char            buf[256];
  char            key[256];
  char m1[12], m2[10], m3[10],m4[10],m5[10],m6[10];
  static int val=-2;
  char mu[3];
  
  if(val>=0) return val;
  if(val==-1) return val;

  FILE *fp=fopen(filename,"r");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", filename);
    val = -1;
    return val;
  }
  
  // printf( "Read affine transformation from file: %s\n", fname );
  while( fgets(buf,256,fp)!=NULL ) {

    for( int i=0; i<(int)strlen(buf); i++ ) 
      if( buf[i]=='#' )  {
        buf[i]='0';                       // cut out comments starting from #
        break;
      }
    
   if( sscanf(buf,"%s",key)!=1 ) continue;
   if      ( !strcmp(key,"Prediction")   )
     {
       sscanf(buf+strlen(key),"%s %s %s %s %s %s ",m1,m2,m3,m4,m5,m6);
       mu[0] = m2[3];
       mu[1] = m2[4];
       mu[2] = m2[5];
       val = atol(mu);
       //       cout << m2[3] << " " << m2[4] << " " << m2[5]<< " " << val << endl;

     }
   if      ( !strcmp(key,"No")   )
     {
       val=-1;
     }

  }
  fclose(fp);	

  return val;
}

void EdbEDAIO::OpenTextEditor(char *filename){
	TEveBrowser* browser = gEve->GetBrowser();
	browser->StartEmbedding(1);
	new TGTextEditor(filename, gClient->GetRoot());
	browser->StopEmbedding();
	
	unsigned int i,k=0;
	for(i=0;i<strlen(filename);i++){
		if(filename[i]=='\\'||filename[i]=='/') k=i+1;
	}
	browser->SetTabTitle(filename+k, 1);
}


EdbPVRec * EdbEDAIO::ReadFilteredText(char *filename){
	int i,j;
	
	if(filename==NULL||strlen(filename)<1){ // File dialog
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "Filtered text", "*.txt", "All files","*",0,0};
		fi->fFileTypes = filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDOpen, fi);
		if(fi->fFilename!=NULL) {
			filename = fi->fFilename;
		}
	}
	if(filename==NULL) return NULL;
	
	// check the file.
	ifstream fin(filename);
	if(!fin) {
		printf("can not open file : %s\n", filename);
		return NULL;
	} 
	else printf("Read Filtered Text File : %s\n", filename);
	

	TObjArray *vtxarr = new TObjArray;
	TObjArray *trkarr = new TObjArray;
	TObjArray *segarr = new TObjArray;	

	
	EdbPVRec *PVR = new EdbPVRec;
	
	EdbDataSet *dset = gEDA->GetDataSet();

	for(i=0;i<dset->N();i++){
		EdbDataPiece *piece = dset->GetPiece(i);
		EdbLayer* l = piece->GetLayer(0);
		EdbPattern *pat = new EdbPattern( 0.0, 0.0, l->Z());
		pat->SetPID(i);
		PVR->AddPattern(pat);
	}
	
	char buf[512];
	while( fin && fin.getline(buf,512) ){
		string line(buf);

		std::istringstream ssline(line);
		std::string column1;
		ssline >> column1;
		
		std::cout <<line <<std::endl;
		if(column1.find("//")!=column1.npos) continue;

		if(column1=="vertex"){
			int id;
			float x, y, z;
			ssline >> id >> x >> y >> z;
			EdbVertex *v = new EdbVertex;
			v->SetXYZ(x,y,z);
			v->SetID(id);
			vtxarr->Add(v);
			PVR->AddVertex(v);
		}
		
		if(column1=="track"){
			int id, flag, ipl1, ipl2, nseg, ivtx1, ivtx2;
			double tx,ty, ip_1ry, ip1, ip2;
			ssline >> id >> flag >> tx >> ty >> ipl1 >> ipl2 >> nseg >> ip_1ry >> ivtx1 >> ip1 >> ivtx2 >> ip2;
			EdbTrackP *t = new EdbTrackP;
			t->SetID(id);
			t->SetFlag(flag);
			for(i=0;i<vtxarr->GetEntries();i++){
				EdbVertex *v = (EdbVertex *) vtxarr->At(i);
				if(v->ID()==ivtx1||v->ID()==ivtx2){
					EdbVTA *vta = new EdbVTA(t, v);
					vta->SetFlag(2);
					v->AddVTA(vta);
				}
			}
			trkarr->Add(t);
			PVR->AddTrack(t);
		}
		
		if(column1=="segment"){
			int itrk, ipl, iseg, sflag, tflag;
			float x, y, z, ax, ay, chi2, w;
			ssline >> itrk >> tflag >> ipl >> iseg >> x >> y >> z >> ax >> ay >> w >> chi2 >> sflag;
			
			EdbSegP *s = new EdbSegP (iseg, x, y, ax, ay, w, sflag);
			s->SetPID(gEDA->GetPID(ipl));
			s->SetZ(z);
			s->SetDZ(300);
			s->SetTrack(itrk);
			s->SetChi2(chi2);
			
			for(i=0;i<trkarr->GetEntries();i++){
				EdbTrackP *t = (EdbTrackP *) trkarr->At(i);
				if(t->ID()==itrk&&t->Flag()==tflag) {
					t->AddSegment(s);
				}
			}
			
			EdbPattern *pat = PVR->GetPattern( gEDA->GetPID(ipl));
			pat->AddSegment(*s);
			segarr->Add(s);
			
		}
		
		if(column1=="area"){
			int ipl;
			double xmin,xmax,ymin,ymax,z;
			ssline >> ipl >> xmin >> xmax >> ymin >> ymax >> z;
			EdbEDAArea *a = new EdbEDAArea(ipl, xmin, xmax, ymin, ymax, z);
			gEDA->GetTrackSet("TS")->GetAreaSet()->AddArea(a);
		}
		
		
	}
	fin.close();

	printf("Vertices %4d\n", vtxarr->GetEntries());
	printf("Tracks   %4d\n", trkarr->GetEntries());
	printf("Segments %4d\n", segarr->GetEntries());
	
	
	// Clear all track set
	for(i=0; i<gEDA->NTrackSets();i++){
		gEDA->GetTrackSet(i)->Clear();
	}
	
	for(i=0;i<trkarr->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *) trkarr->At(i);
		t->SetCounters();
		EdbSegP *s = t->GetSegmentFirst();
		t->Set(t->ID(), s->X(), s->Y(), s->TX(), s->TY(), 0, t->Flag());
		t->SetZ(s->Z());
		
		if( 0 <= t->Flag()) gEDA->GetTrackSet("TS")->AddTrack(t);
		if( -100 < t->Flag() && t->Flag() <     0) gEDA->GetTrackSet("MN")->AddTrack(t);
		if( -200 < t->Flag() && t->Flag() <=  -99) gEDA->GetTrackSet("SB")->AddTrack(t);
		if(-1000 < t->Flag() && t->Flag() <= -200) gEDA->GetTrackSet("SF")->AddTrack(t);
	}
	
	EdbEDAVertexSet *vset = gEDA->GetVertexSet();
	gEDA->ClearSelectedVertex();
	vset->Clear();
	for(i=0;i<vtxarr->GetEntries();i++){
		EdbVertex *v = (EdbVertex *) vtxarr->At(i);
		
		for(j=0;j<v->N();j++){
			EdbVTA    *vta = v->GetVTa(j);
			EdbTrackP *t   = vta->GetTrack();
			EdbSegP   *s   = t->GetSegmentFirst();
			vta->SetZpos( s->Z()>v->Z() ? 1 : 0 );
			vta->SetImp(CalcIP(s,v));
		}
		vset->AddVertex(v);
	}
	
	return PVR;
}

/*

void EdbEDAIO::PrintTrackFeedback(EdbTrackP *t, EdbVertex *v1, EdbVertex *v2, EdbSegP *s1, FILE *fp, bool print_segs){
	// t  : pointer to the track
	// v1 : upstream vertex
	// v2 : downstream vertex
	// s1 : first segment to be used for vertex calculation
	// fp : file pointer to print data.

	if(t==NULL){
		fprintf(fp,     "trkid v1 v2        x        y        z      tx      ty   ip1   ip2 ");
		fprintf(fp, "    p  pmin  pmax mn pa sb dk of ps ");
		fprintf(fp, " n RmaxT RmaxL   rmsT   rmsL pl1 pl2 res\n");
		
		if(print_segs){
			fprintf(fp, "        ipl        x        y        z      tx      ty type irec ngrains\n");
		}
		
		return;
	}
	int ivtx1 = v1 ? v1->ID() : -1;
	int ivtx2 = v2 ? v2->ID() : -1;

	if(s1==NULL) s1=t->GetSegmentFirst();
	EdbSegP *s2 = t->GetSegmentLast();

	float ip1= v1? v1->DistSeg(s1): -1.0;
	float ip2= v2? v1->DistSeg(s2): -1.0;
	
	double p,pmin,pmax;
	CalcP(t, p, pmin, pmax); // pmin = pmin, pmax = pmax

	// Flags.... same as Feedback file.
	int MAN = 0;
	int SCANBACK=0, idscanback=-1;
	int MUON    =0;
	int DARKNESS=0; // no function to define.
	if(gEDA->GetTrackSet("MN")->FindTrack(t)) MAN=1;
	
	EdbTrackP * sbt = gEDA->CheckScanback(t); // check if the track is a scanback track
	if(sbt!=NULL) idscanback = sbt->ID();
	if (idscanback!=-1) SCANBACK = 1;
	if (idscanback==gEDA->IdMuon()&&gEDA->IdMuon()!=-1) MUON =1;

	int OutOfBrick=0; // OutOfBrick: 1= pass through, 2=edge out, 0 others
	int LastPlate=0;  // Last plate: 1= pass though, n=edge out track last plate, 0 others 
	
	int nseg = t->N();
	double rms, rmst, rmsl;
	DTRMSTL1Kink(t, &rms, &rmst, &rmsl);
	
	double RmaxT, RmaxL;
	double Rmax=0.0;
	int iplRmax1, iplRmax2;
	if(nseg==1) {
		RmaxT=RmaxL=rmst=rmsl=0;
		iplRmax1=iplRmax2=s1->Plate();
	}
	else {
		for(int i=0; i<nseg-1;i++){
			EdbSegP *ss1 = t->GetSegment(i);
			EdbSegP *ss2 = t->GetSegment(i+1);
			double dtt,dtl;
			CalcDTTransLongi(ss1, ss2, &dtt, &dtl);
			dtt=fabs(dtt);
			dtl=fabs(dtl);
			double RT =dtt/rmst;
			double RL =dtl/rmsl;
			double R=sqrt(RT*RT+RL*RL);
			if(Rmax<R){
				Rmax  = R;
				RmaxT = RT;
				RmaxL = RL;
				iplRmax1=ss1->Plate();
				iplRmax2=ss2->Plate();
			}
		}
	}
	
	int result=1;
	
	fprintf(fp, "%5d %2d %2d %8.1f %8.1f %8.1f %7.4f %7.4f %5.1f %5.1f ",
		t->ID(), ivtx1, ivtx2, s1->X(), s1->Y(), s1->Z(), s1->TX(), s1->TY(), ip1, ip2);
	fprintf(fp, "%5.1lf %5.1lf %5.1lf %2d %2d %2d %2d %2d %2d ", 
		p,pmin,pmax, MAN, MUON, SCANBACK, DARKNESS, OutOfBrick, LastPlate);
	fprintf(fp, "%2d %5.2f %5.2f %6.4lf %6.4lf %3d %3d %3d\n",
		nseg, RmaxT, RmaxL, rmst, rmsl, iplRmax1, iplRmax2, result);
	
	if(print_segs){
		int type = 0; // 0:BT, 1:MTtop, 2:MTbottom
		int irec = 0;  // 0:TS, 1:PredScan, 2: Manual
		if( gEDA->GetTrackSet("TS")->FindTrack(t)) irec=0;
		if( gEDA->GetTrackSet("SB")->FindTrack(t)) irec=1;
		if( gEDA->GetTrackSet("SF")->FindTrack(t)) irec=1;
		if( gEDA->GetTrackSet("MN")->FindTrack(t)) irec=2;
		
		for(int i=0;i<t->N();i++){
			EdbSegP *s = t->GetSegment(i);
			fprintf(fp,"%11d %8.1f %8.1f %8.1f %7.4f %7.4f %4d %4d %4d\n",
				s->Plate(), s->X(), s->Y(), s->Z(), s->TX(), s->TY(), type, irec, s->W());
		}
	}
}

*/

void EdbEDAIO::WriteFilteredText(char *filename){
	int i,j,k,l;
	FILE *fp;
	
	if(filename==NULL){ // File dialog
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "filtered tracks", "*.txt", "All files","*",0,0};
		fi->fFileTypes = filetypes;
		new TGFileDialog(gClient->GetRoot(), gEve->GetMainWindow(), kFDSave, fi);
		if(fi->fFilename!=NULL) {
			filename = fi->fFilename;
		}
	}
	if(filename==NULL) return;

	fp=fopen(filename,"wt");
	
	if(fp==NULL) {
		printf("File open error : %s\n", filename);
		return;
	}
	
	printf("Write Filtered Text File : %s\n", filename);

	// Vertices
	
	TObjArray *vertices = (gEDA->GetVertexSet())->GetVertices();

	EdbVertex *v_1ry=NULL;
	double z=1e9;
	for(i=0;i<vertices->GetEntries();i++){
		EdbVertex *v = (EdbVertex *) vertices->At(i);
		if(v->Z()<z) {
			z = v->Z();
			v_1ry=v;
		}
	}

	fprintf(fp,"// vtx  id        X        Y        Z 1ry charm tau\n");
	for( i=0; i<vertices->GetEntries(); i++){
		EdbVertex *v = (EdbVertex *) vertices->At(i);
		v->SetID(i);
		int ISPRIMARY = v==v_1ry ? 1 : 0;
		int ISCHARM = 0;
		int ISTAU = 0;
		fprintf(fp,"vertex %3d %8.1lf %8.1lf %8.1lf %3d %5d %3d\n", 
					v->ID(), v->X(), v->Y(), v->Z(),ISPRIMARY,ISCHARM,ISTAU);
	}
	
	printf("vertices %d\n", vertices->GetEntries());
	
//	ID_Track ID_UpVtx ID_DownVtx X Y Z SX SY IP_UpVtx IP_DownVtx P Pmin Pmax Manual Mu/e Scanback Darknes

	fprintf(fp,"// trk    id flag      tx      ty pl1 pl2 nseg   ip1ry vtx1   ip1 vtx2   ip2      p   pmin       pmax man part sb dark\n");

	// Tracks + Segments
	
	for(i=0;i<gEDA->NTrackSets();i++){
		EdbEDATrackSet *set = gEDA->GetTrackSet(i);
		if(set->GetDraw()==0) continue;
		for(j=0;j<set->N();j++){
			EdbTrackP *t = set->GetTrack(j);
			EdbSegP *sf = t->GetSegmentFirst();
			EdbSegP *sl = t->GetSegmentLast();
			EdbVertex * vtx1st = NULL;
			EdbVertex * vtx2nd = NULL;
			for(k=0;k<vertices->GetEntries();k++){
				EdbVertex *v = (EdbVertex *) vertices->At(k);
				for(l=0;l<v->N();l++){
					EdbTrackP *tv = v->GetTrack(l);
					if(t==tv){
						if(NULL==vtx1st) vtx1st = v;
						else {
							if(vtx1st->Z()>v->Z()) { // vtx1st should 1ry.
								vtx2nd = vtx1st;
								vtx1st = v;
							}
							else vtx2nd = v;
			}	}	}	}

			// Flags.... same as Feedback file.
			int MAN = 0;
			int SCANBACK=0, idscanback=-1;
			int MUON    =0;
			int DARKNESS=0; // no function to define.

			if(-99<=t->Flag()&&t->Flag()<0) MAN=1;
			
			EdbTrackP * sbt = gEDA->CheckScanback(t); // check if the track is a scanback track
			if(sbt!=NULL) idscanback = sbt->ID();
			if (idscanback!=-1) SCANBACK = 1;
			if (idscanback==IdMuon()&&IdMuon()!=-1) MUON =1;

			double p,pmin,pmax;
			CalcP(t, p, pmin, pmax); // pmin = pmin, pmax = pmax
			int ivtx1 = vtx1st ? vtx1st->ID() : -1;
			int ivtx2 = vtx2nd ? vtx2nd->ID() : -1;
			float ip1=-1.0, ip2=-1.0;
			if(vtx1st){
				for(k=0;k<vtx1st->N();k++){
					EdbVTA *vta = vtx1st->GetVTa(k);
					if(t==vta->GetTrack()) ip1 = vta->Imp();
				}
			}
			if(vtx2nd){
				for(k=0;k<vtx2nd->N();k++){
					EdbVTA *vta = vtx2nd->GetVTa(k);
					if(t==vta->GetTrack()) ip2 = vta->Imp();
				}
			}
			double ip_1ry = CalcIP(sf, v_1ry);
			
			fprintf(fp,"track %6d %4d %7.4f %7.4f %3d %3d %4d %7.1lf %4d %5.1f %4d %5.1f ",
				t->ID(), t->Flag(), t->TX(), t->TY(), sf->Plate(), sl->Plate(), t->N(), 
				ip_1ry, ivtx1, ip1, ivtx2, ip2);
			fprintf(fp,"%6.2lf %6.2lf %10.2lf %3d %4d %2d %4d\n", p,pmin,pmax, MAN, MUON, SCANBACK, DARKNESS);
		}
		printf("TrackSet : %s %d tracks\n", set->GetName(), set->N());
	}

	fprintf(fp,"// seg   trkid tflag ipl  segid        x        y        z      tx      ty  w   chi2 flag\n");
	
	for(i=0;i<gEDA->NTrackSets();i++){
		EdbEDATrackSet *set = gEDA->GetTrackSet(i);
		if(set->GetDraw()==0) continue;
		for(j=0;j<set->N();j++){
			EdbTrackP *t = set->GetTrack(j);
			for(k=0;k<t->N();k++){
				EdbSegP *s = t->GetSegment(k);
				fprintf(fp,"segment %6d %5d %3d %6d %8.1f %8.1f %8.1f %7.4f %7.4f %2d %6.4f %4d\n",
					t->ID(), t->Flag(), s->Plate(), s->ID(), s->X(),s->Y(),s->Z(),s->TX(),s->TY(),
					(int)s->W(),s->Chi2(),s->Flag());
			}
		}
	}
	
	
	// Scanning area
	fprintf(fp,"// area ipl xmin xmax ymin ymax z\n");
	EdbEDAAreaSet *aset = gEDA->GetTrackSet("TS")->GetAreaSet();
	for(i=0; i<aset->N(); i++){
		EdbEDAArea *a = aset->GetArea(i);
		fprintf(fp,"area %2d %8.1lf %8.1lf %8.1lf %8.1lf %8.1lf\n",
			a->Plate(), a->Xmin(), a->Xmax(), a->Ymin(), a->Ymax(), a->Z());
	}
	
	
	
	fclose(fp);
}





EdbTrackP * EdbEDASelection::GetTrack( TEveElement *e){
	if(IsTrack(e)) return (EdbTrackP*) e->GetUserData();
	if(IsSegment(e)){
		TEveElement *parent = *(e->BeginParents());
		if(IsTrack(parent)) return (EdbTrackP*) parent->GetUserData();
	}
	return NULL;
}


TObjArray * EdbEDASelection::SetSelected(TObjArray *selected)	{
	// fill eSelected by the current selected tracks.

	if(selected){ // fill by given object array.
		*eSelected = *selected;
		return eSelected;
	}
	ClearSelected();
	
	if(gEve->GetCurrentEvent()==NULL) return NULL; // if current event is not set, return.

	TEveSelection *sel = gEve->GetSelection();
	
	for(TEveElement::List_i it=sel->BeginChildren();it!=sel->EndChildren();it++){
		TEveElement *e = *it;
		if( IsSegment (e) || IsTrack(e) ){
			TObject *o = (TObject *)e->GetUserData();
			if(NULL==eSelected->FindObject(o)) eSelected->Add(o); // no double entry
			EdbTrackP *t = GetTrack(e);
			if(t!=NULL&&NULL==eSelectedTracks->FindObject(t)) eSelectedTracks->Add(t);
		}
		if( IsVertex (e)){
			// selection of vertex is managed in Added().
		}
	}
	return eSelected;
}


void EdbEDASelection::SelectAll(){
	TEveSelection *sel = gEve->GetSelection();
	sel->RemoveElements();
	TEveEventManager *ev = gEve->GetCurrentEvent();
	TEveElement::List_i it = ev->BeginChildren();
	for(int i=0;i<ev->NumChildren();i++, it++){
		TEveElement *e = *it;
		if (!IsTrack(e)) continue;
		TEveElement::List_i itseg = e->BeginChildren();
		for(int j=0;j<e->NumChildren();j++, itseg++){
			TEveElement *s = *itseg;
			if(e->FindChild(e->GetElementName())) continue;
			sel->AddElement(s);
		}
	}
}

void EdbEDASelection::SetSelection ( TObjArray * selected){
	if(selected) *eSelected = *selected;
	//if(GetTrackSet("TS")->N()==0) return;
	
	Disable();
	
	// TEveSelection clear
	TEveSelection *sel = gEve->GetSelection();
	sel->Clear();
	
	// loop for all elements in the current event.
	TEveEventManager *eEvent = gEve->GetCurrentEvent();
	if(eEvent==NULL) return;
	int nselected = eSelected->GetEntries();

	for(TEveElement::List_i it = eEvent->BeginChildren(); it!=eEvent->EndChildren(); it++){
		TEveElement *e = *it;
		
		// only track or segment will be target to be selected.
		if(IsTrack(e)){
			// if this track is in the list of eSelected, add to TEveSelection
			for(int i=0; i<nselected;i++){
				if( eSelected->At(i) == e->GetUserData() ) sel->AddElement(e);
			}

			// loop over segments.
			if(e->NumChildren()==0) continue;
			for(TEveElement::List_i itj = e->BeginChildren(); itj!=e->EndChildren(); itj++){
				// if this track is in the list of eSelected, add to TEveSelection
				TEveElement *ee = *itj;
				for(int i=0;i<nselected;i++){
					if( eSelected->At(i) == ee->GetUserData() ) sel->AddElement(ee);
				}
			}
		}
		// coloring of selected vertex is done in EdbEDAVertexSet::DrawSingleVertex()
		/*
		if(IsVertex(e)){
			if(eVertex==(EdbVertex *) e->GetUserData()){
				TEvePointSet *ps = (TEvePointSet *) el;
				ps->SetMarkerColor(kRed);
			}
		}
		*/
	}
	Enable();
}

void EdbEDASelection::Added(TEveElement* el){
	// function which define the actions by mouse.
	// if the action is not by the mouse, return.
	if(eDisable) return; 
	
	printf("%s\n",el ? el->GetElementName() : "");
	gEve->SetStatusLine(el->GetElementName());
	if(el->GetUserData() ==NULL) return;

	// if a vertex was selected, the corresponding tracks will be selected.
	if(IsVertex(el)) {
		Disable();

		EdbVertex *v = (EdbVertex *) el->GetUserData();
		// to avoid double selection of the tracks
		eSel->RemoveElements();
		// add vertex to the selection, avoiding the loop in Added().
		eSel->AddElement(el);
		
		// add track into the selection.
		for(int i=0;i<v->N();i++){
			EdbTrackP *t = v->GetTrack(i);
			TEveElement * eltrack = GetTrackElement(t);
			if(NULL!=eltrack) {
				eSel->AddElement(eltrack);
				printf(" %s\n", eltrack->GetElementName());
			}
		}
		Enable();
		
		// change the color of the vertex point.
		if( eSelectedVertex!= v) {
			TEvePointSet *ps = GetVertexElement(eSelectedVertex);
			if(ps!=NULL) ps->SetMarkerColor(kOrange);
			eSelectedVertex=v;
			ps = (TEvePointSet *) el;
			ps->SetMarkerColor(kRed);
			gEve->Redraw3D();
		}

		// write feedback file
		gEDA->WriteFeedbackFile2008(v,"tmp.old.feedback");
	}

	// revise TObjArray * selected, *selected_tracks
	SetSelected();
	
	// revice extention
	gEDA->GetExtentionSet()->Draw(kFALSE);
	gEve->Redraw3D();
}

void EdbEDASelection::Removed(TEveElement* el){
	printf("Cancel %s\n", el ? el->GetElementName() : "");
	SetSelected();
}

void EdbEDASelection::Cleared(){
	SetSelected();
	if(eDisable) return; 
	printf("Cleared\n");
}
/*
EdbDataSet * EdbEDA::PVRec2DataSet(EdbPVRec *pvr){
	EdbDataSet *dset = new EdbDataSet();
	EdbDataPiece *piece=0;
	EdbDataPiece *pp=0;
	dset->Set0();
	
	for(int i=0; i<pvr->Npatterns(); i++){
		EdbPattern *pat = pvr->GetPattern(i);
		int plateID = pat->PID();
		int pieceID = 0;
		char filename[]="./*";
		int flag = 1;
		piece = new EdbDataPiece(plateID,pieceID,filename,flag);
		piece->MakeName();
		piece->GetMakeLayer(0)->SetZlayer(pat->Z(),0,0);
	}
	return dset;
}
*/
EdbDataProc * EdbEDA::PVRec2DataProc(EdbPVRec *pvr){
	FILE *fp;
	printf("write lnkdummy.def\n");
	fp=fopen("lnkdummy.def","wt");
	fprintf(fp, "INPUT_RUNS_LIST		lnkdummy.lst\n"
				"OUTPUT_DATA_DIR		./data/\n"
				"PARAMETERS_DIR		./pardummy/\n");
	fclose(fp);

	fp = fopen("lnkdummy.lst","wt");
	for(int i=0;i<pvr->Npatterns();i++){
		EdbPattern *pat = pvr->GetPattern(i);
		if(pat==NULL) continue;
		fprintf(fp, "%3d 1 ./data/* 1\n", pat->PID());
	}
	fclose(fp);
	
	printf("make pardummy directry\n");
	system("mkdir pardummy");
	printf("write parameters files\n");
	for(int i=0;i<pvr->Npatterns();i++){
		EdbPattern *pat = pvr->GetPattern(i);
		if(pat==NULL) continue;
		
		fp = fopen(Form("pardummy/%02d_001.par", pat->PID()),"wt");
		fprintf(fp,"ZLAYER 0 %.1lf 0. 0.\n", pat->Z());
		fprintf(fp,"AFFXY 0 1 0 0 1 0 0\n");
		fclose(fp);
	}
	
	EdbDataProc *proc = new EdbDataProc("lnkdummy.def");
	proc->SetPVR(pvr);
	
	system("rm lnkdummy.def");
	system("rm lnkdummy.lst");
	system("rm pardummy/*.*");
	system("rmdir pardummy");

	
	return proc;
}


#include<TGLUtil.h>
#include<TGLIncludes.h>

EdbEDAOverlay::EdbEDAOverlay():eDraw(1), eXa(80), eYa(30), eLength(120), eXb(50), eYb(20){}
void EdbEDAOverlay::SetXY(int pixel_x, int pixel_y){ if(gEDA->GetScaleZ()==1.0) eXb=pixel_x, eYb=pixel_y; else eXa=pixel_x, eYa=pixel_y;}
void EdbEDAOverlay::SetLength(int pixel_length){if(gEDA->GetScaleZ()==1.0) eLength=pixel_length; else eLength=pixel_length;}

void EdbEDAOverlay::Render(TGLRnrCtx& ctx){
	if(eDraw==kFALSE) return;
	if(gEDA->GetScaleZ()==1.0) RenderBar(ctx);
	else RenderAxes(ctx);
}

void EdbEDAOverlay::RenderAxes(TGLRnrCtx& ctx){
	// Render 3 scale axes.

	// view port to world.
	TGLCamera& cam = ctx.RefCamera();
	TGLLine3 l1 = cam.ViewportToWorld(eXa,eYa);
	TGLLine3 l2 = cam.ViewportToWorld(eXa+eLength,eYa);
	
	// find distance between 2 lines.
	const TGLVector3& glv1 = l1.Vector();
	const TGLVector3& glv12 = l2.Start()-l1.Start();
	TVector3 v1(glv1.X(), glv1.Y(), glv1.Z());
	v1.SetMag(1.);
	TVector3 v12(glv12.X(), glv12.Y(), glv12.Z());
	TVector3 vx = v1.Cross(v12);
	double d1 = vx.Mag();
	
	// find 
	double d;
	for(int i=0;i<10;i++){
		if( d1 < 2*TMath::Power(10,i) ) { d=1*TMath::Power(10,i); break; }
		if( d1 < 5*TMath::Power(10,i) ) { d=2*TMath::Power(10,i); break; }
		if( d1 < 10*TMath::Power(10,i) ) { d=5*TMath::Power(10,i); break; }
		d=d1;
	}
	//printf("%lf %lf\r", d, d1);
	
	
	// Draw Text
	TGLLine3 l3 = cam.ViewportToWorld(eXa+10,eYa+20);
	const TGLVertex3& glp31=l3.Start();
	const TGLVertex3& glp32=l3.End();
	TVector3 vec31(glp31.CArr());
	TVector3 vec32(glp32.CArr());
	TVector3 p31 = (vec31+vec32)*0.5;

	TString num=Form(" %d um",(int)d);
#if ROOT_VERSION_CODE>=ROOT_VERSION(5,26,00)
	TGLUtil::Color(kWhite);
	TGLFont font;
	ctx.RegisterFont(12, "arial", TGLFont::kBitmap, font);
	font.PreRender();
	font.Render(num, p31.X(), p31.Y(), p31.Z(), TGLFont::kLeft, TGLFont::kTop);
	font.PostRender();

	TGLUtil::LineWidth(1.0);
#else
	gEve->GetBrowser()->SetStatusText(num,0);
#endif

	// Light off
	TGLCapabilitySwitch lights_off(GL_LIGHTING, kFALSE); // need TGLIncludes.h
	
	// Draw Line
	const TGLVertex3& glp1=l1.Start();
	const TGLVertex3& glp2=l1.End();
	TVector3 vec1(glp1.CArr());
	TVector3 vec2(glp2.CArr());
	TVector3 p1 = (vec1+vec2)*0.5;
	TVector3 p2;

	TGLUtil::Color3f(1.,0.,0.);
	TGLVertex3 glpos(p1.X(), p1.Y(), p1.Z());
	TGLVertex3 glpos2(p1.X()+d, p1.Y(), p1.Z());
	TGLLine3 lx(glpos, glpos2);
	lx.Draw();

	TGLUtil::Color3f(0.,1.,0.);
	glpos2.Set(p1.X(), p1.Y()+d, p1.Z());
	TGLLine3 ly(glpos, glpos2);
	ly.Draw();

	TGLUtil::Color(kCyan);
	glpos2.Set(p1.X(), p1.Y(), p1.Z()+d*gEDA->GetScaleZ());
	TGLLine3 lz(glpos, glpos2);
	lz.Draw();
	
	
}

void EdbEDAOverlay::RenderBar(TGLRnrCtx& ctx){
	// Render a scale bar.

	// view port to world.
	TGLCamera& cam = ctx.RefCamera();
	TGLLine3 l1 = cam.ViewportToWorld(eXb,eYb);
	TGLLine3 l2 = cam.ViewportToWorld(eXb+eLength,eYb);
	
	// find distance between 2 lines.
	const TGLVector3& glv1 = l1.Vector();
	const TGLVector3& glv12 = l2.Start()-l1.Start();
	TVector3 v1(glv1.X(), glv1.Y(), glv1.Z());
	v1.SetMag(1.);
	TVector3 v12(glv12.X(), glv12.Y(), glv12.Z());
	TVector3 vx = v1.Cross(v12);
	double d1 = vx.Mag();
	
	// find 
	double d;
	for(int i=0;i<10;i++){
		if( d1 < 2*TMath::Power(10,i) ) { d=1*TMath::Power(10,i); break; }
		if( d1 < 5*TMath::Power(10,i) ) { d=2*TMath::Power(10,i); break; }
		if( d1 < 10*TMath::Power(10,i) ) { d=5*TMath::Power(10,i); break; }
		d=d1;
	}
	//printf("%lf %lf\r", d, d1);
	
	// position
	const TGLVertex3& p1=l1.Start();
	const TGLVertex3& p2=l2.End();
	TVector3 pp1(p1.X(), p1.Y(), p1.Z());
	TVector3 pp2(p2.X(), p2.Y(), p2.Z());
	TVector3 p=pp1*0.9+pp2*0.1-vx*0.05;
	TString num=Form("%d",(int)d);

	// Light off
	TGLCapabilitySwitch lights_off(GL_LIGHTING, kFALSE); // need TGLIncludes.h

#if ROOT_VERSION_CODE>=ROOT_VERSION(5,26,00)
	TGLUtil::Color(kWhite);
	TGLFont font;
	ctx.RegisterFont(12, "arial", TGLFont::kBitmap, font);
	font.Render(num, p.X(), p.Y(), p.Z(), TGLFont::kLeft, TGLFont::kTop);
	TGLUtil::LineWidth(2.0);
#else
	gEve->GetBrowser()->SetStatusText(num,0);
#endif

	
	// Draw Line
	const Float_t col[4]={1.0,0.0,0.0,0.0};
	TGLUtil::Color4fv(col);
	TGLVector3 vec12=(l2.End()-l1.Start())*(d/d1);
	TGLLine3 l(l1.Start(), vec12);
	l.Draw();
	
	// Draw Text
	
	
	
	// doesn't work.
	//TGLText tx;
	//tx.SetGLTextFont(41);
	//tx.SetTextColor(kWhite);
	//tx.SetTextSize(12);
	//tx.PaintGLText(p.X(), p.Y(), p.Z(),Form("%d", (int) d));

	// doesn't work.
	//	TGLUtil::DrawNumber(num, p); 

	// doesn't work.
	//	TGLFont f;
	//	ctx.RegisterFont(12, "arial", TGLFont::kBitmap, f);
	//	f.Render(num, p.X(), p.Y(), p.Z(), 0);

	// works but need OpenGL32.lib
	/*	const char *txt=Form("%d",(int)d);
		Float_t bb[6];
		TGLFont font;
		ctx.RegisterFont(12, "arial", TGLFont::kBitmap, font);
		font.BBox(txt, bb[0], bb[1], bb[2], bb[3], bb[4], bb[5]);
		glPushMatrix();
		glTranslated(p.X(), p.Y(), p.Z());
		glRasterPos2i(0,0);
		font.Render(txt, );
		glPopMatrix();
	*/	

}


void EdbEDA::Init(void){
	// Initialization of TrackSets and VertexSet.
	gEDA = this;
	eScaleZ=1.0;
	eJapanese=kFALSE;
	
	// Setting of Path, Brick number.
	eScanProc = new EdbScanProc;
	eScanProc->eProcDirClient=FindProcDirClient();
	printf("%s\n", eScanProc->eProcDirClient.Data());
	int ibrick = FindBrickIDFromPath();
	EdbEDABrickData::SetBrick ( ibrick);

	//eScanSet  = new EdbScanSet();
	
	eDrawObjects      = new TObjArray;
	eTrackSets        = new TObjArray;
	
	EdbEDATrackSet *set;
	eTrackSets->Add(set = new EdbEDATrackSet("TS"));
	set->SetDrawAreaSet(kTRUE);
	
	eTrackSets->Add(set = new EdbEDATrackSet("SB"));
	set->SetTrackAttribute(kWhite);
	set->SetID(EdbID(ibrick,0,1,100));
	
	eTrackSets->Add(set = new EdbEDATrackSet("SF"));
	set->SetTrackAttribute(kGreen);
	set->SetID(EdbID(ibrick,0,1,200));
	
	eTrackSets->Add(set = new EdbEDATrackSet("BT"));
	set->SetInterpolation(kFALSE);   // disable to fill dashed line on holes.
	set->SetExtendMode(kExtendDown); // force extention down.

	eTrackSets->Add(set = new EdbEDATrackSet("MN"));
	set->SetTrackAttribute(kViolet);
	eVertexSet = new EdbEDAVertexSet();
	eExt = new EdbEDAExtentionSet();
	

	//AddDrawObject(new EdbEDAObject);
	
}


void EdbEDA::Run(){
	// Start Display.
	
	if(gEve==NULL){
		// TEve setting
#if ROOT_VERSION_CODE<ROOT_VERSION(5,22,00)
		TEveManager::Create();
		eViewer = gEve->GetGLViewer();

#else
		TEveManager::Create(kTRUE,"V");
		eViewer = gEve->GetDefaultGLViewer();
		gEve->GetWindowManager()->HideAllEveDecorations();
		
		gEve->GetDefaultViewer()->SetName("Viewer1");
		TGTab *tab=gEve->GetBrowser()->GetTabRight();
		tab->GetCurrentTab()->SetName("Viewer1");
		tab->Connect("Selected(Int_t)", "EdbEDA", this, "GetGLViewer()");

#endif 
		
		gEve->GetMainWindow()->Resize(1024,730); // resize for note book size 1280x800

		eViewer->SetCurrentCamera(TGLViewer::kCameraOrthoZOY); // Orthogonal Camera
		TGLOrthoCamera& cam = (TGLOrthoCamera &) eViewer->CurrentCamera();
		cam.SetEnableRotate(kTRUE);
		
		eViewer->SetClearColor(kBlack);
		SetOverlay();
		gEve->GetMainWindow()->SetWindowName(Form("OPERA Event Display and Analyser. Brick %d", EdbEDABrickData::GetBrick()));
		//gEve->GetMainWindow()->SetWindowName("OPERA Event Display and Analyser.");
		
		InitSelection(gEve->GetSelection());

		new EdbEDAListTab;
		eMainTab = new EdbEDAMainTab;

		eTrackSetTab = new EdbEDATrackSetTab;
		ePlotTab     = new EdbEDAPlotTab;
		eVertexTab   = new EdbEDAVertexTab;
		eDSTab       = new EdbEDADecaySearchTab;
		eOperationTab= new EdbEDAOperationTab;
		new EdbEDAPredTab;
		new EdbEDAShowerTab;
		new EdbEDAMCTab;
		// new EdbEDAShowerTab;
		//	gEve->GetBrowser()->GetTabBottom()->RemoveTab(0); // remove command tab
		gEve->GetBrowser()->GetTabBottom()->SetTab(0); // set TrackSetTab in front.
	}
	
	printf("GUI setting finished.\n");
	//if(eAreaSet->N()==0&&GetTrackSet("TS")->NBase()!=0) eAreaSet->SetAreas( GetTrackSet("TS")->GetTracksBase());
	Draw(kFALSE);
	if(gEve->GetCurrentEvent()!=NULL) gEve->Redraw3D(kTRUE);
}

void EdbEDA::Draw(int redraw){
	
	if(gEve->GetCurrentEvent()!=0) gEve->GetCurrentEvent()->RemoveElements();
	
	for(int i=0;i<NTrackSets();i++) GetTrackSet(i)->Draw(kFALSE);
	
	eVertexSet->Draw();
	
	for(int i=0;i<NDrawObjects();i++) if(GetDrawObject(i)) GetDrawObject(i)->Draw();
	
	SetSelection();
	
	SetSelected();
	eExt->Draw();
	
	if(redraw) gEve->Redraw3D();
	
	eMainTab->SetDrawCheckBox();
}

void EdbEDA::OpenFile(char *filename, int datatype, TCut rcut){
	//  filename : filename for "LinkDef" or "*.set.root(ScanSet)" or "*.root(linked_track.root format)"
	//  datatype : only for "LinkDef". Data type for EdbDataProc::InitVolume(). put -1 for no InitVolume
	//            if datatype= 100 or 1000. linked tracks will be registred to "TS".
	//  rcut     : Track selection when read.
	
	// if filename is not given. open file browser.
	if(filename==NULL) {
		TGFileInfo *fi=new TGFileInfo;
		fi->fIniDir    = StrDup(".");
		const char *filetypes[] = { "LinkDef", "*.def",
		                            "Linkeda Tracks", "*.root",
		                            "ScanSet", "*.set.root",
		                            "Feedback file","*.feedback*",
		                            "Mxx file","*.all", 
		                            "All files","*",0,0};
		fi->fFileTypes=filetypes;
		new TGFileDialog(gClient->GetRoot(), 0, kFDOpen, fi);
		filename = fi->fFilename;
	}
	
	if(filename==NULL) return;
	// check the existence of the file.
	if(gSystem->AccessPathName(filename, kReadPermission)!=0) {
		printf("Error : %s is not found\n", filename);
		return;
	}
	

	// select an action for the file.
	TString s(filename);
	s.ToLower();
	if(s.EndsWith(".set.root")){
		// Scan set file
		printf("EDA Start with Scan Set : %s\n", s.Data());
		
		Init();
		TFile *f = TFile::Open(filename);
		eScanSet = (EdbScanSet *)f->Get("set");
		eID = *((EdbID *)eScanSet->eIDS.At(0));
		eID.ePlate = 0;
		
		TString s2;
		
		eScanProc->MakeFileName(s2, eID, "trk.root", kFALSE);
		int IsExist = gSystem->AccessPathName(s2.Data(), kReadPermission);
		if( IsExist==kFALSE) {
			GetTrackSet("TS")->SetScanSet(eScanSet);
			GetTrackSet("TS")->ReadTracksTree(eID,rcut);
			GetTrackSet("TS")->SetNsegCut(3);
			GetTrackSet("TS")->DoSelection();
		}
		else{
			GetTrackSet("SB")->ReadPredictionScan(eScanSet);
		}
	}
	else if(s.EndsWith(".def")){
		// Link def file
		printf("EDA Start with LinkDef : %s\n", s.Data());
		Init();
		EdbDataProc *dproc = new EdbDataProc(filename);
		eDataSet  = dproc->GetDataSet();
		
		if(datatype>=0) {
			dproc->InitVolume(datatype,rcut);
			SetDataType(datatype);
			GetTrackSet("TS")->ReadLinkedTracks(dproc);
			GetTrackSet("TS")->SetNsegCut(3);
			GetTrackSet("TS")->SetUpstreamPlate(GetPlateUp());
			GetTrackSet("TS")->DoSelection();
		}
	}
	else if(s.EndsWith(".root")){
		// Linked tracks file
		Init();
		printf("EDA Start with Linked_track : %s\n", s.Data());
		GetTrackSet("TS")->ReadLinkedTracksFile(filename,rcut);
		ePVR=GetTrackSet("TS")->GetPVRec();
		GetTrackSet("TS")->SetNsegCut(3);
		GetTrackSet("TS")->DoSelection();
	}
	else if(s.EndsWith(".feedback")){
		// Feedback
		Init();
		printf("EDA Start with Feedback : %s\n", s.Data());
		printf("Only for viewer!\n");
		ReadFeedbackFile(filename);
	}
	else if(s.EndsWith(".all")){
		// Mxx file (Nagoya style ascii)
		Init();
		SetJapanese();
		printf("EDA Start with Mxx file : %s\n", filename);
		EdbPVRec *pvr = EdbEDAUtil::ReadMxxPVR(filename);
		
		ePVR = pvr;
		GetTrackSet("TS")->AddTracksPVR(pvr);
		GetVertexSet()->AddVertices(pvr->eVTX);
	}
	
	
//		Run();
}


void EdbEDAText::Draw(Option_t *opt){
	if(GetDraw()==kFALSE) return;
	TEveText *tx = new TEveText();
	tx->SetElementName(eText.Data());
	tx->SetText(eText.Data());
	tx->PtrMainTrans()->SetPos(eX, eY, eZ*gEDA->GetScaleZ());
	
	tx->SetFontMode(TGLFont::kBitmap);
	tx->SetFontSize(eTextSize);
	tx->SetFontFile(eTextFont);
	tx->SetMainColor(eTextColor);
	gEve->AddElement(tx);
}

void EdbEDALine::Draw(Option_t *option){
	if(GetDraw()==kFALSE) return;
	TEveLine *l = new TEveLine;
	l->SetElementName(text.Data());
	l->SetNextPoint(X1,Y1,Z1*gEDA->GetScaleZ());
	l->SetNextPoint(X2,Y2,Z2*gEDA->GetScaleZ());
	l->SetLineColor(col);
	l->SetLineWidth(wid);
	gEve->AddElement(l);
}

 EdbEDA.C:1
 EdbEDA.C:2
 EdbEDA.C:3
 EdbEDA.C:4
 EdbEDA.C:5
 EdbEDA.C:6
 EdbEDA.C:7
 EdbEDA.C:8
 EdbEDA.C:9
 EdbEDA.C:10
 EdbEDA.C:11
 EdbEDA.C:12
 EdbEDA.C:13
 EdbEDA.C:14
 EdbEDA.C:15
 EdbEDA.C:16
 EdbEDA.C:17
 EdbEDA.C:18
 EdbEDA.C:19
 EdbEDA.C:20
 EdbEDA.C:21
 EdbEDA.C:22
 EdbEDA.C:23
 EdbEDA.C:24
 EdbEDA.C:25
 EdbEDA.C:26
 EdbEDA.C:27
 EdbEDA.C:28
 EdbEDA.C:29
 EdbEDA.C:30
 EdbEDA.C:31
 EdbEDA.C:32
 EdbEDA.C:33
 EdbEDA.C:34
 EdbEDA.C:35
 EdbEDA.C:36
 EdbEDA.C:37
 EdbEDA.C:38
 EdbEDA.C:39
 EdbEDA.C:40
 EdbEDA.C:41
 EdbEDA.C:42
 EdbEDA.C:43
 EdbEDA.C:44
 EdbEDA.C:45
 EdbEDA.C:46
 EdbEDA.C:47
 EdbEDA.C:48
 EdbEDA.C:49
 EdbEDA.C:50
 EdbEDA.C:51
 EdbEDA.C:52
 EdbEDA.C:53
 EdbEDA.C:54
 EdbEDA.C:55
 EdbEDA.C:56
 EdbEDA.C:57
 EdbEDA.C:58
 EdbEDA.C:59
 EdbEDA.C:60
 EdbEDA.C:61
 EdbEDA.C:62
 EdbEDA.C:63
 EdbEDA.C:64
 EdbEDA.C:65
 EdbEDA.C:66
 EdbEDA.C:67
 EdbEDA.C:68
 EdbEDA.C:69
 EdbEDA.C:70
 EdbEDA.C:71
 EdbEDA.C:72
 EdbEDA.C:73
 EdbEDA.C:74
 EdbEDA.C:75
 EdbEDA.C:76
 EdbEDA.C:77
 EdbEDA.C:78
 EdbEDA.C:79
 EdbEDA.C:80
 EdbEDA.C:81
 EdbEDA.C:82
 EdbEDA.C:83
 EdbEDA.C:84
 EdbEDA.C:85
 EdbEDA.C:86
 EdbEDA.C:87
 EdbEDA.C:88
 EdbEDA.C:89
 EdbEDA.C:90
 EdbEDA.C:91
 EdbEDA.C:92
 EdbEDA.C:93
 EdbEDA.C:94
 EdbEDA.C:95
 EdbEDA.C:96
 EdbEDA.C:97
 EdbEDA.C:98
 EdbEDA.C:99
 EdbEDA.C:100
 EdbEDA.C:101
 EdbEDA.C:102
 EdbEDA.C:103
 EdbEDA.C:104
 EdbEDA.C:105
 EdbEDA.C:106
 EdbEDA.C:107
 EdbEDA.C:108
 EdbEDA.C:109
 EdbEDA.C:110
 EdbEDA.C:111
 EdbEDA.C:112
 EdbEDA.C:113
 EdbEDA.C:114
 EdbEDA.C:115
 EdbEDA.C:116
 EdbEDA.C:117
 EdbEDA.C:118
 EdbEDA.C:119
 EdbEDA.C:120
 EdbEDA.C:121
 EdbEDA.C:122
 EdbEDA.C:123
 EdbEDA.C:124
 EdbEDA.C:125
 EdbEDA.C:126
 EdbEDA.C:127
 EdbEDA.C:128
 EdbEDA.C:129
 EdbEDA.C:130
 EdbEDA.C:131
 EdbEDA.C:132
 EdbEDA.C:133
 EdbEDA.C:134
 EdbEDA.C:135
 EdbEDA.C:136
 EdbEDA.C:137
 EdbEDA.C:138
 EdbEDA.C:139
 EdbEDA.C:140
 EdbEDA.C:141
 EdbEDA.C:142
 EdbEDA.C:143
 EdbEDA.C:144
 EdbEDA.C:145
 EdbEDA.C:146
 EdbEDA.C:147
 EdbEDA.C:148
 EdbEDA.C:149
 EdbEDA.C:150
 EdbEDA.C:151
 EdbEDA.C:152
 EdbEDA.C:153
 EdbEDA.C:154
 EdbEDA.C:155
 EdbEDA.C:156
 EdbEDA.C:157
 EdbEDA.C:158
 EdbEDA.C:159
 EdbEDA.C:160
 EdbEDA.C:161
 EdbEDA.C:162
 EdbEDA.C:163
 EdbEDA.C:164
 EdbEDA.C:165
 EdbEDA.C:166
 EdbEDA.C:167
 EdbEDA.C:168
 EdbEDA.C:169
 EdbEDA.C:170
 EdbEDA.C:171
 EdbEDA.C:172
 EdbEDA.C:173
 EdbEDA.C:174
 EdbEDA.C:175
 EdbEDA.C:176
 EdbEDA.C:177
 EdbEDA.C:178
 EdbEDA.C:179
 EdbEDA.C:180
 EdbEDA.C:181
 EdbEDA.C:182
 EdbEDA.C:183
 EdbEDA.C:184
 EdbEDA.C:185
 EdbEDA.C:186
 EdbEDA.C:187
 EdbEDA.C:188
 EdbEDA.C:189
 EdbEDA.C:190
 EdbEDA.C:191
 EdbEDA.C:192
 EdbEDA.C:193
 EdbEDA.C:194
 EdbEDA.C:195
 EdbEDA.C:196
 EdbEDA.C:197
 EdbEDA.C:198
 EdbEDA.C:199
 EdbEDA.C:200
 EdbEDA.C:201
 EdbEDA.C:202
 EdbEDA.C:203
 EdbEDA.C:204
 EdbEDA.C:205
 EdbEDA.C:206
 EdbEDA.C:207
 EdbEDA.C:208
 EdbEDA.C:209
 EdbEDA.C:210
 EdbEDA.C:211
 EdbEDA.C:212
 EdbEDA.C:213
 EdbEDA.C:214
 EdbEDA.C:215
 EdbEDA.C:216
 EdbEDA.C:217
 EdbEDA.C:218
 EdbEDA.C:219
 EdbEDA.C:220
 EdbEDA.C:221
 EdbEDA.C:222
 EdbEDA.C:223
 EdbEDA.C:224
 EdbEDA.C:225
 EdbEDA.C:226
 EdbEDA.C:227
 EdbEDA.C:228
 EdbEDA.C:229
 EdbEDA.C:230
 EdbEDA.C:231
 EdbEDA.C:232
 EdbEDA.C:233
 EdbEDA.C:234
 EdbEDA.C:235
 EdbEDA.C:236
 EdbEDA.C:237
 EdbEDA.C:238
 EdbEDA.C:239
 EdbEDA.C:240
 EdbEDA.C:241
 EdbEDA.C:242
 EdbEDA.C:243
 EdbEDA.C:244
 EdbEDA.C:245
 EdbEDA.C:246
 EdbEDA.C:247
 EdbEDA.C:248
 EdbEDA.C:249
 EdbEDA.C:250
 EdbEDA.C:251
 EdbEDA.C:252
 EdbEDA.C:253
 EdbEDA.C:254
 EdbEDA.C:255
 EdbEDA.C:256
 EdbEDA.C:257
 EdbEDA.C:258
 EdbEDA.C:259
 EdbEDA.C:260
 EdbEDA.C:261
 EdbEDA.C:262
 EdbEDA.C:263
 EdbEDA.C:264
 EdbEDA.C:265
 EdbEDA.C:266
 EdbEDA.C:267
 EdbEDA.C:268
 EdbEDA.C:269
 EdbEDA.C:270
 EdbEDA.C:271
 EdbEDA.C:272
 EdbEDA.C:273
 EdbEDA.C:274
 EdbEDA.C:275
 EdbEDA.C:276
 EdbEDA.C:277
 EdbEDA.C:278
 EdbEDA.C:279
 EdbEDA.C:280
 EdbEDA.C:281
 EdbEDA.C:282
 EdbEDA.C:283
 EdbEDA.C:284
 EdbEDA.C:285
 EdbEDA.C:286
 EdbEDA.C:287
 EdbEDA.C:288
 EdbEDA.C:289
 EdbEDA.C:290
 EdbEDA.C:291
 EdbEDA.C:292
 EdbEDA.C:293
 EdbEDA.C:294
 EdbEDA.C:295
 EdbEDA.C:296
 EdbEDA.C:297
 EdbEDA.C:298
 EdbEDA.C:299
 EdbEDA.C:300
 EdbEDA.C:301
 EdbEDA.C:302
 EdbEDA.C:303
 EdbEDA.C:304
 EdbEDA.C:305
 EdbEDA.C:306
 EdbEDA.C:307
 EdbEDA.C:308
 EdbEDA.C:309
 EdbEDA.C:310
 EdbEDA.C:311
 EdbEDA.C:312
 EdbEDA.C:313
 EdbEDA.C:314
 EdbEDA.C:315
 EdbEDA.C:316
 EdbEDA.C:317
 EdbEDA.C:318
 EdbEDA.C:319
 EdbEDA.C:320
 EdbEDA.C:321
 EdbEDA.C:322
 EdbEDA.C:323
 EdbEDA.C:324
 EdbEDA.C:325
 EdbEDA.C:326
 EdbEDA.C:327
 EdbEDA.C:328
 EdbEDA.C:329
 EdbEDA.C:330
 EdbEDA.C:331
 EdbEDA.C:332
 EdbEDA.C:333
 EdbEDA.C:334
 EdbEDA.C:335
 EdbEDA.C:336
 EdbEDA.C:337
 EdbEDA.C:338
 EdbEDA.C:339
 EdbEDA.C:340
 EdbEDA.C:341
 EdbEDA.C:342
 EdbEDA.C:343
 EdbEDA.C:344
 EdbEDA.C:345
 EdbEDA.C:346
 EdbEDA.C:347
 EdbEDA.C:348
 EdbEDA.C:349
 EdbEDA.C:350
 EdbEDA.C:351
 EdbEDA.C:352
 EdbEDA.C:353
 EdbEDA.C:354
 EdbEDA.C:355
 EdbEDA.C:356
 EdbEDA.C:357
 EdbEDA.C:358
 EdbEDA.C:359
 EdbEDA.C:360
 EdbEDA.C:361
 EdbEDA.C:362
 EdbEDA.C:363
 EdbEDA.C:364
 EdbEDA.C:365
 EdbEDA.C:366
 EdbEDA.C:367
 EdbEDA.C:368
 EdbEDA.C:369
 EdbEDA.C:370
 EdbEDA.C:371
 EdbEDA.C:372
 EdbEDA.C:373
 EdbEDA.C:374
 EdbEDA.C:375
 EdbEDA.C:376
 EdbEDA.C:377
 EdbEDA.C:378
 EdbEDA.C:379
 EdbEDA.C:380
 EdbEDA.C:381
 EdbEDA.C:382
 EdbEDA.C:383
 EdbEDA.C:384
 EdbEDA.C:385
 EdbEDA.C:386
 EdbEDA.C:387
 EdbEDA.C:388
 EdbEDA.C:389
 EdbEDA.C:390
 EdbEDA.C:391
 EdbEDA.C:392
 EdbEDA.C:393
 EdbEDA.C:394
 EdbEDA.C:395
 EdbEDA.C:396
 EdbEDA.C:397
 EdbEDA.C:398
 EdbEDA.C:399
 EdbEDA.C:400
 EdbEDA.C:401
 EdbEDA.C:402
 EdbEDA.C:403
 EdbEDA.C:404
 EdbEDA.C:405
 EdbEDA.C:406
 EdbEDA.C:407
 EdbEDA.C:408
 EdbEDA.C:409
 EdbEDA.C:410
 EdbEDA.C:411
 EdbEDA.C:412
 EdbEDA.C:413
 EdbEDA.C:414
 EdbEDA.C:415
 EdbEDA.C:416
 EdbEDA.C:417
 EdbEDA.C:418
 EdbEDA.C:419
 EdbEDA.C:420
 EdbEDA.C:421
 EdbEDA.C:422
 EdbEDA.C:423
 EdbEDA.C:424
 EdbEDA.C:425
 EdbEDA.C:426
 EdbEDA.C:427
 EdbEDA.C:428
 EdbEDA.C:429
 EdbEDA.C:430
 EdbEDA.C:431
 EdbEDA.C:432
 EdbEDA.C:433
 EdbEDA.C:434
 EdbEDA.C:435
 EdbEDA.C:436
 EdbEDA.C:437
 EdbEDA.C:438
 EdbEDA.C:439
 EdbEDA.C:440
 EdbEDA.C:441
 EdbEDA.C:442
 EdbEDA.C:443
 EdbEDA.C:444
 EdbEDA.C:445
 EdbEDA.C:446
 EdbEDA.C:447
 EdbEDA.C:448
 EdbEDA.C:449
 EdbEDA.C:450
 EdbEDA.C:451
 EdbEDA.C:452
 EdbEDA.C:453
 EdbEDA.C:454
 EdbEDA.C:455
 EdbEDA.C:456
 EdbEDA.C:457
 EdbEDA.C:458
 EdbEDA.C:459
 EdbEDA.C:460
 EdbEDA.C:461
 EdbEDA.C:462
 EdbEDA.C:463
 EdbEDA.C:464
 EdbEDA.C:465
 EdbEDA.C:466
 EdbEDA.C:467
 EdbEDA.C:468
 EdbEDA.C:469
 EdbEDA.C:470
 EdbEDA.C:471
 EdbEDA.C:472
 EdbEDA.C:473
 EdbEDA.C:474
 EdbEDA.C:475
 EdbEDA.C:476
 EdbEDA.C:477
 EdbEDA.C:478
 EdbEDA.C:479
 EdbEDA.C:480
 EdbEDA.C:481
 EdbEDA.C:482
 EdbEDA.C:483
 EdbEDA.C:484
 EdbEDA.C:485
 EdbEDA.C:486
 EdbEDA.C:487
 EdbEDA.C:488
 EdbEDA.C:489
 EdbEDA.C:490
 EdbEDA.C:491
 EdbEDA.C:492
 EdbEDA.C:493
 EdbEDA.C:494
 EdbEDA.C:495
 EdbEDA.C:496
 EdbEDA.C:497
 EdbEDA.C:498
 EdbEDA.C:499
 EdbEDA.C:500
 EdbEDA.C:501
 EdbEDA.C:502
 EdbEDA.C:503
 EdbEDA.C:504
 EdbEDA.C:505
 EdbEDA.C:506
 EdbEDA.C:507
 EdbEDA.C:508
 EdbEDA.C:509
 EdbEDA.C:510
 EdbEDA.C:511
 EdbEDA.C:512
 EdbEDA.C:513
 EdbEDA.C:514
 EdbEDA.C:515
 EdbEDA.C:516
 EdbEDA.C:517
 EdbEDA.C:518
 EdbEDA.C:519
 EdbEDA.C:520
 EdbEDA.C:521
 EdbEDA.C:522
 EdbEDA.C:523
 EdbEDA.C:524
 EdbEDA.C:525
 EdbEDA.C:526
 EdbEDA.C:527
 EdbEDA.C:528
 EdbEDA.C:529
 EdbEDA.C:530
 EdbEDA.C:531
 EdbEDA.C:532
 EdbEDA.C:533
 EdbEDA.C:534
 EdbEDA.C:535
 EdbEDA.C:536
 EdbEDA.C:537
 EdbEDA.C:538
 EdbEDA.C:539
 EdbEDA.C:540
 EdbEDA.C:541
 EdbEDA.C:542
 EdbEDA.C:543
 EdbEDA.C:544
 EdbEDA.C:545
 EdbEDA.C:546
 EdbEDA.C:547
 EdbEDA.C:548
 EdbEDA.C:549
 EdbEDA.C:550
 EdbEDA.C:551
 EdbEDA.C:552
 EdbEDA.C:553
 EdbEDA.C:554
 EdbEDA.C:555
 EdbEDA.C:556
 EdbEDA.C:557
 EdbEDA.C:558
 EdbEDA.C:559
 EdbEDA.C:560
 EdbEDA.C:561
 EdbEDA.C:562
 EdbEDA.C:563
 EdbEDA.C:564
 EdbEDA.C:565
 EdbEDA.C:566
 EdbEDA.C:567
 EdbEDA.C:568
 EdbEDA.C:569
 EdbEDA.C:570
 EdbEDA.C:571
 EdbEDA.C:572
 EdbEDA.C:573
 EdbEDA.C:574
 EdbEDA.C:575
 EdbEDA.C:576
 EdbEDA.C:577
 EdbEDA.C:578
 EdbEDA.C:579
 EdbEDA.C:580
 EdbEDA.C:581
 EdbEDA.C:582
 EdbEDA.C:583
 EdbEDA.C:584
 EdbEDA.C:585
 EdbEDA.C:586
 EdbEDA.C:587
 EdbEDA.C:588
 EdbEDA.C:589
 EdbEDA.C:590
 EdbEDA.C:591
 EdbEDA.C:592
 EdbEDA.C:593
 EdbEDA.C:594
 EdbEDA.C:595
 EdbEDA.C:596
 EdbEDA.C:597
 EdbEDA.C:598
 EdbEDA.C:599
 EdbEDA.C:600
 EdbEDA.C:601
 EdbEDA.C:602
 EdbEDA.C:603
 EdbEDA.C:604
 EdbEDA.C:605
 EdbEDA.C:606
 EdbEDA.C:607
 EdbEDA.C:608
 EdbEDA.C:609
 EdbEDA.C:610
 EdbEDA.C:611
 EdbEDA.C:612
 EdbEDA.C:613
 EdbEDA.C:614
 EdbEDA.C:615
 EdbEDA.C:616
 EdbEDA.C:617
 EdbEDA.C:618
 EdbEDA.C:619
 EdbEDA.C:620
 EdbEDA.C:621
 EdbEDA.C:622
 EdbEDA.C:623
 EdbEDA.C:624
 EdbEDA.C:625
 EdbEDA.C:626
 EdbEDA.C:627
 EdbEDA.C:628
 EdbEDA.C:629
 EdbEDA.C:630
 EdbEDA.C:631
 EdbEDA.C:632
 EdbEDA.C:633
 EdbEDA.C:634
 EdbEDA.C:635
 EdbEDA.C:636
 EdbEDA.C:637
 EdbEDA.C:638
 EdbEDA.C:639
 EdbEDA.C:640
 EdbEDA.C:641
 EdbEDA.C:642
 EdbEDA.C:643
 EdbEDA.C:644
 EdbEDA.C:645
 EdbEDA.C:646
 EdbEDA.C:647
 EdbEDA.C:648
 EdbEDA.C:649
 EdbEDA.C:650
 EdbEDA.C:651
 EdbEDA.C:652
 EdbEDA.C:653
 EdbEDA.C:654
 EdbEDA.C:655
 EdbEDA.C:656
 EdbEDA.C:657
 EdbEDA.C:658
 EdbEDA.C:659
 EdbEDA.C:660
 EdbEDA.C:661
 EdbEDA.C:662
 EdbEDA.C:663
 EdbEDA.C:664
 EdbEDA.C:665
 EdbEDA.C:666
 EdbEDA.C:667
 EdbEDA.C:668
 EdbEDA.C:669
 EdbEDA.C:670
 EdbEDA.C:671
 EdbEDA.C:672
 EdbEDA.C:673
 EdbEDA.C:674
 EdbEDA.C:675
 EdbEDA.C:676
 EdbEDA.C:677
 EdbEDA.C:678
 EdbEDA.C:679
 EdbEDA.C:680
 EdbEDA.C:681
 EdbEDA.C:682
 EdbEDA.C:683
 EdbEDA.C:684
 EdbEDA.C:685
 EdbEDA.C:686
 EdbEDA.C:687
 EdbEDA.C:688
 EdbEDA.C:689
 EdbEDA.C:690
 EdbEDA.C:691
 EdbEDA.C:692
 EdbEDA.C:693
 EdbEDA.C:694
 EdbEDA.C:695
 EdbEDA.C:696
 EdbEDA.C:697
 EdbEDA.C:698
 EdbEDA.C:699
 EdbEDA.C:700
 EdbEDA.C:701
 EdbEDA.C:702
 EdbEDA.C:703
 EdbEDA.C:704
 EdbEDA.C:705
 EdbEDA.C:706
 EdbEDA.C:707
 EdbEDA.C:708
 EdbEDA.C:709
 EdbEDA.C:710
 EdbEDA.C:711
 EdbEDA.C:712
 EdbEDA.C:713
 EdbEDA.C:714
 EdbEDA.C:715
 EdbEDA.C:716
 EdbEDA.C:717
 EdbEDA.C:718
 EdbEDA.C:719
 EdbEDA.C:720
 EdbEDA.C:721
 EdbEDA.C:722
 EdbEDA.C:723
 EdbEDA.C:724
 EdbEDA.C:725
 EdbEDA.C:726
 EdbEDA.C:727
 EdbEDA.C:728
 EdbEDA.C:729
 EdbEDA.C:730
 EdbEDA.C:731
 EdbEDA.C:732
 EdbEDA.C:733
 EdbEDA.C:734
 EdbEDA.C:735
 EdbEDA.C:736
 EdbEDA.C:737
 EdbEDA.C:738
 EdbEDA.C:739
 EdbEDA.C:740
 EdbEDA.C:741
 EdbEDA.C:742
 EdbEDA.C:743
 EdbEDA.C:744
 EdbEDA.C:745
 EdbEDA.C:746
 EdbEDA.C:747
 EdbEDA.C:748
 EdbEDA.C:749
 EdbEDA.C:750
 EdbEDA.C:751
 EdbEDA.C:752
 EdbEDA.C:753
 EdbEDA.C:754
 EdbEDA.C:755
 EdbEDA.C:756
 EdbEDA.C:757
 EdbEDA.C:758
 EdbEDA.C:759
 EdbEDA.C:760
 EdbEDA.C:761
 EdbEDA.C:762
 EdbEDA.C:763
 EdbEDA.C:764
 EdbEDA.C:765
 EdbEDA.C:766
 EdbEDA.C:767
 EdbEDA.C:768
 EdbEDA.C:769
 EdbEDA.C:770
 EdbEDA.C:771
 EdbEDA.C:772
 EdbEDA.C:773
 EdbEDA.C:774
 EdbEDA.C:775
 EdbEDA.C:776
 EdbEDA.C:777
 EdbEDA.C:778
 EdbEDA.C:779
 EdbEDA.C:780
 EdbEDA.C:781
 EdbEDA.C:782
 EdbEDA.C:783
 EdbEDA.C:784
 EdbEDA.C:785
 EdbEDA.C:786
 EdbEDA.C:787
 EdbEDA.C:788
 EdbEDA.C:789
 EdbEDA.C:790
 EdbEDA.C:791
 EdbEDA.C:792
 EdbEDA.C:793
 EdbEDA.C:794
 EdbEDA.C:795
 EdbEDA.C:796
 EdbEDA.C:797
 EdbEDA.C:798
 EdbEDA.C:799
 EdbEDA.C:800
 EdbEDA.C:801
 EdbEDA.C:802
 EdbEDA.C:803
 EdbEDA.C:804
 EdbEDA.C:805
 EdbEDA.C:806
 EdbEDA.C:807
 EdbEDA.C:808
 EdbEDA.C:809
 EdbEDA.C:810
 EdbEDA.C:811
 EdbEDA.C:812
 EdbEDA.C:813
 EdbEDA.C:814
 EdbEDA.C:815
 EdbEDA.C:816
 EdbEDA.C:817
 EdbEDA.C:818
 EdbEDA.C:819
 EdbEDA.C:820
 EdbEDA.C:821
 EdbEDA.C:822
 EdbEDA.C:823
 EdbEDA.C:824
 EdbEDA.C:825
 EdbEDA.C:826
 EdbEDA.C:827
 EdbEDA.C:828
 EdbEDA.C:829
 EdbEDA.C:830
 EdbEDA.C:831
 EdbEDA.C:832
 EdbEDA.C:833
 EdbEDA.C:834
 EdbEDA.C:835
 EdbEDA.C:836
 EdbEDA.C:837
 EdbEDA.C:838
 EdbEDA.C:839
 EdbEDA.C:840
 EdbEDA.C:841
 EdbEDA.C:842
 EdbEDA.C:843
 EdbEDA.C:844
 EdbEDA.C:845
 EdbEDA.C:846
 EdbEDA.C:847
 EdbEDA.C:848
 EdbEDA.C:849
 EdbEDA.C:850
 EdbEDA.C:851
 EdbEDA.C:852
 EdbEDA.C:853
 EdbEDA.C:854
 EdbEDA.C:855
 EdbEDA.C:856
 EdbEDA.C:857
 EdbEDA.C:858
 EdbEDA.C:859
 EdbEDA.C:860
 EdbEDA.C:861
 EdbEDA.C:862
 EdbEDA.C:863
 EdbEDA.C:864
 EdbEDA.C:865
 EdbEDA.C:866
 EdbEDA.C:867
 EdbEDA.C:868
 EdbEDA.C:869
 EdbEDA.C:870
 EdbEDA.C:871
 EdbEDA.C:872
 EdbEDA.C:873
 EdbEDA.C:874
 EdbEDA.C:875
 EdbEDA.C:876
 EdbEDA.C:877
 EdbEDA.C:878
 EdbEDA.C:879
 EdbEDA.C:880
 EdbEDA.C:881
 EdbEDA.C:882
 EdbEDA.C:883
 EdbEDA.C:884
 EdbEDA.C:885
 EdbEDA.C:886
 EdbEDA.C:887
 EdbEDA.C:888
 EdbEDA.C:889
 EdbEDA.C:890
 EdbEDA.C:891
 EdbEDA.C:892
 EdbEDA.C:893
 EdbEDA.C:894
 EdbEDA.C:895
 EdbEDA.C:896
 EdbEDA.C:897
 EdbEDA.C:898
 EdbEDA.C:899
 EdbEDA.C:900
 EdbEDA.C:901
 EdbEDA.C:902
 EdbEDA.C:903
 EdbEDA.C:904
 EdbEDA.C:905
 EdbEDA.C:906
 EdbEDA.C:907
 EdbEDA.C:908
 EdbEDA.C:909
 EdbEDA.C:910
 EdbEDA.C:911
 EdbEDA.C:912
 EdbEDA.C:913
 EdbEDA.C:914
 EdbEDA.C:915
 EdbEDA.C:916
 EdbEDA.C:917
 EdbEDA.C:918
 EdbEDA.C:919
 EdbEDA.C:920
 EdbEDA.C:921
 EdbEDA.C:922
 EdbEDA.C:923
 EdbEDA.C:924
 EdbEDA.C:925
 EdbEDA.C:926
 EdbEDA.C:927
 EdbEDA.C:928
 EdbEDA.C:929
 EdbEDA.C:930
 EdbEDA.C:931
 EdbEDA.C:932
 EdbEDA.C:933
 EdbEDA.C:934
 EdbEDA.C:935
 EdbEDA.C:936
 EdbEDA.C:937
 EdbEDA.C:938
 EdbEDA.C:939
 EdbEDA.C:940
 EdbEDA.C:941
 EdbEDA.C:942
 EdbEDA.C:943
 EdbEDA.C:944
 EdbEDA.C:945
 EdbEDA.C:946
 EdbEDA.C:947
 EdbEDA.C:948
 EdbEDA.C:949
 EdbEDA.C:950
 EdbEDA.C:951
 EdbEDA.C:952
 EdbEDA.C:953
 EdbEDA.C:954
 EdbEDA.C:955
 EdbEDA.C:956
 EdbEDA.C:957
 EdbEDA.C:958
 EdbEDA.C:959
 EdbEDA.C:960
 EdbEDA.C:961
 EdbEDA.C:962
 EdbEDA.C:963
 EdbEDA.C:964
 EdbEDA.C:965
 EdbEDA.C:966
 EdbEDA.C:967
 EdbEDA.C:968
 EdbEDA.C:969
 EdbEDA.C:970
 EdbEDA.C:971
 EdbEDA.C:972
 EdbEDA.C:973
 EdbEDA.C:974
 EdbEDA.C:975
 EdbEDA.C:976
 EdbEDA.C:977
 EdbEDA.C:978
 EdbEDA.C:979
 EdbEDA.C:980
 EdbEDA.C:981
 EdbEDA.C:982
 EdbEDA.C:983
 EdbEDA.C:984
 EdbEDA.C:985
 EdbEDA.C:986
 EdbEDA.C:987
 EdbEDA.C:988
 EdbEDA.C:989
 EdbEDA.C:990
 EdbEDA.C:991
 EdbEDA.C:992
 EdbEDA.C:993
 EdbEDA.C:994
 EdbEDA.C:995
 EdbEDA.C:996
 EdbEDA.C:997
 EdbEDA.C:998
 EdbEDA.C:999
 EdbEDA.C:1000
 EdbEDA.C:1001
 EdbEDA.C:1002
 EdbEDA.C:1003
 EdbEDA.C:1004
 EdbEDA.C:1005
 EdbEDA.C:1006
 EdbEDA.C:1007
 EdbEDA.C:1008
 EdbEDA.C:1009
 EdbEDA.C:1010
 EdbEDA.C:1011
 EdbEDA.C:1012
 EdbEDA.C:1013
 EdbEDA.C:1014
 EdbEDA.C:1015
 EdbEDA.C:1016
 EdbEDA.C:1017
 EdbEDA.C:1018
 EdbEDA.C:1019
 EdbEDA.C:1020
 EdbEDA.C:1021
 EdbEDA.C:1022
 EdbEDA.C:1023
 EdbEDA.C:1024
 EdbEDA.C:1025
 EdbEDA.C:1026
 EdbEDA.C:1027
 EdbEDA.C:1028
 EdbEDA.C:1029
 EdbEDA.C:1030
 EdbEDA.C:1031
 EdbEDA.C:1032
 EdbEDA.C:1033
 EdbEDA.C:1034
 EdbEDA.C:1035
 EdbEDA.C:1036
 EdbEDA.C:1037
 EdbEDA.C:1038
 EdbEDA.C:1039
 EdbEDA.C:1040
 EdbEDA.C:1041
 EdbEDA.C:1042
 EdbEDA.C:1043
 EdbEDA.C:1044
 EdbEDA.C:1045
 EdbEDA.C:1046
 EdbEDA.C:1047
 EdbEDA.C:1048
 EdbEDA.C:1049
 EdbEDA.C:1050
 EdbEDA.C:1051
 EdbEDA.C:1052
 EdbEDA.C:1053
 EdbEDA.C:1054
 EdbEDA.C:1055
 EdbEDA.C:1056
 EdbEDA.C:1057
 EdbEDA.C:1058
 EdbEDA.C:1059
 EdbEDA.C:1060
 EdbEDA.C:1061
 EdbEDA.C:1062
 EdbEDA.C:1063
 EdbEDA.C:1064
 EdbEDA.C:1065
 EdbEDA.C:1066
 EdbEDA.C:1067
 EdbEDA.C:1068
 EdbEDA.C:1069
 EdbEDA.C:1070
 EdbEDA.C:1071
 EdbEDA.C:1072
 EdbEDA.C:1073
 EdbEDA.C:1074
 EdbEDA.C:1075
 EdbEDA.C:1076
 EdbEDA.C:1077
 EdbEDA.C:1078
 EdbEDA.C:1079
 EdbEDA.C:1080
 EdbEDA.C:1081
 EdbEDA.C:1082
 EdbEDA.C:1083
 EdbEDA.C:1084
 EdbEDA.C:1085
 EdbEDA.C:1086
 EdbEDA.C:1087
 EdbEDA.C:1088
 EdbEDA.C:1089
 EdbEDA.C:1090
 EdbEDA.C:1091
 EdbEDA.C:1092
 EdbEDA.C:1093
 EdbEDA.C:1094
 EdbEDA.C:1095
 EdbEDA.C:1096
 EdbEDA.C:1097
 EdbEDA.C:1098
 EdbEDA.C:1099
 EdbEDA.C:1100
 EdbEDA.C:1101
 EdbEDA.C:1102
 EdbEDA.C:1103
 EdbEDA.C:1104
 EdbEDA.C:1105
 EdbEDA.C:1106
 EdbEDA.C:1107
 EdbEDA.C:1108
 EdbEDA.C:1109
 EdbEDA.C:1110
 EdbEDA.C:1111
 EdbEDA.C:1112
 EdbEDA.C:1113
 EdbEDA.C:1114
 EdbEDA.C:1115
 EdbEDA.C:1116
 EdbEDA.C:1117
 EdbEDA.C:1118
 EdbEDA.C:1119
 EdbEDA.C:1120
 EdbEDA.C:1121
 EdbEDA.C:1122
 EdbEDA.C:1123
 EdbEDA.C:1124
 EdbEDA.C:1125
 EdbEDA.C:1126
 EdbEDA.C:1127
 EdbEDA.C:1128
 EdbEDA.C:1129
 EdbEDA.C:1130
 EdbEDA.C:1131
 EdbEDA.C:1132
 EdbEDA.C:1133
 EdbEDA.C:1134
 EdbEDA.C:1135
 EdbEDA.C:1136
 EdbEDA.C:1137
 EdbEDA.C:1138
 EdbEDA.C:1139
 EdbEDA.C:1140
 EdbEDA.C:1141
 EdbEDA.C:1142
 EdbEDA.C:1143
 EdbEDA.C:1144
 EdbEDA.C:1145
 EdbEDA.C:1146
 EdbEDA.C:1147
 EdbEDA.C:1148
 EdbEDA.C:1149
 EdbEDA.C:1150
 EdbEDA.C:1151
 EdbEDA.C:1152
 EdbEDA.C:1153
 EdbEDA.C:1154
 EdbEDA.C:1155
 EdbEDA.C:1156
 EdbEDA.C:1157
 EdbEDA.C:1158
 EdbEDA.C:1159
 EdbEDA.C:1160
 EdbEDA.C:1161
 EdbEDA.C:1162
 EdbEDA.C:1163
 EdbEDA.C:1164
 EdbEDA.C:1165
 EdbEDA.C:1166
 EdbEDA.C:1167
 EdbEDA.C:1168
 EdbEDA.C:1169
 EdbEDA.C:1170
 EdbEDA.C:1171
 EdbEDA.C:1172
 EdbEDA.C:1173
 EdbEDA.C:1174
 EdbEDA.C:1175
 EdbEDA.C:1176
 EdbEDA.C:1177
 EdbEDA.C:1178
 EdbEDA.C:1179
 EdbEDA.C:1180
 EdbEDA.C:1181
 EdbEDA.C:1182
 EdbEDA.C:1183
 EdbEDA.C:1184
 EdbEDA.C:1185
 EdbEDA.C:1186
 EdbEDA.C:1187
 EdbEDA.C:1188
 EdbEDA.C:1189
 EdbEDA.C:1190
 EdbEDA.C:1191
 EdbEDA.C:1192
 EdbEDA.C:1193
 EdbEDA.C:1194
 EdbEDA.C:1195
 EdbEDA.C:1196
 EdbEDA.C:1197
 EdbEDA.C:1198
 EdbEDA.C:1199
 EdbEDA.C:1200
 EdbEDA.C:1201
 EdbEDA.C:1202
 EdbEDA.C:1203
 EdbEDA.C:1204
 EdbEDA.C:1205
 EdbEDA.C:1206
 EdbEDA.C:1207
 EdbEDA.C:1208
 EdbEDA.C:1209
 EdbEDA.C:1210
 EdbEDA.C:1211
 EdbEDA.C:1212
 EdbEDA.C:1213
 EdbEDA.C:1214
 EdbEDA.C:1215
 EdbEDA.C:1216
 EdbEDA.C:1217
 EdbEDA.C:1218
 EdbEDA.C:1219
 EdbEDA.C:1220
 EdbEDA.C:1221
 EdbEDA.C:1222
 EdbEDA.C:1223
 EdbEDA.C:1224
 EdbEDA.C:1225
 EdbEDA.C:1226
 EdbEDA.C:1227
 EdbEDA.C:1228
 EdbEDA.C:1229
 EdbEDA.C:1230
 EdbEDA.C:1231
 EdbEDA.C:1232
 EdbEDA.C:1233
 EdbEDA.C:1234
 EdbEDA.C:1235
 EdbEDA.C:1236
 EdbEDA.C:1237
 EdbEDA.C:1238
 EdbEDA.C:1239
 EdbEDA.C:1240
 EdbEDA.C:1241
 EdbEDA.C:1242
 EdbEDA.C:1243
 EdbEDA.C:1244
 EdbEDA.C:1245
 EdbEDA.C:1246
 EdbEDA.C:1247
 EdbEDA.C:1248
 EdbEDA.C:1249
 EdbEDA.C:1250
 EdbEDA.C:1251
 EdbEDA.C:1252
 EdbEDA.C:1253
 EdbEDA.C:1254
 EdbEDA.C:1255
 EdbEDA.C:1256
 EdbEDA.C:1257
 EdbEDA.C:1258
 EdbEDA.C:1259
 EdbEDA.C:1260
 EdbEDA.C:1261
 EdbEDA.C:1262
 EdbEDA.C:1263
 EdbEDA.C:1264
 EdbEDA.C:1265
 EdbEDA.C:1266
 EdbEDA.C:1267
 EdbEDA.C:1268
 EdbEDA.C:1269
 EdbEDA.C:1270
 EdbEDA.C:1271
 EdbEDA.C:1272
 EdbEDA.C:1273
 EdbEDA.C:1274
 EdbEDA.C:1275
 EdbEDA.C:1276
 EdbEDA.C:1277
 EdbEDA.C:1278
 EdbEDA.C:1279
 EdbEDA.C:1280
 EdbEDA.C:1281
 EdbEDA.C:1282
 EdbEDA.C:1283
 EdbEDA.C:1284
 EdbEDA.C:1285
 EdbEDA.C:1286
 EdbEDA.C:1287
 EdbEDA.C:1288
 EdbEDA.C:1289
 EdbEDA.C:1290
 EdbEDA.C:1291
 EdbEDA.C:1292
 EdbEDA.C:1293
 EdbEDA.C:1294
 EdbEDA.C:1295
 EdbEDA.C:1296
 EdbEDA.C:1297
 EdbEDA.C:1298
 EdbEDA.C:1299
 EdbEDA.C:1300
 EdbEDA.C:1301
 EdbEDA.C:1302
 EdbEDA.C:1303
 EdbEDA.C:1304
 EdbEDA.C:1305
 EdbEDA.C:1306
 EdbEDA.C:1307
 EdbEDA.C:1308
 EdbEDA.C:1309
 EdbEDA.C:1310
 EdbEDA.C:1311
 EdbEDA.C:1312
 EdbEDA.C:1313
 EdbEDA.C:1314
 EdbEDA.C:1315
 EdbEDA.C:1316
 EdbEDA.C:1317
 EdbEDA.C:1318
 EdbEDA.C:1319
 EdbEDA.C:1320
 EdbEDA.C:1321
 EdbEDA.C:1322
 EdbEDA.C:1323
 EdbEDA.C:1324
 EdbEDA.C:1325
 EdbEDA.C:1326
 EdbEDA.C:1327
 EdbEDA.C:1328
 EdbEDA.C:1329
 EdbEDA.C:1330
 EdbEDA.C:1331
 EdbEDA.C:1332
 EdbEDA.C:1333
 EdbEDA.C:1334
 EdbEDA.C:1335
 EdbEDA.C:1336
 EdbEDA.C:1337
 EdbEDA.C:1338
 EdbEDA.C:1339
 EdbEDA.C:1340
 EdbEDA.C:1341
 EdbEDA.C:1342
 EdbEDA.C:1343
 EdbEDA.C:1344
 EdbEDA.C:1345
 EdbEDA.C:1346
 EdbEDA.C:1347
 EdbEDA.C:1348
 EdbEDA.C:1349
 EdbEDA.C:1350
 EdbEDA.C:1351
 EdbEDA.C:1352
 EdbEDA.C:1353
 EdbEDA.C:1354
 EdbEDA.C:1355
 EdbEDA.C:1356
 EdbEDA.C:1357
 EdbEDA.C:1358
 EdbEDA.C:1359
 EdbEDA.C:1360
 EdbEDA.C:1361
 EdbEDA.C:1362
 EdbEDA.C:1363
 EdbEDA.C:1364
 EdbEDA.C:1365
 EdbEDA.C:1366
 EdbEDA.C:1367
 EdbEDA.C:1368
 EdbEDA.C:1369
 EdbEDA.C:1370
 EdbEDA.C:1371
 EdbEDA.C:1372
 EdbEDA.C:1373
 EdbEDA.C:1374
 EdbEDA.C:1375
 EdbEDA.C:1376
 EdbEDA.C:1377
 EdbEDA.C:1378
 EdbEDA.C:1379
 EdbEDA.C:1380
 EdbEDA.C:1381
 EdbEDA.C:1382
 EdbEDA.C:1383
 EdbEDA.C:1384
 EdbEDA.C:1385
 EdbEDA.C:1386
 EdbEDA.C:1387
 EdbEDA.C:1388
 EdbEDA.C:1389
 EdbEDA.C:1390
 EdbEDA.C:1391
 EdbEDA.C:1392
 EdbEDA.C:1393
 EdbEDA.C:1394
 EdbEDA.C:1395
 EdbEDA.C:1396
 EdbEDA.C:1397
 EdbEDA.C:1398
 EdbEDA.C:1399
 EdbEDA.C:1400
 EdbEDA.C:1401
 EdbEDA.C:1402
 EdbEDA.C:1403
 EdbEDA.C:1404
 EdbEDA.C:1405
 EdbEDA.C:1406
 EdbEDA.C:1407
 EdbEDA.C:1408
 EdbEDA.C:1409
 EdbEDA.C:1410
 EdbEDA.C:1411
 EdbEDA.C:1412
 EdbEDA.C:1413
 EdbEDA.C:1414
 EdbEDA.C:1415
 EdbEDA.C:1416
 EdbEDA.C:1417
 EdbEDA.C:1418
 EdbEDA.C:1419
 EdbEDA.C:1420
 EdbEDA.C:1421
 EdbEDA.C:1422
 EdbEDA.C:1423
 EdbEDA.C:1424
 EdbEDA.C:1425
 EdbEDA.C:1426
 EdbEDA.C:1427
 EdbEDA.C:1428
 EdbEDA.C:1429
 EdbEDA.C:1430
 EdbEDA.C:1431
 EdbEDA.C:1432
 EdbEDA.C:1433
 EdbEDA.C:1434
 EdbEDA.C:1435
 EdbEDA.C:1436
 EdbEDA.C:1437
 EdbEDA.C:1438
 EdbEDA.C:1439
 EdbEDA.C:1440
 EdbEDA.C:1441
 EdbEDA.C:1442
 EdbEDA.C:1443
 EdbEDA.C:1444
 EdbEDA.C:1445
 EdbEDA.C:1446
 EdbEDA.C:1447
 EdbEDA.C:1448
 EdbEDA.C:1449
 EdbEDA.C:1450
 EdbEDA.C:1451
 EdbEDA.C:1452
 EdbEDA.C:1453
 EdbEDA.C:1454
 EdbEDA.C:1455
 EdbEDA.C:1456
 EdbEDA.C:1457
 EdbEDA.C:1458
 EdbEDA.C:1459
 EdbEDA.C:1460
 EdbEDA.C:1461
 EdbEDA.C:1462
 EdbEDA.C:1463
 EdbEDA.C:1464
 EdbEDA.C:1465
 EdbEDA.C:1466
 EdbEDA.C:1467
 EdbEDA.C:1468
 EdbEDA.C:1469
 EdbEDA.C:1470
 EdbEDA.C:1471
 EdbEDA.C:1472
 EdbEDA.C:1473
 EdbEDA.C:1474
 EdbEDA.C:1475
 EdbEDA.C:1476
 EdbEDA.C:1477
 EdbEDA.C:1478
 EdbEDA.C:1479
 EdbEDA.C:1480
 EdbEDA.C:1481
 EdbEDA.C:1482
 EdbEDA.C:1483
 EdbEDA.C:1484
 EdbEDA.C:1485
 EdbEDA.C:1486
 EdbEDA.C:1487
 EdbEDA.C:1488
 EdbEDA.C:1489
 EdbEDA.C:1490
 EdbEDA.C:1491
 EdbEDA.C:1492
 EdbEDA.C:1493
 EdbEDA.C:1494
 EdbEDA.C:1495
 EdbEDA.C:1496
 EdbEDA.C:1497
 EdbEDA.C:1498
 EdbEDA.C:1499
 EdbEDA.C:1500
 EdbEDA.C:1501
 EdbEDA.C:1502
 EdbEDA.C:1503
 EdbEDA.C:1504
 EdbEDA.C:1505
 EdbEDA.C:1506
 EdbEDA.C:1507
 EdbEDA.C:1508
 EdbEDA.C:1509
 EdbEDA.C:1510
 EdbEDA.C:1511
 EdbEDA.C:1512
 EdbEDA.C:1513
 EdbEDA.C:1514
 EdbEDA.C:1515
 EdbEDA.C:1516
 EdbEDA.C:1517
 EdbEDA.C:1518
 EdbEDA.C:1519
 EdbEDA.C:1520
 EdbEDA.C:1521
 EdbEDA.C:1522
 EdbEDA.C:1523
 EdbEDA.C:1524
 EdbEDA.C:1525
 EdbEDA.C:1526
 EdbEDA.C:1527
 EdbEDA.C:1528
 EdbEDA.C:1529
 EdbEDA.C:1530
 EdbEDA.C:1531
 EdbEDA.C:1532
 EdbEDA.C:1533
 EdbEDA.C:1534
 EdbEDA.C:1535
 EdbEDA.C:1536
 EdbEDA.C:1537
 EdbEDA.C:1538
 EdbEDA.C:1539
 EdbEDA.C:1540
 EdbEDA.C:1541
 EdbEDA.C:1542
 EdbEDA.C:1543
 EdbEDA.C:1544
 EdbEDA.C:1545
 EdbEDA.C:1546
 EdbEDA.C:1547
 EdbEDA.C:1548
 EdbEDA.C:1549
 EdbEDA.C:1550
 EdbEDA.C:1551
 EdbEDA.C:1552
 EdbEDA.C:1553
 EdbEDA.C:1554
 EdbEDA.C:1555
 EdbEDA.C:1556
 EdbEDA.C:1557
 EdbEDA.C:1558
 EdbEDA.C:1559
 EdbEDA.C:1560
 EdbEDA.C:1561
 EdbEDA.C:1562
 EdbEDA.C:1563
 EdbEDA.C:1564
 EdbEDA.C:1565
 EdbEDA.C:1566
 EdbEDA.C:1567
 EdbEDA.C:1568
 EdbEDA.C:1569
 EdbEDA.C:1570
 EdbEDA.C:1571
 EdbEDA.C:1572
 EdbEDA.C:1573
 EdbEDA.C:1574
 EdbEDA.C:1575
 EdbEDA.C:1576
 EdbEDA.C:1577
 EdbEDA.C:1578
 EdbEDA.C:1579
 EdbEDA.C:1580
 EdbEDA.C:1581
 EdbEDA.C:1582
 EdbEDA.C:1583
 EdbEDA.C:1584
 EdbEDA.C:1585
 EdbEDA.C:1586
 EdbEDA.C:1587
 EdbEDA.C:1588
 EdbEDA.C:1589
 EdbEDA.C:1590
 EdbEDA.C:1591
 EdbEDA.C:1592
 EdbEDA.C:1593
 EdbEDA.C:1594
 EdbEDA.C:1595
 EdbEDA.C:1596
 EdbEDA.C:1597
 EdbEDA.C:1598
 EdbEDA.C:1599
 EdbEDA.C:1600
 EdbEDA.C:1601
 EdbEDA.C:1602
 EdbEDA.C:1603
 EdbEDA.C:1604
 EdbEDA.C:1605
 EdbEDA.C:1606
 EdbEDA.C:1607
 EdbEDA.C:1608
 EdbEDA.C:1609
 EdbEDA.C:1610
 EdbEDA.C:1611
 EdbEDA.C:1612
 EdbEDA.C:1613
 EdbEDA.C:1614
 EdbEDA.C:1615
 EdbEDA.C:1616
 EdbEDA.C:1617
 EdbEDA.C:1618
 EdbEDA.C:1619
 EdbEDA.C:1620
 EdbEDA.C:1621
 EdbEDA.C:1622
 EdbEDA.C:1623
 EdbEDA.C:1624
 EdbEDA.C:1625
 EdbEDA.C:1626
 EdbEDA.C:1627
 EdbEDA.C:1628
 EdbEDA.C:1629
 EdbEDA.C:1630
 EdbEDA.C:1631
 EdbEDA.C:1632
 EdbEDA.C:1633
 EdbEDA.C:1634
 EdbEDA.C:1635
 EdbEDA.C:1636
 EdbEDA.C:1637
 EdbEDA.C:1638
 EdbEDA.C:1639
 EdbEDA.C:1640
 EdbEDA.C:1641
 EdbEDA.C:1642
 EdbEDA.C:1643
 EdbEDA.C:1644
 EdbEDA.C:1645
 EdbEDA.C:1646
 EdbEDA.C:1647
 EdbEDA.C:1648
 EdbEDA.C:1649
 EdbEDA.C:1650
 EdbEDA.C:1651
 EdbEDA.C:1652
 EdbEDA.C:1653
 EdbEDA.C:1654
 EdbEDA.C:1655
 EdbEDA.C:1656
 EdbEDA.C:1657
 EdbEDA.C:1658
 EdbEDA.C:1659
 EdbEDA.C:1660
 EdbEDA.C:1661
 EdbEDA.C:1662
 EdbEDA.C:1663
 EdbEDA.C:1664
 EdbEDA.C:1665
 EdbEDA.C:1666
 EdbEDA.C:1667
 EdbEDA.C:1668
 EdbEDA.C:1669
 EdbEDA.C:1670
 EdbEDA.C:1671
 EdbEDA.C:1672
 EdbEDA.C:1673
 EdbEDA.C:1674
 EdbEDA.C:1675
 EdbEDA.C:1676
 EdbEDA.C:1677
 EdbEDA.C:1678
 EdbEDA.C:1679
 EdbEDA.C:1680
 EdbEDA.C:1681
 EdbEDA.C:1682
 EdbEDA.C:1683
 EdbEDA.C:1684
 EdbEDA.C:1685
 EdbEDA.C:1686
 EdbEDA.C:1687
 EdbEDA.C:1688
 EdbEDA.C:1689
 EdbEDA.C:1690
 EdbEDA.C:1691
 EdbEDA.C:1692
 EdbEDA.C:1693
 EdbEDA.C:1694
 EdbEDA.C:1695
 EdbEDA.C:1696
 EdbEDA.C:1697
 EdbEDA.C:1698
 EdbEDA.C:1699
 EdbEDA.C:1700
 EdbEDA.C:1701
 EdbEDA.C:1702
 EdbEDA.C:1703
 EdbEDA.C:1704
 EdbEDA.C:1705
 EdbEDA.C:1706
 EdbEDA.C:1707
 EdbEDA.C:1708
 EdbEDA.C:1709
 EdbEDA.C:1710
 EdbEDA.C:1711
 EdbEDA.C:1712
 EdbEDA.C:1713
 EdbEDA.C:1714
 EdbEDA.C:1715
 EdbEDA.C:1716
 EdbEDA.C:1717
 EdbEDA.C:1718
 EdbEDA.C:1719
 EdbEDA.C:1720
 EdbEDA.C:1721
 EdbEDA.C:1722
 EdbEDA.C:1723
 EdbEDA.C:1724
 EdbEDA.C:1725
 EdbEDA.C:1726
 EdbEDA.C:1727
 EdbEDA.C:1728
 EdbEDA.C:1729
 EdbEDA.C:1730
 EdbEDA.C:1731
 EdbEDA.C:1732
 EdbEDA.C:1733
 EdbEDA.C:1734
 EdbEDA.C:1735
 EdbEDA.C:1736
 EdbEDA.C:1737
 EdbEDA.C:1738
 EdbEDA.C:1739
 EdbEDA.C:1740
 EdbEDA.C:1741
 EdbEDA.C:1742
 EdbEDA.C:1743
 EdbEDA.C:1744
 EdbEDA.C:1745
 EdbEDA.C:1746
 EdbEDA.C:1747
 EdbEDA.C:1748
 EdbEDA.C:1749
 EdbEDA.C:1750
 EdbEDA.C:1751
 EdbEDA.C:1752
 EdbEDA.C:1753
 EdbEDA.C:1754
 EdbEDA.C:1755
 EdbEDA.C:1756
 EdbEDA.C:1757
 EdbEDA.C:1758
 EdbEDA.C:1759
 EdbEDA.C:1760
 EdbEDA.C:1761
 EdbEDA.C:1762
 EdbEDA.C:1763
 EdbEDA.C:1764
 EdbEDA.C:1765
 EdbEDA.C:1766
 EdbEDA.C:1767
 EdbEDA.C:1768
 EdbEDA.C:1769
 EdbEDA.C:1770
 EdbEDA.C:1771
 EdbEDA.C:1772
 EdbEDA.C:1773
 EdbEDA.C:1774
 EdbEDA.C:1775
 EdbEDA.C:1776
 EdbEDA.C:1777
 EdbEDA.C:1778
 EdbEDA.C:1779
 EdbEDA.C:1780
 EdbEDA.C:1781
 EdbEDA.C:1782
 EdbEDA.C:1783
 EdbEDA.C:1784
 EdbEDA.C:1785
 EdbEDA.C:1786
 EdbEDA.C:1787
 EdbEDA.C:1788
 EdbEDA.C:1789
 EdbEDA.C:1790
 EdbEDA.C:1791
 EdbEDA.C:1792
 EdbEDA.C:1793
 EdbEDA.C:1794
 EdbEDA.C:1795
 EdbEDA.C:1796
 EdbEDA.C:1797
 EdbEDA.C:1798
 EdbEDA.C:1799
 EdbEDA.C:1800
 EdbEDA.C:1801
 EdbEDA.C:1802
 EdbEDA.C:1803
 EdbEDA.C:1804
 EdbEDA.C:1805
 EdbEDA.C:1806
 EdbEDA.C:1807
 EdbEDA.C:1808
 EdbEDA.C:1809
 EdbEDA.C:1810
 EdbEDA.C:1811
 EdbEDA.C:1812
 EdbEDA.C:1813
 EdbEDA.C:1814
 EdbEDA.C:1815
 EdbEDA.C:1816
 EdbEDA.C:1817
 EdbEDA.C:1818
 EdbEDA.C:1819
 EdbEDA.C:1820
 EdbEDA.C:1821
 EdbEDA.C:1822
 EdbEDA.C:1823
 EdbEDA.C:1824
 EdbEDA.C:1825
 EdbEDA.C:1826
 EdbEDA.C:1827
 EdbEDA.C:1828
 EdbEDA.C:1829
 EdbEDA.C:1830
 EdbEDA.C:1831
 EdbEDA.C:1832
 EdbEDA.C:1833
 EdbEDA.C:1834
 EdbEDA.C:1835
 EdbEDA.C:1836
 EdbEDA.C:1837
 EdbEDA.C:1838
 EdbEDA.C:1839
 EdbEDA.C:1840
 EdbEDA.C:1841
 EdbEDA.C:1842
 EdbEDA.C:1843
 EdbEDA.C:1844
 EdbEDA.C:1845
 EdbEDA.C:1846
 EdbEDA.C:1847
 EdbEDA.C:1848
 EdbEDA.C:1849
 EdbEDA.C:1850
 EdbEDA.C:1851
 EdbEDA.C:1852
 EdbEDA.C:1853
 EdbEDA.C:1854
 EdbEDA.C:1855
 EdbEDA.C:1856
 EdbEDA.C:1857
 EdbEDA.C:1858
 EdbEDA.C:1859
 EdbEDA.C:1860
 EdbEDA.C:1861
 EdbEDA.C:1862
 EdbEDA.C:1863
 EdbEDA.C:1864
 EdbEDA.C:1865
 EdbEDA.C:1866
 EdbEDA.C:1867
 EdbEDA.C:1868
 EdbEDA.C:1869
 EdbEDA.C:1870
 EdbEDA.C:1871
 EdbEDA.C:1872
 EdbEDA.C:1873
 EdbEDA.C:1874
 EdbEDA.C:1875
 EdbEDA.C:1876
 EdbEDA.C:1877
 EdbEDA.C:1878
 EdbEDA.C:1879
 EdbEDA.C:1880
 EdbEDA.C:1881
 EdbEDA.C:1882
 EdbEDA.C:1883
 EdbEDA.C:1884
 EdbEDA.C:1885
 EdbEDA.C:1886
 EdbEDA.C:1887
 EdbEDA.C:1888
 EdbEDA.C:1889
 EdbEDA.C:1890
 EdbEDA.C:1891
 EdbEDA.C:1892
 EdbEDA.C:1893
 EdbEDA.C:1894
 EdbEDA.C:1895
 EdbEDA.C:1896
 EdbEDA.C:1897
 EdbEDA.C:1898
 EdbEDA.C:1899
 EdbEDA.C:1900
 EdbEDA.C:1901
 EdbEDA.C:1902
 EdbEDA.C:1903
 EdbEDA.C:1904
 EdbEDA.C:1905
 EdbEDA.C:1906
 EdbEDA.C:1907
 EdbEDA.C:1908
 EdbEDA.C:1909
 EdbEDA.C:1910
 EdbEDA.C:1911
 EdbEDA.C:1912
 EdbEDA.C:1913
 EdbEDA.C:1914
 EdbEDA.C:1915
 EdbEDA.C:1916
 EdbEDA.C:1917
 EdbEDA.C:1918
 EdbEDA.C:1919
 EdbEDA.C:1920
 EdbEDA.C:1921
 EdbEDA.C:1922
 EdbEDA.C:1923
 EdbEDA.C:1924
 EdbEDA.C:1925
 EdbEDA.C:1926
 EdbEDA.C:1927
 EdbEDA.C:1928
 EdbEDA.C:1929
 EdbEDA.C:1930
 EdbEDA.C:1931
 EdbEDA.C:1932
 EdbEDA.C:1933
 EdbEDA.C:1934
 EdbEDA.C:1935
 EdbEDA.C:1936
 EdbEDA.C:1937
 EdbEDA.C:1938
 EdbEDA.C:1939
 EdbEDA.C:1940
 EdbEDA.C:1941
 EdbEDA.C:1942
 EdbEDA.C:1943
 EdbEDA.C:1944
 EdbEDA.C:1945
 EdbEDA.C:1946
 EdbEDA.C:1947
 EdbEDA.C:1948
 EdbEDA.C:1949
 EdbEDA.C:1950
 EdbEDA.C:1951
 EdbEDA.C:1952
 EdbEDA.C:1953
 EdbEDA.C:1954
 EdbEDA.C:1955
 EdbEDA.C:1956
 EdbEDA.C:1957
 EdbEDA.C:1958
 EdbEDA.C:1959
 EdbEDA.C:1960
 EdbEDA.C:1961
 EdbEDA.C:1962
 EdbEDA.C:1963
 EdbEDA.C:1964
 EdbEDA.C:1965
 EdbEDA.C:1966
 EdbEDA.C:1967
 EdbEDA.C:1968
 EdbEDA.C:1969
 EdbEDA.C:1970
 EdbEDA.C:1971
 EdbEDA.C:1972
 EdbEDA.C:1973
 EdbEDA.C:1974
 EdbEDA.C:1975
 EdbEDA.C:1976
 EdbEDA.C:1977
 EdbEDA.C:1978
 EdbEDA.C:1979
 EdbEDA.C:1980
 EdbEDA.C:1981
 EdbEDA.C:1982
 EdbEDA.C:1983
 EdbEDA.C:1984
 EdbEDA.C:1985
 EdbEDA.C:1986
 EdbEDA.C:1987
 EdbEDA.C:1988
 EdbEDA.C:1989
 EdbEDA.C:1990
 EdbEDA.C:1991
 EdbEDA.C:1992
 EdbEDA.C:1993
 EdbEDA.C:1994
 EdbEDA.C:1995
 EdbEDA.C:1996
 EdbEDA.C:1997
 EdbEDA.C:1998
 EdbEDA.C:1999
 EdbEDA.C:2000
 EdbEDA.C:2001
 EdbEDA.C:2002
 EdbEDA.C:2003
 EdbEDA.C:2004
 EdbEDA.C:2005
 EdbEDA.C:2006
 EdbEDA.C:2007
 EdbEDA.C:2008
 EdbEDA.C:2009
 EdbEDA.C:2010
 EdbEDA.C:2011
 EdbEDA.C:2012
 EdbEDA.C:2013
 EdbEDA.C:2014
 EdbEDA.C:2015
 EdbEDA.C:2016
 EdbEDA.C:2017
 EdbEDA.C:2018
 EdbEDA.C:2019
 EdbEDA.C:2020
 EdbEDA.C:2021
 EdbEDA.C:2022
 EdbEDA.C:2023
 EdbEDA.C:2024
 EdbEDA.C:2025
 EdbEDA.C:2026
 EdbEDA.C:2027
 EdbEDA.C:2028
 EdbEDA.C:2029
 EdbEDA.C:2030
 EdbEDA.C:2031
 EdbEDA.C:2032
 EdbEDA.C:2033
 EdbEDA.C:2034
 EdbEDA.C:2035
 EdbEDA.C:2036
 EdbEDA.C:2037
 EdbEDA.C:2038
 EdbEDA.C:2039
 EdbEDA.C:2040
 EdbEDA.C:2041
 EdbEDA.C:2042
 EdbEDA.C:2043
 EdbEDA.C:2044
 EdbEDA.C:2045
 EdbEDA.C:2046
 EdbEDA.C:2047
 EdbEDA.C:2048
 EdbEDA.C:2049
 EdbEDA.C:2050
 EdbEDA.C:2051
 EdbEDA.C:2052
 EdbEDA.C:2053
 EdbEDA.C:2054
 EdbEDA.C:2055
 EdbEDA.C:2056
 EdbEDA.C:2057
 EdbEDA.C:2058
 EdbEDA.C:2059
 EdbEDA.C:2060
 EdbEDA.C:2061
 EdbEDA.C:2062
 EdbEDA.C:2063
 EdbEDA.C:2064
 EdbEDA.C:2065
 EdbEDA.C:2066
 EdbEDA.C:2067
 EdbEDA.C:2068
 EdbEDA.C:2069
 EdbEDA.C:2070
 EdbEDA.C:2071
 EdbEDA.C:2072
 EdbEDA.C:2073
 EdbEDA.C:2074
 EdbEDA.C:2075
 EdbEDA.C:2076
 EdbEDA.C:2077
 EdbEDA.C:2078
 EdbEDA.C:2079
 EdbEDA.C:2080
 EdbEDA.C:2081
 EdbEDA.C:2082
 EdbEDA.C:2083
 EdbEDA.C:2084
 EdbEDA.C:2085
 EdbEDA.C:2086
 EdbEDA.C:2087
 EdbEDA.C:2088
 EdbEDA.C:2089
 EdbEDA.C:2090
 EdbEDA.C:2091
 EdbEDA.C:2092
 EdbEDA.C:2093
 EdbEDA.C:2094
 EdbEDA.C:2095
 EdbEDA.C:2096
 EdbEDA.C:2097
 EdbEDA.C:2098
 EdbEDA.C:2099
 EdbEDA.C:2100
 EdbEDA.C:2101
 EdbEDA.C:2102
 EdbEDA.C:2103
 EdbEDA.C:2104
 EdbEDA.C:2105
 EdbEDA.C:2106
 EdbEDA.C:2107
 EdbEDA.C:2108
 EdbEDA.C:2109
 EdbEDA.C:2110
 EdbEDA.C:2111
 EdbEDA.C:2112
 EdbEDA.C:2113
 EdbEDA.C:2114
 EdbEDA.C:2115
 EdbEDA.C:2116
 EdbEDA.C:2117
 EdbEDA.C:2118
 EdbEDA.C:2119
 EdbEDA.C:2120
 EdbEDA.C:2121
 EdbEDA.C:2122
 EdbEDA.C:2123
 EdbEDA.C:2124
 EdbEDA.C:2125
 EdbEDA.C:2126
 EdbEDA.C:2127
 EdbEDA.C:2128
 EdbEDA.C:2129
 EdbEDA.C:2130
 EdbEDA.C:2131
 EdbEDA.C:2132
 EdbEDA.C:2133
 EdbEDA.C:2134
 EdbEDA.C:2135
 EdbEDA.C:2136
 EdbEDA.C:2137
 EdbEDA.C:2138
 EdbEDA.C:2139
 EdbEDA.C:2140
 EdbEDA.C:2141
 EdbEDA.C:2142
 EdbEDA.C:2143
 EdbEDA.C:2144
 EdbEDA.C:2145
 EdbEDA.C:2146
 EdbEDA.C:2147
 EdbEDA.C:2148
 EdbEDA.C:2149
 EdbEDA.C:2150
 EdbEDA.C:2151
 EdbEDA.C:2152
 EdbEDA.C:2153
 EdbEDA.C:2154
 EdbEDA.C:2155
 EdbEDA.C:2156
 EdbEDA.C:2157
 EdbEDA.C:2158
 EdbEDA.C:2159
 EdbEDA.C:2160
 EdbEDA.C:2161
 EdbEDA.C:2162
 EdbEDA.C:2163
 EdbEDA.C:2164
 EdbEDA.C:2165
 EdbEDA.C:2166
 EdbEDA.C:2167
 EdbEDA.C:2168
 EdbEDA.C:2169
 EdbEDA.C:2170
 EdbEDA.C:2171
 EdbEDA.C:2172
 EdbEDA.C:2173
 EdbEDA.C:2174
 EdbEDA.C:2175
 EdbEDA.C:2176
 EdbEDA.C:2177
 EdbEDA.C:2178
 EdbEDA.C:2179
 EdbEDA.C:2180
 EdbEDA.C:2181
 EdbEDA.C:2182
 EdbEDA.C:2183
 EdbEDA.C:2184
 EdbEDA.C:2185
 EdbEDA.C:2186
 EdbEDA.C:2187
 EdbEDA.C:2188
 EdbEDA.C:2189
 EdbEDA.C:2190
 EdbEDA.C:2191
 EdbEDA.C:2192
 EdbEDA.C:2193
 EdbEDA.C:2194

» Last changed: 2017-07-03 09:59 » Last generated: 2017-07-03 09:59
This page has been automatically generated. For comments or suggestions regarding the documentation or ROOT in general please send a mail to ROOT support.