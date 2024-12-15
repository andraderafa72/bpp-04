#include "program.h"

int main(int argc, char* argv[]) {
  Program program;

  program.initialize(argc, argv);

  while(not program.is_quitting()){
    program.process_events();
    program.update();
    program.render();
  }

  return 0;
}