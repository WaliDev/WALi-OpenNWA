#include "tick.h"

int secret = 1234; //perhaps should generalize in the future
int n = 10;

void checkSecret(int guess, int t) {
    int i;
    if(guess <= secret) {
        if(t==1) {
            tick(1)
        } else if(t == 2) {
            for(i=0; i < n*n; i++) {
                tick(1);
            }
        } else {
            for(i=0; i< n*n*n; i++) {
                tick(1);
            }
        }
    } else {
        if(t==1) {
            tick(1);
        } else if(t == 2) {
            for(i=0; i< n * n; i++){
                tick(1);
            }
        } else {
            for(i=0; i< n * n * n; i++){
                tick(1);
            }
        }
    }
}

int main(int argc, char ** argv) {
    init_tick(0);
    if(argc != 3) return -1;
    int guess = atoi(argv[1]);
    int t = atoi(argv[2]);
    checkSecret(guess, t);
    return 0;
}

/*
//Only Best & Worst Case Bounds Considered
public class Category3_not_vulnerable {
  private static final int secret = 1234;
  private static final int n = 10;
  private static void checkSecret(int guess, int t) throws InterruptedException {
    if(guess <= secret){
      if(t == 1){Thread.sleep(1);}
      else if(t == 2){
        for(int i = 0; i<n*n;i++){
          Thread.sleep(1);
        }
      }
      else{
        for(int i = 0; i<n*n*n;i++){
          Thread.sleep(1);
        }
      }
    }
    else{
      if(t == 1){Thread.sleep(1);}
      else if(t == 2){
        for(int i = 0; i<n*n;i++){
          Thread.sleep(1);
        }
      }
      else{
        for(int i = 0; i<n*n*n;i++){
          Thread.sleep(1);
        }
      }
    }
  }
  public static void main(String[] args) throws InterruptedException {
    if(args.length!=2){ System.exit(-1);}
    int guess = Integer.parseInt(args[0]);
    int t = Integer.parseInt(args[1]);
    checkSecret(guess,t);
  }
}
*/
