void plot() {
    auto tf = new TFile("DcaDist.root");

    auto c = new TCanvas();

    gStyle->SetOptStat(0);

    TH1F *h1z, *h1xy;
    TH2F *h2z, *h2xy;
    TProfile *tpz, *tpxy;

    auto leg = new TLegend(0.6, 0.1, 0.85, 0.33);

    TF1* z_upper = new TF1("z_upper", "[0]+[1]/pow(x, [2])", 1, 600);
    TF1* xy_upper = new TF1("xy_upper", "[0]+[1]/pow(x, [2])", 1, 600);
    TF1* z_lower = new TF1("z_lower", "[0]+[1]/pow(x, [2])", 1, 600);
    TF1* xy_lower = new TF1("xy_lower", "[0]+[1]/pow(x, [2])", 1, 600);

    double init_par_upper[3] = {1.0, 1.0, 2.0};
    double init_par_lower[3] = {1.0, -1.0, 2.0};

    z_upper->SetParameters(init_par_upper);
    xy_upper->SetParameters(init_par_upper);
    z_lower->SetParameters(init_par_lower);
    xy_lower->SetParameters(init_par_lower);

    tf->GetObject("hDCAz", h1z);
    tf->GetObject("hDCAxy", h1xy);
    tf->GetObject("h2DCAzRef", h2z);
    tf->GetObject("h2DCAxyRef", h2xy);
    tf->GetObject("tpDCAzRef", tpz);
    tf->GetObject("tpDCAxyRef", tpxy);

    TLatex* lat = new TLatex();
    lat->SetTextFont(64);
    lat->SetTextSize(16);
    lat->SetTextColor(2);
    lat->SetTextAlign(22);

    const char* sname = "dca_cuts.pdf";
    double nsig = 5;


    c->Clear();
    c->cd();
    lat->DrawLatexNDC(0.5, 0.5, "14.6 GeV mean DCAz/xy cut");
    c->Print(Form("%s(", sname));

    c->Clear();
    c->Divide(2, 1);
    c->cd(1);
    gPad->SetLogy();
    h1z->Draw("hist");
    c->cd(2);
    gPad->SetLogy();
    h1xy->Draw("hist");
    c->Print(sname);

    // do fit
    const int np = 360; // depends on the refmult
    // make sure that np is less than number of X bins (refmult)
    double mean[np];
    double sig[np];
    double upper[np];
    double lower[np];
    double x[np];

    ofstream fout;
    fout.open("pars.txt");

    c->Clear();
    c->cd();
    gPad->SetLogy(false);
    gPad->SetLogz();
    h2z->GetYaxis()->SetRangeUser(-3, 3);
    h2z->Draw("col");
    tpz->BuildOptions(0, 0, "s");
    tpz->Draw("epsame");
    lat->SetTextAlign(11);
    lat->DrawLatexNDC(0.7, 0.8, "STAR BES-II");
    lat->DrawLatexNDC(0.7, 0.7, "Au+Au @ 14.6 GeV");
    for (int i=0; i<np; i++) {
        mean[i] = tpz->GetBinContent(i+1);
        sig[i] = tpz->GetBinError(i+1);
        upper[i] = mean[i] + sig[i]*nsig;
        lower[i] = mean[i] - sig[i]*nsig;
        x[i] = i+1;
    }
    TGraph* tgu = new TGraph(np, x, upper);
    TGraph* tgl = new TGraph(np, x, lower);

    tgu->SetMarkerStyle(29);
    tgu->SetMarkerColor(1);
    tgl->SetMarkerStyle(29);
    tgl->SetMarkerColor(1);
    tgu->Fit(z_upper, "RQ0");
    tgl->Fit(z_lower, "RQ0");

    tgu->Draw("psame");
    tgl->Draw("psame");
    z_upper->Draw("lsame");
    z_lower->Draw("lsame");

    leg->AddEntry(z_upper, Form("#mu#pm%.0f#sigma cut", nsig), "l");
    leg->AddEntry(tpz, "#mu#pm#sigma", "ep");

    leg->Draw("same");

    std::cout << "[LOG] parameters: \n DCAz upper: {" << z_upper->GetParameter(0) << ", " << z_upper->GetParameter(1) << ", " << z_upper->GetParameter(2) << "}" << std::endl;
    std::cout << "[LOG] parameters: \n DCAz lower: {" << z_lower->GetParameter(0) << ", " << z_lower->GetParameter(1) << ", " << z_lower->GetParameter(2) << "}" << std::endl;

    fout << "DCAz pars:\n";
    fout << "{" << z_upper->GetParameter(0) << ", " << z_upper->GetParameter(1) << ", " << z_upper->GetParameter(2) << "}" << std::endl;
    fout << "{" << z_lower->GetParameter(0) << ", " << z_lower->GetParameter(1) << ", " << z_lower->GetParameter(2) << "}" << std::endl;

    c->Print(sname);

    c->Clear();
    c->cd();
    gPad->SetLogy(false);
    gPad->SetLogz();
    h2xy->GetYaxis()->SetRangeUser(-3, 3);
    h2xy->Draw("col");
    tpxy->BuildOptions(0, 0, "s");
    tpxy->Draw("epsame");
    lat->SetTextAlign(11);
    lat->DrawLatexNDC(0.7, 0.8, "STAR BES-II");
    lat->DrawLatexNDC(0.7, 0.7, "Au+Au @ 14.6 GeV");
    for (int i=0; i<np; i++) {
        mean[i] = tpxy->GetBinContent(i+1);
        sig[i] = tpxy->GetBinError(i+1);
        upper[i] = mean[i] + sig[i]*nsig;
        lower[i] = mean[i] - sig[i]*nsig;
        x[i] = i+1;
    }
    TGraph* tgu2 = new TGraph(np, x, upper);
    TGraph* tgl2 = new TGraph(np, x, lower);

    tgu2->SetMarkerStyle(29);
    tgu2->SetMarkerColor(1);
    tgl2->SetMarkerStyle(29);
    tgl2->SetMarkerColor(1);
    
    tgu2->Fit(xy_upper, "RQ0");
    tgl2->Fit(xy_lower, "RQ0");

    tgu2->Draw("psame");
    tgl2->Draw("psame");

    xy_upper->Draw("lsame");
    xy_lower->Draw("lsame");

    leg->Draw("same");

    std::cout << "[LOG] parameters: \n DCAxy upper: {" << xy_upper->GetParameter(0) << ", " << xy_upper->GetParameter(1) << ", " << xy_upper->GetParameter(2) << "}" << std::endl;
    std::cout << "[LOG] parameters: \n DCAxy lower: {" << xy_lower->GetParameter(0) << ", " << xy_lower->GetParameter(1) << ", " << xy_lower->GetParameter(2) << "}" << std::endl;

    fout << "DCAxy pars:\n";
    fout << "{" << xy_upper->GetParameter(0) << ", " << xy_upper->GetParameter(1) << ", " << xy_upper->GetParameter(2) << "}" << std::endl;
    fout << "{" << xy_lower->GetParameter(0) << ", " << xy_lower->GetParameter(1) << ", " << xy_lower->GetParameter(2) << "}" << std::endl;

    c->Print(sname);

    c->Clear();
    c->Print(Form("%s)", sname));

    fout.close();

}
