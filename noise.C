#include "Riostream.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TLegend.h"

void noise(){

    // Per intervalli di molteplicità
    double multis[] = {3, 4, 5, 7, 10, 13, 19, 28, 37, 46, 55, 64};
    double multRange[] = {0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 4.5, 4.5, 4.5, 4.5, 4.5, 4.5};

    int nM = std::size(multis);
    double rM0[] = {259.288,233.579,208.845,182.809,152.714,133.865,112.333,90.8206,78.4421,70.113,64.1728,59.3566};
    double eRM0[] = {1.39752,1.08641,0.871056,0.410526,0.347971,0.345071,0.215487,0.23056,0.238928,0.24547,0.246892,0.253671};
    double eM0[] = {0.929956,0.96947,0.985304,0.995975,0.999203,0.999797,0.999894,0.999989,1,1,1,1};
    double eeM0[] = {0.00107884,0.00073981,0.000525353,0.000164558,7.96248e-05,4.66654e-05,2.49807e-05,1.0929e-05,0,0,0,0};

    double rM1[] =  {259.638,230.487,210.15,182.029,151.418,133.474,112.085,90.6381,78.6376,70.4497,64.728,59.2874};
    double eRM1[] = {1.42164,1.05931,0.883828,0.404919,0.344031,0.340012,0.21416,0.228212,0.238804,0.244641,0.248896,0.255729};
    double eM1[] = {0.926655,0.966904,0.985681,0.995551,0.999337,0.999819,0.999959,0.999978,0.999984,0.999979,1,1};
    double eeM1[] = {0.00110306,0.000767922,0.000516692,0.000173601,7.27789e-05,4.38286e-05,1.5547e-05,1.54728e-05,1.61761e-05,2.08474e-05,0,0};

    double  rM2[] =  {259.059,230.622,209.719,182.357,152.52,133.489,112.054,91.5224,78.1914,70.2538,64.0574,59.4778};
    double eRM2[] = {1.40625,1.05319,0.872213,0.407563,0.343856,0.343121,0.214809,0.231537,0.238658,0.242621,0.24514,0.259277};
    double  eM2[] = {0.924077,0.964269,0.984735,0.995326,0.999281,0.999734,0.999953,0.999989,1,1,1,1};
    double eeM2[] = {0.00112071,0.00079682,0.000533227,0.000177907,7.57837e-05,5.31477e-05,1.66204e-05,1.0941e-05,0,0,0,0};

    double  rM3[] = {258.906,229.779,210.191,180.856,152.922,134.159,112.555,90.8087,78.8293,70.4844,64.1725,58.8866};
    double eRM3[] = {1.40042,1.05343,0.881497,0.404342,0.347912,0.348509,0.214789,0.232248,0.241401,0.242104,0.250546,0.249857};
    double  eM3[] = {0.918992,0.960362,0.983146,0.994789,0.999201,0.999713,0.999959,0.999967,1,1,1,1};
    double eeM3[] = {0.00115444,0.000837548,0.000559839,0.000187807,7.98798e-05,5.52321e-05,1.5547e-05,1.89501e-05,0,0,0,0};

    double  rM4[] = {258.903,231.677,208.833,181.413,153.187,133.589,112.477,90.6745,78.5942,70.7562,64.4053,59.6157};
    double eRM4[] = {1.4086,1.07299,0.866909,0.405595,0.347547,0.346449,0.216241,0.233578,0.238623,0.243239,0.24881,0.26057};
    double  eM4[] = {0.910722,0.958445,0.981425,0.994387,0.999201,0.999766,0.999947,0.999967,1,1,1,1};
    double eeM4[] = {0.00120648,0.000856701,0.000587218,0.000194867,7.98798e-05,4.98577e-05,1.76285e-05,1.89501e-05,0,0,0,0};

    double  rM5[] = {257.874,230.312,210.593,181.005,152.113,133.626,112.426,91.2043,78.816,70.4284,64.3088,59.4158};
    double eRM5[] = {1.39923,1.0604,0.88588,0.406388,0.343306,0.340991,0.213846,0.230652,0.241619,0.242884,0.244834,0.25889};
    double  eM5[] = {0.904008,0.954244,0.98029,0.99385,0.999169,0.999745,0.999935,0.999967,1,1,1,1};
    double eeM5[] = {0.0012464,0.000896995,0.000604542,0.000203928,8.14605e-05,5.20741e-05,1.9489e-05,1.89501e-05,0,0,0,0};

    double  rM6[] = {254.897,228.929,209.646,181.936,152.303,133.694,111.88,91.0875,78.4793,70.3712,64.4038,59.4857};
    double eRM6[] = {1.37715,1.05738,0.885378,0.408349,0.342873,0.343169,0.213094,0.231379,0.239459,0.246909,0.247472,0.258517};
    double  eM6[] = {0.897492,0.949803,0.976602,0.993183,0.999065,0.999755,0.999929,0.999956,1,1,1,1};
    double eeM6[] = {0.00128336,0.00093733,0.000657444,0.000214626,8.63974e-05,5.0978e-05,2.03555e-05,2.18816e-05,0,0,0,0};

    TCanvas* c1 = new TCanvas("c1", "c1", 0, 1, 700, 400);
    c1->cd();

    TGraphErrors *gEffMult6 = new TGraphErrors(nM, multis, eM6, multRange, eeM6);
    gEffMult6->SetTitle("Efficiency vs Multiplicity");
    gEffMult6->GetXaxis()->SetTitle("Multiplicity");
    gEffMult6->GetYaxis()->SetTitle("Efficiency");
    gEffMult6->SetMarkerSize(0.8);
    gEffMult6->SetMarkerStyle(21);
    gEffMult6->SetMarkerColor(kRed);
    gEffMult6->Draw("AP");

    TGraphErrors *gEffMult5 = new TGraphErrors(nM, multis, eM5, multRange, eeM5);
    gEffMult5->SetMarkerColor(kOrange);
    gEffMult5->SetMarkerSize(0.8);
    gEffMult5->SetMarkerStyle(21);
    gEffMult5->Draw("P same");

    TGraphErrors *gEffMult4 = new TGraphErrors(nM, multis, eM4, multRange, eeM4);
    gEffMult4->SetMarkerColor(kYellow-10);
    gEffMult4->SetMarkerSize(0.8);
    gEffMult4->SetMarkerStyle(21);
    gEffMult4->Draw("P same");

    TGraphErrors *gEffMult3 = new TGraphErrors(nM, multis, eM3, multRange, eeM3);
    gEffMult3->SetMarkerColor(kGreen);
    gEffMult3->SetMarkerSize(0.8);
    gEffMult3->SetMarkerStyle(21);
    gEffMult3->Draw("P same");

    TGraphErrors *gEffMult2 = new TGraphErrors(nM, multis, eM2, multRange, eeM2);
    gEffMult2->SetMarkerColor(kCyan);
    gEffMult2->SetMarkerSize(0.8);
    gEffMult2->SetMarkerStyle(21);
    gEffMult2->Draw("P same");

    TGraphErrors *gEffMult1 = new TGraphErrors(nM, multis, eM1, multRange, eeM1);
    gEffMult1->SetMarkerColor(kBlue);
    gEffMult1->SetMarkerSize(0.8);
    gEffMult1->SetMarkerStyle(21);
    gEffMult1->Draw("P same");

    TGraphErrors *gEffMult0 = new TGraphErrors(nM, multis, eM0, multRange, eeM0);
    gEffMult0->SetMarkerColor(kViolet);
    gEffMult0->SetMarkerSize(0.8);
    gEffMult0->SetMarkerStyle(21);
    gEffMult0->Draw("P same");

    TLegend *leg = new TLegend(0.7, 0.1, 0.9, 0.3);

    leg->AddEntry(gEffMult0, "Avg. noise = 0", "ep");
    leg->AddEntry(gEffMult1, "Avg. noise = 1", "ep");
    leg->AddEntry(gEffMult2, "Avg. noise = 2", "ep");
    leg->AddEntry(gEffMult3, "Avg. noise = 3", "ep");
    leg->AddEntry(gEffMult4, "Avg. noise = 4", "ep");
    leg->AddEntry(gEffMult5, "Avg. noise = 5", "ep");
    leg->AddEntry(gEffMult6, "Avg. noise = 6", "ep");

    leg->Draw("same");

    TCanvas* c2 = new TCanvas("c2", "c2", 0, 1, 700, 400);
    c2->cd();

    TGraphErrors *gResMult6 = new TGraphErrors(nM, multis, rM6, multRange, eRM6);
    gResMult6->SetTitle("Resolution vs Multiplicity");
    gResMult6->GetXaxis()->SetTitle("Multiplicity");
    gResMult6->GetYaxis()->SetTitle("Resolution (#mu m)");
    gResMult6->SetMarkerSize(0.8);
    gResMult6->SetMarkerStyle(21);
    gResMult6->SetMarkerColor(kRed);
    gResMult6->Draw("AP");

    TGraphErrors *gResMult5 = new TGraphErrors(nM, multis, rM5, multRange, eRM5);
    gResMult5->SetMarkerColor(kOrange);
    gResMult5->SetMarkerSize(0.8);
    gResMult5->SetMarkerStyle(21);
    gResMult5->Draw("P same");

    TGraphErrors *gResMult4 = new TGraphErrors(nM, multis, rM4, multRange, eRM4);
    gResMult4->SetMarkerColor(kYellow-10);
    gResMult4->SetMarkerSize(0.8);
    gResMult4->SetMarkerStyle(21);
    gResMult4->Draw("P same");

    TGraphErrors *gResMult3 = new TGraphErrors(nM, multis, rM3, multRange, eRM3);
    gResMult3->SetMarkerColor(kGreen);
    gResMult3->SetMarkerSize(0.8);
    gResMult3->SetMarkerStyle(21);
    gResMult3->Draw("P same");

    TGraphErrors *gResMult2 = new TGraphErrors(nM, multis, rM2, multRange, eRM2);
    gResMult2->SetMarkerColor(kCyan);
    gResMult2->SetMarkerSize(0.8);
    gResMult2->SetMarkerStyle(21);
    gResMult2->Draw("P same");

    TGraphErrors *gResMult1 = new TGraphErrors(nM, multis, rM1, multRange, eRM1);
    gResMult1->SetMarkerColor(kBlue);
    gResMult1->SetMarkerSize(0.8);
    gResMult1->SetMarkerStyle(21);
    gResMult1->Draw("P same");

    TGraphErrors *gResMult0 = new TGraphErrors(nM, multis, rM0, multRange, eRM0);
    gResMult0->SetMarkerColor(kViolet);
    gResMult0->SetMarkerSize(0.8);
    gResMult0->SetMarkerStyle(21);
    gResMult0->Draw("P same");

    TLegend *leg2 = new TLegend(0.7, 0.7, 0.9, 0.9);

    leg2->AddEntry(gResMult0, "Avg. noise = 0", "ep");
    leg2->AddEntry(gResMult1, "Avg. noise = 1", "ep");
    leg2->AddEntry(gResMult2, "Avg. noise = 2", "ep");
    leg2->AddEntry(gResMult3, "Avg. noise = 3", "ep");
    leg2->AddEntry(gResMult4, "Avg. noise = 4", "ep");
    leg2->AddEntry(gResMult5, "Avg. noise = 5", "ep");
    leg2->AddEntry(gResMult6, "Avg. noise = 6", "ep");

    leg2->Draw("same");

    double noises[] = {0, 1, 2, 3, 4, 5, 6};
    double  eff[] = {0.990122, 0.989698, 0.989212, 0.988478, 0.987576, 0.986731, 0.985618};
    double  res[] = {130.357,   129.945,  129.951,  129.908,  129.678,  129.663,  129.512};
    double eres[] = {0.502431,  0.50511, 0.499296, 0.508485, 0.498558, 0.507926, 0.505904};

    double enoise[7];
    double eeff[7];
    for (int i=0; i<7; i++){
        enoise[i] = 0.;
        eeff[i] = sqrt(eff[i]*(1-eff[i])/1000000.);
    }

    TCanvas* c3 = new TCanvas("c3", "c3", 0, 1, 700, 400);
    c3->cd();

    TGraphErrors *gResNoise = new TGraphErrors(7, noises, res, enoise, eres);
    gResNoise->SetTitle("Resolution vs Noise");
    gResNoise->GetXaxis()->SetTitle("Avg. noise");
    gResNoise->GetYaxis()->SetTitle("Resolution (#mu m)");
    gResNoise->SetMarkerSize(0.8);
    gResNoise->SetMarkerStyle(21);
    gResNoise->SetMarkerColor(kRed);
    gResNoise->Draw("ALP");

    TCanvas* c4 = new TCanvas("c4", "c4", 0, 1, 700, 400);
    c4->cd();

    TGraphErrors *gEffNoise = new TGraphErrors(7, noises, eff, enoise, eeff);
    gEffNoise->SetTitle("Efficiency vs Noise");
    gEffNoise->GetXaxis()->SetTitle("Avg. noise");
    gEffNoise->GetYaxis()->SetTitle("Efficiency");
    gEffNoise->SetMarkerSize(0.8);
    gEffNoise->SetMarkerStyle(21);
    gEffNoise->SetMarkerColor(kRed);
    gEffNoise->Draw("ALP");
}