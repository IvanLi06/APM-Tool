#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main( int argc, char* argv[] )
{
    int size;
    char *ip_addr = argv[ 1 ];
    char command[ 50 ];

    int min = 50000;
    int max = 64000;
    
    int min_interval = 10;
    int max_interval = 20;
    int interval;

    int time1;
    int time2;
    int time_diff;
    
    if( argc != 2 )
    {
        printf( "You must provide an IP address!\n" );
        return 1;
    }

    srand( time( 0 ) );

    sleep( 60 );    

    while( 1 ) 
    {
        time1 = time( 0 );
        time_diff = 0;

        size = ( rand() % ( max - min + 1 ) ) + min;
        
        interval = ( rand() % ( max_interval - min_interval + 1 ) ) + min_interval;

        //printf( "BW_burst: This interval will be %d seconds, ping size %d\n", interval, size );

        while( time_diff < interval ) 
        {
            sprintf( command, "ping -c 1 -s %d %s > /dev/null", size, ip_addr );

            system( command );

            sleep( 1 );
            time2 = time( 0 );
            time_diff = time2 - time1;
            //printf( "Time diff = %d\n", time_diff );
        }
        //printf( "BW_burst: Interval was %d seconds\n", time_diff );
        sleep( 45 );
    }
    return 0;
}
