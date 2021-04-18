

//TString path = "/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_m/";
TString path = "/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_noMatchScore_v02/";

void drawText(TString textString,float size,int style,float x, float y)
{
  TLatex text;
  text.SetNDC();
  text.SetTextAlign(13);
  text.SetTextSize(size);
  text.SetTextFont(style);
  text.DrawLatex(x,y,textString);
}

double integrate(TH1D* hist, double threshold)
{
  return hist->Integral( hist->GetXaxis()->FindBin(threshold), hist->GetNbinsX()+1 ) / hist->Integral(0,hist->GetNbinsX()+1 );
}



TH1* retrieveHist(TString  rootFileName,TString name)
{
  TFile* f = TFile::Open(path+"/"+rootFileName,"READ");
  TH1* h = dynamic_cast<TH1*>( f->Get(name ) );
  return h;

}

TH1D* getHist(TString rootFileName,TString name)
{

  TFile* f = TFile::Open(path+"/"+rootFileName,"READ");
  TTree* tree = dynamic_cast<TTree*>( f->Get("vsitree") );

  const int nbins=22;
  Double_t xbins[nbins+1] = {-10,-6,-4,-3,-2,-1.5,-1,-0.8,-0.6,-0.4,-0.2,0,0.2,0.4,0.6,0.8,1,1.5,2,3,4,6,10};

  TH1D* h = new TH1D(name.Data(),"",nbins,xbins);
  tree->Draw( TString::Format("DVeff.recoVtx_r-DVeff.LLP_decay_r>>+%s",name.Data()) );

  for (int i=1; i<nbins+1; i++)
    h->SetBinContent(i,h->GetBinContent(i)/(h->GetBinWidth(i)/0.2));

  h->Scale(1.0/h->Integral());
  return h;


}

TH1D* getMatchScoreHist(TString rootFileName,TString name)
{

  TFile* f = TFile::Open(path+"/"+rootFileName,"READ");
  TTree* tree = dynamic_cast<TTree*>( f->Get("vsitree") );

  const int nbins=24;
  Double_t xbins[nbins+1] = {0.0 , 0.05 , 0.1 , 0.15 , 0.2 , 0.25 , 0.3 , 0.35 , 0.4 , 0.45 , 0.5 , 0.55 , 0.6 , 0.65 , 0.7 , 0.75 , 0.8 , 0.85 , 0.9 , 0.95 , 0.96, 0.97, 0.98, 0.99, 1.0000001};

  TH1D* h = new TH1D(name.Data(),"",nbins,xbins);
  tree->Draw( TString::Format("DVeff.recoVtx_matchScore>>+%s",name.Data()) );

  for (int i=1; i<nbins+1; i++)
    h->SetBinContent(i,h->GetBinContent(i)/(h->GetBinWidth(i)/0.01));

  //tree->Draw( TString::Format("DVeff.recoVtx_matchScore>>%s(103,0,1.03)",name.Data()) );
  //tree->Draw( TString::Format("DVeff.recoVtx_matchScore>>%s(20,0,1)",name.Data()) );
  //TH1D* h = (TH1D*)tree->GetHistogram();
  h->Scale(1.0/h->Integral());
  return h;


}

void setHistStyle(TH1D*& gr,Color_t color,int style)
{

  gr->GetYaxis()->SetLabelSize(0.045);
  gr->GetYaxis()->SetTitleSize(0.045);
  gr->GetXaxis()->SetLabelSize(0.045);
  gr->GetXaxis()->SetTitleSize(0.045);
  gr->GetYaxis()->SetTitleOffset(1.50);
  gr->SetTitle("");
  //gr->SetMarkerStyle(style);
  gr->SetLineColor(color);
  gr->SetLineStyle(style);
  gr->SetLineWidth(2);
  gr->SetMarkerColor(color);
  //gr->SetMarkerSize(1.2);


}

void doMatchScore()
{


  TH1D* g = getMatchScoreHist("merged_448182_MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.root","g");
  TH1D* h = getMatchScoreHist("merged_310187_Pythia8EvtGen_A14NNPDF23LO_WmuHNL50_15G_lt100dd_mu.root","h");
  TH1D* b = getMatchScoreHist("merged_309862_PowhegPythia8EvtGen_WpmH_H125_a55a55_4b_ctau100.root","b");

  TCanvas* c1 = new TCanvas();
  c1->SetBorderSize(0);
  c1->SetFillColor(kWhite);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(0.15);
  c1->SetBottomMargin(0.12);
  c1->SetLogy();
  gStyle->SetOptStat(0);

  setHistStyle(g,kBlue,7);
  setHistStyle(h,kRed,1);
  setHistStyle(b,kGreen+2,3);

  g->GetYaxis()->SetTitle("Probability / 0.01");
  g->GetXaxis()->SetTitle("Match score s(l,v)");
  g->GetYaxis()->SetRangeUser(0.00008,200);

  g->Draw("hist E0");
  h->Draw("hist E0 same");
  b->Draw("hist E0 same");

  TH1D* gc=(TH1D*)g->Clone("gc");
  gc->SetLineStyle(1);
  TH1D* hc=(TH1D*)h->Clone("hc");
  hc->SetLineStyle(1);
  TH1D* bc=(TH1D*)b->Clone("bc");
  bc->SetLineStyle(1);
  gc->Draw("E0 X0 same");
  hc->Draw("E0 X0 same");
  bc->Draw("E0 X0 same");

  float minusSize = 0.005;

  drawText( "ATLAS", 0.05-minusSize,72,0.20,0.85);
  drawText( "Simulation" ,0.055-minusSize,42,0.31,0.85);
  drawText( "Preliminary" , 0.055-minusSize,42,0.465,0.85);
  drawText( TString::Format("#sqrt{s} = %i TeV",13), 0.05-minusSize,42,0.65,0.85);

  TLegend* leg = new TLegend(0.16,0.62,0.67,0.78);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetFillColor(kWhite);

  leg->AddEntry(g,"pp #rightarrow #tilde{g}#tilde{g},#tilde{g}#rightarrow qq, #tilde{#chi}^{0}_{1}, #tilde{#chi}^{0}_{1} #rightarrow qqq, m(#tilde{#chi}^{0}_{1}) = 2050 GeV, c#tau(#tilde{#chi}^{0}_{1}) = 300 mm");
  leg->AddEntry(b,"pp #rightarrow W^{#pm}h, h#rightarrow aa, a#rightarrow bb, m(a) = 55 GeV, c#tau(a) = 100 mm");
  leg->AddEntry(h,"pp #rightarrow W^{#pm} #rightarrow #mu N, N #rightarrow #mu#nu#mu, m(N) = 15 GeV, c#tau(N) = 100 mm");
  leg->Draw("same");


  c1->SaveAs("VSI_matchScore_inclusive.pdf");
  c1->SaveAs("VSI_matchScore_inclusive.eps");


  double HNL = integrate(h,1.0);
  double gg  = integrate(g,1.0);
  double abb = integrate(b,1.0);
  std::cout << "Purity of HNL for matchScore ==1 : " << HNL << std::endl; 
  std::cout << "Purity of gg for matchScore ==1 : " << gg << std::endl;
  std::cout << "Purity of abb for matchScore ==1 : " << abb << std::endl;


}

void doRes()
{

  TH1D* g = getHist("merged_mc16_13TeV.448182.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.VsiAnaModular.root","g");
  TH1D* h = getHist("merged_mc16_13TeV.310187.Pythia8EvtGen_A14NNPDF23LO_WmuHNL50_15G_lt100dd_mu.FullSim.VsiAnaModular.root","h");
  TH1D* b = getHist("merged_mc16_13TeV.309862pm.PowhegPythia8EvtGen__WH_H125_a55a55_4b_ctau100.VsiAnaModular.root","b");

  TCanvas* c1 = new TCanvas();
  c1->SetBorderSize(0);
  c1->SetFillColor(kWhite);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(0.15);
  c1->SetBottomMargin(0.12);
  c1->SetLogy();
  gStyle->SetOptStat(0);

  setHistStyle(g,kBlue,7); //7   
  setHistStyle(h,kRed,1);
  setHistStyle(b,kGreen+2,3); //2

  g->GetYaxis()->SetTitle("Probability / 0.2 mm");
  g->GetXaxis()->SetTitle("r_{reco} - r_{LLP decay}^{truth} [mm]");
  g->GetYaxis()->SetRangeUser(0.0002,200);

  g->Draw("hist E0");
  //u->Draw("hist E0");
  h->Draw("hist E0 same");
  b->Draw("hist E0 same");

  TH1D* gc=(TH1D*)g->Clone("gc");
  gc->SetLineStyle(1);
  TH1D* hc=(TH1D*)h->Clone("hc");
  hc->SetLineStyle(1);
  TH1D* bc=(TH1D*)b->Clone("bc");
  bc->SetLineStyle(1);
  gc->Draw("E0 X0 same");
  hc->Draw("E0 X0 same");
  bc->Draw("E0 X0 same");

  float minusSize = 0.005;

  drawText( "ATLAS", 0.05-minusSize,72,0.20,0.85);
  drawText( "Simulation" ,0.055-minusSize,42,0.31,0.85);
  drawText( "Preliminary" , 0.055-minusSize,42,0.465,0.85);
  drawText( TString::Format("#sqrt{s} = %i TeV",13), 0.05-minusSize,42,0.65,0.85);

  TLegend* leg = new TLegend(0.17,0.62,0.67,0.78);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetFillColor(kWhite);

  leg->AddEntry(g,"pp #rightarrow #tilde{g}#tilde{g},#tilde{g}#rightarrow qq, #tilde{#chi}^{0}_{1}, #tilde{#chi}^{0}_{1} #rightarrow qqq, m(#tilde{#chi}^{0}_{1}) = 2050 GeV, c#tau(#tilde{#chi}^{0}_{1}) = 300 mm");
  //leg->AddEntry(u,"pp #rightarrow Vh, h#rightarrow aa, a#rightarrow qq, m(a) = 55 GeV, c#tau(a) = 100 mm");
  leg->AddEntry(b,"pp #rightarrow W^{#pm}h, h#rightarrow aa, a#rightarrow bb, m(a) = 55 GeV, c#tau(a) = 100 mm");
  leg->AddEntry(h,"pp #rightarrow W^{#pm} #rightarrow #mu N, N #rightarrow #mu#nu#mu, m(N) = 15 GeV, c#tau(N) = 100 mm");
  leg->Draw("same");


  c1->SaveAs("VSI_positionResolution_inclusive.pdf");
  c1->SaveAs("VSI_positionResolution_inclusive.eps");



}

TGraphErrors* getGraph(TH2F*& h)
{

  TGraphErrors* gr = new TGraphErrors();
  for( int x=1; x<h->GetNbinsX()+1; x++){

    TH1D* p = h->ProjectionY( TString::Format("%i",x),x,x);
    p->Draw();
    std::cout << "Bin " << x << " and RMS " << p->GetRMS() << std::endl;
    int N = gr->GetN();
    gr->SetPoint(N,h->GetXaxis()->GetBinCenter(x),p->GetStdDev()*1000.0);
    gr->SetPointError(N,0,p->GetStdDevError()*1000.0 );
  }
  return gr;


}

void setGraph(TGraphErrors*& gr,Color_t color, int style)
{

  gr->GetYaxis()->SetLabelSize(0.05);
  gr->GetYaxis()->SetTitleSize(0.05);
  gr->GetXaxis()->SetLabelSize(0.05);
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->SetTitle("");
  gr->SetMarkerStyle(style);
  gr->SetMarkerColor(color);
  gr->SetLineColor(color);
  gr->SetMarkerSize(1.2);


}

void doResolution(TString suffix,TString saveName,TString xTitle="",int yMax=-1,TString option="",TString text="")
{

  TH2F* hr    = (TH2F*)retrieveHist("processed_mc16_13TeV.448182.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.VsiAnaModular.root",TString::Format("residual1_r%s",suffix.Data()) );
  TH2F* hz    = (TH2F*)retrieveHist("processed_mc16_13TeV.448182.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.VsiAnaModular.root",TString::Format("residual1_z%s",suffix.Data()));
  TH2F* hrphi = (TH2F*)retrieveHist("processed_mc16_13TeV.448182.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.VsiAnaModular.root",TString::Format("residual1_rphi%s",suffix.Data()));


  TGraphErrors* gr    = getGraph(hr);
  TGraphErrors* gz    = getGraph(hz);
  TGraphErrors* grphi = getGraph(hrphi);

  TCanvas* c1 = new TCanvas();
  c1->SetBorderSize(0);
  c1->SetFillColor(kWhite);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetLeftMargin(0.15);
  c1->SetBottomMargin(0.12);
  if( option.Contains("logx")){
    c1->SetLogx();
  }
  gStyle->SetOptStat(0);


  setGraph(gr,kRed,8);
  setGraph(gz,kOrange,25);
  setGraph(grphi,kBlue,23);
  if( yMax>0 ){
    gr->GetHistogram()->GetYaxis()->SetRangeUser(0,yMax);
  }
  

  gr->GetHistogram()->GetYaxis()->SetTitle("Resolution [#mum]");
  gr->GetHistogram()->GetXaxis()->SetTitle(xTitle);

  gr->Draw("AP");
  gz->Draw("same P");
  grphi->Draw("same P");

  float minusSize = 0.005;

  drawText( "ATLAS", 0.05-minusSize,72,0.20,0.85);
  drawText( "Simulation" ,0.055-minusSize,42,0.31,0.85);
  drawText( "Preliminary" , 0.055-minusSize,42,0.465,0.85);
  drawText( TString::Format("#sqrt{s} = %i TeV",13), 0.05-minusSize,42,0.65,0.85);

  TLegend* leg = 0;
  if( option.Contains("left") ){
    leg = new TLegend(0.20,0.60,0.50,0.78);
  }
  else{
    leg = new TLegend(0.50,0.58,0.67,0.78);
  }

  leg->SetBorderSize(0);
  leg->SetTextSize(0.05);
  leg->SetFillColor(kWhite);
  leg->AddEntry(gr,"#sigma_{r}","pe");
  leg->AddEntry(gz,"#sigma_{z}","pe");
  leg->AddEntry(grphi,"r#times#sigma_{#phi}","pe");
  leg->Draw("same");

  c1->SaveAs( TString::Format("VertexPostion_%s.pdf",saveName.Data() ) );

}

void plotRes()
{

  //doResolution("_mult","nTrk","Number of selected tracks",500,"");
  //doResolution("","radius","r [mm]",500,"left");
  //doRes();
  doMatchScore();
  
}

