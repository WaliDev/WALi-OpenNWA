#include "tick.h"

//typedef struct {
//    int * m;
//    int m_length;
//} Foo;
int m_length;

void foo(/*Foo * self,*/ int n) {
    //self->m = malloc(n * n * sizeof(int));
    /*self->*/m_length = n * n;
}

void bar(/*Foo * self*/) {
    for(int i = 0; i < /*self->*/m_length; i++) {
        tick(1);
    }
    __VERIFIER_print_hull(__cost);
}

int main(int argc, char ** argv) {
    init_tick(0);
    if (argc != 1 || atoi(argv[0]) > 99) { return -1; }

    int n = atoi(argv[0]);
    //Foo f;
    foo(/*&f,*/ n);
    bar(/*&f*/);
    return 0;
}

// Java source:
/*

//Focus is Limited to Localized Behavior
public class Category7_vulnerable {
  private static class Foo{
    private int[] m;

    Foo(int n){
      m = new int[n*n];
    }

    void Bar() throws InterruptedException {
      for(int i : m){
        Thread.sleep(1);
      }
    }
  }
  public static void main(String[] args) throws InterruptedException {
    if(args.length!=1 || Integer.parseInt(args[0]) > 99){ System.exit(-1);}

    int n = Integer.parseInt(args[0]);
    Foo foo = new Foo(n);
    foo.Bar();
  }
}

*/
