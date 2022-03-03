//clas12-elSpectro --ft --ebeam 10.6 --seed 2132 --trig 10 CLAS12_pEtaPrime_RhoGamma_Flat_s.C
void CLAS12_pEtaPrime_RhoGamma_Flat_s(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();


  
  //produced rho decaying to pi+ pi-
  mass_distribution(113,new DistTF1{TF1("hh","TMath::BreitWigner(x,0.775,0.149)",0.27,3)});
  auto rho=particle(113,model(new PhaseSpaceDecay{{},{211,-211}}));
  
  //produced eta' decaying to rho gamma(22)
  auto eta_pr=particle(331,model(new PhaseSpaceDecay{{rho},{22}}));
  
  //decay of gamma* + p  to p + eta
  auto pGammaStarDecay = model(new DecayModelst{ {eta_pr},{2212} });
  

  mesonex( elBeam,prTarget ,  new DecayModelQ2W{0, pGammaStarDecay });


  c12process(config);
  
}
