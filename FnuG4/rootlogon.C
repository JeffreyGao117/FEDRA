void rootlogon(){
	gSystem->Load("FnuG4_C"); // after compilation of FnuG4_C.dll
	gSystem->Load("libEdr");
	gSystem->Load("libEDA");
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

