#include "tick.h"

int secret = 1234; //perhaps should generalize in the future
int n = 32;

void checkSecret(int guess) {
    int i, t;
    if(guess <= secret) {
        for(i=0; i<n; i++) {
            for(t=0; t<n; t++) {
                tick(1);
            }
        }
    } else {
        for(i=0; i<n; i++) {
            for(t=0; t<n; t++) {
                tick(2);
            }
        }
    }
}

int main(int argc, char ** argv) {
    init_tick(0);
    if(argc != 2) return -1;
    int guess = atoi(argv[1]);
    checkSecret(guess);
    return 0;
}

/*
public class Category1_not_vulnerable {
  private static final int secret = 1234;
  private static final int n = 32;
	private static void checkSecret(int guess) throws InterruptedException {
		if(guess <= secret){
			for(int i=0;i<n;i++){
				for(int t=0;t<n;t++) {
					Thread.sleep(1);
				}
			}
		}
		else{
			for(int i=0;i<n;i++){
				for(int t=0;t<n;t++) {
					Thread.sleep(2);
				}
			}
		}
	}
	public static void main(String[] args) throws InterruptedException {
		if(args.length!=1){ System.exit(-1);}
		int guess = Integer.parseInt(args[0]);
		checkSecret(guess);
  }
}
*/
