
void CLAS12_eD_NJpsi_t_s(C12Config config) {

  config.Print();
  Float_t tslope=5.; //tslope
  Float_t flat = 0.; //relative amount of flat CM production angle
  Int_t nucleonPDG = 2112.; //default production on neutron
  Bool_t decayToMuons=kFALSE;
  
  //Do some misc string decoding
  //example string $nucleon=2212 $tslope=4 $flat=1 $muons
  auto tokens=config._misc.Tokenize("$");
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
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
    if(sentry.Contains("nucleon=")){//relative amount of flat to t
      TString fstring= sentry;
      fstring.ReplaceAll("nucleon=","");
      nucleonPDG =  fstring.Atoi();
    }
    if(sentry.Contains("muons")){//relative amount of flat to t
      decayToMuons=kTRUE;
    }
  }
  
  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();

  //construct coliding particle with species=nucleonPDG and momentum stationary
  //auto qfTarget = initial(nucleonPDG,0); //free nucleon target

  // construct coliding particle with species=nucleonPDG from a nuclear breakup
  // to proton and neutron
  // mometum = 0 
  // Nucleus = 1000010020 = deuteron
  // use default initial nucleon momentum distribution
  //auto qfTarget = initial(nucleonPDG,0,1000010020,model(new NuclearBreakup(2212,2112)),new QuasiFreeNucleon()); //approximate paramterisation
  //Use CDBonn potential to give nucleon momentum
  auto qfTarget = initial(nucleonPDG,0,1000010020,model(new NuclearBreakup(2212,2112)),new QuasiFreeNucleon(QuasiFree::CDBonnMomentum()));//CDBonn potential

  //get 4-momentum of target nucleon
  auto nucleon=qfTarget->GetInteracting4Vector();
  
  
  //produced jpsi decaying to e- e+
  Int_t leptonPDG=11;
  if(decayToMuons) leptonPDG=13;
  auto jpsi=particle(443,model(new PhaseSpaceDecay({},{leptonPDG,-leptonPDG})));
  
  //decay of gamma* + N  to N' + Jpsi
  //depends on s and t
  auto pGammaStarDecay = static_cast<DecayModelst*>(model(new DecayModelst{ {jpsi},{nucleonPDG} }));
  
  // 
  //create mesonex electroproduction of X + proton
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  //  mesonex( ebeamE ,  new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{0, 1, 5 , 0 , 0} });
  std::cout<<"using tslope "<<tslope<<" flat fraction "<<flat<<endl;
  auto production=mesonex( elBeam,qfTarget ,
	   new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat,1,tslope,0,0}});

  c12process(config);
  
}
