#include "gui.h"
#include "render_fns.h"
#include "variables.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

  srand(time(NULL));

  initposition(current.board);

  if (init_gui() != 0) {
    return -1;
  }

  run_gui_loop();

  cleanup_gui();
  return 0;
}
