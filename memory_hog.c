#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main( int argc, char* argv[] )
{
    int z;
    int s;
    int **matrix;
    int i, j;

    int time1;
    int time2;
    int time_diff;

    int min_sleep = 60;
    int max_sleep = 90;
    int sleep_time;

    int min_run = 30;
    int max_run = 45;
    int run_time;

    int z_min = 30000;
    int z_max = 50000;

    if( argc != 2 )
    {
        printf( "You must provide an IP address!\n" );
        return 1;
    }

    srand( time( 0 ) );

    while( 1 )
    {
        z = ( rand() % ( z_max - z_min + 1 ) ) + z_min;
        s = z + 1;
        matrix = (int**) malloc( z * sizeof( void* ) );
        //printf( "MH size of z = %d\n", z );

        for( i = 0; i < z; i++ )
        {
            matrix[ i ] = ( int* ) malloc( s * sizeof( int ) );
        }

        run_time = ( rand() % ( max_run - min_run + 1 ) ) + min_run;
        //printf( "MH grabbed memory, processing for %d seconds\n", run_time );

        time1 = time( 0 );
        time2 = time( 0 );
        time_diff = time2 - time1;

        while( time_diff < run_time )
        {
            time2 = time( 0 );
            time_diff = time2 - time1;
        }            

        sleep_time = ( rand() % ( max_sleep - min_sleep + 1 ) ) + min_sleep;
        //printf( "MH freeing memory, then going to sleep for %d seconds\n", sleep_time );

        for( i = 0; i < z; i++ )
        {
            free( matrix[ i ] );
        }

        free( matrix );

        sleep( sleep_time );        
    }
    return 0;
}
