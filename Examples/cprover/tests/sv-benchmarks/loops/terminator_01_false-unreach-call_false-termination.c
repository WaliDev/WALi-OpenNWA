int main()
{
  int x=__VERIFIER_nondet_int();
  int *p = &x;
 
  while(x<100) {
   (*p)++;
  }                       
  __VERIFIER_assert(0);    

  return 0;
}

