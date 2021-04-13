
void CLAS12_pEtaPrime_RhoGamma_Flat_s(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
  LorentzVector prtarget(0,0,0,escat::M_pr());

   
  //produced rho decaying to pi+ pi-
  mass_distribution(113,new DistTF1{TF1("hh","TMath::BreitWigner(x,0.775,0.149)",0.27,3)});
  auto rho=particle(113,model(new PhaseSpaceDecay{{},{211,-211}}));
  
  //produced eta' decaying to rho gamma(22)
  auto eta_pr=particle(331,model(new PhaseSpaceDecay{{rho},{22}}));
  
  //decay of gamma* + p  to p + eta
  auto pGammaStarDecay = model(new DecayModelst{ {eta_pr},{2212} });
  

  mesonex( ebeamP ,  new DecayModelQ2W{0, pGammaStarDecay });


  c12process(config);
  
}
