//to run
// clas12-elSpectro --ebeam 22 --seed 2132 --trig 10 --misc '--tslope=4 --flat=1 --muons' CLAS24_JPAC_nZc.C
//--trig => nevents
//--jpac=1 => model selection, none by default, >0 => use JPAC model  
//--muons => decay to muons not electrons (if ommitted will decay to electrons)
//--tslope => give t distribution slope
//--flat => give relative amount of flat production angle compared to t distribution
#include "amplitude_blend.hpp"
#include "reaction_kinematics.hpp"
#include "regge_trajectory.hpp"
#include "amplitudes/vector_exchange.hpp"
#include "amplitudes/pseudoscalar_exchange.hpp"
#include "amplitudes/pomeron_exchange.hpp"
#include "amplitudes/amplitude_sum.hpp"
jpacPhoto::amplitude_blend* Amplitude(Int_t mode);

void CLAS24_JPAC_nZc(C12Config config) {

  config.Print();
  Float_t tslope=5.; //tslope
  Float_t flat = 0.; //relative amount of flat CM production angle
  Int_t jpacModel = -1; //default no jpac model
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
    if(sentry.Contains("jpac=")){// use jpac amp 
      TString fstring= sentry;
      fstring.ReplaceAll("jpac=","");
      jpacModel =  fstring.Atoi();
    }
    if(sentry.Contains("muons")){//Jpsi decay to muons
      decayToMuons=kTRUE;
    }
  }
  
  auto ebeamP=config._beamP;

  //define e- beam, pdg =11 momentum = _beamP
  auto elBeam = initial(11,ebeamP);
  auto elin=elBeam->GetInteracting4Vector();

  //proton target at rest
  auto prTarget= initial(2212,0);
  auto prin=prTarget->GetInteracting4Vector();

  //produced jpsi decaying to e- e+
  Int_t leptonPDG=11;
  if(decayToMuons) leptonPDG=13;
  auto jpsi=particle(443,model(new PhaseSpaceDecay({},{leptonPDG,-leptonPDG})));
  //Zc
  double minMass = 3.6;
  double maxMass = 4.6;
  mass_distribution(9995,new DistTF1{TF1("hh",Form("TMath::BreitWigner(x,%lf,0.05)",jpacPhoto::M_ZC3900),minMass,maxMass)});
  auto Z=particle(9995,model(new PhaseSpaceDecay{{jpsi},{211}}));
  Z->SetPdgMass(jpacPhoto::M_ZC3900);

  amplitude_blend *jpac_amp=nullptr;
  if(jpacModel<0){
    //decay of gamma* + N  to N' + Jpsi
    //depends on s and t
    auto pGammaStarDecay = static_cast<DecayModelst*>(model(new DecayModelst{ {Z},{2112} }));
    mesonex( elBeam,prTarget ,
	     new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat,1,tslope,0,0}});

  }
  else{

    jpac_amp = Amplitude(jpacModel);
    auto pGammaStarDecay = new JpacModelst{jpac_amp, {Z},{2112} };
    flat=0;
    tslope=2.5;
    mesonex( elBeam,prTarget ,
	     new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat,1,tslope,0,0}});
    
  }

    

  c12process(config);

  if(jpac_amp!=nullptr)delete jpac_amp;
}
jpacPhoto::amplitude_blend* Amplitude(Int_t mode){
  using namespace jpacPhoto;
  // ---------------------------------------------------------------------------
  // AMPLITUDES
  // ---------------------------------------------------------------------------

  double g_NN = sqrt(4. * M_PI * 13.81); // Nucleon coupling same for all
  double LamPi = .9;  // 900 MeV cutoff for formfactor
  double bPi = 1. / (LamPi * LamPi);

  // Zc(3900)
  auto kZc = new reaction_kinematics{M_ZC3900};
  kZc->set_JP(1, 1);

  double gc_Psi = 1.91; // psi coupling before VMD scaling
  double gc_Gamma = E * F_JPSI * gc_Psi / M_JPSI;
  std::vector<double> Zc_couplings = {gc_Gamma, g_NN};

  // Pion trajectory 
  int signature = +1;
  double alpha_prime = 0.7; // GeV^-2
  double alpha_0 =  - alpha_prime * M2_PION;
  auto alpha = new linear_trajectory{signature, alpha_0, alpha_prime};

  // ---------------------------------------------------------------------------
  // low => Fixed-spin amplitudes
  // ---------------------------------------------------------------------------
  auto ampZcLow = new pseudoscalar_exchange{kZc, M_PION, "#it{Z_{c}} (3900)^{+}"};
  ampZcLow->set_params(Zc_couplings);
  ampZcLow->set_formfactor(true, bPi);
 // ---------------------------------------------------------------------------
  // high => Reggeized amplitudes
  // ---------------------------------------------------------------------------
  auto ampZcHigh = new pseudoscalar_exchange{kZc, alpha, "#it{Z_{c}}(3900)^{+}"};
  ampZcHigh->set_params(Zc_couplings);
  ampZcHigh->set_formfactor(true, bPi);


  double low_s = 15*15.;//GeV
  double high_s = 20*20.;//GeV
  auto  ampZc= new amplitude_blend{kZc,ampZcLow,low_s,ampZcHigh,high_s, "#it{Z_{c}}(3900)^{+} blend"};
  return ampZc;

  
}
