#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
using std::to_string;

double* fitDoubleGauss(double* rs, TF1* dg, TH1D* h){ 
    
    dg->SetParameters(h->GetMaximum(), 0.85, 0, 0.7*h->GetRMS(), 1.2*h->GetRMS());
    dg->SetParLimits(1, 0, 1);
    dg->SetParLimits(3, 0.02, 1);
    dg->SetParLimits(4, 0.02, 5);
    h->Fit(dg, "RQ+");
    double weight = dg->GetParameter(1);
    double sCore = dg->GetParameter(3);
    double sTail = dg->GetParameter(4);
    rs[0] = sqrt(sCore*sCore*weight + sTail*sTail*(1-weight));

    double weightErr = dg->GetParError(1);
    double sCoreErr= dg->GetParError(3);
    double sTailErr= dg->GetParError(4);
    rs[1] = sqrt(pow(sCore*weight*sCoreErr,2) + pow(sTail*(1-weight)*sTailErr,2) + 0.25*pow((sCore*sCore - sTail*sTail)*weightErr,2)) / rs[0];

    return rs;
}

void Efficiency(double *effic = nullptr, double *rms = nullptr) {

    cout << "\n----------------------------------" << endl;
    cout << "-------- EFFICIENCY --------------" << endl;
    cout << "----------------------------------" << endl << endl;

    // Lettura file input e ntuple
    TFile fileIn("treeReconstructed.root");
    float flag, zRec, zTrue, mult;
    TNtuple *ntuple = (TNtuple*) fileIn.Get("nt");
    ntuple->SetBranchAddress("flag", &flag);
    ntuple->SetBranchAddress("zrec", &zRec);
    ntuple->SetBranchAddress("ztrue", &zTrue);
    ntuple->SetBranchAddress("mult", &mult);

    // Creazione file output in cui salvare gli istogrammi
    TFile fileOut("histograms4.root", "RECREATE");

    // Variabili ausiliarie
    const double zMin = -200;
    const double zMax = 200;
    const double residMin = -0.8;
    const double residMax = 0.8;
    const int multMin = 0;
    const int multMax = 70;

    // funzione di fit
    TF1* dg = new TF1("dg", "[0]*([1]*exp(-0.5*((x-[2])/[3])*((x-[2])/[3])) + (1-[1])*exp(-0.5*((x-[2])/[4])*((x-[2])/[4])))", -0.5, 0.5);

    // Creazione istogrammi
    TH2D *histo1 = new TH2D("hist1", "Residuals vs multiplicity", 71, multMin-0.5, multMax+0.5, 401, residMin, residMax);
    TH2D *histo2 = new TH2D("hist2", "Residuals vs Ztrue", 200, zMin, zMax, 401, residMin, residMax);
    double resid;

    for(int ev=0; ev<ntuple->GetEntries(); ev++){
        
        ntuple->GetEvent(ev);

        // Riempio histo1 e histo 2
        resid = zRec - zTrue;
        if (flag == 1) {
            histo1->Fill(mult, resid);
            histo2->Fill(zTrue, resid);
        }
        else {
            histo1->Fill(mult, residMin-1);
            histo2->Fill(zTrue, residMin-1);
        }

    }
    histo1->Write();
    histo2->Write();

    
    // RESIDUI
    // Per qualsiasi molteplicità
    TCanvas *c1 = new TCanvas("c1", "c1", 0, 1, 600, 400);
    c1->cd();

    TH1D *hRes = histo1->ProjectionY("hRes");
    hRes->SetTitle("Residuals");

    double resolution[2];
    fitDoubleGauss(resolution, dg, hRes);
    
    hRes->Write();

    if (rms != nullptr){
        *rms = hRes->GetRMS() * 1000; //rms in micron
    }
    else{
        cout << " RMS: "<< hRes->GetRMS() * 1000 << " resol.: " << resolution[0]*1000 << " pm " << resolution[1]*1000 << " um" << endl;
    }
    
    double rec_ev = hRes->Integral();
    double tot_ev = hRes->GetEntries();
    
    if (effic != nullptr){     
        *effic = rec_ev / tot_ev;
    }
    else{
        cout << " Efficiency: "<< rec_ev / tot_ev << endl;
    }
    

    // Per intervalli di molteplicità
    double multis[] = {3, 4, 5, 7, 10, 13, 19, 28, 37, 46, 55, 64};
    double multRange[] = {0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 4.5, 4.5, 4.5, 4.5, 4.5, 4.5};

    int nM = std::size(multis);
    TH1D *hResMult[nM];
    double resolutionMult[nM];
    double errResolutionMult[nM];
    char name[80];
    char title[100];
    
    TF1* fG = new TF1("fG", "gaus(0)", -0.5, 0.5);

    for(int i=0; i<nM; i++) {
        sprintf(name, "hResMult%.0f", multis[i]);
        sprintf(title, "Residuals with multiplicity at %.1f +/- %.1f", multis[i], multRange[i]);
        hResMult[i] = histo1->ProjectionY(name, histo1->GetXaxis()->FindBin(multis[i]-multRange[i]+0.001), histo1->GetXaxis()->FindBin(multis[i]+multRange[i]-0.001));
        hResMult[i]->SetTitle(title);

        //fitDoubleGauss(res, dg, hResMult[i]);
        hResMult[i]->Fit(fG, "RQ+");
        
        resolutionMult[i] = fG -> GetParameter(2) * 1000; //in micron
        errResolutionMult[i] = fG -> GetParError(2) * 1000;
        
        hResMult[i]->Write();

        if (rms == nullptr){
            cout << "Mult.: " << multis[i] << " RMS: "<< hResMult[i]->GetRMS()*1000 << " resol.: " << resolutionMult[i] << " pm " << errResolutionMult[i] << " um"<<  endl;
        }
    }

    // Efficienza vs molteplicità
    double effMult[nM];
    double erreffMult[nM];
    double num;
    double den;
    for(int i=0; i<nM; i++) {
        num = hResMult[i]->Integral();
        den = hResMult[i]->GetEntries();
        if(den != 0) {
            effMult[i] = num/den;
            erreffMult[i] = sqrt(effMult[i]*(1-effMult[i])/(hResMult[i]->GetEntries()));
        }
        else {
            effMult[i] = 0;
            erreffMult[i] = 1. / (hResMult[i]->GetEntries());
        }

        delete hResMult[i];
        hResMult[i] = nullptr;
    }

    //TGraph *gEffMult = new TGraph(nM, multis, effMult);
    TGraphErrors *gEffMult = new TGraphErrors(nM, multis, effMult, multRange, erreffMult);
    gEffMult->SetTitle("Efficiency vs Multiplicity");
    gEffMult->GetXaxis()->SetTitle("Multiplicity");
    gEffMult->GetYaxis()->SetTitle("Efficiency");
    gEffMult->SetMarkerSize(0.8);
    gEffMult->SetMarkerStyle(21);
    gEffMult->SetMarkerColor(kRed);
    gEffMult->Draw("ALP");
    gEffMult->Write("EffvsMult");

    //TGraph *gResMult = new TGraph(nM, multis, resolutionMult);
    TGraphErrors* gResMult = new TGraphErrors(nM, multis, resolutionMult, multRange, errResolutionMult);
    gResMult->SetTitle("Resolution vs Multiplicity");
    gResMult->GetXaxis()->SetTitle("Multiplicity");
    gResMult->GetYaxis()->SetTitle("Resolution (#mu m)");
    gResMult->SetMarkerSize(0.8);
    gResMult->SetMarkerStyle(21);
    gResMult->SetMarkerColor(kRed);
    gResMult->Draw("ALP");
    gResMult->Write("ResvsMult");

    delete gEffMult;
    gEffMult = nullptr;
    delete gResMult;
    gResMult = nullptr;

    // Efficienza vs zTrue
    // double nZTrue[] = {-175, -125, -75, -37.5, -12.5, 12.5, 37.5, 75, 125, 175};
    // double zRange[] = {  25,   25,  25,  12.5,  12.5, 12.5, 12.5, 25,  25,  25};
     double nZTrue[] = {-150, -75, -37.5, -12.5, 12.5, 37.5, 75, 150};
    double zRange[] =  {  50,  25,  12.5,  12.5, 12.5, 12.5, 25,  50};
    int nZ = std::size(nZTrue);

    TH1D *hResZTrue[nZ];
    double resolutionZtrue[nZ];
    double errResolutionZtrue[nZ];
    double res[2];
    for(int i=0; i<nZ; i++) {
        sprintf(name, "hResZTrue%.0f", nZTrue[i]);
        sprintf(title, "Residuals with vertex at %.1f +/- %.1f", nZTrue[i], zRange[i]);
        hResZTrue[i] = histo2->ProjectionY(name, histo2->GetXaxis()->FindBin(nZTrue[i]-zRange[i]+0.001), histo2->GetXaxis()->FindBin(nZTrue[i]+zRange[i]-0.001));
        hResZTrue[i]->SetTitle(title);
        fitDoubleGauss(res, dg, hResZTrue[i]);
        hResZTrue[i]->Write();

        resolutionZtrue[i] = res[0]*1000;
        errResolutionZtrue[i] = res[1]*1000;
    }

    double effZTrue[nZ];
    double erreffZTrue[nZ];
    for(int i=0; i<nZ; i++) {
        num = hResZTrue[i]->Integral();
        den = hResZTrue[i]->GetEntries();
        if(den != 0) {
            effZTrue[i] = num/den;
            erreffZTrue[i] = sqrt(effZTrue[i]*(1-effZTrue[i])/(hResZTrue[i]->GetEntries()));

        }
        else {
            effZTrue[i] = 0;
            erreffZTrue[i] = 1. /(hResZTrue[i]->GetEntries());

        } 

        delete hResZTrue[i];
        hResZTrue[i] = nullptr;
    }

    //TGraph *gResZTrue = new TGraph(nZ, nZTrue, resolutionZtrue);
    TGraphErrors *gResZTrue = new TGraphErrors(nZ, nZTrue, resolutionZtrue, zRange, errResolutionZtrue);
    gResZTrue->SetTitle("Resolution vs Z_{vert}");
    gResZTrue->GetXaxis()->SetTitle("Z_{true}");
    gResZTrue->GetYaxis()->SetTitle("Resolution (#mu m)");
    gResZTrue->SetMarkerSize(0.8);
    gResZTrue->SetMarkerStyle(21);
    gResZTrue->SetMarkerColor(kRed);
    gResZTrue->Draw("ALP");
    gResZTrue->Write("ResvsZTrue");


    //TGraph *gEffZTrue = new TGraph(nZ, nZTrue, effZTrue);
    TGraphErrors *gEffZTrue = new TGraphErrors(nZ, nZTrue, effZTrue, zRange, erreffZTrue);
    gEffZTrue->SetTitle("Efficiency vs Z_{vert}");
    gEffZTrue->GetXaxis()->SetTitle("Z_{true}");
    gEffZTrue->GetYaxis()->SetTitle("Efficiency");
    gEffZTrue->SetMarkerSize(0.8);
    gEffZTrue->SetMarkerStyle(21);
    gEffZTrue->SetMarkerColor(kRed);
    gEffZTrue->Draw("ALP");
    gEffZTrue->Write("EffvsZTrue");

    c1->Close();

    delete c1;
    c1 = nullptr;
    delete gResZTrue;
    gResZTrue = nullptr;
    delete gEffZTrue;
    gEffZTrue = nullptr;
    if (dg) { delete dg; dg = nullptr; }
    if (fG) { delete fG; fG = nullptr; }
    if (hRes) { delete hRes; hRes = nullptr; }
    if (histo1) { delete histo1; histo1 = nullptr; }
    if (histo2) { delete histo2; histo2 = nullptr; }

    fileIn.Close();
    fileOut.Close();




}