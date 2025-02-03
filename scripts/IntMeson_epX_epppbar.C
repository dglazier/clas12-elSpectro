// clas12-elSpectro --ebeam 10.2 --trig 10000 --min_e_th 6 IntMeson_epX_epppbar.C
// clas12-elSpectro --ebeam 10.2 --trig 10000 --ft IntMeson_epX_epppbar.C
// Can also give it massDist, tslope and flat options using
// --misc "--tlsope= --flat= --M=" IntMeson_epX_epppbar.C

void IntMeson_epX_epppbar(C12Config config) {
  config.Print(); 

  // Retrieve beam energy from configuration
  auto ebeamP = config._beamP;

  // Initialize electron beam and proton target
  auto elBeam = initial(11, ebeamP);
  auto elin = elBeam->GetInteracting4Vector();
  auto prTarget = initial(2212, 0);
  auto prin = prTarget->GetInteracting4Vector();
  // General mass distribution
  TString massDist = "TMath::BreitWigner(x, 2., .4)";
  Float_t tslope = 1;
  Float_t flat = 0;
  Float_t tfrac = 1;

  auto tokens = config._misc.Tokenize("$");

  for(auto entry:*tokens)
  {
    TString sentry = entry->GetName();
    if(sentry.Contains("M="))
    {
      massDist = sentry;
      massDist.ReplaceAll("M=", "");
    }

    if(sentry.Contains("tslope="))
    {
      TString tstring= sentry;
      tstring.ReplaceAll("tslope=", "");
      tslope = tstring.Atof();
    }
    if(sentry.Contains("flat="))
    {
      TString fstring= sentry;
      fstring.ReplaceAll("flat=", "");
      flat = fstring.Atof();
    }
    if(sentry.Contains("tfrac="))
    {
      TString tfstring = sentry;
      tfstring.ReplaceAll("tfrac", "");
      tfrac = tfstring.Atof();
    }

  } 
  // Define the mass distribution for X using Breit-Wigner
  mass_distribution(9995, new DistTF1{TF1("hh", massDist, 0., (*elin + *prin).M())});

  // Define the intermediate meson X
  auto X = particle(9995, model(new PhaseSpaceDecay{{}, {2212, -2212}})); 
  // 2212: proton, -2212: anti-proton

  // Define the decay of the virtual photon + proton system into X + scattered proton
  auto pGammaStarDecay = model(new DecayModelst{{X}, {2212}});
  
  // Create meson electroproduction process
  mesonex(elBeam, prTarget, new DecayModelQ2W{0, pGammaStarDecay, new TwoBody_stu{flat, tfrac, tslope, 0, 0}});

  // Process the configuration
  c12process(config);
}
