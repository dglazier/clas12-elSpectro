
void MesonEx_p2pi_Flat_s_give_t_M2pi(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
  LorentzVector prtarget(0,0,0,escat::M_pr());

  TString massDist = "0.9*TMath::BreitWigner(x,0.78,0.149) + 0.1*TMath::BreitWigner(x,1.27,0.187)+0.1";
  if(config._misc.Contains("M2pi=")){ //look to see if misc overrides mass distribition
    massDist = config._misc;
    massDist.ReplaceAll("M2pi=","");
  }
  //add a Breit-Wigner resonance for particle id 9995
  cout<<"Mass distribution is "<<massDist<<endl;
  mass_distribution(9995,new DistTF1{TF1("hh",massDist,0.,(elbeam+prtarget).M())});
  
  //produced meson decaying to pi+ pi- with mass distribution 9995
  auto X=static_cast<DecayingParticle*>(particle(9995,model(new PhaseSpaceDecay{{},{211,-211}})) );
  
  //decay of gamma* + p  to p + X
  //depends on s and t
  auto pGammaStarDecay = static_cast<DecayModelst*>(model(new DecayModelst{ {X},{2212} }));
  
  // 
  //create mesonex electroproduction of X + proton
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  mesonex( ebeamP ,  new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{0, 1, 5 , 0 , 0} });


  c12process(config);
  
}
