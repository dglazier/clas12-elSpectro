# clas12-elSpectro
clas12-mcgen friendly interface to elSpectro event generator.

elSpectro provides efficient sampling of electron scattering phase space
distributions for any final state, from which arbitrary models can be built on.
In particular an interface to jpacPhoto is used to build models
based on scattering amplitudes.


# INSTALL

      git clone  --recurse-submodules https://github.com/dglazier/clas12-elSpectro

Need to set env paths for user clas12-elSpectro repo and elSpectro

      export C12ELSPECTRO=$PWD/clas12-elSpectro/
      
      export ELSPECTRO=$C12ELSPECTRO/elSpectro/

or

      setenv C12ELSPECTRO $PWD/clas12-elSpectro/
      setenv ELSPECTRO $C12ELSPECTRO/elSpectro/

Build with cmake


      cd $C12ELSPECTRO
      mkdir build; cd build; cmake ../
      cmake --build . --target install

Runs like

      clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 TestScript.C

Where a script (here TestScript.C) is used to configure a users
own generator model. The script name must be the last option!

To run on OSG the script must be pushed to the clas12-elSpectro repository in the scripts directory.


# Adding you own script

You script should configure your desired final state using the additional
information passed by C12Config config :

	    Long64_t _nEvents=1;//--trig  , number of events in output file
  	    UInt_t _seed=0; //--seed  , zero=>Random seed, 
	    TString _outFile={"clas12-elSpectro.dat"}; //--out , output filename
	    TString _misc={""}; //--misc , additional string users can use in their script
	    Double_t _beamP=10.4; //--ebeam , e- beam energy in GeV

There are also options for specifiying physical limits for the experiment

    //limits e.g. for FT or FD on e-,
    //set with --ft or --min_e_th 6 (for 6degrees minimum electron angle)
    Double_t _min_limit_e_th=0;
    Double_t _max_limit_e_th=TMath::Pi();
    Double_t _min_limit_e_p=0;

Note the predefined FT limts, applied if option --ft is given, are

    _min_limit_e_th=2*TMath::DegToRad();
    _max_limit_e_th=5*TMath::DegToRad();
    _min_limit_e_p=0.4;
    _max_limit_e_p=6;

    //target cell position
    Double_t _tarPos=-3.; //set with e.g. --tarPos 2
    Double_t _tarLength=5.; //set with e.g. --tarLen 10

Your script should have a clear name and inside you should define a function with the same name which will be called by clas2-elspectro. e.g. in MesonEx_p2pi_Flat_s_give_t_M2pi.C

           //function to describe the production and decay of
	   //ep->e'pX with X->pi+pi-
     	    void MesonEx_p2pi_Flat_s_give_t_M2pi(C12Config config) {

Typically you can start by setting the beam and target LorentzVectors, getting the beamenergy from the config :

	  auto ebeamP=config._eBeam;
	  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
	  LorentzVector prtarget(0,0,0,escat::M_pr());

Now as a trick we can take a hardcoded mass distribution or provide one with a commond line string via the misc option

    TString massDist = "0.9*TMath::BreitWigner(x,0.78,0.149) + 0.1*TMath::BreitWigner(x,1.27,0.187)+0.1";
    if(config._misc.Contains("M2pi=")){ //look to see if misc overrides mass distribition
      massDist = config._misc;
      massDist.ReplaceAll("M2pi=","");
    }

    mass_distribution(9995,new DistTF1{TF1("hh",massDist,0.,(elbeam+prtarget).M())});

So I can add command line options like --misc 'M2pi=TMath::BreitWigner(x,0.78,0.149)'  Similarly you can figure your macro decoding the misc string as you need it.

This mass distribution is for the meson X, next we define its decay (note the 9995 linking to the mass distribuiton)

    //produced meson decaying to pi+ pi- with mass distribution 9995
    auto X=particle(9995,model(new PhaseSpaceDecay{{},{211,-211}}));

No we can define our final state as X and a proton(2212)

    //decay of gamma* + p  to p + X
    //depends on s and t
    auto pGammaStarDecay = model(new DecayModelst{ {X},{2212} });

And finally our production mechanism. This gives an electroproduction reaction producing a pgamma* intermediates state which decays with CM angular distribution which has a t-slope of 5.

    mesonex(ebeamP,new DecayModelQ2W{0, pGammaStarDecay,new TwoBody_stu{0, 1, 5 , 0 , 0} });

Note the angular distribution can be overwritten by a more sophisticated model which would replace DecayModelst above, such as jpacModelst, in which case in should instead be tuned to optimise generator speed via importance sampling. Note the first parameter which is 0 gives a flat uniform analguar distribution component which you may mix with the t-slope. The first and second parameteres give the relative strength of s and t contributions. Ignore the last two parameters.

Then run the generator


  c12process(config);


