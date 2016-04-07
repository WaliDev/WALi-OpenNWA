// #include <stdio.h> /* printf, scanf, NULL */
// #include <stdlib.h> /* srand, rand */
// #include <time.h> /* time */

int __VERIFIER_nondet_int();

/* model searching in buffer */
/* 0: no match; 1: match */

// int nondet( int bs, int ms) {
//     
//     if (bs == 0)
//     return 0;
//     
//     /* intialize the generator */
//     srand(time(NULL));
//     
//     /* random number 0 or 1 */
//     if ((rand() % bs) > ms)
//         return 1;
//     else return 0;
// }

int nondet(int a, int b) {
  int temp;

  temp = __VERIFIER_nondet_int();
  __VERIFIER_assume(0 <= temp && temp <= a);
  if (temp > b) return 1;
  return 0;
  }

/* Note: To compile with gcc, comment out the tick command */
unsigned int tick = 0;

/* n is the number of symbols in input text */
int compress (int n) {
    /* max buffer size */
    int bufferSize = 1024;
    /* intialize current match size */
    int currentMatch = 0;
    /* intialize current buffer size */
    int searchBuffer = 0;
    
    /* coded string length */
    int codedStrLen = 6;
    
    /* output text */
    int m = 0;
    int matchIndex = 0;
    int tempBuffer = 0;
    
    while (n > 0) {
        /* if match then append next symbol to currentMatch */
        /* and update index of match */
        
        matchIndex = nondet(searchBuffer,currentMatch + 1);
        
        /* tick(searchBuffer); */
        tempBuffer = searchBuffer;  /* tempBuffer = mSearchBuffer; */
        while (tempBuffer > 0) {
            tempBuffer = tempBuffer - 1;
            tick++; /* tick(1); */
        }
        
        if (matchIndex) {
            currentMatch = currentMatch + 1;
        }
        /* found longest match */
        else {
            /* if the coded string shorter than raw text? */
            if (codedStrLen <= currentMatch) {
                /* output coded string */
                m = m + codedStrLen;
                /* append the match string to the search buffer */
                searchBuffer = searchBuffer + currentMatch + 1;
                
                tick++; /* tick(1); */
                
                /* reset the match */
                currentMatch = 0;
                matchIndex = 0;
            }
            else {
                /* otherwise, output symbols one at a time */
                /* from the currentMatch until we find a new */
                /* match or run out of symbols */
                currentMatch = currentMatch + 1;
                matchIndex = 0;
                while (currentMatch > 1 && matchIndex == 0) {
                    /* output one symbol */
                    m = m + 1;
                    searchBuffer = searchBuffer + 1;
                    currentMatch = currentMatch - 1;
                    matchIndex = nondet(searchBuffer,currentMatch);
                    
                    /* tick(searchBuffer); */
                    tempBuffer = searchBuffer;  /* tempBuffer = mSearchBuffer; */
                    while (tempBuffer > 0) {
                        tempBuffer = tempBuffer - 1;
                        tick++; /* tick(1); */
                    }
                }
                
            }
        }
        /* adjust search buffer size if it is bigger than max size */
        if (searchBuffer > 1024)
        searchBuffer = searchBuffer - 1024;
        
        tick++; /* tick(1); */
        
        /* get next symbol */
        n = n - 1;
    }
    
    return m;
}

/* test the compress */
int main() {
    int n;
    /*  printf ("Input the raw text length: \n"); */
    n = __VERIFIER_nondet_int();   /* scanf("%d", &n); */
    int m = compress(n);
    /*    printf ("Raw text length: %d \n", n); */
    /*    printf ("Compressed text length: %d \n", m); */
    
    return 0;
}
