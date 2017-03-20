int cost;

void func(int n)
{ 
    //int b=n*n;
    //for (int i=0; i<b;i++){
    for (int i=0; i<n*n;i++){
      cost ++;
    }
}

int main()
{
    cost = 0;
    func(15);
    return 0;
}
