int main()
{
  unsigned int N_LIN=1;
  unsigned int N_COL=1;      
  unsigned int j,k;
  int matriz[1][1], maior;			// replaced N_LIN and N_COL with 1 and 1
  
  maior = __VERIFIER_nondet_int();
  for(j=0;j<N_COL;j++)
    for(k=0;k<N_LIN;k++)
    {       
       matriz[j][k] = __VERIFIER_nondet_int();
       
       if(matriz[j][k]>=maior)
          maior = matriz[j][k];                          
    }                       
    
  __VERIFIER_assert(matriz[0][0]<=maior);    

  return 0;
}

