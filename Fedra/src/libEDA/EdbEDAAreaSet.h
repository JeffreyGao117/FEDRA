#ifndef __EDA_SETS_H__
#define __EDA_SETS_H__

#include"EdbEDAUtil.h"
using namespace EdbEDAUtil;

class EdbEDAArea : public TObject {
	private :
	int ePlate;
	double eXmin;
	double eXmax;
	double eYmin;
	double eYmax;
	double eZ;
	bool eDraw;
	
	public:
	
	EdbEDAArea():eXmin(1e10), eXmax(-1e10), eYmin(1e10), eYmax(-1e10), eDraw(0){}
	EdbEDAArea(int ipl, double xmin, double xmax, double ymin, double ymax, double z){
		ePlate = ipl; eXmin = xmin; eXmax = xmax; eYmin = ymin; eYmax = ymax; eZ = z; eDraw=1;}
	
	void SetAreaMinMax (int ipl, double xmin, double xmax, double ymin, double ymax, double z){
		ePlate = ipl; eXmin = xmin; eXmax = xmax; eYmin = ymin; eYmax = ymax; eZ = z; eDraw=1;}
	void SetAreaXYRadius (int ipl, double x, double y, double dx, double dy, double z){
		ePlate = ipl; eXmin = x-dx; eXmax = x+dx; eYmin = y-dy; eYmax = y+dy; eZ = z; eDraw=1;}
	
	int    Plate() {return ePlate;}
	double Xmin () {return eXmin ;}
	double Xmax () {return eXmax ;}
	double Ymin () {return eYmin ;}
	double Ymax () {return eYmax ;}
	double X() { return (Xmin()+Xmax())/2.0;}
	double Y() { return (Ymin()+Ymax())/2.0;}
	double Z    () {return eZ ;}
	double RX() { return fabs( Xmin()-X() ); }
	double RY() { return fabs( Ymin()-Y() ); }
	
	void SetPlate(int ipl)  { ePlate=ipl;}
	void SetXmin (double v) { eXmin = v; eDraw=1;}
	void SetXmax (double v) { eXmax = v; eDraw=1;}
	void SetYmin (double v) { eYmin = v; eDraw=1;}
	void SetYmax (double v) { eYmax = v; eDraw=1;}
	void SetZ    (double v) { eZ    = v; }
	void SetDraw (bool b)   { eDraw = b;}
	bool GetDraw () { return eDraw;}
	
	void Print() { 
		printf("ePlate = %3d, eXmin = %8.1lf, eXmax = %8.1lf, eYmin = %8.1lf, eYmax = %8.1lf, eZ = %8.1lf\n",
				ePlate, eXmin, eXmax, eYmin, eYmax, eZ);
	}
	ClassDef(EdbEDAArea,0) // class for scanning area data.
};

class EdbEDAAreaSet : public TObject{
	
	TObjArray *eAreas;
	int eLineColor;
	int eLineWidth;
	int eTextSize;
	int eTextColor;
	TEveCompound * eCompound;
	bool eDraw;  // Draw control
	bool eDrawG; // Global Draw control by Main tab.
	
	//TArray eIndex; // to do. IPL->Area conversion index.
	
	public :
	
	EdbEDAAreaSet(){Init();};
	EdbEDAAreaSet(EdbPVRec *pvr){
		Init();
		SetAreas(pvr);
	}
	EdbEDAAreaSet(EdbDataSet *dset, double x, double y, double dx, double dy){
		Init();
		SetAreas(dset,x,y,dx,dy);
	}
	
	void SetAreas(EdbPVRec *pvr);
	void SetAreas(EdbScanSet *ss);
	
	void SetAreas(EdbDataSet *dset, double x, double y, double dx, double dy){
		if(dset==NULL) return;
		for(int i=0;i<dset->N(); i++){
			EdbDataPiece *piece = dset->GetPiece(i);
			if(NULL==piece) continue;
			EdbLayer* l = piece->GetLayer(0);
			EdbEDAArea *area = new EdbEDAArea( piece->Plate(), x-dx, x+dx, y-dy, y+dy, l->Z());
			eAreas->Add(area);
		}
	}
	
	void SetAreas(TObjArray *tracks);
	
	void Init (){
		eAreas = new TObjArray();
		eLineColor = kGray+2;
		eLineWidth = 1;
		eTextSize  = 13;
		eTextColor = kGray;
		eCompound  = NULL;
		eDraw      = kFALSE;
		eDrawG     = kTRUE;
	}
	void SetDraw(bool draw)  { eDraw = draw;}
	void SetDrawG(bool draw) { eDrawG = draw;}
	

	void AddArea(int ipl, double xmin, double xmax, double ymin, double ymax, double z){
		EdbEDAArea *area = new EdbEDAArea(ipl, xmin, xmax, ymin, ymax, z);
		eAreas->Add(area);
	}
	void AddArea(EdbEDAArea *area) { eAreas->Add(area);}	
	
	void Print(){
		printf("EdbEDAAreaSet eLineColor = %d, eLineWidth = %d, eTextSize = %d, eTextColor = %d nAreas = %d\n",
				eLineColor, eLineWidth, eTextSize, eTextColor, eAreas->GetEntries());
		for(int i=0;i<eAreas->GetEntries();i++) ((EdbEDAArea*)eAreas->At(i))->Print();
	}
	
	TObjArray *GetAreas () {return eAreas;}

	void Draw(int redraw=0);
	void UnDraw(int redraw = 0){
		TEveEventManager *ev = gEve->GetCurrentEvent();
		TEveElement *el = ev->FindChild("Scanning Areas");
		if(el) el->Destroy();
		eCompound=NULL;
		if(redraw) gEve->Redraw3D();
	}
	
	int N() { return eAreas->GetEntries();}
	EdbEDAArea * GetArea(int i) { if(0<=i&&i<N()) return (EdbEDAArea *) eAreas->At(i); else return NULL;}
	EdbEDAArea * GetAreaIPL(int ipl) { for(int i=0; i<N(); i++) if(GetArea(i)->Plate()==ipl) return GetArea(i); return NULL;}
	
	void Clear() { eAreas->Clear();}
	
	
	ClassDef(EdbEDAAreaSet,0) // class for scanning area draw.
};

class EdbEDAVertexSet:public TObject{
	
	private:
	
	public:
	
	int    eVertexColor;
	int    eVertexColorSelected;
	int    eVertexMarkerStyle;
	
	TObjArray *eVertices;     // array of vertexs to be drawn
	TObjArray *eVerticesBase; // array of vertexs, base of vertex search
	TObjArray *eVerticesPrevious;     // previous array of vertexs
	
	
	EdbEDAVertexSet(){ Init();}
	
	void Init(){
		eVertices         = new TObjArray;
		eVerticesBase     = new TObjArray;
		eVerticesPrevious = new TObjArray;
		SetVertexAttribute();
	}
	void SetVertexAttribute(
		int    vertex_color  =  kOrange, 
		int    vertex_color_selected = kRed,
		int    vertex_marker_style = 4){
		// set vertex attribute. 
		eVertexColor          = vertex_color;
		eVertexColorSelected  = vertex_color_selected;
		eVertexMarkerStyle    = vertex_marker_style;
	}
	void SetColorMode(int mode) { SetVertexAttribute(); eVertexColor = mode==kBLACKWHITE ? kBlack: kOrange;}
	
	void ResetVertexAttribute(void) { SetVertexAttribute(); }
	
	int  GetVertexColor         (void) {return eVertexColor;}
	int  GetVertexColorSelected (void) {return eVertexColorSelected;}
	int  GetVertexMarkerStyle   (void) {return eVertexMarkerStyle;}

	void AddVertex(EdbVertex *v) { 
//		if(eVerticesBase->FindObject(v)==NULL) eVerticesBase->Add(v);
//		if(eVertices->FindObject(v)==NULL)     eVertices->Add(v); 
		eVerticesBase->Add(v);
		eVertices->Add(v); 
	}
	void AddVertices(TObjArray *vertices){ if(NULL==vertices) return; for(int i=0;i<vertices->GetEntries();i++) AddVertex((EdbVertex *)vertices->At(i));}

	void SetVertex(EdbVertex *v) { if(eVertices->FindObject(v)==NULL) eVertices->Add(v);}
	void SetVertices(TObjArray *vertices){ 
		for(int i=0;i<vertices->GetEntries();i++) SetVertex((EdbVertex *)vertices->At(i));}
	void SetVerticesBase(TObjArray *vertices){ *eVerticesBase = *vertices;}
	void SetVerticesAll(void)          { *eVertices = *eVerticesBase;}
	void SetVerticesNtrk(int ntrkcut=3){
		ClearVertices();
		for(int i=0; i<NBase(); i++){
			EdbVertex *v = GetVertexBase(i);
			if(v->N()<ntrkcut) continue;
			SetVertex(v);
		}
	}
	void RestorePrevious(void) { *eVertices = *eVerticesPrevious;}
	void StorePrevious(void)   { *eVerticesPrevious = *eVertices;}
	
	
	void RemoveVertex(EdbVertex *v){ eVertices->Remove(v); eVertices->Sort();}
	void RemoveVertices(TObjArray *vertices) { for(int i=0;i<vertices->GetEntries();i++) RemoveVertex((EdbVertex *) vertices->At(i));}


	
	void ClearVertices() { eVertices->Clear();}
	void Clear() { eVertices->Clear(); eVerticesBase->Clear();}
	int N() { return eVertices->GetEntries();}
	EdbVertex * GetVertex(int i) { return (EdbVertex *) eVertices->At(i);}
	TObjArray * GetVertices() { return eVertices;}
	
	int NBase() { return eVerticesBase->GetEntries();}
	EdbVertex * GetVertexBase(int i) { return (EdbVertex *) eVerticesBase->At(i);}
	TObjArray * GetVerticesBase() { return eVerticesBase;}
	
	void DrawSingleVertex(EdbVertex *v);
	void Draw(int redraw = 0);
	void UnDraw();
	
	ClassDef(EdbEDAVertexSet,0) // Data set for vertices
};

class EdbEDAExtentionSet: public TObject{
	
	int eEnable;
	int eColor;
	double eZ;
	TEveCompound * eCompound;
	public :
	
	EdbEDAExtentionSet():eEnable(0),eColor(kGray),eZ(0.0),eCompound(NULL){};
	void Draw(int redraw=0, int fix = 0);
	void UnDraw(int redraw=0);
	void DrawResolution(int redraw =0);
	
	void Enable() { eEnable=1;}
	void Disable() { eEnable=0;}
	void SetZ(double z) { eZ=z;}
	double GetZ()   { return eZ;}
	void SetColor( int color) { eColor=color;}
	
	ClassDef(EdbEDAExtentionSet,0) // class for draw extention
};


#endif //__EDA_SETS_H__
 EdbEDASets.h:1
 EdbEDASets.h:2
 EdbEDASets.h:3
 EdbEDASets.h:4
 EdbEDASets.h:5
 EdbEDASets.h:6
 EdbEDASets.h:7
 EdbEDASets.h:8
 EdbEDASets.h:9
 EdbEDASets.h:10
 EdbEDASets.h:11
 EdbEDASets.h:12
 EdbEDASets.h:13
 EdbEDASets.h:14
 EdbEDASets.h:15
 EdbEDASets.h:16
 EdbEDASets.h:17
 EdbEDASets.h:18
 EdbEDASets.h:19
 EdbEDASets.h:20
 EdbEDASets.h:21
 EdbEDASets.h:22
 EdbEDASets.h:23
 EdbEDASets.h:24
 EdbEDASets.h:25
 EdbEDASets.h:26
 EdbEDASets.h:27
 EdbEDASets.h:28
 EdbEDASets.h:29
 EdbEDASets.h:30
 EdbEDASets.h:31
 EdbEDASets.h:32
 EdbEDASets.h:33
 EdbEDASets.h:34
 EdbEDASets.h:35
 EdbEDASets.h:36
 EdbEDASets.h:37
 EdbEDASets.h:38
 EdbEDASets.h:39
 EdbEDASets.h:40
 EdbEDASets.h:41
 EdbEDASets.h:42
 EdbEDASets.h:43
 EdbEDASets.h:44
 EdbEDASets.h:45
 EdbEDASets.h:46
 EdbEDASets.h:47
 EdbEDASets.h:48
 EdbEDASets.h:49
 EdbEDASets.h:50
 EdbEDASets.h:51
 EdbEDASets.h:52
 EdbEDASets.h:53
 EdbEDASets.h:54
 EdbEDASets.h:55
 EdbEDASets.h:56
 EdbEDASets.h:57
 EdbEDASets.h:58
 EdbEDASets.h:59
 EdbEDASets.h:60
 EdbEDASets.h:61
 EdbEDASets.h:62
 EdbEDASets.h:63
 EdbEDASets.h:64
 EdbEDASets.h:65
 EdbEDASets.h:66
 EdbEDASets.h:67
 EdbEDASets.h:68
 EdbEDASets.h:69
 EdbEDASets.h:70
 EdbEDASets.h:71
 EdbEDASets.h:72
 EdbEDASets.h:73
 EdbEDASets.h:74
 EdbEDASets.h:75
 EdbEDASets.h:76
 EdbEDASets.h:77
 EdbEDASets.h:78
 EdbEDASets.h:79
 EdbEDASets.h:80
 EdbEDASets.h:81
 EdbEDASets.h:82
 EdbEDASets.h:83
 EdbEDASets.h:84
 EdbEDASets.h:85
 EdbEDASets.h:86
 EdbEDASets.h:87
 EdbEDASets.h:88
 EdbEDASets.h:89
 EdbEDASets.h:90
 EdbEDASets.h:91
 EdbEDASets.h:92
 EdbEDASets.h:93
 EdbEDASets.h:94
 EdbEDASets.h:95
 EdbEDASets.h:96
 EdbEDASets.h:97
 EdbEDASets.h:98
 EdbEDASets.h:99
 EdbEDASets.h:100
 EdbEDASets.h:101
 EdbEDASets.h:102
 EdbEDASets.h:103
 EdbEDASets.h:104
 EdbEDASets.h:105
 EdbEDASets.h:106
 EdbEDASets.h:107
 EdbEDASets.h:108
 EdbEDASets.h:109
 EdbEDASets.h:110
 EdbEDASets.h:111
 EdbEDASets.h:112
 EdbEDASets.h:113
 EdbEDASets.h:114
 EdbEDASets.h:115
 EdbEDASets.h:116
 EdbEDASets.h:117
 EdbEDASets.h:118
 EdbEDASets.h:119
 EdbEDASets.h:120
 EdbEDASets.h:121
 EdbEDASets.h:122
 EdbEDASets.h:123
 EdbEDASets.h:124
 EdbEDASets.h:125
 EdbEDASets.h:126
 EdbEDASets.h:127
 EdbEDASets.h:128
 EdbEDASets.h:129
 EdbEDASets.h:130
 EdbEDASets.h:131
 EdbEDASets.h:132
 EdbEDASets.h:133
 EdbEDASets.h:134
 EdbEDASets.h:135
 EdbEDASets.h:136
 EdbEDASets.h:137
 EdbEDASets.h:138
 EdbEDASets.h:139
 EdbEDASets.h:140
 EdbEDASets.h:141
 EdbEDASets.h:142
 EdbEDASets.h:143
 EdbEDASets.h:144
 EdbEDASets.h:145
 EdbEDASets.h:146
 EdbEDASets.h:147
 EdbEDASets.h:148
 EdbEDASets.h:149
 EdbEDASets.h:150
 EdbEDASets.h:151
 EdbEDASets.h:152
 EdbEDASets.h:153
 EdbEDASets.h:154
 EdbEDASets.h:155
 EdbEDASets.h:156
 EdbEDASets.h:157
 EdbEDASets.h:158
 EdbEDASets.h:159
 EdbEDASets.h:160
 EdbEDASets.h:161
 EdbEDASets.h:162
 EdbEDASets.h:163
 EdbEDASets.h:164
 EdbEDASets.h:165
 EdbEDASets.h:166
 EdbEDASets.h:167
 EdbEDASets.h:168
 EdbEDASets.h:169
 EdbEDASets.h:170
 EdbEDASets.h:171
 EdbEDASets.h:172
 EdbEDASets.h:173
 EdbEDASets.h:174
 EdbEDASets.h:175
 EdbEDASets.h:176
 EdbEDASets.h:177
 EdbEDASets.h:178
 EdbEDASets.h:179
 EdbEDASets.h:180
 EdbEDASets.h:181
 EdbEDASets.h:182
 EdbEDASets.h:183
 EdbEDASets.h:184
 EdbEDASets.h:185
 EdbEDASets.h:186
 EdbEDASets.h:187
 EdbEDASets.h:188
 EdbEDASets.h:189
 EdbEDASets.h:190
 EdbEDASets.h:191
 EdbEDASets.h:192
 EdbEDASets.h:193
 EdbEDASets.h:194
 EdbEDASets.h:195
 EdbEDASets.h:196
 EdbEDASets.h:197
 EdbEDASets.h:198
 EdbEDASets.h:199
 EdbEDASets.h:200
 EdbEDASets.h:201
 EdbEDASets.h:202
 EdbEDASets.h:203
 EdbEDASets.h:204
 EdbEDASets.h:205
 EdbEDASets.h:206
 EdbEDASets.h:207
 EdbEDASets.h:208
 EdbEDASets.h:209
 EdbEDASets.h:210
 EdbEDASets.h:211
 EdbEDASets.h:212
 EdbEDASets.h:213
 EdbEDASets.h:214
 EdbEDASets.h:215
 EdbEDASets.h:216
 EdbEDASets.h:217
 EdbEDASets.h:218
 EdbEDASets.h:219
 EdbEDASets.h:220
 EdbEDASets.h:221
 EdbEDASets.h:222
 EdbEDASets.h:223
 EdbEDASets.h:224
 EdbEDASets.h:225
 EdbEDASets.h:226
 EdbEDASets.h:227
 EdbEDASets.h:228
 EdbEDASets.h:229
 EdbEDASets.h:230
 EdbEDASets.h:231
 EdbEDASets.h:232
 EdbEDASets.h:233
 EdbEDASets.h:234
 EdbEDASets.h:235
 EdbEDASets.h:236
 EdbEDASets.h:237
 EdbEDASets.h:238
 EdbEDASets.h:239
 EdbEDASets.h:240
 EdbEDASets.h:241
 EdbEDASets.h:242
 EdbEDASets.h:243
 EdbEDASets.h:244
 EdbEDASets.h:245
 EdbEDASets.h:246
 EdbEDASets.h:247
 EdbEDASets.h:248
 EdbEDASets.h:249
 EdbEDASets.h:250
 EdbEDASets.h:251
 EdbEDASets.h:252
 EdbEDASets.h:253

» Last changed: 2017-07-03 09:59 » Last generated: 2017-07-03 09:59
This page has been automatically generated. For comments or suggestions regarding the documentation or ROOT in general please send a mail to ROOT support.