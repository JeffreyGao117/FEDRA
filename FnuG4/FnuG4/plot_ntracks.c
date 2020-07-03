




plot_ntracks(){
	
	gStyle->SetPadRightMargin(0.05);
	
	TCanvas *c1 = new TCanvas("c1","",1200,900);
	c1->Divide(2,2);
	int ic1=1;
	
	
	TFile *_file0 = TFile::Open("ntracks.root");
	// ntracks2, after bug fix concerning the neutral hadron.
	
	
	float offsetZ = 603.3;
	
//	TCut cut=Form("sqrt(x*x+y*y)<100", offsetZ);
//	TCut cut=Form("sqrt(x*x+y*y)<100&&z<%f-260", offsetZ);
	TCut cut="1";
	
	
	c1->cd(ic1++);
	TH1F *hz = new TH1F("hz","", 15, offsetZ-1400, offsetZ+100);
	hz->SetFillColor(kGray);
	hz->SetLineWidth(0);
	hz->SetXTitle("#nu interaction Z (mm)");
	nt->Draw("z >>hz",cut);
	
	TH1F *hzCC = hz->Clone("hzCC");
	hzCC->SetFillColor(kAzure-9);
	nt->Draw("z >>hzCC",  cut&&"CC==1","same");
	
	TH1F *hzCC1 = hz->Clone("hzCC1");
	hzCC1->SetFillColor(kAzure+7);
	nt->Draw("z >>hzCC1", cut&&"CC==1&&ntracks>=1","same");
	
	TH1F *hzCC2 = hz->Clone("hzCC2");
	hzCC2->SetFillColor(kSpring+9);
	nt->Draw("z >>hzCC2", cut&&"CC==1&&ntracks>=2","same");
	
	TH1F *hzCC3 = hz->Clone("hzCC3");
	hzCC3->SetFillColor(kCyan-7);
	nt->Draw("z >>hzCC3", cut&&"CC==1&&ntracks>=3","same");
	
	
	
	hz->Draw();
	hzCC->Draw("same");
	hzCC1->Draw("same");
	hzCC2->Draw("same");
	hzCC3->Draw("same");
	hz->Draw("axis, same");
	
	TLegend *leg = new TLegend(0.11, 0.8, 0.3, 0.95);
	leg->AddEntry(hz,"All","f");
	leg->AddEntry(hzCC,"CC","f");
	leg->Draw();
	leg = new TLegend(0.32, 0.73, 0.77, 0.95);
	leg->AddEntry(hzCC1,"CC connected with #geq1 trks","f");
	leg->AddEntry(hzCC2,"CC connected with #geq2 trks","f");
	leg->AddEntry(hzCC3,"CC connected with #geq3 trks","f");
	leg->Draw();
	
	c1->cd(ic1++);
	
	TH2F *h2ZN = new TH2F ("h2ZN","", 75, offsetZ-1400, offsetZ+100, 31, -0.5,30.5);
	nt->Draw("ntracks:z >>h2ZN", cut&&"CC==1&&ntracks<50","col");
	
	c1->cd(ic1++);
	
	TH1F *hzCCEff3 = hzCC3->Clone("hzCCEff3");
	hzCCEff3->Divide(hzCC);
	hzCCEff3->SetStats(0);
	hzCCEff3->SetMaximum(1.15);
	
	TH1F *hzCCEff2 = hzCC2->Clone("hzCCEff2");
	hzCCEff2->Divide(hzCC);
	TH1F *hzCCEff1 = hzCC1->Clone("hzCCEff1");
	hzCCEff1->Divide(hzCC);
	
	hzCCEff1->SetYTitle("Efficiency");
	hzCCEff1->SetStats(0);
	hzCCEff1->Draw();
	hzCCEff2->Draw("same");
	hzCCEff3->Draw("same");
	
	hzCCEff1->Draw("axis, same");
	gPad->SetGridy();
	leg = new TLegend(0.12, 0.35, 0.6, 0.62);
	leg->AddEntry(hzCC1,"CC connected with #geq1 trks","f");
	leg->AddEntry(hzCC2,"CC connected with #geq2 trks","f");
	leg->AddEntry(hzCC3,"CC connected with #geq3 trks","f");
	 
	leg->Draw();
	
	
	hzCC->Print();
	hzCC1->Print();
	hzCC2->Print();
	hzCC3->Print();
	
	printf("start z<2 lamba int\n");
	
	printf("hzCC = %.0f\n", hzCC->Integral(0,12));
	printf("hzCC1 = %.0f\n", hzCC2->Integral(0,12));
	printf("hzCC2 = %.0f\n", hzCC2->Integral(0,12));
	printf("hzCC3 = %.0f\n", hzCC3->Integral(0,12));
	
	c1->Print("FASERnu-IFT.pdf");
	
	TCanvas *c2 = new TCanvas("c2","",800,1200);
	c2->Divide(2,4);
	int ic2=1;
	
//	TH1F *h1d0 = new TH1F("h1d0","",100, -0.5*1000, 0.5*1000);
	
	c2->cd(ic2++);
	ntReso->Draw("dx0*1000 >>hDX0(100, -500, 500)");
	ntReso->Draw("dx0*1000 >>hDX0m(100, -500, 500)","pdg==13","same");
	hDX0->SetFillColor(kAzure-9);
	hDX0m->SetFillColor(kCyan-7);
	
	TLegend *leg = new TLegend(0.6,0.4,0.9,0.7);
	leg->AddEntry(hDX0,"All","F");
	leg->AddEntry(hDX0m,"muons","F");
	leg->Draw();
	
	
	
	c2->cd(ic2++);
	ntReso->Draw("dx1*1000 >>hDX1(100, -500, 500)");
	ntReso->Draw("dx1*1000 >>hDX1m(100, -500, 500)","pdg==13","same");
	hDX1->SetFillColor(kAzure-9);
	hDX1m->SetFillColor(kCyan-7);
	
	c2->cd(ic2++);
	ntReso->Draw("dx2*1000 >>hDX2(100, -500, 500)");
	ntReso->Draw("dx2*1000 >>hDX2m(100, -500, 500)","pdg==13","same");
	hDX2->SetFillColor(kAzure-9);
	hDX2m->SetFillColor(kCyan-7);
	
	c2->cd(ic2++);
	ntReso->Draw("dx3*1000 >>hDX3(100, -500, 500)");
	ntReso->Draw("dx3*1000 >>hDX3m(100, -500, 500)","pdg==13","same");
	hDX3->SetFillColor(kAzure-9);
	hDX3m->SetFillColor(kCyan-7);
	
	c2->cd(ic2++);
	TH2F *h2 = new TH2F("h2","", 5, -1.5, 3.5, 100, -500, 500);
	ntReso->Draw("dx0*1000:-1 >>h2");
	ntReso->Draw("dx1*1000:1 >>+h2");
	ntReso->Draw("dx2*1000:2 >>+h2");
	ntReso->Draw("dx3*1000:3 >>+h2");
	
}


