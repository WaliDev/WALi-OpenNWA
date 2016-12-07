#include "tick.h"

void function(int x) {
    double N = 10000000005.0;
    double z = 0;
    for(int i = 0; i<x; i++){ // z = N*x
        z+=N;
    }
    double w = z/x; // w = z/x = N*x/x = N
    if((long)fabs(N - w) != 0){
        tick(10000);
    }
    __VERIFIER_print_hull(__cost);
}

int main(int argc, char ** argv) {
    init_tick(0);
    if (argc != 2) { return -1; }
    int val = atoi(argv[1]);
    if (val < 1 || val > 10000000) { return -1; }
    function(val);
    return 0;
}

// Java source:
/*

//Modeling Floating Point Computations
public class Category9_vulnerable {
    private static void function(int x) throws InterruptedException {
        double N = 10000000005.0;
        double z = 0;
        for(int i = 0; i<x; i++){ // z = N*x
            z+=N;
        }
        double w = z/x; // w = z/x = N*x/x = N
        if((long)Math.abs(N - w) != 0){
            // Do computationally expensive calculation
            // Shouldn't happen since w == N
            Thread.sleep(10000);
        }
  }

    public static void main (String[] args) throws InterruptedException {
        //Slow x -> [6,755,397 to 7,355,881] ~ 6% of values
        //Fast x -> all other x [1 to 10,000,000]
        if(args.length!=1) {System.exit(-1);}
        int val = Integer.parseInt(args[0]);
        if (val < 1 || val > 10000000 ){System.exit(-1);}
        function(val);
  }

*/
