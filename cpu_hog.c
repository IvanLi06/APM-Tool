#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main( int argc, char* argv[] )
{
    volatile unsigned x=0, y=1;

    int time1;
    int time2;
    int time_diff;
    
    int min_sleep = 30;
    int max_sleep = 45;
    int sleep_time;

    int min_run = 60;
    int max_run = 75;
    int run_time;
    
    if( argc != 2 )
    {
        printf( "You must provide an IP address!\n" );
        return 1;
    }

    srand( time( 0 ) );

    sleep_time = ( rand() % ( max_sleep - min_sleep + 1 ) ) + min_sleep;
    run_time = ( rand() % ( max_run - min_run + 1 ) ) + min_run;

    //printf( "CH running for %d seconds\n", run_time );

    time1 = time( 0 );

    while( x++ || y++ )
    {
        time2 = time( 0 );
        time_diff = time2 - time1;

        if( time_diff > run_time )
        {
            //printf( "CH sleeping for %d seconds\n", sleep_time );
            sleep( sleep_time );
            sleep_time = ( rand() % ( max_sleep - min_sleep + 1 ) ) + min_sleep;

            run_time = ( rand() % ( max_run - min_run + 1 ) ) + min_run;
            //printf( "CH running for %d seconds\n", run_time );
            time1 = time( 0 );
        }
    }

    return 0;
}
