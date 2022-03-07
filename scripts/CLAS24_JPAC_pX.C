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

void CLAS24_JPAC_pX(C12Config config) {

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
  //rho
  mass_distribution(113,new DistTF1{TF1("hhRho","TMath::BreitWigner(x,0.775,0.151)",0.25,1)});
  auto rho=particle(113,model(new PhaseSpaceDecay({},{211,-211})));
  //X
  mass_distribution(9995,new DistTF1{TF1("hh","TMath::BreitWigner(x,3.872,0.001)",3.85,3.89)});
  auto X=particle(9995,model(new PhaseSpaceDecay{{jpsi,rho},{}}));
  X->SetPdgMass(M_X3872);


  amplitude_blend *jpac_amp=nullptr;
  if(jpacModel<0){
    //decay of gamma* + N  to N' + Jpsi
    //depends on s and t
    auto pGammaStarDecay = static_cast<DecayModelst*>(model(new DecayModelst{ {X},{2212} }));
    mesonex( elBeam,prTarget ,
	     new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{flat,1,tslope,0,0}});

  }
  else{

    jpac_amp = Amplitude(jpacModel);
    auto pGammaStarDecay = new JpacModelst{jpac_amp, {X},{2212} };
    flat=0;
    tslope=1.5;
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

  // X(3872)
  auto kX = new reaction_kinematics{M_X3872};
  kX->set_JP(1, 1);

  // Nucleon couplings and cutoffs
  double gV_omega = 16., gT_omega = 0.;
  double LamOmega = 1.2;
  double gV_rho = 2.4, gT_rho = 14.6;
  double LamRho = 1.4;
  double gV_phi = -6.2, gT_phi = 2.1;
  double gV_psi = 1.6E-3, gT_psi = 0.;

  // Top couplings
  double gX_omega = 8.2E-3;
  double gX_rho = 3.6E-3;
    
  // Linear trajectory for the rho
  auto alpha = new linear_trajectory{-1, 0.5, 0.9, "#rho - #omega"};

  // ---------------------------------------------------------------------------
  // High or Low  - Energy Amplitudes
  // ---------------------------------------------------------------------------
  //////////////////
  // X(3872)
  auto  X_omega_high=new vector_exchange{kX, alpha, "#omega"};
  X_omega_high->set_params({gX_omega, gV_omega, gT_omega});
  X_omega_high->set_formfactor(true, LamOmega);
  auto X_omega_low = new vector_exchange{kX, M_OMEGA, "#omega"};
  X_omega_low->set_params({gX_omega, gV_omega, gT_omega});
  X_omega_low->set_formfactor(true, LamOmega);

  auto  X_rho_high = new vector_exchange{kX, alpha, "#rho"};
  X_rho_high->set_params({gX_rho, gV_rho, gT_rho});
  X_rho_high->set_formfactor(true, LamRho);
  auto  X_rho_low= new vector_exchange{kX, M_RHO, "#rho"};
  X_rho_low->set_params({gX_rho, gV_rho, gT_rho});
  X_rho_low->set_formfactor(true, LamRho);

  std::vector<amplitude*> X_exchanges_low = {X_omega_low, X_rho_low};
  auto jpac_amp_low=new amplitude_sum(kX, X_exchanges_low, "#it{X}(3872) low");
  std::vector<amplitude*> X_exchanges_high = {X_omega_high, X_rho_high};
  auto jpac_amp_high= new amplitude_sum(kX, X_exchanges_high, "#it{X}(3872) high");

  double low_s = 7*7.;//GeV
  double high_s = 20*20.;//GeV
  auto jpac_amp=new amplitude_blend{kX,jpac_amp_low,low_s,jpac_amp_high,high_s, "#it{X} blend"};
  
  ///////////////////////////////////////////////////////////////////////
  return jpac_amp;

  
}
