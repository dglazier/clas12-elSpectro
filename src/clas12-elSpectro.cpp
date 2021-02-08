#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include <TString.h>
#include <TSystem.h>
#include <TRandom.h>

#include "C12Config.h"
#include "C12Utils.h"

int main(int argc, char **argv) {
  
  C12Config config; //collect command line arguments
  // printf("what about this \n");
  TString macroName="TestScript.C";
  
   for(Int_t i=1;i<argc;i+=2){
 
    TString opt=argv[i];
   
    TString val=argv[i+1];
    if(val.Contains("--"))
      printf("Warning command line argument seems to be missing between %s and %s \n",opt.Data(),val.Data());
    
    //  std::cout<<"opt "<<opt<<"\n";
    if((opt.Contains("--docker"))) {
      i=i-1;
      continue;
      }
    if((opt.Contains(".C"))) macroName=opt;
    if((opt.Contains("--trig"))) config._nEvents=val.Atoi();
    if((opt.Contains("--seed"))) config._seed=val.Atoi();
    if((opt.Contains("--ebeam"))) config._beamP=val.Atof();
    if((opt.Contains("--out"))) config._outFile=val;
    if((opt.Contains("--misc"))) config._misc=val;
    if((opt.Contains("--ft"))) {
      config.setFT();
      i=i-1;
      continue;
    }
    if((opt.Contains("--min_e_th"))) config.setMin_elAngle(val.Atof());
    if((opt.Contains("--tarPos"))) config._tarPos=val.Atof();
    if((opt.Contains("--tarLength"))) config._tarLength=val.Atof();
    
  }
   
   gRandom->SetSeed(config._seed);
 
   auto  app =  std::unique_ptr<TRint>{new TRint("elSpectro", &argc, argv)};
   app->ProcessLine(Form(".x %s/core/src/Load.C",gSystem->Getenv("ELSPECTRO")));
   app->ProcessLine("gROOT->SetMacroPath(\"$C12ELSPECTRO/scripts/\")");
   //Load C12Config class
   TString configPath=Form("%s/src/C12Config.h",gSystem->Getenv("C12ELSPECTRO"));
   if(configPath.Contains("//")) configPath.ReplaceAll("//","/");//tidy string
   app->ProcessLine(Form(".L %s",configPath.Data()));

   TString utilsPath=Form("%s/src/C12Utils.h",gSystem->Getenv("C12ELSPECTRO"));
   if(utilsPath.Contains("//")) utilsPath.ReplaceAll("//","/");//tidy string
   app->ProcessLine(Form(".L %s",utilsPath.Data()));

   //check if macro exists in scripts
   TString fname("$C12ELSPECTRO/scripts/");
   fname+=macroName;
   if(gSystem->FindFile("",fname)==nullptr){//script does not exist
     //////////////////////////////////////
     ///Here we allow the possibility the script exists
     ///on github but has not been released yet....
     gSystem->Exec(Form("wget https://raw.githubusercontent.com/dglazier/clas12-elSpectro/main/scripts/%s",macroName.Data()));
     gSystem->Exec(Form("mv %s $C12ELSPECTRO/scripts/.",macroName.Data()));
   }
   //e.g. C12Config config(1,2132,"clas12-elspectro.dat",10.600000);
   app->ProcessLine(config.GetContructorString());
    //run macro
   app->ProcessLine(Form(".x %s(config)",macroName.Data()));
   
   
   app->Terminate(0);
   return 0;
}
