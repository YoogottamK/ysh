#include "utils.h"

int main() {
    //  setup up arrow history function
    rl_bind_keyseq("\\e[A", upHist);

    // sets up stuff
    init();

    // the main loop
    repl();

    // prepare for shutdown
    teardown();

    return 0;
}
