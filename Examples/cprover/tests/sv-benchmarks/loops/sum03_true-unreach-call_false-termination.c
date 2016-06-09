#define a (2)

int main() { 
  int sn=0;
  unsigned int loop1=__VERIFIER_nondet_uint(), n1=__VERIFIER_nondet_uint();
  unsigned int x=0;

  while(x < 1000000){
    sn = sn + a;
    x++;
    __VERIFIER_assert(sn==x*a || sn == 0);
  }
}

