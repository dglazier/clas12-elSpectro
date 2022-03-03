//clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 --misc '--tslope=4 --flat=1' MesonEx_nKPiK_Flat_s_give_t_MKPiK.C

//--tslope => give t distribution slope
//--flat => give relative amount of flat production angle compared to t distribution
//--MKmPi=TMath::BreitWigner(x,0.78,0.149) => alternative MKmPi disribution
//--MKPiK=TMath::BreitWigner(x,1.5,0.3) => alternative MKPiK disribution

void MesonEx_nKPiK_Flat_s_give_t_MKPiK(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();

  TString massDistY = "1";
  TString massDistX = "1";

  Float_t tslope=4.; //tslope
  Float_t flat = 0.; //relative amount of flat CM production angle
  //Do some misc string decoding
  //example string $M2pi=0.9*TMath::BreitWigner(x,0.78,0.149) $tslope=4 $flat=1
  auto tokens=config._misc.Tokenize("$");
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
     if(sentry.Contains("MKmPi=")){ //look to see if misc overrides mass distribition Y
      massDistY = sentry;
      massDistY.ReplaceAll("MKmPi=","");
    }
    if(sentry.Contains("MKPiK=")){ //look to see if misc overrides mass distribition X
      massDistX = sentry;
      massDistX.ReplaceAll("MKPiK=","");
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
  mass_distribution(9996,new DistTF1{TF1("hh",massDistY,0.,(*elin+*prin).M())});

  auto Y=static_cast<DecayingParticle*>(particle(9996,model(new PhaseSpaceDecay{{},{211,-321}})) );

  cout<<"Mass distribution is "<<massDistX<<endl;
  mass_distribution(9995,new DistTF1{TF1("hh",massDistX,0.,(*elin+*prin).M())});
  
  //produced meson decaying to pi+ K+ K- with mass distribution 9995
  auto X=static_cast<DecayingParticle*>(particle(9995,model(new PhaseSpaceDecay{{Y},{321}})) );
  
  
  //decay of gamma* + p  to n + X
  //depends on s and t
  auto pGammaStarDecay = model(new DecayModelst{ {X},{2112} });
  
  // 
  //create mesonex electroproduction of X + neutron
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  cout<<"using t slope "<<tslope<<" with relative flat amount "<<flat<<endl;

  mesonex( elBeam,prTarget ,  new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat, 1, tslope , 0 , 0} });


  c12process(config);
  
}
