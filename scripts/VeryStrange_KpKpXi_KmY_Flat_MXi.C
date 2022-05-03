//clas12-elSpectro --ft --ebeam 10.6 --seed 2132 --trig 10 --misc '--MXi=TMath::BreitWigner(x,1.823,0.024) --Hyperon=3122' VeryStrange_KpKpXi_KmY_Flat_MXi.C
//clas12-elSpectro --min_e_th 8 --ebeam 10.6 --seed 2132 --trig 10 --misc '--MXi=TMath::BreitWigner(x,1.950,0.060) --Hyperon=3212' VeryStrange_KpKpXi_KmY_Flat_MXi.C

void VeryStrange_KpKpXi_KmY_Flat_MXi(C12Config config) {

  config.Print(); 

  auto ebeamP=config._beamP;
  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();

  //default mass distribution unless changed on caommand line
  TString massDist = "TMath::BreitWigner(x,1.823,0.024)";
  //defulat hyperon == Lambda
  Int_t hypID = 3122;

  Double_t fixMass=0.0;
  
  //Do some misc string decoding
  //example string MXi=TMath::BreitWigner(x,1.322,0.001)
  auto tokens=config._misc.Tokenize("$");
 
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
    cout<<sentry<<endl;
    if(sentry.Contains("MXi=")){ //look to see if misc overrides mass distribition
      massDist = sentry;
      if(massDist.Contains("MXi==")){
	massDist.ReplaceAll("MXi==","");
 	
	if(massDist.Atof()!=0)fixMass=massDist.Atof();
      }
      else{
	massDist.ReplaceAll("MXi=","");
      }
    }
     if(sentry.Contains("Hyperon=")){ //look to see if misc overrides mass distribition
      TString shypID = sentry;
      shypID.ReplaceAll("Hyperon=","");
      hypID=shypID.Atoi();
    }
    //no other options currently supported
  }
  
   //produced 2K system decaying to 2K+  with phase space mass distribution
  mass_distribution(9995,new DistTF1{TF1("hh","1",0.98,(*elin+*prin).M())});
  auto X=particle(9995,model(new PhaseSpaceDecay{{},{321,321}}));
 

  //add a Breit-Wigner resonance for particle id 9996
  if(fixMass==0.0)mass_distribution(9996,new DistTF1{TF1("hh",massDist,1.5,(*elin+*prin).M())});
  //Xi- decay to Lambda + K-
  auto Xi=particle(9996,model(new PhaseSpaceDecay{{},{hypID,-321}}));
  if(fixMass!=0.0)Xi->SetPdgMass(fixMass);
  
  //decay of gamma* + p  to Xi + 2K+
  //depends on s and t
  auto pGammaStarDecay = model(new  DecayModelst{{X,Xi},{} });
   
  // 
  //create mesonex electroproduction of X + proton
  mesonex( elBeam,prTarget ,  new DecayModelQ2W{0, pGammaStarDecay });


  c12process(config);
  
}
