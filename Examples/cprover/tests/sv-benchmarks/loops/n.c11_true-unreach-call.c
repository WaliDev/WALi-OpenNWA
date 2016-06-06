int main(){
   int a[5];
   unsigned int len=0;

   int i;


   while(__VERIFIER_nondet_bool()){
     
      if (len==4)
         len =0;
      
      a[len]=0;

      len++;
   }
   __VERIFIER_assert(len>=0 && len<5);
   return 1;

   
}

