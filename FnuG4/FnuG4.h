//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr 14 00:08:31 2020 by ROOT version 5.34/26
// from TTree FASERnu/
// found on file: FASERnu_numu.dump._001.root
//////////////////////////////////////////////////////////

#ifndef FnuG4_h
#define FnuG4_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLeaf.h>
#include <TTree.h>
#include <TObject.h>
#include <TObjArray.h>
#include "../Fedra/include/EdbPattern.h"
#include "../Fedra/include/EdbSegP.h"
#include "../Fedra/include/EdbTrackP.h"
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.


class Hit: public TObject{
	public:
	int iz, izsub, pdgid, id, idParent;
	float charge, x, y, z, px, py, pz, e1, e2, len, edep;
	ClassDef(Hit, 1);
};

class Particle: public TObject{
	public:
	
	int id;
	int index;
	int idParent;
	EdbTrackP *t;
	Particle *parent;
	TObjArray *daughters;
	TObjArray *hits;
	
	Particle(){
		daughters = new TObjArray;
		hits = new TObjArray;
	}
	~Particle(){
		delete daughters;
		delete hits;
	}
	void Print();
	ClassDef(Particle, 1);
};

void Particle::Print(){
//	printf("%6d %5d %4d -> %4d \n", id, ev->pdgid[index], t->GetSegmentFirst()->PID(), t->GetSegmentLast()->PID());
}






class FnuG4 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        e_beam;
   Int_t           id_beam;
   Double_t        x_beam;
   Double_t        y_beam;
   Int_t           pdgnu_nuEvt;
   Int_t           pdglep_nuEvt;
   Double_t        Enu_nuEvt;
   Double_t        Plep_nuEvt;
   Int_t           cc_nuEvt;
   Double_t        x_nuEvt;
   Double_t        y_nuEvt;
   Double_t        z_nuEvt;
   Int_t           nhits;
   Int_t           chamber_count;
   Int_t           chamber[846048];   //[chamber_count]
   Int_t           iz_count;
   Int_t           iz[846048];   //[iz_count]
   Int_t           izsub_count;
   Int_t           izsub[846048];   //[izsub_count]
   Int_t           pdgid_count;
   Int_t           pdgid[846048];   //[pdgid_count]
   Int_t           id_count;
   Int_t           id[846048];   //[id_count]
   Int_t           idParent_count;
   Int_t           idParent[846048];   //[idParent_count]
   Int_t           charge_count;
   Double_t        charge[846048];   //[charge_count]
   Int_t           x_count;
   Double_t        x[846048];   //[x_count]
   Int_t           y_count;
   Double_t        y[846048];   //[y_count]
   Int_t           z_count;
   Double_t        z[846048];   //[z_count]
   Int_t           px_count;
   Double_t        px[846048];   //[px_count]
   Int_t           py_count;
   Double_t        py[846048];   //[py_count]
   Int_t           pz_count;
   Double_t        pz[846048];   //[pz_count]
   Int_t           e1_count;
   Double_t        e1[846048];   //[e1_count]
   Int_t           e2_count;
   Double_t        e2[846048];   //[e2_count]
   Int_t           len_count;
   Double_t        len[846048];   //[len_count]
   Int_t           edep_count;
   Double_t        edep[846048];   //[edep_count]

   // List of branches
   TBranch        *b_row_wise_branch;   //!

	TObjArray *particles;
   


   FnuG4(TTree *tree=0);
   virtual ~FnuG4();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void Print();
   Particle* FindParticle(int id);
//   Particle*  AddParticle(int idx);
   
};

#endif

#ifdef FnuG4_cxx
FnuG4::FnuG4(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("FASERnu_numu.dump._001.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("FASERnu_numu.dump._001.root");
      }
      f->GetObject("FASERnu",tree);

   }
   Init(tree);
   
   particles = new TObjArray;
   particles->SetOwner(1);
}

FnuG4::~FnuG4()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t FnuG4::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   
   delete particles;
   particles = new TObjArray;
   particles->SetOwner(1);
   
   int ret = fChain->GetEntry(entry);
   nhits = chamber_count;

   return ret;
}
Long64_t FnuG4::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void FnuG4::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("row_wise_branch", &e_beam, &b_row_wise_branch);


 
   fChain->GetLeaf("e_beam")->SetAddress( &e_beam);
   fChain->GetLeaf("id_beam")->SetAddress( &id_beam);
   fChain->GetLeaf("x_beam")->SetAddress( &x_beam);
   fChain->GetLeaf("y_beam")->SetAddress( &y_beam);
   fChain->GetLeaf("pdgnu_nuEvt")->SetAddress( &pdgnu_nuEvt);
   fChain->GetLeaf("pdglep_nuEvt")->SetAddress( &pdglep_nuEvt);
   fChain->GetLeaf("Enu_nuEvt")->SetAddress( &Enu_nuEvt);
   fChain->GetLeaf("Plep_nuEvt")->SetAddress( &Plep_nuEvt);
   fChain->GetLeaf("cc_nuEvt")->SetAddress( &cc_nuEvt);
   fChain->GetLeaf("x_nuEvt")->SetAddress( &x_nuEvt);
   fChain->GetLeaf("y_nuEvt")->SetAddress( &y_nuEvt);
   fChain->GetLeaf("z_nuEvt")->SetAddress( &z_nuEvt);
//   fChain->GetLeaf("chamber_count")->SetAddress( &nhits);
   fChain->GetLeaf("chamber_count")->SetAddress( &chamber_count);
   fChain->GetLeaf("chamber")->SetAddress(chamber);   //[chamber_count]
   fChain->GetLeaf("iz_count")->SetAddress( &iz_count);
   fChain->GetLeaf("iz")->SetAddress(iz);   //[iz_count]
   fChain->GetLeaf("izsub_count")->SetAddress( &izsub_count);
   fChain->GetLeaf("izsub")->SetAddress(izsub);   //[izsub_count]
   fChain->GetLeaf("pdgid_count")->SetAddress( &pdgid_count);
   fChain->GetLeaf("pdgid")->SetAddress(pdgid);   //[pdgid_count]
   fChain->GetLeaf("id_count")->SetAddress( &id_count);
   fChain->GetLeaf("id")->SetAddress(id);   //[id_count]
   fChain->GetLeaf("idParent_count")->SetAddress( &idParent_count);
   fChain->GetLeaf("idParent")->SetAddress(idParent);   //[idParent_count]
   fChain->GetLeaf("charge_count")->SetAddress( &charge_count);
   fChain->GetLeaf("charge")->SetAddress(charge);   //[charge_count]
   fChain->GetLeaf("x_count")->SetAddress( &x_count);
   fChain->GetLeaf("x")->SetAddress(x);   //[x_count]
   fChain->GetLeaf("y_count")->SetAddress( &y_count);
   fChain->GetLeaf("y")->SetAddress(y);   //[y_count]
   fChain->GetLeaf("z_count")->SetAddress( &z_count);
   fChain->GetLeaf("z")->SetAddress(z);   //[z_count]
   fChain->GetLeaf("px_count")->SetAddress( &px_count);
   fChain->GetLeaf("px")->SetAddress(px);   //[px_count]
   fChain->GetLeaf("py_count")->SetAddress( &py_count);
   fChain->GetLeaf("py")->SetAddress(py);   //[py_count]
   fChain->GetLeaf("pz_count")->SetAddress( &pz_count);
   fChain->GetLeaf("pz")->SetAddress(pz);   //[pz_count]
   fChain->GetLeaf("e1_count")->SetAddress( &e1_count);
   fChain->GetLeaf("e1")->SetAddress(e1);   //[e1_count]
   fChain->GetLeaf("e2_count")->SetAddress( &e2_count);
   fChain->GetLeaf("e2")->SetAddress(e2);   //[e2_count]
   fChain->GetLeaf("len_count")->SetAddress( &len_count);
   fChain->GetLeaf("len")->SetAddress(len);   //[len_count]
   fChain->GetLeaf("edep_count")->SetAddress( &edep_count);
   fChain->GetLeaf("edep")->SetAddress(edep);   //[edep_count]


   Notify();
}

Bool_t FnuG4::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void FnuG4::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t FnuG4::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


void FnuG4::Print(){
	
   printf("e_beam %lf ", e_beam);
   printf("id_beam %d ", id_beam);
   printf("x_beam %lf ", x_beam);
   printf("y_beam %lf ", y_beam);
   printf("pdgnu_nuEvt %d ", pdgnu_nuEvt);
   printf("pdglep_nuEvt %d ", pdglep_nuEvt);
   printf("Enu_nuEvt %lf ", Enu_nuEvt);
   printf("Plep_nuEvt %lf ", Plep_nuEvt);
   printf("cc_nuEvt %d ", cc_nuEvt);
   printf("x_nuEvt %lf ", x_nuEvt);
   printf("y_nuEvt %lf ", y_nuEvt);
   printf("z_nuEvt %lf ", z_nuEvt);
   printf("nhits %d ", nhits);
/*   Int_t           iz[846048];   //[iz_count]
   Int_t           izsub[846048];   //[izsub_count]
   Int_t           pdgid[846048];   //[pdgid_count]
   Int_t           id[846048];   //[id_count]
   Int_t           idParent[846048];   //[idParent_count]
   Double_t        charge[846048];   //[charge_count]
   Double_t        x[846048];   //[x_count]
   Double_t        y[846048];   //[y_count]
   Double_t        z[846048];   //[z_count]
   Double_t        px[846048];   //[px_count]
   Double_t        py[846048];   //[py_count]
   Double_t        pz[846048];   //[pz_count]
   Double_t        e1[846048];   //[e1_count]
   Double_t        e2[846048];   //[e2_count]
   Double_t        len[846048];   //[len_count]
   Double_t        edep[846048];   //[edep_count]
*/
	
}

/*
   // Declaration of leaf types
   Double_t        e_beam;
   Int_t           id_beam;
   Double_t        x_beam;
   Double_t        y_beam;
   Int_t           pdgnu_nuEvt;
   Int_t           pdglep_nuEvt;
   Double_t        Enu_nuEvt;
   Double_t        Plep_nuEvt;
   Int_t           cc_nuEvt;
   Double_t        x_nuEvt;
   Double_t        y_nuEvt;
   Double_t        z_nuEvt;
   Int_t           nhits;
//   Int_t           chamber_count;
   Int_t           chamber[846048];   //[chamber_count]
   Int_t           iz_count;
   Int_t           iz[846048];   //[iz_count]
   Int_t           izsub_count;
   Int_t           izsub[846048];   //[izsub_count]
   Int_t           pdgid_count;
   Int_t           pdgid[846048];   //[pdgid_count]
   Int_t           id_count;
   Int_t           id[846048];   //[id_count]
   Int_t           idParent_count;
   Int_t           idParent[846048];   //[idParent_count]
   Int_t           charge_count;
   Double_t        charge[846048];   //[charge_count]
   Int_t           x_count;
   Double_t        x[846048];   //[x_count]
   Int_t           y_count;
   Double_t        y[846048];   //[y_count]
   Int_t           z_count;
   Double_t        z[846048];   //[z_count]
   Int_t           px_count;
   Double_t        px[846048];   //[px_count]
   Int_t           py_count;
   Double_t        py[846048];   //[py_count]
   Int_t           pz_count;
   Double_t        pz[846048];   //[pz_count]
   Int_t           e1_count;
   Double_t        e1[846048];   //[e1_count]
   Int_t           e2_count;
   Double_t        e2[846048];   //[e2_count]
   Int_t           len_count;
   Double_t        len[846048];   //[len_count]
   Int_t           edep_count;
   Double_t        edep[846048];   //[edep_count]


*/


Particle* FnuG4::FindParticle(int id){
	
	int nparticles = particles->GetEntriesFast();
	for(int i=0; i<nparticles; i++){
		Particle *p = (Particle *) particles->At(i);
		if(p->id==id) return p;
	}
	return NULL;
}


// Particle* FnuG4::AddParticle(int idx){
	
//	Particle *p = new Particle;
//	p->id = id[idx];
//	p->index = idx;
//	p->idParent = idParent[idx];
//	p->t = new EdbTrackP;
	
//	particles->Add(p);
	
//	return p;
//}



#endif // #ifdef FnuG4_cxx



