#pragma once

#include <TObject.h>
#include <TMath.h>

class C12Config : public TObject {

  
 public :
  C12Config()=default;
  
 C12Config(Long64_t n,UInt_t seed, const char* file,Double_t ebeam,
	   const char*  misc,
	   Double_t mineth,Double_t maxeth,Double_t minep,Double_t maxep,
	   Double_t tarP,Double_t tarL)
   :_nEvents(n),_seed{seed},_outFile{file},_beamP{ebeam}
  ,_misc{misc}
  ,_min_limit_e_th{mineth},_max_limit_e_th{maxeth}
  ,_min_limit_e_p{minep},_max_limit_e_p{maxep}
  ,_tarPos{tarP},_tarLength{tarL}
  {}

    
  TString GetContructorString() const{
    //Note the order of aruments given here
    //must match the order in the contructor
    
    TString construct="C12Config config(";
    construct+=Form("%lld,",_nEvents);
    construct+=Form("%d,",_seed);
    construct+=Form("\"%s\",",_outFile.Data());
    construct+=Form("%lf,",_beamP);
    construct+=Form("\"%s\",",_misc.Data());
    construct+=Form("%lf,",_min_limit_e_th);
    construct+=Form("%lf,",_max_limit_e_th);
    construct+=Form("%lf,",_min_limit_e_p);
    construct+=Form("%lf,",_max_limit_e_p);
    construct+=Form("%lf,",_tarPos);
    construct+=Form("%lf",_tarLength);
    construct+=")";
    return construct;
  }

  void setMin_elAngle(Double_t mina){
    _min_limit_e_th=mina*TMath::DegToRad();
  }
  void setFT(){
    _min_limit_e_th=2*TMath::DegToRad();
    _max_limit_e_th=5*TMath::DegToRad();
    _min_limit_e_p=0.4;
    _max_limit_e_p=6;
  }

  Float_t getMaxelP()const{ return _max_limit_e_p==0?_beamP:_max_limit_e_p;}
  void setMaxelP(Float_t p){_max_limit_e_p=p;}
  
  Long64_t _nEvents=1;//--trig  , number of events in output file
  
  UInt_t _seed=0; //--seed  , zero=>Random seed, 

  TString _outFile={"clas12-elSpectro.dat"}; //--out , output filename

  TString _misc={""}; //--misc , additional string users can use in their script

  Double_t _beamP=10.4; //--ebeam , e- beam energy in GeV


  //limits e.g. for FT or FD on e-
  Double_t _min_limit_e_th=0;
  Double_t _max_limit_e_th=TMath::Pi();
  Double_t _min_limit_e_p=0;


  Double_t _tarPos=-3.;
  Double_t _tarLength=5.;

  void Print(){
    std::cout<<"configuration :"<<"\n\t"<<"nEvents = "<<_nEvents\
	     <<"\n\t"<<"seed "<<_seed<<"\n\t"<<"outfile "<<_outFile<<"\n\t"<<"beam energy  "<<_beamP<<" "<<"\n\t"<<"misc "<<_misc<<"\n";
    std::cout<<"\n \t"<<"target position = "<<_tarPos<<"\t Length = "<<_tarLength<<"\n";
    std::cout<<"\n \t"<<"electron Limits angle = "<<_min_limit_e_th<<"-"<<_max_limit_e_th;
    std::cout<<"\t"<<"electron Limits momentum = "<<_min_limit_e_p<<"-"<<getMaxelP()<<std::endl;
  }
 private:
  Double_t _max_limit_e_p=0;

  
};
#pragma link C++ class C12Config;

