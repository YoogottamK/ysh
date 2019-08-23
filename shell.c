#include "utils.h"

int main() {
    // sets up stuff
    init();

    // the mail loop
    repl();

    // prepare for shutdown
    teardown();

    return 0;
}
