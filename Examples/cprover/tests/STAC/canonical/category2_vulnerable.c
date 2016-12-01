#include "tick.h"

void doProcess(int n) {
    for(int i = 0; i < n; i++) {
        tick(1000);
        for(int t = 0; t < n; t++) {
            tick(1);
        }
    }
    __VERIFIER_print_hull(__cost);
}

int main(int argc, char ** argv) {
    init_tick(0);
    if (argc != 2 || atoi(argv[1]) > 99) { return -1; }
    doProcess(atoi(argv[1]));
    return 0;
}

// Java source:
/*

//Lower Orders are Disregarded
public class Category2_vulnerable {
  private static void doProcess(int n) throws InterruptedException {
    for (int i = 0; i < n; i++) {
      Thread.sleep(1000);
      for(int t = 0; t < n; t++){
        Thread.sleep(1);
      }
    }
  }

  public static void main(String[] args) throws InterruptedException {
    if(args.length!=1 || Integer.parseInt(args[0]) > 99){ System.exit(-1);}
    doProcess(Integer.parseInt(args[0]));
  }
}

*/
