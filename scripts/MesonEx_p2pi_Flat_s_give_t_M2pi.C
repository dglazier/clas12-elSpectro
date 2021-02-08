
void MesonEx_p2pi_Flat_s_give_t_M2pi(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
  LorentzVector prtarget(0,0,0,escat::M_pr());

  TString massDist = "0.9*TMath::BreitWigner(x,0.78,0.149) + 0.1*TMath::BreitWigner(x,1.27,0.187)+0.1";

  Float_t tslope=5.; //tslope
  Float_t flat = 0.; //relative amount of flat CM production angle
  //Do some misc string decoding
  //example string $M2pi=0.9*TMath::BreitWigner(x,0.78,0.149) $tslope=4 $flat=1
  auto tokens=config._misc.Tokenize("$");
  cout<<"tkens "<<config._misc<<" "<<tokens->GetEntries()<<endl;
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
    cout<<sentry<<endl;
    if(sentry.Contains("M2pi=")){ //look to see if misc overrides mass distribition
      massDist = sentry;
      massDist.ReplaceAll("M2pi=","");
    }
    if(sentry.Contains("tslope=")){ //slope
      TString tstring= sentry;
      tstring.ReplaceAll("tslope=","");
      tslope =  tstring.Atof();
    }
    if(sentry.Contains("flat=")){//relative amount of flat to t
      TString fstring= sentry;
      fstring.ReplaceAll("flat=","");
      flat =  fstring.Atof();
    }
  }
  
  //add a Breit-Wigner resonance for particle id 9995
  cout<<"Mass distribution is "<<massDist<<endl;
  mass_distribution(9995,new DistTF1{TF1("hh",massDist,0.,(elbeam+prtarget).M())});
  
  //produced meson decaying to pi+ pi- with mass distribution 9995
  auto X=particle(9995,model(new PhaseSpaceDecay{{},{211,-211}}));
  
  //decay of gamma* + p  to p + X
  //depends on s and t
  auto pGammaStarDecay = model(new DecayModelst{ {X},{2212} });
  
  // 
  //create mesonex electroproduction of X + proton
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  cout<<"using t slope "<<tslope<<" with relative flat amount "<<flat<<endl;

  mesonex( ebeamP ,  new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat, 1, tslope , 0 , 0} });


  c12process(config);
  
}
