#pragma once

#include "Interface.h"
#include "LundWriter.h"
#include "DistUniform.h"

namespace elSpectro{
  
  void c12process(const C12Config& config){

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
    
    
    while(finishedGenerator()==false){
      nextEvent();
      countGenEvent();
    }
    
  }


}
#pragma link C++ defined_in "C12Utils.h";
