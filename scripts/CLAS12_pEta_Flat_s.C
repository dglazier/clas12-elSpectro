
void CLAS12_pEta_Flat_s(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();

   
  //produced rho decaying to pi+ pi-
  auto eta=particle(221,model(new PhaseSpaceDecay{{},{111,211,-211}}));
    
  //decay of gamma* + p  to p + eta
  auto pGammaStarDecay = model(new DecayModelst{ {eta},{2212} });
  

  mesonex( elBeam,prTarget ,  new DecayModelQ2W{0, pGammaStarDecay });


  c12process(config);
  
}
