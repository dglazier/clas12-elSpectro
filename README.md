# clas12-elSpectro
clas12-mcgen friendly interface to elSpectro event generator

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