#include "AtlasStyle.C"
#include "AtlasUtils.C"

void selEff() {
    SetAtlasStyle();

    std::map<int, std::string> filenames {
        { 50, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448162.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_50_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
        { 200, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448190.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_200_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },        
        { 450, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448166.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_450_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
        { 850, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448170.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_850_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
        { 1250, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448174.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_1250_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
        { 1650, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448178.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_1650_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
        { 2050, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448182.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
        { 2350, std::string("/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_cleaned_31319/processed_user.mgignac.mc16_13TeV.448186.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2350_rpvLF_1ns.20190228_TrkCleaning_v01_VsiAnaModular.root") },
    };

    auto* eff_noAttachment = new TGraphAsymmErrors;
    auto* eff_Attachment   = new TGraphAsymmErrors;

    eff_noAttachment->SetMarkerColor(kGray+1);
    eff_noAttachment->SetLineColor(kGray+1);
    eff_noAttachment->SetLineWidth(2);
    eff_noAttachment->SetMarkerStyle(20);
    
    eff_Attachment->SetMarkerColor(kAzure);
    eff_Attachment->SetLineColor(kAzure);
    eff_Attachment->SetLineWidth(2);
    eff_Attachment->SetMarkerStyle(20);
    
    std::map<int, TEfficiency*> effs;
    for( auto& pair: filenames ) {
        auto mass = pair.first;
        auto* file = TFile::Open( pair.second.c_str() );
        auto* eff = dynamic_cast<TEfficiency*>( file->Get("selEff") );
        eff->SetName( Form("selEff_%d", mass) );
        effs.emplace( std::make_pair( mass, eff ) );

        eff_noAttachment->SetPoint( eff_noAttachment->GetN(), mass, eff->GetEfficiency(1) );
        eff_noAttachment->SetPointEYhigh( eff_noAttachment->GetN()-1, eff->GetEfficiencyErrorUp(1) );
        eff_noAttachment->SetPointEYlow( eff_noAttachment->GetN()-1, eff->GetEfficiencyErrorLow(1) );
        
        eff_Attachment->SetPoint( eff_Attachment->GetN(), mass, eff->GetEfficiency(2) );
        eff_Attachment->SetPointEYhigh( eff_Attachment->GetN()-1, eff->GetEfficiencyErrorUp(2) );
        eff_Attachment->SetPointEYlow( eff_Attachment->GetN()-1, eff->GetEfficiencyErrorLow(2) );
    }

    auto* h = new TH1F("h", ";m(#tilde{#chi}_{1}^{0}) [GeV];#varepsilon_{sel}", 1, 30, 4000);
    h->SetMaximum(1.45);
    h->Draw();
    gPad->SetLogx();

    eff_noAttachment->Draw("pe,same");
    eff_Attachment->SetMarkerStyle(21);
    eff_Attachment->Draw("pe,same");

    ATLAS_LABEL(0.2, 0.87);

      TLatex text;
      text.SetTextAlign(12);
      text.SetTextSize(0.04);
      text.SetTextFont(42);
      text.DrawLatexNDC(0.32,0.885,"Simulation Preliminary");

    TLatex latex;
    latex.SetTextFont(42);
    latex.SetTextAlign(12);
    latex.SetTextSize(0.04);
    latex.DrawLatexNDC(0.61, 0.885, "#sqrt{s} = 13 TeV");
    latex.DrawLatexNDC(0.2, 0.81, "pp #rightarrow #tilde{g}#tilde{g},  #tilde{g} #rightarrow qq #tilde{#chi}_{1}^{0},  #tilde{#chi}_{1}^{0} #rightarrow qqq, c#tau(#tilde{#chi}^{0}_{1}) = 300 mm" );
    latex.DrawLatexNDC(0.2, 0.74, "Selection Criteria: n_{track} #geq 5,  m > 10 GeV" );

    auto* lgd = new TLegend(0.55, 0.20, 1.0, 0.4);
    lgd->SetBorderSize(0);
    lgd->SetFillStyle(0);
    lgd->SetTextFont(42);
    lgd->AddEntry( eff_Attachment, "w/ Track Attachment", "pe" );
    lgd->AddEntry( eff_noAttachment, "w/o Track Attachment", "pe" );
    lgd->Draw();

    auto* line1 = new TLine(30.0, 1.0, 4000, 1.0);
    line1->SetLineStyle(3);
    line1->Draw();
    gPad->SaveAs("selEff.pdf");
}
