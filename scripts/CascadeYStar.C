/*
TH1F hQ2("Q2","Q2",1000,0,2);
TH1D heE("eE","eE",1000,0,11);
TH1D heTh("eTh","eTh",1000,0,20);
TH1D hePhi("ePh","ePh",90,-180,180);
TH1F hW("W","W",1000,0,6);
TH1F ht("t","t",1000,0,10);
TH1F hgE("gE","gE",1000,0,20);
TH1F hM2K("M2K","; 2K Mass (GeV)",1000,0,3);
TH1F hMXi("MXi","; Xi Mass (GeV)",1000,0,3);
TH1D hPiP("piP","#pi P",100,0,5);
TH1D hPiTh("piTh","#pi #theta",180,0,90);
TH1D hLamP("lamP","#Lambda P",100,0,5);
TH1D hLamTh("lamTh","#Lambda #theta",180,0,90);
TH1D hK1P("K1P","K1 P",100,0,10);
TH1D hK1Th("K1Th","K1 #theta",180,0,90);
TH1D hK2P("K2P","K2 P",100,0,10);
TH1D hK2Th("K2Th","K2 #theta",180,0,90);
*/
void CascadeYStar(C12Config config) {
  
  // Set minimum electron angle to look at events with electron in FD. This can be changed to look at events with electron in FT.
  config.setMin_elAngle(5);
  // Print configuration.
  config.Print();
  // Get beam configuration.
  auto ebeamP=config._beamP;

  //Declare Lorentz Vectors of beam and target.
  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
  LorentzVector prtarget(0,0,0,escat::M_pr());

  //tslope for the Ystar K+ system 
  Float_t tslope = 2.; 
  Float_t flat = 0.; //relative amount of flat CM production angle

  auto tokens=config._misc.Tokenize("$");
  // Define the custom mass distribution for the Y* hyperon.
  TString massDistYStar = "TMath::BreitWigner(x, 3, 1.2)"; 
  mass_distribution(3124, new DistTF1{TF1("YStarDecay", massDistYStar, 2.8, (elbeam+prtarget).M())}); 
  // Custom mass distribution for Xi*
  TString massDistXiStar = "TMath::BreitWigner(x, 1.6, 1)";
  mass_distribution(9995, new DistTF1{TF1("XiStarDecay", massDistXiStar, 1.6, (elbeam+prtarget).M())});  


  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string
    cout<<sentry<<endl;
    if(sentry.Contains("MXi=")){ //look to see if misc overrides mass distribition
      massDistXiStar = sentry;
      massDistXiStar.ReplaceAll("MXi=","");
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
  
  

  // Xi* decaying into Lambda and K-
  auto XiExcited=particle(9995,model(new PhaseSpaceDecay{{},{-321, 3122}}));

  // Y* particle decaying into K+ and Xi*
  auto YStar = particle(3124, model(new PhaseSpaceDecay{{XiExcited}, {321}})); 

  // Define the full decay chain: el+beam -> e' K+ Y*-> e' K+ Xi* K+ -> e' K+ K- Lambda K+
  auto decayChain = model(new DecayModelst{{YStar}, {321}});  
  
  //TwoBody_stu{0.1, 0.9, 3 ,0,0} //0.1 strength  s distribution (flat angular dist.),  0.9 strength t distribution with slope b = 3
  cout<<"using t slope "<<tslope<<" with relative flat amount "<<flat<<endl;
  // I think DecayModelQ2W models electroproduction?
  auto photoprod=new DecayModelQ2W{0, decayChain, new TwoBody_stu{flat, 1, tslope , 0 , 0} };
  mesonex( ebeamP , photoprod );

  // generate electron scattering?
  auto production =  dynamic_cast<ElectronScattering*>(generator().Reaction());

  production->SetLimitTarRest_eThmin(config._min_limit_e_th);
  production->SetLimitTarRest_eThmax(config._max_limit_e_th);
  
  //target limits
  production->GiveZVertexDist
    ( new DistUniform(config._tarPos-config._tarLength/2,config._tarPos+config._tarLength/2) );
  
  c12process(config);
 
   // ---------------------------------------------------------------------------
  //initilase the generator, may take some time for making distribution tables 
  // ---------------------------------------------------------------------------
  /*
  initGenerator();
  generator().SetNEvents(10000);
  
  // ---------------------------------------------------------------------------
  // Get event particles for filling histograms
  // ---------------------------------------------------------------------------
  
  auto k1 = decayChain->Products()[0];
  auto YStarPart = decayChain->Products()[1];

  auto k2 = static_cast<DecayingParticle*>(YStar)->Model()->Products()[1];
  auto Cascade = static_cast<DecayingParticle*>(YStar)->Model()->Products()[0];

  auto km = static_cast<DecayingParticle*>(XiExcited)->Model()->Products()[0];
  auto Lambda = static_cast<DecayingParticle*>(XiExcited)->Model()->Products()[1];


  // Gets scattered electron
  auto electron = photoprod->GetScatteredElectron();

  
 // ---------------------------------------------------------------------------
  // Generate events
  // ---------------------------------------------------------------------------
  
  gBenchmark->Start("generator");//timer

  while(finishedGenerator()==false){
    nextEvent();
    countGenEvent();
    if(generator().GetNDone()%1000==0) std::cout<<"event number "<<generator().GetNDone()<<std::endl;
    
    auto photon = elbeam - electron->P4();
    double Q2 = -photon.M2();
    double W = (photon+prtarget).M();
    
    hQ2.Fill(Q2);
    hW.Fill(W);
    ht.Fill(YStarPart->P4().M());
    hgE.Fill(photon.E());
    

    heE.Fill(electron->P4().P());
    heTh.Fill(electron->P4().Theta()*TMath::RadToDeg());
    //hPiP.Fill(pionXi->P4().P());
    //hPiTh.Fill(pionXi->P4().Theta()*TMath::RadToDeg());
    //hLamP.Fill(lambdaXi->P4().P());
    //hLamTh.Fill(lambdaXi->P4().Theta()*TMath::RadToDeg());
    hK1P.Fill(k1->P4().P());
    hK1Th.Fill(k1->P4().Theta()*TMath::RadToDeg());
    hK2P.Fill(k2->P4().P());
    hK2Th.Fill(k2->P4().Theta()*TMath::RadToDeg());

    //hM2K.Fill((k1->P4()+k2->P4()).M());
    // Missing mass of Xi*.
    auto missmassXi = elbeam + prtarget - k1->P4() - k2->P4() - electron->P4();
    hMXi.Fill(missmassXi.M());
  }

  auto histfile=TFile::Open("LambdaKChannelYStar.root","recreate");
  hQ2.Write();
  hW.Write();
  ht.Write();
  hgE.Write();
  hPiP.Write();
  hPiTh.Write();
  heE.Write();
  heTh.Write();
  hLamP.Write();
  hLamTh.Write();
  hK1P.Write();
  hK1Th.Write();
  hK2P.Write();
  hK2Th.Write();
  hM2K.Write();
  hMXi.Write();
 delete histfile;
  */
}
