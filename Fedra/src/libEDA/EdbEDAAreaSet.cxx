#include"EdbEDASets.h"
#include"EdbEDA.h"
#include"TEveVSDStructs.h"
extern EdbEDA *gEDA;

using namespace EdbEDAUtil;

ClassImp(EdbEDAArea)
ClassImp(EdbEDAAreaSet)
ClassImp(EdbEDAVertexSet)
ClassImp(EdbEDAExtentionSet)



void EdbEDAAreaSet::SetAreas(EdbPVRec *pvr){
	if(pvr==NULL) return;
	for(int i=0;i<pvr->Npatterns(); i++){
		EdbPattern *p = pvr->GetPattern(i);
		if(p==NULL) continue;
		if(p->N()==0) continue;
		
		int ipl = p->GetSegment(0)->Plate();
		EdbEDAArea *area = new EdbEDAArea( ipl, p->Xmin(), p->Xmax(), p->Ymin(), p->Ymax(), p->Z());
		eAreas->Add(area);
	}
}

void EdbEDAAreaSet::SetAreas(EdbScanSet *ss){
	if(ss){
		EdbBrickP *b = &ss->eB;
		for(int i=0;i<b->Npl();i++){
			EdbPlateP *pl = b->GetPlate(i);
			EdbLayer *l = pl->GetLayer(0);
			EdbEDAArea *a = new EdbEDAArea(pl->ID(), l->Xmin(), l->Xmax(), l->Ymin(), l->Ymax(), l->Z());
			eAreas->Add(a);
		}
		return;
	}
}

void EdbEDAAreaSet::SetAreas(TObjArray *tracks){
	printf("Set scanning areas from TS tracks %d\n", tracks->GetEntries());
	
	TObjArray *areas = new TObjArray;
	EdbScanSet *ss = gEDA->GetScanSet();
	if(ss){
		EdbBrickP *b = &ss->eB;
		for(int i=0;i<b->Npl();i++){
			EdbPlateP *pl = b->GetPlate(i);
			//EdbLayer *l = pl->GetLayer(0);
			//EdbEDAArea *a = new EdbEDAArea(pl->ID(), l->Xmin(), l->Xmax(), l->Ymin(), l->Ymax(), l->Z());
			EdbEDAArea *a = new EdbEDAArea;
			a->SetPlate(pl->ID());
			a->SetZ(pl->Z());
			areas->Add(a);
		}
		for(int i=0;i<tracks->GetEntries();i++){
			EdbTrackP *t = (EdbTrackP *)tracks->At(i);
			if(t==NULL) continue;
			for(int j=0;j<t->N();j++){
				EdbSegP *s = t->GetSegment(j);
				EdbEDAArea *a=NULL;
				for(int k=0;k<areas->GetEntries();k++){
					a = (EdbEDAArea *) areas->At(k);
					if(a->Plate()==s->Plate()) break;
				}
					
				if(s->X()<a->Xmin()) a->SetXmin(s->X());
				if(s->X()>a->Xmax()) a->SetXmax(s->X());
				if(s->Y()<a->Ymin()) a->SetYmin(s->Y());
				if(s->Y()>a->Ymax()) a->SetYmax(s->Y());
			}
		}
		
		for(int i=0;i<areas->GetEntries();i++){ AddArea( (EdbEDAArea *)areas->At(i));}
		return;
	}
	
	
	EdbDataSet *dset = gEDA->GetDataSet();
	for(int i=0;i<dset->N();i++){
		EdbEDAArea *a = new EdbEDAArea;
		a->SetZ(gEDA->GetZPID(i));
		a->SetPlate(gEDA->GetIPL(i));
		areas->Add(a);
	}
	
	for(int i=0;i<tracks->GetEntries();i++){
		EdbTrackP *t = (EdbTrackP *)tracks->At(i);
		if(t==NULL) continue;
		for(int j=0;j<t->N();j++){
			EdbSegP *s = t->GetSegment(j);
			EdbEDAArea *a = GetAreaIPL(s->Plate());
			if(s->X()<a->Xmin()) {a->SetXmin(s->X()); if(s->PID()==0)printf("xmin %lf\n", s->X());}
			if(s->X()>a->Xmax()) {a->SetXmax(s->X()); if(s->PID()==0)printf("xman %lf\n", s->X());}
			if(s->Y()<a->Ymin()) a->SetYmin(s->Y());
			if(s->Y()>a->Ymax()) a->SetYmax(s->Y());
		}
	}
	
	for(int i=0;i<areas->GetEntries();i++){ AddArea( (EdbEDAArea *)areas->At(i));}
//	areas->Delete();
}

//#include<TEveGeoNode.h>
//#include<TGeoBBox.h>
void EdbEDAAreaSet::Draw(int redraw){
	if(eCompound) UnDraw();
	if(eDraw==kFALSE||eDrawG==kFALSE) return;
	if(eAreas->GetEntries()==0) return;
	eCompound = new TEveCompound;
	eCompound->SetElementName("Scanning Areas");
	gEve->AddElement(eCompound);

	for(int i=0;i<eAreas->GetEntries();i++){
		EdbEDAArea *area = (EdbEDAArea *) eAreas->At(i);
		if(area->GetDraw()==kFALSE) continue;
		TEveLine *l = new TEveLine;
		l->SetLineColor(eLineColor);
		l->SetLineWidth(eLineWidth);
		if(gEDA->Japanese()) l->SetName(
			Form("PL %2d (%2d), X: %8.1lf -> %8.1lf, Y: %8.1lf -> %8.1lf, Z: %8.1lf, Area : %6d x %6d = %.1lfcm2", 
			area->Plate(), 58-area->Plate(), area->Xmin(),area->Xmax(),area->Ymin(),area->Ymax(),area->Z(),
			(int)(area->Xmax()-area->Xmin()), (int)(area->Ymax()-area->Ymin()), 
			(area->Xmax()-area->Xmin())*(area->Ymax()-area->Ymin())/1e8));
		else l->SetName(
			Form("PL %2d, X: %8.1lf -> %8.1lf, Y: %8.1lf -> %8.1lf, Z: %8.1lf, Area : %6d x %6d = %.1lfcm2", 
			area->Plate(),area->Xmin(),area->Xmax(),area->Ymin(),area->Ymax(),area->Z(),
			(int)(area->Xmax()-area->Xmin()), (int)(area->Ymax()-area->Ymin()), 
			(area->Xmax()-area->Xmin())*(area->Ymax()-area->Ymin())/1e8));
		l->SetNextPoint(area->Xmin(), area->Ymin(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmin(), area->Ymax(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmax(), area->Ymax(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmax(), area->Ymin(), area->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(area->Xmin(), area->Ymin(), area->Z()*gEDA->GetScaleZ());
		eCompound->AddElement(l);
		
		TEveText* t = new TEveText;
		t->SetText(Form("%d",area->Plate()));
		t->PtrMainTrans()->SetPos( area->Xmax(), area->Ymax(), area->Z()*gEDA->GetScaleZ());
		t->SetFontSize(eTextSize);
		t->SetMainColor(eTextColor);
		t->SetFontMode(TGLFont::kBitmap);
		l->AddElement(t);

		/*
		// Trial using transparent geometories.
		// Make Shape
		TEveGeoShape *s = new TEveGeoShape(Form("PL %2d, X: %8.1lf -> %8.1lf, Y: %8.1lf -> %8.1lf, Z: %6.1lf", 
			area->Plate(),area->Xmin(),area->Xmax(),area->Ymin(),area->Ymax(),area->Z()));
		// register Box
		s->SetShape(new TGeoBBox("BOX", area->RX(), area->RY(), 300));
		
		// Set position
		s->PtrMainTrans()->SetPos(area->X(), area->Y(), area->Z()*gEDA->GetScaleZ());
		
		// Set Color
		s->SetMainColor(kCyan);
		
		// Set Transparency. 
		s->SetMainTransparency( 90 );
		
		eCompound->AddElement(s);
		*/
	}
	if(redraw) gEve->Redraw3D();
}


void EdbEDAVertexSet::DrawSingleVertex(EdbVertex *v){
	// Draw single vertex.
	TEvePointSet *ps = new TEvePointSet();
	ps->SetElementName(Form("vtx %3d %d %8.1lf %8.1lf %8.1lf", v->ID(), v->N(), v->X(), v->Y(), v->Z()));
	ps->SetMarkerStyle(eVertexMarkerStyle);
	ps->SetNextPoint(v->X(), v->Y(), v->Z()*gEDA->GetScaleZ());
	
	
	// in case the coloring of selected vertex is not done EdbEDA::SetSelection()
	EdbVertex *selected_vertex = gEDA->GetSelectedVertex();
	if(selected_vertex==v) {
		ps->SetMarkerColor(kRed);
	} else {
		ps->SetMarkerColor(eVertexColor);
	}
	
	
	// coloring of selected vertex is done in EdbEDA::SetSelection()
	//ps->SetMarkerColor(eVertexColor);
	ps->SetUserData(v);
	
	gEve->AddElement(ps);
	
}

void EdbEDAVertexSet::Draw(int redraw){
	UnDraw();
	
	int nvertices = eVertices->GetEntries();
	printf("VertexSet   Draw %4d %s\n", nvertices, nvertices<=1 ? "vertex" : "vertices");
	for(int i=0;i<nvertices;i++){
		EdbVertex *v = (EdbVertex *) eVertices->At(i);
		DrawSingleVertex(v);
	}
	
	if(redraw) gEve->Redraw3D();
}

void EdbEDAVertexSet::UnDraw(void){
	TEveEventManager *ev = gEve->GetCurrentEvent();
	if(ev==NULL) return;
	if(ev->NumChildren()==0) return;
	
	TEveElement::List_i it = ev->BeginChildren();
	for(;;it++){
		if(it==ev->EndChildren()) break;
		TEveElement *e = *it;
		if(IsVertex(e)){
			e->Destroy();
			it--;
		}
	}
}


void EdbEDAExtentionSet::Draw(int redraw, int fix){
	if(!eEnable) return;
	if(eCompound) UnDraw();
	
	TObjArray *selected = gEDA->GetSelected();
	if(selected->GetEntries()==0) return;
	eCompound = new TEveCompound;
	eCompound->SetName("ExtentionSet");
	gEve->AddElement(eCompound);

	for(int i=0;i<selected->GetEntries();i++){
		EdbSegP   *s = (EdbSegP *)   selected->At(i);
		EdbTrackP *t = gEDA->GetTrack(s);
		TEveLine *l = new TEveLine;
		l->SetName(Form("seg itrk %4d flag %4d pl %2d id %6d ph %2d %8.1lf %8.1lf %8.1lf %7.4lf %7.4lf %4d", 
			t?t->ID():0, t?t->Flag():0, s->Plate(), (int)s->ID(), (int)s->W(), 
			s->X(), s->Y(), s->Z(), s->TX(), s->TY(), s->Flag()));
		l->SetLineColor(eColor);
		l->SetNextPoint(s->X(), s->Y(), s->Z()*gEDA->GetScaleZ());
		l->SetNextPoint(s->X()+s->TX()*(eZ-s->Z()),
						s->Y()+s->TY()*(eZ-s->Z()),
						(eZ)*gEDA->GetScaleZ());
		//l->SetUserData(s);
		if(fix) gEve->AddElement(l);
		else eCompound->AddElement(l);
	}
	
	if(redraw) gEve->Redraw3D();
}

void EdbEDAExtentionSet::UnDraw(int redraw){
	if(gEve->GetCurrentEvent()!=NULL){
		TEveElement *ext = gEve->GetCurrentEvent()->FindChild("ExtentionSet");
		if(ext) ext->Destroy();
		//if(ext) gEve->GetCurrentEvent()->RemoveElement(ext);
	}
	eCompound=NULL;
	if(redraw) gEve->Redraw3D();
}

void EdbEDAExtentionSet::DrawResolution(int redraw){
	TObjArray *selected = gEDA->GetSelected();
	TObjArray *selected_tracks = gEDA->GetSelectedTracks();
	
	for(int i=0;i<selected->GetEntries();i++){
		EdbSegP *s = (EdbSegP *) selected->At(i);
		EdbTrackP *t = (EdbTrackP*) selected_tracks->At(i); // temporary
		
		double p,pmin,pmax;
		if(t!=NULL){
			p=t->P();
			if(p<0)	CalcP(t, p, pmin, pmax);
			if(p<0.01) p = 1.0;
		} else p = 1.0;
		
		
		// definition of cone set.
		TEveBoxSet* cones = new TEveBoxSet("ConeSet");
		cones->Reset(TEveBoxSet::kBT_Cone, kFALSE, 64);
		
		// setting for colors. seems necessary.
		gStyle->SetPalette(-1);
		TEveRGBAPalette* pal = new TEveRGBAPalette(0, 50);
		cones->SetPalette(pal);
		
		TEveVector dir, pos;
		int direction = eZ<s->Z() ? -1 : 1; // parity of cone direction.

		double angle  = sqrt(s->TX()*s->TX()+s->TY()*s->TY());
		double dtheta = 0.002+ 0.002*angle;
		double dr     = 0.5 + angle;
		
		double step = 20;
		for(double dz=10;dz<fabs(eZ-s->Z())+step;dz+=step){ // dz : at the circle.
			int flag_last=0; // if it is the last cone, put cap on it.
			if(dz > fabs(eZ-s->Z())) {
				dz = fabs(eZ-s->Z());
				flag_last=1;
			}
			dir.Set( s->TX(), s->TY(), (Float_t)1.0);  // direction of cone.
			double height = dz<300? dz : 300; // hight of cone.
			dir *= direction * height;
			
			// resolutions. currently calculated as all in lead.
			double dz2 = direction*fabs(dz-height);       // dz2 at the vertex.
			double l = dz*sqrt(1+angle*angle);           // length from segment
			double dr_theta = dtheta*l;                   // dr from angle resolution
			double dr_scatt = 0.0136/p*sqrt(l/3/5700.0)*l; // dr from scatterning
			double r = sqrt(dr*dr+dr_theta*dr_theta+dr_scatt*dr_scatt);  // radius = 1sigma of error

			pos.Set(s->X()+dz2*s->TX(), s->Y()+dz2*s->TY(), s->Z()+dz2); // position of vertex
			cones->AddCone(pos, dir, r);
			cones->DigitValue(18); 
			
			if(flag_last){ // Cap
				dir.Set( s->TX(), s->TY(), (Float_t)1.0);
				dir *= -direction*r;
				dz2 = direction*fabs(dz+r);
				pos.Set(s->X()+dz2*s->TX(), s->Y()+dz2*s->TY(), s->Z()+dz2);
				cones->AddCone(pos, dir, r);
				cones->DigitValue(18);
				printf("Extention with resolution, track %4d p = %.2lf, resolution = %.1lf um at z = %.0lf (dz = %.0lf)\n", t->ID(), p, r, s->Z()+direction*dz, dz);
			}
		}
		gEve->AddElement(cones);
	}
	printf("draw\n");
	if(redraw) gEve->Redraw3D();
}
 EdbEDASets.C:1
 EdbEDASets.C:2
 EdbEDASets.C:3
 EdbEDASets.C:4
 EdbEDASets.C:5
 EdbEDASets.C:6
 EdbEDASets.C:7
 EdbEDASets.C:8
 EdbEDASets.C:9
 EdbEDASets.C:10
 EdbEDASets.C:11
 EdbEDASets.C:12
 EdbEDASets.C:13
 EdbEDASets.C:14
 EdbEDASets.C:15
 EdbEDASets.C:16
 EdbEDASets.C:17
 EdbEDASets.C:18
 EdbEDASets.C:19
 EdbEDASets.C:20
 EdbEDASets.C:21
 EdbEDASets.C:22
 EdbEDASets.C:23
 EdbEDASets.C:24
 EdbEDASets.C:25
 EdbEDASets.C:26
 EdbEDASets.C:27
 EdbEDASets.C:28
 EdbEDASets.C:29
 EdbEDASets.C:30
 EdbEDASets.C:31
 EdbEDASets.C:32
 EdbEDASets.C:33
 EdbEDASets.C:34
 EdbEDASets.C:35
 EdbEDASets.C:36
 EdbEDASets.C:37
 EdbEDASets.C:38
 EdbEDASets.C:39
 EdbEDASets.C:40
 EdbEDASets.C:41
 EdbEDASets.C:42
 EdbEDASets.C:43
 EdbEDASets.C:44
 EdbEDASets.C:45
 EdbEDASets.C:46
 EdbEDASets.C:47
 EdbEDASets.C:48
 EdbEDASets.C:49
 EdbEDASets.C:50
 EdbEDASets.C:51
 EdbEDASets.C:52
 EdbEDASets.C:53
 EdbEDASets.C:54
 EdbEDASets.C:55
 EdbEDASets.C:56
 EdbEDASets.C:57
 EdbEDASets.C:58
 EdbEDASets.C:59
 EdbEDASets.C:60
 EdbEDASets.C:61
 EdbEDASets.C:62
 EdbEDASets.C:63
 EdbEDASets.C:64
 EdbEDASets.C:65
 EdbEDASets.C:66
 EdbEDASets.C:67
 EdbEDASets.C:68
 EdbEDASets.C:69
 EdbEDASets.C:70
 EdbEDASets.C:71
 EdbEDASets.C:72
 EdbEDASets.C:73
 EdbEDASets.C:74
 EdbEDASets.C:75
 EdbEDASets.C:76
 EdbEDASets.C:77
 EdbEDASets.C:78
 EdbEDASets.C:79
 EdbEDASets.C:80
 EdbEDASets.C:81
 EdbEDASets.C:82
 EdbEDASets.C:83
 EdbEDASets.C:84
 EdbEDASets.C:85
 EdbEDASets.C:86
 EdbEDASets.C:87
 EdbEDASets.C:88
 EdbEDASets.C:89
 EdbEDASets.C:90
 EdbEDASets.C:91
 EdbEDASets.C:92
 EdbEDASets.C:93
 EdbEDASets.C:94
 EdbEDASets.C:95
 EdbEDASets.C:96
 EdbEDASets.C:97
 EdbEDASets.C:98
 EdbEDASets.C:99
 EdbEDASets.C:100
 EdbEDASets.C:101
 EdbEDASets.C:102
 EdbEDASets.C:103
 EdbEDASets.C:104
 EdbEDASets.C:105
 EdbEDASets.C:106
 EdbEDASets.C:107
 EdbEDASets.C:108
 EdbEDASets.C:109
 EdbEDASets.C:110
 EdbEDASets.C:111
 EdbEDASets.C:112
 EdbEDASets.C:113
 EdbEDASets.C:114
 EdbEDASets.C:115
 EdbEDASets.C:116
 EdbEDASets.C:117
 EdbEDASets.C:118
 EdbEDASets.C:119
 EdbEDASets.C:120
 EdbEDASets.C:121
 EdbEDASets.C:122
 EdbEDASets.C:123
 EdbEDASets.C:124
 EdbEDASets.C:125
 EdbEDASets.C:126
 EdbEDASets.C:127
 EdbEDASets.C:128
 EdbEDASets.C:129
 EdbEDASets.C:130
 EdbEDASets.C:131
 EdbEDASets.C:132
 EdbEDASets.C:133
 EdbEDASets.C:134
 EdbEDASets.C:135
 EdbEDASets.C:136
 EdbEDASets.C:137
 EdbEDASets.C:138
 EdbEDASets.C:139
 EdbEDASets.C:140
 EdbEDASets.C:141
 EdbEDASets.C:142
 EdbEDASets.C:143
 EdbEDASets.C:144
 EdbEDASets.C:145
 EdbEDASets.C:146
 EdbEDASets.C:147
 EdbEDASets.C:148
 EdbEDASets.C:149
 EdbEDASets.C:150
 EdbEDASets.C:151
 EdbEDASets.C:152
 EdbEDASets.C:153
 EdbEDASets.C:154
 EdbEDASets.C:155
 EdbEDASets.C:156
 EdbEDASets.C:157
 EdbEDASets.C:158
 EdbEDASets.C:159
 EdbEDASets.C:160
 EdbEDASets.C:161
 EdbEDASets.C:162
 EdbEDASets.C:163
 EdbEDASets.C:164
 EdbEDASets.C:165
 EdbEDASets.C:166
 EdbEDASets.C:167
 EdbEDASets.C:168
 EdbEDASets.C:169
 EdbEDASets.C:170
 EdbEDASets.C:171
 EdbEDASets.C:172
 EdbEDASets.C:173
 EdbEDASets.C:174
 EdbEDASets.C:175
 EdbEDASets.C:176
 EdbEDASets.C:177
 EdbEDASets.C:178
 EdbEDASets.C:179
 EdbEDASets.C:180
 EdbEDASets.C:181
 EdbEDASets.C:182
 EdbEDASets.C:183
 EdbEDASets.C:184
 EdbEDASets.C:185
 EdbEDASets.C:186
 EdbEDASets.C:187
 EdbEDASets.C:188
 EdbEDASets.C:189
 EdbEDASets.C:190
 EdbEDASets.C:191
 EdbEDASets.C:192
 EdbEDASets.C:193
 EdbEDASets.C:194
 EdbEDASets.C:195
 EdbEDASets.C:196
 EdbEDASets.C:197
 EdbEDASets.C:198
 EdbEDASets.C:199
 EdbEDASets.C:200
 EdbEDASets.C:201
 EdbEDASets.C:202
 EdbEDASets.C:203
 EdbEDASets.C:204
 EdbEDASets.C:205
 EdbEDASets.C:206
 EdbEDASets.C:207
 EdbEDASets.C:208
 EdbEDASets.C:209
 EdbEDASets.C:210
 EdbEDASets.C:211
 EdbEDASets.C:212
 EdbEDASets.C:213
 EdbEDASets.C:214
 EdbEDASets.C:215
 EdbEDASets.C:216
 EdbEDASets.C:217
 EdbEDASets.C:218
 EdbEDASets.C:219
 EdbEDASets.C:220
 EdbEDASets.C:221
 EdbEDASets.C:222
 EdbEDASets.C:223
 EdbEDASets.C:224
 EdbEDASets.C:225
 EdbEDASets.C:226
 EdbEDASets.C:227
 EdbEDASets.C:228
 EdbEDASets.C:229
 EdbEDASets.C:230
 EdbEDASets.C:231
 EdbEDASets.C:232
 EdbEDASets.C:233
 EdbEDASets.C:234
 EdbEDASets.C:235
 EdbEDASets.C:236
 EdbEDASets.C:237
 EdbEDASets.C:238
 EdbEDASets.C:239
 EdbEDASets.C:240
 EdbEDASets.C:241
 EdbEDASets.C:242
 EdbEDASets.C:243
 EdbEDASets.C:244
 EdbEDASets.C:245
 EdbEDASets.C:246
 EdbEDASets.C:247
 EdbEDASets.C:248
 EdbEDASets.C:249
 EdbEDASets.C:250
 EdbEDASets.C:251
 EdbEDASets.C:252
 EdbEDASets.C:253
 EdbEDASets.C:254
 EdbEDASets.C:255
 EdbEDASets.C:256
 EdbEDASets.C:257
 EdbEDASets.C:258
 EdbEDASets.C:259
 EdbEDASets.C:260
 EdbEDASets.C:261
 EdbEDASets.C:262
 EdbEDASets.C:263
 EdbEDASets.C:264
 EdbEDASets.C:265
 EdbEDASets.C:266
 EdbEDASets.C:267
 EdbEDASets.C:268
 EdbEDASets.C:269
 EdbEDASets.C:270
 EdbEDASets.C:271
 EdbEDASets.C:272
 EdbEDASets.C:273
 EdbEDASets.C:274
 EdbEDASets.C:275
 EdbEDASets.C:276
 EdbEDASets.C:277
 EdbEDASets.C:278
 EdbEDASets.C:279
 EdbEDASets.C:280
 EdbEDASets.C:281
 EdbEDASets.C:282
 EdbEDASets.C:283
 EdbEDASets.C:284
 EdbEDASets.C:285
 EdbEDASets.C:286
 EdbEDASets.C:287
 EdbEDASets.C:288
 EdbEDASets.C:289
 EdbEDASets.C:290
 EdbEDASets.C:291
 EdbEDASets.C:292
 EdbEDASets.C:293
 EdbEDASets.C:294
 EdbEDASets.C:295
 EdbEDASets.C:296
 EdbEDASets.C:297
 EdbEDASets.C:298
 EdbEDASets.C:299
 EdbEDASets.C:300
 EdbEDASets.C:301
 EdbEDASets.C:302
 EdbEDASets.C:303
 EdbEDASets.C:304
 EdbEDASets.C:305
 EdbEDASets.C:306
 EdbEDASets.C:307
 EdbEDASets.C:308
 EdbEDASets.C:309
 EdbEDASets.C:310
 EdbEDASets.C:311
 EdbEDASets.C:312
 EdbEDASets.C:313
 EdbEDASets.C:314
 EdbEDASets.C:315
 EdbEDASets.C:316
 EdbEDASets.C:317
 EdbEDASets.C:318
 EdbEDASets.C:319
 EdbEDASets.C:320
 EdbEDASets.C:321
 EdbEDASets.C:322
 EdbEDASets.C:323
 EdbEDASets.C:324
 EdbEDASets.C:325
 EdbEDASets.C:326
 EdbEDASets.C:327
 EdbEDASets.C:328
 EdbEDASets.C:329
 EdbEDASets.C:330
 EdbEDASets.C:331
 EdbEDASets.C:332
 EdbEDASets.C:333

» Last changed: 2017-07-03 09:59 » Last generated: 2017-07-03 09:59
This page has been automatically generated. For comments or suggestions regarding the documentation or ROOT in general please send a mail to ROOT support.