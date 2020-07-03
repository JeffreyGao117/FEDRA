#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

void HistEntries(){
TFile *f = new TFile("FASERnu_numu.dump._001-Aki.root");
TTree *tree = f->Get("FASERnu");
int entries = tree->GetEntries();
std::cout << "Aki's root file contains " << entries << " entries." << std::endl;
}
