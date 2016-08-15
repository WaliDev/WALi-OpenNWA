int main()
{
  unsigned int N_LIN=__VERIFIER_nondet_uint();
  unsigned int N_COL=__VERIFIER_nondet_uint();
  unsigned int j,k;
  int matriz[100][100], maior;			// replaced N_LIN and N_COL with 100 and 100
  
  maior = __VERIFIER_nondet_int();
  for(j=0;j<N_COL;j++)
    for(k=0;k<N_LIN;k++)
    {       
       matriz[j][k] = __VERIFIER_nondet_int();
       
       if(matriz[j][k]>maior)
          maior = matriz[j][k];                          
    }                       
    
  for(j=0;j<N_COL;j++)
    for(k=0;k<N_LIN;k++)
      __VERIFIER_assert(matriz[j][k]<maior);    
}

