// e.g. final state proton eta(pi0,pi+,pi-)
//clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 --misc '$baryon=2212: $meson=221:111,211,-211' CLAS12_Simple_Flat_s.C
//e.g. final state Lambda0(p,pi-) K+
//clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 --misc '$baryon=3122:2212,-211 $meson=321' CLAS12_Simple_Flat_s.C

void CLAS12_Simple_Flat_s(C12Config config) {

  config.Print();

  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();
  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();
  
  auto tokens=config._misc.Tokenize("$");

  Int_t pdg_meson=0;
  Int_t pdg_baryon=0;
  vector<Int_t> decay_meson;
  vector<Int_t> decay_baryon;

  
  for(auto entry:*tokens) {
    TString sentry= entry->GetName();///get actual string

    if(sentry.Contains("baryon=")){ //baryon should be pdg code
      TString bstring= sentry;
      bstring.ReplaceAll("baryon=","");
      if(bstring.Contains(':')==kFALSE){//no decay
	pdg_baryon =  bstring.Atoi();
	continue;
      }
      pdg_baryon = TString(bstring(0,bstring.First(':'))).Atoi();
      TString decaystring(bstring(bstring.First(':')+1,bstring.Length()));
      auto decaytokens=decaystring.Tokenize(",");
      for(auto dec:*decaytokens){
	auto dec_pdg=TString(dec->GetName()).Atoi();
	if(TDatabasePDG::Instance()->GetParticle(dec_pdg)&&dec_pdg!=0)
	  decay_baryon.push_back(dec_pdg);
	else
	  Warning("CLAS12_General_Flat_s",Form("invalid PDG code for baryon decay product %d",dec_pdg),"");
      }
    }
    if(sentry.Contains("meson=")){//meson should be pdg:decay1,decay2,decay3,...
      TString mstring= sentry;
      mstring.ReplaceAll("meson=","");
      if(mstring.Contains(':')==kFALSE){//no decay
	pdg_meson =  mstring.Atoi();
	continue;
      }
      pdg_meson = TString(mstring(0,mstring.First(':'))).Atoi();
  
      TString decaystring(mstring(mstring.First(':')+1,mstring.Length()));
      auto decaytokens=decaystring.Tokenize(",");
      for(auto dec:*decaytokens){
	auto dec_pdg=TString(dec->GetName()).Atoi();
	if(TDatabasePDG::Instance()->GetParticle(dec_pdg)&&dec_pdg!=0)
	  decay_meson.push_back(dec_pdg);
	else
	  Warning("CLAS12_General_Flat_s",Form("invalid PDG code for meson decay product %d",dec_pdg),"");
      }
     }
  }
  for(auto& m:decay_meson)
    cout<<"meson "<<pdg_meson<<" "<<m<<endl;
    
  Particle* meson=nullptr;
  if(decay_meson.empty()==true)
    meson=particle(pdg_meson);
  else
    meson=particle(pdg_meson,model(new PhaseSpaceDecay{{},decay_meson}));
  
   Particle* baryon=nullptr;
  
  if(decay_baryon.empty()==true)
    baryon=particle(pdg_baryon);
  else
    baryon=particle(pdg_baryon,model(new PhaseSpaceDecay{{},decay_baryon}));
  
  //decay of gamma* + p  to p + eta
  auto pGammaStarDecay = model(new DecayModelst{ {baryon,meson},{} });
  

  mesonex( elBeam, prTarget ,  new DecayModelQ2W{0, pGammaStarDecay });

  
  c12process(config);
  
}
