from ROOT import *
gROOT.SetBatch(True)
gROOT.LoadMacro('AtlasStyle.C')
SetAtlasStyle()

#inputDir='TestFiles'
#inputDir='/nfs/slac/atlas/fs1/d/mgignac/VSI_merged/v03/'
inputDir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_m_31319'

#- - - - - - - - - - - - - - - - 

inputFiles={ 448182: 'processed_mc16_13TeV.448182.MGPy8EG_A14NNPDF23LO_GG_qqn1_2400_2050_rpvLF_1ns.VsiAnaModular.root',
             309852: 'processed_mc16_13TeV.309862pm.PowhegPythia8EvtGen__WH_H125_a55a55_4b_ctau100.VsiAnaModular.root',
             309866: 'processed_mc16_13TeV.309866pm.PowhegPythia8EvtGen__WH_H125_a55a55_4u_ctau100.VsiAnaModular.root',
             310187: 'processed_mc16_13TeV.310187.Pythia8EvtGen_A14NNPDF23LO_WmuHNL50_15G_lt100dd_mu.50k.FullSim.VsiAnaModular.root',
             310186: 'processed_mc16_13TeV.310186.Pythia8EvtGen_A14NNPDF23LO_WmuHNL50_10G_lt10dd.VsiAnaModular.root',
           } 

legendNames={ 448182: 'pp #rightarrow #tilde{g}#tilde{g},#tilde{g}#rightarrow qq #tilde{#chi}^{0}_{1}, #tilde{#chi}^{0}_{1} #rightarrow qqq, m(#tilde{#chi}^{0}_{1}) = 2050 GeV, c#tau(#tilde{#chi}^{0}_{1}) = 300 mm',
              309852: 'pp #rightarrow W^{#pm}h, h#rightarrow aa, a#rightarrow bb, m(a) = 55 GeV, c#tau(a) = 100 mm',
              309866: 'pp #rightarrow W^{#pm}h, h#rightarrow aa, a#rightarrow uu, m(a) = 55 GeV, c#tau(a) = 100 mm',
              310187: 'pp #rightarrow W^{#pm} #rightarrow #mu N, N #rightarrow #mu#nu#mu, m(N) = 15 GeV, c#tau(N) = 100 mm',
              310186: 'pp #rightarrow W^{#pm} #rightarrow #mu N, N #rightarrow #mu#nu#mu, m(N) = 10 GeV, c#tau(N) = 10 mm',
            }

colors={ 448182 : kBlue,
         309852 : kGreen+2,
         309866 : kCyan,
         310187 : kRed,
         310186 : kGreen,
        }

styles = { 448182 : 8,
           309852 : 22,
           309866 : 10,
           310187 : 24,
           310186 : 4,
        }
            
###################################################################################################

class Plot:
    def __init__(self, name, distribution, xMin=None, xMax=None, yMin=0, yMax=1.0, legendCoords=[0.18,0.70,0.93,0.85], xTitle=None, yTitle=None,options=''):
        self.name=name
        self.distribution=distribution
        self.xMin=xMin
        self.xMax=xMax
        self.yMin=yMin
        self.yMax=yMax
        self.xTitle=xTitle
        self.yTitle=yTitle
        self.options=options

        self.legend=TLegend(legendCoords[0],legendCoords[1],legendCoords[2],legendCoords[3])
        self.legend.SetBorderSize(0);
        self.legend.SetFillColor(kWhite);
        self.legend.SetTextSize(0.03)
        self.legend.SetTextFont(42);

        self.line = TF1("line", "1", -1e5, 1e5);
        self.line.SetLineColor(1);
        self.line.SetLineStyle(2);

        self.output=TFile(name+'.root','recreate')
        self.canvas=TCanvas()
        self.empty=True

    #- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        
    def add(self, ID):
        f=TFile(inputDir+'/'+inputFiles[ID])
        self.output.cd()
        try: o=f.Get(self.distribution).Clone(str(ID)+'_'+self.distribution)
        except: 
            print str(ID),self.distribution
            raise Exception()
        SetOwnership(o,False)
        o.SetLineColor(colors[ID])
        o.SetLineWidth(2)
        o.SetMarkerColor(colors[ID])
        o.SetMarkerSize(1.2);
        o.SetMarkerStyle(styles[ID]); 
        o.Write()

        if self.empty: 
            if 'logX'in self.options: self.canvas.SetLogx()
            o.SetTitle("; %s ; %s" % (self.xTitle,self.yTitle));
            o.Draw()
            gPad.Update()
            pgr = o.GetPaintedGraph(); 
            pgr.SetMinimum(self.yMin)
            pgr.SetMaximum(self.yMax)
            gPad.Update()

            line = TF1("line", "1", -1e5, 1e5);
            line.SetLineColor(1);
            line.SetLineWidth(2)
            line.Draw("same"); 
            gPad.Update()

            self.empty=False
        else: 
            o.Draw('same')

        self.legend.AddEntry(o,legendNames[ID],'ple')
        self.legend.Draw('same')

        gROOT.LoadMacro("AtlasUtils.C")
        ATLAS_LABEL(0.2,0.865)
        myText(0.32,0.865,1,"Simulation Preliminary")
        myText(0.66,0.865,1,"#sqrt{s}= 13 TeV")

        self.line.Draw('same');

        self.canvas.Modified()
        self.canvas.Update()
    
    #- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        
    def save(self):
        self.canvas.Print(self.name+'.pdf')
        self.canvas.Print(self.name+'.eps')
        self.canvas.Print(self.name+'.png')
        self.output.Write()

###################################################################################################

if __name__=='__main__':

    common_acc=Plot('eff_radius_acceptance','acceptance_fiducial1_r',yMin=0,yMax=1.6,xTitle='r [mm]',yTitle='Acceptance',options='logX')
    common_acc.add(448182)
    common_acc.add(309852)
    common_acc.add(310187)
    common_acc.save()

    common_seed=Plot('eff_radius_seed','seedEff_r',yMin=0,yMax=1.6,xTitle='r [mm]',yTitle='#varepsilon_{seed}',options='logX')
    common_seed.add(448182)
    common_seed.add(309852)
    common_seed.add(310187)
    common_seed.save()

    common_core=Plot('eff_radius_core','recoEff_fiducial2_r',yMin=0,yMax=1.6,xTitle='r [mm]',yTitle='#varepsilon_{core}',options='logX')
    common_core.add(448182)
    common_core.add(309852)
    common_core.add(310187)
    common_core.save()

    common_total=Plot('eff_radius_total','recoEff_r',yMin=0,yMax=1.6,xTitle='r [mm]',yTitle='#varepsilon_{total}',options='logX')
    common_total.add(448182)
    common_total.add(309852)
    common_total.add(310187)
    common_total.save()

    common_alg=Plot('eff_radius_alg','recoEff_fiducial1_r',yMin=0,yMax=1.6,xTitle='r [mm]',yTitle='#varepsilon_{alg}',options='logX')
    common_alg.add(448182)
    common_alg.add(309852)
    common_alg.add(310187)
    common_alg.save()

    ###################################################################################################

    ntrk_core=Plot('eff_ntrk_core','recoEff_fiducial2_mult',yMin=0,yMax=1.6,xTitle='Number of selected tracks',yTitle='#varepsilon_{core}')
    ntrk_core.add(448182)
    ntrk_core.add(309852)
    ntrk_core.save()

    ntrk_total=Plot('eff_ntrk_total','recoEff_mult',yMin=0,yMax=1.6,xTitle='Number of selected tracks',yTitle='#varepsilon_{total}')
    ntrk_total.add(448182)
    ntrk_total.add(309852)
    ntrk_total.save()

    ntrk_alg=Plot('eff_ntrk_alg','recoEff_fiducial1_mult',yMin=0,yMax=1.6,xTitle='Number of selected tracks',yTitle='#varepsilon_{alg}')
    ntrk_alg.add(448182)
    ntrk_alg.add(309852)
    ntrk_alg.save()

    ###################################################################################################

    common_total=Plot('selEff','selEff',yMin=0,yMax=1.6,xTitle='r [mm]',yTitle='Selection Efficiency')
    common_total.add(448182)
    common_total.add(309852)
    common_total.save()

    ###################################################################################################
    
    openAngle_core=Plot('eff_openAngle_core','recoEff_fiducial2_openAngle',yMin=0,yMax=1.6,xTitle='Maximum #DeltaR',yTitle='#varepsilon_{core}')
    #openAngle_core.add(310186)
    openAngle_core.add(310187)
    openAngle_core.save()

    openAngle_total=Plot('eff_openAngle_total','recoEff_openAngle',yMin=0,yMax=1.6,xTitle='Maximum #DeltaR',yTitle='#varepsilon_{total}')
    #openAngle_total.add(310186)
    openAngle_total.add(310187)
    openAngle_total.save()


    split_r=Plot('splitRate_radius','splitRate_radius',yMin=0,yMax=0.20,xTitle='Radius [mm]',yTitle='Split rate',options='logX')
    split_r.add(448182)
    split_r.add(309852)
    split_r.add(309866)
    split_r.save()


    split_d=Plot('splitRate_ntrk','splitRate_trackMult',yMin=0,yMax=0.20,xTitle='Number of reconstructible truth particles',yTitle='Split rate')
    split_d.add(448182)
    split_d.add(309852)
    split_d.add(309866)
    split_d.save()


    ###################################################################################################
    averageMu_acc=Plot('acceptance_averageMu','acceptance_fiducial1_averageMu',yMin=0,yMax=1.6,xTitle='Average number of pp interactions',yTitle='Acceptance')
    averageMu_acc.add(448182)
    averageMu_acc.add(309852)
    averageMu_acc.add(310187)
    averageMu_acc.save()

    averageMu_total=Plot('eff_averageMu_total','recoEff_averageMu',yMin=0,yMax=1.6,xTitle='Average number of pp interactions',yTitle='#varepsilon_{total}')
    averageMu_total.add(448182)
    averageMu_total.add(309852)
    averageMu_total.add(310187)
    averageMu_total.save()

    averageMu_seed=Plot('eff_averageMu_seed','seedEff_averageMu',yMin=0,yMax=1.6,xTitle='Average number of pp interactions',yTitle='#varepsilon_{seed}')
    averageMu_seed.add(448182)
    averageMu_seed.add(309852)
    averageMu_seed.add(310187)
    averageMu_seed.save()

    averageMu_core=Plot('eff_averageMu_core','recoEff_fiducial2_averageMu',yMin=0,yMax=1.6,xTitle='Average number of pp interactions',yTitle='#varepsilon_{core}')
    averageMu_core.add(448182)
    averageMu_core.add(309852)
    averageMu_core.add(310187)
    averageMu_core.save()
    ###################################################################################################


