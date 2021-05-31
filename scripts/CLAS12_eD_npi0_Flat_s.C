
void CLAS12_eD_npi0_Flat_s(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  
  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto qfTarget = initial(2112,0,1000010020,model(new NuclearBreakup(2212,2112)),new QuasiFreeNucleon(QuasiFree::CDBonnMomentum()));//CDBonn potential
  auto neutin=qfTarget->GetInteracting4Vector();

   
  //produced pi0 decaying to gammagamma
  auto pi0=particle(111,model(new PhaseSpaceDecay{{},{22,22}}));
    
  //decay of gamma* + p  to n + pi0
  auto pGammaStarDecay = model(new DecayModelst{ {pi0},{2112} });
  

  mesonex( elBeam,qfTarget ,  new DecayModelQ2W{0, pGammaStarDecay });


  c12process(config);
  
}
