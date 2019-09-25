#include "utils.h"

int main() {
    // sets up stuff
    init();

    // the main loop
    repl();

    // prepare for shutdown
    teardown();

    return 0;
}
