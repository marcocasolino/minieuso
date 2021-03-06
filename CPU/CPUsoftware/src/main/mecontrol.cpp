/*-------------------------------
                                 
Mini-EUSO CPU software                 
https://github.com/cescalara
                                  
Francesca Capel                  
capel.francesca@gmail.com         
                                 
--------------------------------*/
#include "mecontrol.h"


/* main program */
/*--------------*/
int main(int argc, char ** argv) {

  InputParser * input = new InputParser(argc, argv);

  /* parse command line options */
  CmdLineInputs * CmdLine = input->ParseCmdLineInputs();
  if (CmdLine == NULL) {
    return 1;
  }
  else {

    if (CmdLine->help) {
      /* exit when help message called */
      return 0;
    }
    if (CmdLine->ver) {
      /* exit when version info called */
      return 0;
    }
    
  }
  
  /* run instrument according to specifications */
  RunInstrument  MiniEuso(CmdLine);
  MiniEuso.Start();

  return 0; 
}

  
