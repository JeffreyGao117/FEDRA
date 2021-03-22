#include "FnuG4.h"
#include "TDatabasePDG.h"
#include <TNtuple.h>
#include <TGraph.h>
//#include "../Fedra/include/EdbPattern.h"
//#include "../Fedra/include/EdbSegP.h"
//#include "../Fedra/include/EdbPVRec.h"
//#include "../Fedra/include/EdbEDA.h"
#include "TSeqCollection.h"

FnuG4 *ev;
EdbEDA *eda;
/*
class Particle: public TObject{
	public:
	
	int id;
	int index;
	int idParent;
	EdbTrackP *t;
	Particle *parent;
	TObjArray *daughters;
	
	void Print();
};
*/

void Particle::Print(){
	printf("%6d %5d %4d -> %4d \n", id, ev->pdgid[index], t->GetSegmentFirst()->PID(), t->GetSegmentLast()->PID());
}

TObjArray *particles = new TObjArray;
TObjArray *reachingParticles = new TObjArray;

Particle* FindParticle(int id){
	
	int nparticles = particles->GetEntriesFast();
	for(int i=0; i<nparticles; i++){
		Particle *p = (Particle *) particles->At(i);
		if(p->id==id) return p;
	}
	return NULL;
}


Particle* AddParticle(int idx){
	
	Particle *p = new Particle;
	p->id = ev->id[idx];
	p->index = idx;
	p->idParent = ev->idParent[idx];
	p->t = new EdbTrackP;
	p->t->SetP( sqrt( ev->px[idx]*ev->px[idx]+ev->py[idx]*ev->py[idx]+ev->pz[idx]*ev->pz[idx] ) );
	particles->Add(p);
	
	return p;
}

///  SCT related
void GetSctStrip(float x, float y, int *iu, int *iv){
	
	float qu = 0.02; // 20 mrad tilted
	float qv = -0.02; // -20 mrad tilted
	float xu = (120-x)*tan(qu) + y; // absolute position
	float xv = (120-x)*tan(qv) + y;
	
	*iu = (int) xu*cos(qu)/80.e-3;
	*iv = (int) xv*cos(qv)/80.e-3;
	
	printf("xu = %.1f, iu = %d\n", xu, *iu);
	printf("xv = %.1f, iv = %d\n", xv, *iv);

}




int trackcount=0;

void trackfollow(Particle *pparent, int depth){
	// Follow-down tracks. This function is nested.
	// If it reach the Z coordinate bigger than 603200, say "It reach the end of FASERnu".
	// Add it in the global TObjArray *reachingParticles.
	
	if(ev->e1[pparent->index]<1) return;
	printf("stop z = %f\n", pparent->t->GetSegmentLast()->Z());
	if( pparent->t->GetSegmentLast()->Z() >603200) {
		trackcount++;
		printf("Reached to the end of FASERnu\n");
		reachingParticles->Add(pparent);
//		pparent->t->PrintNice();
	}
	
	for(int i=0; i<particles->GetEntriesFast(); i++){
		Particle *p = (Particle *) particles->At(i);
		if(p->idParent == pparent->id){
			// this is the daughter of the parent
			for(int j=0; j<depth+1; j++) printf(" ");
			printf("%dry daughter %5d ", depth, p->id);
			if(ev->charge[p->index]==0) printf("neutral, stop.\n");
			else if(p->t->P()<1000) printf("Low energy, stop.\n");
			else {
				printf("charged, follow down.\n");
				trackfollow(p, depth+1);
			}
		}
	}
}	
	
	
	
	





void recTree(){
	
	
	int nhits = ev->nhits;
	particles->Clear(); // clear previous event.
	reachingParticles->Clear();
	
	int nchargedhits = 0;
	for(int i=0; i<nhits; i++){
		
		
		if(ev->izsub[i]!=0) continue; // There are two hits for 1 emulsion film. Ignore one of them.
		if(ev->pdgid[i]==-11 || ev->pdgid[i]==11 || ev->pdgid[i]==22) continue; // ignore electrons. 
		if(ev->e1[i]<100) continue; // 100 GeV
		if(fabs(ev->charge[i])==0) continue;
		
//		printf("%6d %6d %5d %2d %7.1f %7.1f %7.3f %3d %2d %7.2f %7.2f %7.2f %7.2f\n", 
//			ev->id[i], ev->idParent[i],
//		    ev->pdgid[i], ev->charge[i], ev->x[i], ev->y[i], ev->z[i], ev->iz[i], ev->izsub[i], ev->px[i], ev->py[i], ev->pz[i], ev->e2[i]);
		
		nchargedhits++;
		
		
		Particle *p = FindParticle(ev->id[i]); // what is id[i]
		if( p==NULL ) p = AddParticle(i);
		
		EdbSegP *s = new EdbSegP(i, ev->x[i]*1e3, ev->y[i]*1e3, ev->px[i]/ev->pz[i], ev->py[i]/ev->pz[i], ev->edep[i]*200+16, ev->pdgid[i]);
			s->SetZ(ev->z[i]*1e3); // Xin's simulation is in mm, we use um.
			s->SetPID(ev->iz[i]); // Set pattern ID to data from film iz[i]. According to Aki, does not have meaning.
			s->SetPlate(ev->iz[i]); // Set the emulsion plate to index iz[i].
			s->SetFlag(ev->chamber[i]);
		p->t->AddSegment(s);
		
	}
	printf("ncharged hits %d\n", nchargedhits); // same muon in each layer or what? one particle multiple charged hits
	// one original turns into multiple muons
	
	
	printf("%d particles \n", particles->GetEntriesFast());
	// single or multiple muons?
// nhits what it looks like what are remaining hits charge pdgid xy and momentunm log file table and explain


/*	for(int i=0; i<particles->GetEntriesFast(); i++){
		
		Particle *p = (Particle *) particles->At(i);
		TObjArray parents;
		Particle *pp = p;
		while{pp}{
			printf("%d --> %d", pp->id, pp->idParent);
			pp = FindParticle( pp->idParent);
			if(pp) parents.Add(pp);

		}
		printf("\n%5d parents ", parents.GetEntriesFast());
		for(int j=parents.GetEntriesFast()-1; j>=0; j--){
			Particle *par = (Particle *) parents.At(j);
			printf("%5d --> ", par->id);
		}
		printf("%5d. ", p->id);
		p->Print();
		
	}
	
*/

	TObjArray *primaries = new TObjArray;
	for(int i=0; i<particles->GetEntriesFast(); i++){
		Particle *p = (Particle *) particles->At(i);
		if(p->idParent==0) primaries->Add(p);
	}
	printf("%d primaries\n", primaries->GetEntriesFast());
	
	int depth=1;
	for(int i=0; i<primaries->GetEntriesFast(); i++){
		Particle *p = (Particle *) primaries->At(i);
		int charge = ev->charge[p->index];
		if(charge==0) {
			printf(" id %d, not charged, track follow stopped\n", p->id);
			continue;
		}
		printf(" id %d, charged\n", p->id);
		trackfollow(p, depth+1);
	}
	
	
	delete primaries;
	
}





EdbPVRec *psuedRec(FnuG4 *ev){
	
	EdbPVRec *pvr = new EdbPVRec;
	int nhits = ev->nhits;
	TDatabasePDG db;
	
	for(int i=0; i<nhits; i++){
//		if(i<100) printf("%5d %2d %7.1f %7.1f %7.1f %7.2f %7.2f %7.2f %7.2f\n", 
//		    ev->pdgid[i], ev->charge[i], ev->x[i], ev->y[i], ev->z[i], ev->px[i], ev->py[i], ev->pz[i], ev->e2[i]);
		
		if(ev->charge[i]==0) continue;
		if(ev->izsub[i]==1) continue;
		double slope = sqrt( ev->px[i]*ev->px[i]/ev->pz[i]/ev->pz[i] + ev->py[i]*ev->py[i]/ev->pz[i]/ev->pz[i]);
		if(slope>10) continue;

		EdbSegP *s = new EdbSegP(i, ev->x[i]*1e3, ev->y[i]*1e3, ev->px[i]/ev->pz[i], ev->py[i]/ev->pz[i], ev->edep[i]*200+16, ev->pdgid[i]);
		// Emulsion scanning takes 16 frames each in top/bottom layer. Require at least 8 in each. 200 chosen arbitrarily so that
		// minimum ionizing particles appear green and heavy ionizing particles appear red.
		s->SetZ(ev->z[i]*1e3); // Xin's sim is in mm, we use um.
		s->SetPID(ev->iz[i]); // Set pattern id to data from emulsion film iz[i]. Not really physical and is overwritten when running tracking algorithm.
		s->SetPlate(ev->iz[i]); // Set plate to index iz[i].
		s->SetP( sqrt(ev->px[i]*ev->px[i]+ev->py[i]*ev->py[i]+ev->pz[i]*ev->pz[i])); // Set magnitude of momentum.
		
		//if(sqrt(s->TX()**2+s->TY()**2)>1) continue;
		
		EdbTrackP *t = new EdbTrackP(s);
		
		pvr->AddTrack(t);
		
		
	}
	
	return pvr;
	
}

// Commented for debugging
/*
void display(EdbPVRec *pvr){
	EdbEDA *eda = new EdbEDA(pvr,0);
	eda->GetTrackSet("TS")->SetExtendMode(1);
	eda->Run();
}
void display(){
	printf("Open Xin's file\n");
	ev = new FnuG4();

	int iev = 0;
	ev->GetEntry(iev);

	EdbPVRec *pvr = psuedRec(ev);
	display(pvr);
}
*/





void virtualrec(){
	
	printf("Open Xin's file\n");
	
	ev = new FnuG4();
	
	TFile f("ntracks.root","recreate");
	TNtuple *nt = new TNtuple("nt","","iev:CC:Enu:x:y:z:ntracks");
	TNtuple *ntReso = new TNtuple("ntReso","","iev:CC:pdg:p:dx0:dy0:dx1:dy1:dx2:dy2:dx3:dy3");
	TNtuple *ntResoSmeared = new TNtuple("ntResoSmeared","","iev:CC:pdg:p:dx0:dy0:dx1:dy1:dx2:dy2:dx3:dy3");
	
	for(int iev=0; iev<5; iev++){
//	{ int iev=6;
		
		
		ev->GetEntry(iev);
		
		int nhits = ev->nhits;
		
		printf("\n\n\n\n [[[[[ iev %d ]]]]] nhits=%d \n", iev, nhits);
		
		ev->Print();
		trackcount=0;
		
		// fake reconstruction.
		// track followdown in this func.
		recTree();
		
		nt->Fill(iev, ev->cc_nuEvt, ev->Enu_nuEvt, ev->x_nuEvt, ev->y_nuEvt, ev->z_nuEvt, trackcount);
		
		printf("[iev %d] %d tracks reached the end of FASERnu\n", iev, trackcount);
		
		
		int nReaching = reachingParticles->GetEntries();
		
		// calculation for resolution.
		for(int i=0; i<nReaching; i++){
			Particle *p = (Particle *) reachingParticles->At(i);
			EdbTrackP *t = p->t;
			std::vector<int> hitsFASERnu;
			std::vector<int> hitsIFT;
			
			for(int j=0; j<t->N(); j++){
				EdbSegP *s = t->GetSegment(j);
				int idx = s->ID();
				if(ev->chamber[idx]==0 ) hitsFASERnu.push_back(idx);
				else                     hitsIFT.push_back(idx);
			}
			
			if( hitsFASERnu.size()<13) continue;
			if( hitsIFT.size()<3) continue;
			
			float dx0=1e6, dy0=1e6;
			float dx1=1e6, dy1=1e6;
			float dx2=1e6, dy2=1e6;
			float dx3=1e6, dy3=1e6;
			TGraph grFnuX;
			TGraph grFnuY;
			for(int j=0; j<3; j++){ 
				// use the last 13-11th hits. (10 tungsten plates, instead of the mechanical support)
				int idx = hitsFASERnu[hitsFASERnu.size()-13+j];
				grFnuX.SetPoint(j, ev->z[idx], ev->x[idx]);
				grFnuY.SetPoint(j, ev->z[idx], ev->y[idx]);
			}
			grFnuX.Fit("pol1","Q");
			grFnuY.Fit("pol1","Q");
			
			
			for(int j=0; j<hitsIFT.size(); j++){
				int idx = hitsIFT[j];
				double z = ev->z[idx];
				double xI = ev->x[idx];
				double yI = ev->y[idx];
				
				double xF = grFnuX.GetFunction("pol1")->Eval(z);
				double yF = grFnuY.GetFunction("pol1")->Eval(z);
				
				int iz = ev->iz[idx];
				printf("iz = %d, FASERnu -> IFT %f %f %f %f %f %f\n", iz, xI, yI, xF, yF, (xI-xF), (yI-yF));
				
				switch(iz){
					case 0: dx1 = xI-xF; dy1 = yI-yF; break;
					case 1: dx2 = xI-xF; dy2 = yI-yF; break;
					case 2: dx3 = xI-xF; dy3 = yI-yF; break;
					default: break;
				}
			}
			
			TGraph grIFTX;
			TGraph grIFTY;
			
			for(int j=0; j<hitsIFT.size(); j++){
				int idx = hitsIFT[j];
				grIFTX.SetPoint(j, ev->z[idx], ev->x[idx]);
				grIFTY.SetPoint(j, ev->z[idx], ev->y[idx]);
			}
			grIFTX.Fit("pol1","Q");
			grIFTY.Fit("pol1","Q");
			
			{
				int idx = hitsFASERnu[hitsFASERnu.size()-1]; // Should not be used because this is the last plate in FASERnu,
				// but we should be ignoring the last 10 plates???
				double z = ev->z[idx];
				double xF = ev->x[idx];
				double yF = ev->y[idx];
				double xI = grIFTX.GetFunction("pol1")->Eval(z);
				double yI = grIFTY.GetFunction("pol1")->Eval(z);
				dx0 = xI-xF;
				dy0 = yI-yF;
				int iz = ev->iz[idx];
				printf("iz = %d, IFT -> FASERnu %f %f \n", iz, dx0, dy0);
				
			}
			
			ntReso->Fill(iev, ev->cc_nuEvt, ev->pdgid[p->index], p->t->P(), dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3);
		}
		
	}
	
	
	nt->Write();
	ntReso->Write();
	f.Close();
//     EdbPVRec *pvr = psuedRec(ev); 	display(pvr);
	
	
}

