
double BW(double x, double M, double W);
double BW_boson(double x, double M, double W);
double Pcm(double M, double m1, double m2);

void test_BW(){

  double MH = 100.;
  double MW = 80.;
  double WW = 2.5;

  int N = 100.;
  double xmin = 0.;
  double xmax = 90.;

  TH1D *hist[2];
  hist[0] = new TH1D("hist0","hist0",N,xmin,xmax);
  hist[1] = new TH1D("hist1","hist1",N,xmin,xmax);
  
  TH2D *hist2D[2];
  hist2D[0] = new TH2D("hist2D0","hist2D0",N,xmin,xmax,N,xmin,xmax);
  hist2D[1] = new TH2D("hist2D1","hist2D1",N,xmin,xmax,N,xmin,xmax);

  for(int i = 0; i < N; i++){
    double x = hist[0]->GetBinCenter(i+1);
    //hist[0]->SetBinContent(i+1,BW(x,MW,WW));
    //hist[1]->SetBinContent(i+1,BW_boson(x,MW,WW));
    for(int j = 0; j < N; j++){
      double y = hist[0]->GetBinCenter(j+1);
      hist2D[0]->SetBinContent(i+1,j+1, Pcm(MH,x,y)*BW(x,MW,WW)*BW(y,MW,WW) );
      hist2D[1]->SetBinContent(i+1,j+1, Pcm(MH,x,y)*BW_boson(x,MW,WW)*BW_boson(y,MW,WW) );
      hist[0]->Fill(x,Pcm(MH,x,y)*BW(x,MW,WW)*BW(y,MW,WW));
      hist[1]->Fill(x,Pcm(MH,x,y)*BW_boson(x,MW,WW)*BW_boson(y,MW,WW));
    }
  }
  hist[0]->Scale(1./hist[0]->Integral());
  hist[1]->Scale(1./hist[1]->Integral());
  hist2D[0]->Scale(1./hist2D[0]->Integral());
  hist2D[1]->Scale(1./hist2D[1]->Integral());

  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  c1->Draw();
  hist[0]->Draw();
  hist[1]->SetLineColor(kRed);
  hist[1]->Draw("SAME");

  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  c2->Draw();
  hist2D[0]->Draw("COLZ");

  TCanvas *c3 = new TCanvas("c3","c3",600,600);
  c3->Draw();
  hist2D[1]->Draw("COLZ");

}

double BW(double x, double M, double W){
  return 1./( (x*x-M*M)*(x*x-M*M) + M*M*W*W );
}
double BW_boson(double x, double M, double W){
  return x*x/( (x*x-M*M)*(x*x-M*M) + x*x*x*x*W*W/M/M );
}
double Pcm(double M, double m1, double m2){
  if(M-m1-m2 <= 0.0) return 0.;
  return m1*m1*m2*m2*sqrt( (M*M-m1*m1-m2*m2)*(M*M-m1*m1-m2*m2) - 4.*m1*m1*m2*m2 )/2./M;
}
