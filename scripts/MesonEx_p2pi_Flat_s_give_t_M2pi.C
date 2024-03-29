//clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 --misc '--tslope=4 --flat=1' MesonEx_p2pi_Flat_s_give_t_M2pi.C
//--tslope => give t distribution slope
//--flat => give relative amount of flat production angle compared to t distribution
//--M2pi=0.9*TMath::BreitWigner(x,0.78,0.149) => alternative M2pi disribution


void MesonEx_p2pi_Flat_s_give_t_M2pi(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();

  TString massDist = "0.9*TMath::BreitWigner(x,0.78,0.149) + 0.1*TMath::BreitWigner(x,1.27,0.187)+0.1";

  Float_t tslope=5.; //tslope
  Float_t flat = 0.; //relative amount of flat CM production angle
  //Do some misc string decoding
  //example string $M2pi=0.9*TMath::BreitWigner(x,0.78,0.149) $tslope=4 $flat=1
  auto tokens=config._misc.Tokenize("$");
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
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
  mass_distribution(9995,new DistTF1{TF1("hh",massDist,0.,(*elin+*prin).M())});
  
  //produced meson decaying to pi+ pi- with mass distribution 9995
  auto X=particle(9995,model(new PhaseSpaceDecay{{},{211,-211}}));
  
  //decay of gamma* + p  to p + X
  //depends on s and t
  auto pGammaStarDecay = model(new DecayModelst{ {X},{2212} });
  
  // 
  //create mesonex electroproduction of X + proton
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  cout<<"using t slope "<<tslope<<" with relative flat amount "<<flat<<endl;

  mesonex( elBeam,prTarget ,
	   new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat,1,tslope,0,0}});

  c12process(config);
  
}
