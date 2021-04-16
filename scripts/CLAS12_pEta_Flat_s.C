
void CLAS12_pEta_Flat_s(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
  LorentzVector prtarget(0,0,0,escat::M_pr());

   
  //produced rho decaying to pi+ pi-
  auto eta=particle(221,model(new PhaseSpaceDecay{{},{111,211,-211}}));
    
  //decay of gamma* + p  to p + eta
  auto pGammaStarDecay = model(new DecayModelst{ {eta},{2212} });
  

  mesonex( ebeamP ,  new DecayModelQ2W{0, pGammaStarDecay });


  c12process(config);
  
}
