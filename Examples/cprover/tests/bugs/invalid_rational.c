int cost;

void func(int n)
{ 
    //for (int i=0; i<n*n;i++){
    //for (int i=0; i<2*(n-1)*(n-1) + 3*(n-1);i++){
    //for (int i=0; i<n*n+n+1;i++){
    //for (int i=0; i<2*(n-1)*(n-1) + 4;i++){ 
    for (int i=0; i<2*(n-1)*(n-1) + 2*(n-1)+4;i++){ // this one gives the Z3 exception "invalid rational..." 
    //for (int i=0; i<2*(n-1)*(n-1) + 3*(n-1)+4;i++){ // this one gives the Z3 exception "invalid rational..."
      cost ++;
    }
}

int main()
{
    cost = 0;
    func(15);
    return 0;
}
