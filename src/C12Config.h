#pragma once

#include <TObject.h>

class C12Config : public TObject {

  
 public :
  C12Config()=default;
  
 C12Config(Long64_t n,UInt_t seed, const char* file,Double_t ebeam,const char*  misc)
   :_nEvents(n),_seed{seed},_outFile{file},_eBeam{ebeam},_misc{misc}{}

    
  TString GetContructorString() const{
    TString construct="C12Config config(";
    construct+=Form("%lld,",_nEvents);
    construct+=Form("%d,",_seed);
    construct+=Form("\"%s\",",_outFile.Data());
    construct+=Form("%lf,",_eBeam);
    construct+=Form("\"%s\"",_misc.Data());
    construct+=")";
    return construct;
  }
  
  Long64_t _nEvents=1;//--trig  , number of events in output file
  
  UInt_t _seed=0; //--seed  , zero=>Random seed, 

  TString _outFile={"clas12-elSpectro.dat"}; //--out , output filename

  TString _misc={""}; //--misc , additional tring users can use in their script

  Double_t _eBeam=10.4; //--ebeam , e- beam energy in GeV

  void Print(){
    std::cout<<"configuration :"<<"\n\t"<<"nEvents = "<<_nEvents\
	     <<"\n\t"<<"seed "<<_seed<<"\n\t"<<"outfile "<<_outFile<<"\n\t"<<"beam energy  "<<_eBeam<<" "<<"\n\t"<<"misc "<<_misc<<std::endl;
  }
  
};
#pragma link C++ class C12Config;

