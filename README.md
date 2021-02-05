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

Build with cmake

      mkdir build; cd build; cmake ../
      cmake --build . --target install

Runs like

      clas12-elSpectro --ebeam 10.6 --seed 2132 --trig 10 TestScript.C

Where a script (here TestScript.C) is used to configure a users
own generator model.


# Adding you own script

You script should configure your desired final state using the additional
information passed by C12Config config :

	    Long64_t _nEvents=1;//--trig  , number of events in output file
  	    UInt_t _seed=0; //--seed  , zero=>Random seed, 
	    TString _outFile={"clas12-elSpectro.dat"}; //--out , output filename
	    TString _misc={""}; //--misc , additional string users can use in their script
	    Double_t _beamP=10.4; //--ebeam , e- beam energy in GeV

Your script should have a clear name and inside you should define a function with the same name which will be called by clas2-elspectro. e.g. in MesonEx_p2pi_Flat_s_give_t_M2pi.C

     	    void MesonEx_p2pi_Flat_s_give_t_M2pi(C12Config config) {

Typically you can start by setting the beam and target LorentzVectors, getting the beamenergy from the config :

	  auto ebeamP=config._eBeam;
	  LorentzVector elbeam(0,0,ebeamP,escat::E_el(ebeamP));
	  LorentzVector prtarget(0,0,0,escat::M_pr());

