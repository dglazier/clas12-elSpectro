//clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 --misc '--tslope=4 --flat=1' VeryStrange_KKXi_Flat_s_give_t_MXi.C

void VeryStrange_KKXi_Flat_s_give_t_MXi(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;
  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();

  //default mass distribution unless changed on caommand line
  TString massDist = "TMath::BreitWigner(x,1.322,0.001)+0.5*TMath::BreitWigner(x,1.530,0.01)";

  Float_t tslope=2.; //tslope for 2K system
  Float_t flat = 0.; //relative amount of flat CM production angle
  
  //Do some misc string decoding
  //example string $MXi=TMath::BreitWigner(x,1.322,0.001) $tslope=2 $flat=0.1
  auto tokens=config._misc.Tokenize("$");
 
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
    cout<<sentry<<endl;
    if(sentry.Contains("MXi=")){ //look to see if misc overrides mass distribition
      massDist = sentry;
      massDist.ReplaceAll("MXi=","");
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
  
  
  //produced 2K system decaying to 2K+  with phase space mass distribution
  mass_distribution(9995,new DistTF1{TF1("hh","1",1.,(*elin+*prin).M())});
  auto X=particle(9995,model(new PhaseSpaceDecay{{},{321,321}}));
 

  //add a Breit-Wigner resonance for particle id 9996
  mass_distribution(9996,new DistTF1{TF1("hh",massDist,1.2,(*elin+*prin).M())});
  //Xi- decay to Lambda + pi-
  auto Xi=particle(9996,model(new PhaseSpaceDecay{{},{3122,-211}}));
 
  //decay of gamma* + p  to Xi + X
  //depends on s and t
  //auto pGammaStarDecay = model(new  PhaseSpaceDecay{{X,Xi},{} });
  auto pGammaStarDecay = model(new DecayModelst{ {X,Xi},{} });
  
  // 
  //create mesonex electroproduction of X + proton
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  cout<<"using t slope "<<tslope<<" with relative flat amount "<<flat<<endl;
  mesonex( elBeam,prTarget ,  new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat, 1, tslope , 0 , 0} });


  c12process(config);
  
}
