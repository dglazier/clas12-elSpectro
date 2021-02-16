TH1F hQ2("Q2","Q2",1000,0,2);
TH1D heE("eE","eE",1000,0,11);
TH1D heTh("eTh","eTh",1000,0,20);
TH1D hePhi("ePh","ePh",90,-180,180);
TH1F hW("W","W",1000,0,6);
TH1F ht("t","t",1000,0,10);
TH1F hgE("gE","gE",1000,0,20);
TH1F hM2K("M2K","; 2K Mass (GeV)",1000,0.9,3);
TH1F hMXi("MXi","; Xi Mass (GeV)",1000,0.9,3);
TH1D hPiP("piP","#pi P",100,0,5);
TH1D hPiTh("piTh","#pi #theta",180,0,90);
TH1D hLamP("lamP","#Lambda P",100,0,5);
TH1D hLamTh("lamTh","#Lambda #theta",180,0,90);
TH1D hK1P("K1P","K1 P",100,0,10);
TH1D hK1Th("K1Th","K1 #theta",180,0,90);
TH1D hK2P("K2P","K2 P",100,0,10);
TH1D hK2Th("K2Th","K2 #theta",180,0,90);

void Hist_VeryStrange_KKXi_Flat_s_give_t_MXi(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
  LorentzVector prtarget(0,0,0,escat::M_pr());

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
  
  
  //produced 2K system decaying to 2K+ pi- with phase space mass distribution
  mass_distribution(9995,new DistTF1{TF1("hh","1",1.,(elbeam+prtarget).M())});
  auto X=particle(9995,model(new PhaseSpaceDecay{{},{321,321}}));
 

  //add a Breit-Wigner resonance for particle id 9996
  cout<<"Mass distribution is "<<massDist<<endl;
  mass_distribution(9996,new DistTF1{TF1("hh",massDist,1.2,(elbeam+prtarget).M())});
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
  auto photoprod=new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat, 1, tslope , 0 , 0} };
  mesonex( ebeamP , photoprod );

  auto production =  dynamic_cast<ElectronScattering*>(generator().Reaction());
  //FT limits
  production->SetLimitTarRest_eThmin(config._min_limit_e_th);
  production->SetLimitTarRest_eThmax(config._max_limit_e_th);
  production->SetLimitTarRest_ePmin(config._min_limit_e_p);
  production->SetLimitTarRest_ePmax(config.getMaxelP());
  
  //target limits
  production->GiveZVertexDist
    ( new DistUniform(config._tarPos-config._tarLength/2,config._tarPos+config._tarLength/2) );
  
 
   // ---------------------------------------------------------------------------
  //initilase the generator, may take some time for making distribution tables 
  // ---------------------------------------------------------------------------
  initGenerator();
  generator().SetNEvents(config._nEvents);
  
  // ---------------------------------------------------------------------------
  // Get event particles for filling histograms
  // ---------------------------------------------------------------------------
  auto lambdaXi = static_cast<DecayingParticle*>(Xi)->Model()->Products()[0];
  auto pionXi = static_cast<DecayingParticle*>(Xi)->Model()->Products()[1];

  auto k1 = static_cast<DecayingParticle*>(X)->Model()->Products()[0];
  auto k2 = static_cast<DecayingParticle*>(X)->Model()->Products()[1];

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
    double t = -1*(Xi->P4()-prtarget).M2();
    hQ2.Fill(Q2);
    hW.Fill(W);
    ht.Fill(t);
    hgE.Fill(photon.E());
    

    heE.Fill(electron->P4().P());
    heTh.Fill(electron->P4().Theta()*TMath::RadToDeg());
    hPiP.Fill(pionXi->P4().P());
    hPiTh.Fill(pionXi->P4().Theta()*TMath::RadToDeg());
    hLamP.Fill(lambdaXi->P4().P());
    hLamTh.Fill(lambdaXi->P4().Theta()*TMath::RadToDeg());
    hK1P.Fill(k1->P4().P());
    hK1Th.Fill(k1->P4().Theta()*TMath::RadToDeg());
    hK2P.Fill(k2->P4().P());
    hK2Th.Fill(k2->P4().Theta()*TMath::RadToDeg());

    hM2K.Fill((k1->P4()+k2->P4()).M());
    hMXi.Fill((pionXi->P4()+lambdaXi->P4()).M());
  }

  auto histfile=TFile::Open("Hist_VeryStrange_KKXi.root","recreate");
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
  
}
