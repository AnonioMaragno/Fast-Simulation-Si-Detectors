#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TROOT.h"
using std::to_string;


const int nEvents = 100000;

void scanProcess(int n, double* var, double var2, double *eff, double *rms, const char* varname){
    for (int i=0; i<n; i++){
        eff[i] = 0;
        rms[i] = 0;
    }
  
    TString recString = "Reconstruction(";

    int barWidth = 70; // Larghezza della barra in caratteri
    std::cout << varname << " scan..." << std::endl;
  
    std::cout << "[";
    for (int j = 0; j < barWidth; ++j) { std::cout << " ";}
    std::cout << "] 0 %\r" << std::flush;

    for (int i=0; i<n; i++){
        if (var2 < 0.2){ //scan in wSize
          gROOT->ProcessLine((recString + TString::Format("%.2f", var[i]) + "," + TString::Format("%.3f", var2) + ");").Data());
        }
        else{
          gROOT->ProcessLine((recString + TString::Format("%.2f", var2) + "," + TString::Format("%.2f", var[i]) + ");").Data());
        }
        gROOT->ProcessLine(Form("Efficiency( (double*) %p, (double*) %p);", &eff[i], &rms[i]));


        // ------------- PROGRESS BAR --------------
        float progress = (float) (i+1.) / n;


        std::cout << "[";
        int pos = barWidth * progress;
        for (int j = 0; j < barWidth; ++j) {
            if (j < pos) std::cout << "=";
            else if (j == pos) std::cout << ">";
            else std::cout << " ";
        }
        // \r riporta il cursore all'inizio, std::flush forza la stampa immediata
        std::cout << "] " << int(progress * 100.0) << " %\r" << std::flush;

    } 

    const char* varname2; 
    const char* udm;
    const char* udm2;
    
    if (strcmp(varname, "wSize") == 0){
        varname2 = "dPhi";
        udm = "mm";
        udm2 = "rad";
    }
    else{
        varname2 = "wSize";
        udm = "rad";
        udm2 = "mm";
    }

    std::cout << std::endl; // Vai a capo alla fine
    std::cout << "Scan in " << varname << " completato." << std::endl;

    gROOT->ProcessLine(Form("plottaRisultati(%d, (double*) %p, (double*) %p, \"%s (%s)\", \"Efficiency\", \"Eff. vs %s (smear+MS) - %s a %.2f %s\", \"Effvs%s\", false)", n, var, eff, varname, udm, varname, varname2, var2, udm2, varname));
    gROOT->ProcessLine(Form("plottaRisultati(%d, (double*) %p, (double*) %p, \"%s (%s)\", \"Resolution (#mu m)\", \"Res. vs %s (smear+MS) - %s a %.2f %s\", \"Resvs%s\", false)", n, var, rms, varname, udm, varname, varname2, var2, udm2, varname));
    gROOT->ProcessLine(Form("plottaRisultati(%d, (double*) %p, (double*) %p, \"%s (%s)\", \"Efficiency\", \"Eff. vs %s (smear+MS) - %s a %.2f %s\", \"Effvs%s\", true)", n, var, eff, varname, udm, varname, varname2, var2, udm2, varname));
    gROOT->ProcessLine(Form("plottaRisultati(%d, (double*) %p, (double*) %p, \"%s (%s)\", \"Resolution (#mu m)\", \"Res. vs %s (smear+MS) - %s a %.2f %s\", \"Resvs%s\", true)", n, var, rms, varname, udm, varname, varname2, var2, udm2, varname));

}

void plottaRisultati(int n, double *x, double *y, const char *nameX, const char *nameY, const char *title, const char* imm, bool kLog = false)
{
    char titImm[100] = "./images/scans/";
    strcat(titImm, imm);
    if (kLog){
        strcat(titImm, "Log.jpg");
    }
    else{
        strcat(titImm, ".jpg");
    }
    

    TCanvas* c = new TCanvas(imm, imm, 0, 1, 700, 400);
    c->cd();
    if (kLog){
        c->SetLogx();
    }
    

    double errx[n];
    double erry[n];

    if (strcmp(nameY, "Efficiency") == 0){ //eff. case
        for (int i=0; i<n; i++){
            errx[i] = 0;
            erry[i] = sqrt(y[i]*(1-y[i])/nEvents);
        } 
    }
    else{        // resolution case
        for (int i=0; i<n; i++){
            errx[i] = 0;
            erry[i] = 0.2;
        }
    }

    TGraphErrors *g = new TGraphErrors(n, x, y, errx, erry);
    g->SetTitle(title);
    g->GetXaxis()->SetTitle(nameX);
    g->GetYaxis()->SetTitle(nameY);
    g->SetMarkerSize(0.8);
    g->SetMarkerStyle(21);
    if(strcmp(nameX, "dPhi (rad)") == 0){ //dPhi case
        g->SetMarkerColor(kRed);
    }
    else{
        g->SetMarkerColor(kBlue);
    }
    g->Draw("ALP");

    c->SaveAs(titImm);

    c->Close();
}
