#pragma once

#include "Interface.h"
#include "LundWriter.h"
#include "DistUniform.h"
#include <iostream>
#include <vector>

namespace elSpectro{
  using std::cout;
  using std::endl;
  
  inline void c12init(const C12Config& config){

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
    // Initialize LUND
    // ---------------------------------------------------------------------------
    
    writer(new LundWriter{config._outFile.Data()});
    
    //initilase the generator, may take some time for making distribution tables 
    initGenerator();
    generator().SetNEvents(config._nEvents);
    
    
  }
  //////////////////////////////////////////////////////////////////
  inline std::vector<Particle*>  c12restrictions(const C12Config& config){

    std::vector<Particle*> restrictTheta;
    //Look if any particles need their theta restricted
    if(config._minParticleAngle>0){
      auto parts = particles().StableParticles();
      auto restrictThetaFor=config.restrictThetaFor();
      for(auto part: parts){
	//check if specific PDGs asked for
	if(restrictThetaFor.empty()==false){
	  for(auto& restrictPDG:restrictThetaFor){
	    if(part->Pdg()==restrictPDG){
	      restrictTheta.push_back(part);
	    }
	  }
	  
	}
	//if nothing specific restrict all!
	else{
	  restrictTheta.push_back(part);
	}
      }
    }
    return restrictTheta;
  }
  /////////////////////////////////////////////////////////////////
  inline void c12process_all(const C12Config& config){
    //intialisase
    c12init(config);

  
    while(finishedGenerator()==false){
      nextEvent();
      countGenEvent();
    }
    
  }
  /////////////////////////////////////////////////////////////////
  inline void c12process(const C12Config& config){
    
  
    auto restrictTheta = c12restrictions(config);
    if(restrictTheta.empty()==true)
      //no restrictions just process all
      //and do not follow the rest of this function
      return c12process_all(config);

    
    //intialisase
    c12init(config);

    //run generator but only write events if they pass
    //angle cut
    //set angle with command line option --min_part_th val (in degrees)
    //add particle to restrict with --restrict_theta 221:-211 etc.
    //calls this function
    while(finishedGenerator()==false){
      generator().Clear();
      generator().Reaction()->GenerateProducts();

      Bool_t inAcceptance=kTRUE;
      for( auto part: restrictTheta){
	if(part->P4().Theta() <config._minParticleAngle){
	  inAcceptance=kFALSE;
	  break;
	}
      }
      if(inAcceptance==kTRUE){
	generator().Write();
	countGenEvent();
      }
    }
 
  }
  
 
}
#pragma link C++ defined_in "C12Utils.h";
