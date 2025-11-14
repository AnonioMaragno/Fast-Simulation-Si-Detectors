#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
using std::to_string;

void plottaRisultati(int n, double* x, double* y, const char* nameX, const char* nameY, const char* title, const char* imm);

const int nEvents = 100000;

void scans(){
  
    int nWstep = 17;
    int nPhistep = 15;

    double wSize[] = {0.06, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1., 1.5, 2., 3., 5., 10.};
    double phis[] = {0.02, 0.04, 0.06, 0.08, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.7, 1., 1.5, 2., 3.};

    // wSize scan - senza MS senza noise senza smearing (dPhi a 0.1 rad)
    // double res1[] = {49, 49.4, 50.6, 51.3, 51.5, 51.3, 51.1, 50.7, 50.1, 49.9, 49.6, 49.2, 49.0, 49.2, 49.7, 50.7, 53.6};
    // double eff1[] = {81815, 90091, 96137, 98027, 98685, 98959, 99122, 99154, 99161, 99165, 99166, 99166, 99166, 99166, 99166, 99166, 99166};

    // // wSize scan - MS senza noise senza smearing (0.1 rad)
    // double res1[] = {67.0, 65.2, 64.1, 63.9, 64.1, 64.6, 66.2, 67.5, 68.8, 70., 71.1, 72.5, 74.3, 74.8, 75.8, 77.4, 80.6};
    // double eff1[] = {71710, 83382, 93179, 96542, 97812, 98402, 98881, 99011, 99083, 99120, 99142, 99151, 99158, 99158, 99157, 99156, 99156};

    // wSize scan -  MS+smear senza noise (0.05 rad)
    double res1[] = {188.2, 186., 183.4, 181.4, 179.5, 177.7, 173.4, 168.9, 162.6, 156.9, 151.3, 143.7, 135, 134.1, 134.3, 134.9, 136.8};
    double eff1[] = {47879, 56506, 69620, 78576, 85136, 89795, 95428, 97394, 98522, 98931, 99078, 99152, 99177, 99177, 99177, 99176, 99176};

    for (int j = 0; j < nWstep; j++){
        eff1[j] = eff1[j]/nEvents;
    }

    plottaRisultati(nWstep, wSize, res1, "Wind.size (mm)", "Resolution (#mu m)", "Resolution vs wSize (con MS e Smearing, no Noise) - dPhi a 0.05 rad", "ResvsWin");
    plottaRisultati(nWstep, wSize, eff1, "Wind.size (mm)", "Efficiency", "Efficiency vs wSize (con MS e Smearing, no Noise) - dPhi a 0.05 rad", "EffvsWin");

    // dPhi scan - senza MS senza noise senza smearing (wSize a 0.3 mm)
    // double res2[] = {51.4, 51.4, 51.4, 51.3, 51.3, 51.3, 51.2, 51.1, 51.1, 51.0, 50.9, 50.7, 50.4, 50.2, 49.9};
    // double eff2[] = {99059, 99035, 99009, 98989, 98959, 98899, 98842, 98738, 98653, 98559, 98383, 98146, 97790, 97479, 97038};

    // // dPhi scan - MS senza noise senza smearing (0.5 mm)
    // double res2[] = {67.5, 67.4, 67.5, 67.5, 67.5, 67.4, 67.3, 67.2, 67.1, 67.0, 66.8, 66.5, 66.1, 65.9, 65.4};
    // double eff2[] = {99028, 99064, 99053, 99035, 99011, 98949, 98864, 98722, 98593, 98499, 98320, 98070, 97736, 97492, 97045};

    // // dPhi scan 2 - MS senza noise senza smearing (1.5 mm)
    // double res2[] = {73.1, 73.3, 73.6, 74, 74.3, 74.8, 75, 75.2, 75.4, 75.6, 75.9, 76.5, 77.3, 78.2, 79.6};
    // double eff2[] = {99133, 99178, 99181, 99174, 99158, 99111, 99051, 98934, 98829, 98754, 98596, 98402, 98134, 97926, 97534};

    // dPhi scan - MS+smear senza noise (1.5 mm)
    double res2[] = {135.5, 135.1, 135.1, 135.1, 135.2, 135.3, 135.4, 135.4, 135.4, 135.5, 135.6, 135.7, 136, 136.3, 136.7};
    double eff2[] = {99131, 99178, 99179, 99171, 99152, 99109, 99047, 98925, 98813, 98737, 98572, 98374, 98101, 97874, 97470};

    for (int j = 0; j < nPhistep; j++){
        eff2[j] = eff2[j]/nEvents;
    }

    plottaRisultati(nPhistep, phis, res2, "dPhi (rad)", "Resolution (#mu m)", "Resolution vs dPhi (con MS e Smearing, no Noise) - wSize a 1.5 mm", "ResvsPhi");
    plottaRisultati(nPhistep, phis, eff2, "dPhi (rad)", "Efficiency", "Efficiency vs dPhi (con MS e Smearing, no Noise) - wSize a 1.5 mm", "EffvsPhi");

}

void plottaRisultati(int n, double *x, double *y, const char *nameX, const char *nameY, const char *title, const char* imm)
{
    char titImm[100] = "./images/scans/";
    strcat(titImm, imm);
    strcat(titImm, ".jpg");

    TCanvas* c = new TCanvas(imm, imm);
    c->cd();
    //c->SetLogx();

    double errx[n];
    double erry[n];

    if (y[0] > 1){ //resolution case
        for (int i=0; i<n; i++){
            errx[i] = 0;
            erry[i] = 0.2;
        }
    }
    else{
        for (int i=0; i<n; i++){
            errx[i] = 0;
            erry[i] = sqrt(y[i]*(1-y[i])/nEvents);
        } 
    }

    TGraphErrors *g = new TGraphErrors(n, x, y, errx, erry);
    g->SetTitle(title);
    g->GetXaxis()->SetTitle(nameX);
    g->GetYaxis()->SetTitle(nameY);
    g->SetMarkerSize(0.8);
    g->SetMarkerStyle(21);
    if( n == 15){ //dPhi case
        g->SetMarkerColor(kRed);
    }
    else{
        g->SetMarkerColor(kBlue);
    }
    g->Draw("ALP");

    c->SaveAs(titImm);

    c->Close();
}
