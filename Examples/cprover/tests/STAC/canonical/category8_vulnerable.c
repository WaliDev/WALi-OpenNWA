#include "tick.h"

void f3(int p) {
    int i;
    for(i=0; i<p; i++) {
        tick(1);
    }
    __VERIFIER_print_hull(__cost);
}

void f2(int m, int p) {
    int i;
    for(i=0; i<m; i++) {
        f3(p);
    }
    __VERIFIER_print_hull(__cost);
}

void f1(int n, int m, int p) {
    int i;
    for(i=0; i<n; i++) {
        f2(m, p);
    }
    __VERIFIER_print_hull(__cost);
}

int main(int argc, char ** argv) {
    init_tick(0);
    if(argc != 4 || atoi(argv[1])>9 || atoi(argv[2])>9 || atoi(argv[3]>99)) return -1;
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int p = atoi(argv[3]);
    f1(n, m, p);
    return 0;
}

// Java source:
/*

//Only Consider a Single Dimension of Input
public class Category8_vulnerable {
  private static void f1(int n, int m, int p) {
    for(int i = 0; i < n; i++) {
      f2(m, p);
    }
  }
  private static void f2(int m, int p) {
    for(int i = 0; i < m; i++) {
      f3(p);
    }
  }
  private static void f3(int p) {
    for(int i = 0; i < p; i++) {
      try {
        Thread.sleep(1);
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
  }

  public static void main(String[] args) throws InterruptedException {
    if(args.length!=3 || Integer.parseInt(args[0]) > 9
            || Integer.parseInt(args[1]) > 9
            || Integer.parseInt(args[2]) > 99){ System.exit(-1);}
    int n = Integer.parseInt(args[0]);
    int m = Integer.parseInt(args[1]);
    int p = Integer.parseInt(args[2]);
    f1(n, m, p);
  }
}
*/
