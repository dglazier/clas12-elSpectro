cmake_minimum_required(VERSION 2.9)
project(clas12-elSpectro)

#set(CMAKE_INSTALL_BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_BINDIR})

find_package(ROOT REQUIRED MathMore RooFit GenVector EG Minuit2)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}) 
#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")


##MAKE ELSPETRO FRAMEWORK
include_directories(elSpectro/core)
add_subdirectory(elSpectro)
    
    
##INSTALL CLAS12-ELSPECTRO BINARY
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
file(GLOB_RECURSE EXE_FILES "src/*.cpp")
foreach( exefile ${EXE_FILES} )
  get_filename_component( exename ${exefile} NAME_WE)
  add_executable( ${exename} ${exefile} )
  target_link_libraries( ${exename} elSpectro)
  target_link_libraries( ${exename} jpacPhoto)
  target_link_libraries( ${exename} ${ROOT_LIBRARIES} -lRooFit -lMathMore -lEG -lGenVector)
endforeach( exefile ${EXE_FILES} )

##need to install jpacphoto ourseves as we don't have control
##over its CMakeFiles
install(TARGETS jpacPhoto
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
