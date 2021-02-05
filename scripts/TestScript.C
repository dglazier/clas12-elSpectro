void TestScript(C12Config config){

  config.Print();
  
  auto ebeamE=config._beamP;
  
  elSpectro::LorentzVector elbeam(0,0,ebeamE,elSpectro::escat::E_el(ebeamE));
  elSpectro::LorentzVector prbeam(0,0,0,elSpectro::escat::M_pr());
  
  //flat pure phase sapce distribution =1 for particle id 9995
  //mass_distribution(9995,new DistTF1{TF1("hh","1",0.,(elbeam+prbeam).M())});
  //add a Breit-Wigner resonance for particle id 9995
  mass_distribution(9995,new DistTF1{TF1("hh","0.9*TMath::BreitWigner(x,0.78,0.149) + 0.1*TMath::BreitWigner(x,1.27,0.187)+0.1",0.,(elbeam+prbeam).M())});
  
  //produced meson decaying to pi+ pi- with mass distribution 9995
  auto X=static_cast<DecayingParticle*>(particle(9995,model(new PhaseSpaceDecay{{},{211,-211}})) );
  
  //decay of gamma* + p  to p + X
  //depends on s and t
  auto pGammaStarDecay = static_cast<DecayModelst*>(model(new DecayModelst{ {X},{2212} }));
  
  // 
  //create mesonex electroproduction of X + proton
  auto production=mesonex( ebeamE ,  new DecayModelQ2W{0, pGammaStarDecay });
  production->GiveZVertexDist( new DistUniform(-3-2.5,-3+2.5) );


  c12process(config);


}
