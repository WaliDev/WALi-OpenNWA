void main()
{
    int x=rand();
    int y=rand();

    if (y>0) {
	while(x<100) {
	    x=x+y;
	}
    }
           
    assert(y<=0 || (y>0 && x>=100));    
}


