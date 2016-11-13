#include "assert.h"
int i = 0;

void loop() {
    if (i >= 50)
        return;
    else {
       i++;
       loop();
    }
}

void main() {
   loop();
   __VERIFIER_assert(i==50);
}
