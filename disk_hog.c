#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main( int argc, char* argv[] )
{
    int j;
    int num;
    FILE *outfile;
    
    int min = 500000;
    int max = 1000000;    
    
    int time1;
    int time2;
    int time_diff;
    
    if( argc != 2 )
    {
        printf( "You must provide an IP address!\n" );
        return 1;
    }

    srand( time( 0 ) );

    outfile = fopen( "/tmp/tmp.txt", "w" );
    fclose( outfile );

    time1 = time( 0 );

    sleep( 60 );

    while( 1 ) 
    {
        outfile = fopen( "/tmp/tmp.txt", "aw" );

        num = ( rand() % ( max - min + 1 ) ) + min;

        for( j = 0; j < num; j++ )
        {
            fprintf( outfile, "I will build a great file!\n" );
        }

        fclose( outfile );
        
        sleep( 1 );
        time2 = time( 0 );
        time_diff = time2 - time1;

        //if( time_diff % 60 == 0 )
        //    printf( "DH running for %d seconds\n", time_diff );

        if( time_diff > 840 )
        {
            system( "rm /tmp/tmp.txt" );
            //printf( "DH leaving while loop\n" );
            break;
        }
    }

    while( 1 );
    return 0;
}
