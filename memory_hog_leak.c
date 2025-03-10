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

    int min_sleep = 10;
    int max_sleep = 15;
    int sleep_time;

    int z_min = 500;
    int z_max = 1000;

    if( argc != 2 )
    {
        printf( "You must provide an IP address!\n" );
        return 1;
    }

    srand( time( 0 ) );

    time1 = time( 0 );

    while( 1 )
    {
        z = ( rand() % ( z_max - z_min + 1 ) ) + z_min;
        s = z + 1;
        matrix = (int**) malloc( z * sizeof( void* ) );
        //printf( "MHL size of z = %d\n", z );

        for( i = 0; i < z; i++ )
        {
            matrix[ i ] = ( int* ) malloc( s * sizeof( int ) );
        }        

        sleep_time = ( rand() % ( max_sleep - min_sleep + 1 ) ) + min_sleep;
        //printf( "MHL leaking memory, then going to sleep for %d seconds\n", sleep_time );

        free( matrix );

        sleep( sleep_time );

        time2 = time( 0 );
        time_diff = time2 - time1;

        //printf( "MHL running time = %d seconds\n", time_diff );      
    }
    return 0;
}
