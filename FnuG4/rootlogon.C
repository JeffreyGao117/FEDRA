#include "TROOT.h"
#include "TSystem.h"

void rootlogon(){
	gSystem->Load("FnuG4_C"); // after compilation of FnuG4_C.dll
	gSystem->Load("libRIO");
    gSystem->Load("libMatrix");
    gSystem->Load("libHist");
    gSystem->Load("libGraf");
    gSystem->Load("libNet");
    gSystem->Load("libGpad");
    gSystem->Load("libGraf3d");
    gSystem->Load("libTree");
    gSystem->Load("libTreePlayer");
    //gErrorIgnoreLevel=5000;
    //gDebug=7;
    gSystem->Load("/afs/cern.ch/user/j/jegao/virtualrec/FnuG4/virtualrec_c");
    gSystem->AddIncludePath(" -I/afs/cern.ch/user/j/jegao/virtualrec/Fedra/include");
}

//----------------------------------------------------------
void loadlib(const char *lib, const char *key)
{
  if (!TClassTable::GetDict(key)) { 
    if(gSystem->Load(lib)) printf("\n ERROR: \n%s is NOT loaded!",lib);
    else printf(" %s",lib);
  }
}

//----------------------------------------------------------
void loadlibEOracle()
{
  if (TClassTable::GetDict("TOracleServerE")) return;

  if( strncmp(gSystem->GetName(),"Unix",4)==0 ) {
    if( (gSystem->Load("libclntsh")==0) && 
	(gSystem->Load("libocci")==0) &&
        (gSystem->Load("libEOracle")==0) )
      printf(" libEOracle(Unux)"); return;
  } else {
    if( (gSystem->Load("oraocci10")==0) && 
	(gSystem->Load("libEOracle")==0) )
      printf(" libEOracle(Win32)"); return;
  }
  printf("\nERROR: \n libEOracle is NOT loaded!\n");
}

