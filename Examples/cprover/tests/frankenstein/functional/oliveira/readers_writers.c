
/* generalization of the readers-writers problem, by Sankaranarayanan */

void readers_writers(int c1,int c2, int k0)
pre( c1 >= 0 && c2 >= 0 && k0 >=0 ); 
    {
    int r,w,k;

    r = 0;
    w = 0;
    k = k0;

    inv( r*c1 + w*c2 + k == k0 && r*w==0 );
    while( true ) 
        { 
        if ( w==0 )
            { 
            r = r+1;
            k = k-c1;
            }
        [] ( r==0 )
            { 
            w = w+1;
            k = k-c2;
            }
        [] ( w==0 )
            { 
            r = r-1;
            k = k+c1;
            }
        [] ( r==0 )
            {
            w = w-1;
            k = k+c2;
            }
        } 
    }

post( true );
