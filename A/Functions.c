/*
# Names: Elijah Atienza, Dylan Burges
# Description: This proggram will test the fundamentals of text file input and output
# Date: 5/9/2025
*/
#pragma warning(disable : 4996)
//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Song.h"
// From song.h, truncated name for brevity
int FBS = FIELD_BUFFER_SIZE;

int writeSongRecord(char* filename, char* name, char* artist, int length)
{
    // open file successfully before writing
    FILE* file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("ERROR: File won't open.\n");
        return -1;
    }
    printf(file, "%s\n%s\n%d\n", name, artist, length);
    // close file
    fclose(file);
    return 0;
}

int readSongRecords(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR: File won't open.\n");
        return -1;
    }

    char names[FBS];
    char artists[FBS];
    char lengths[FBS];
    return 0;
}



int totalTimeOfSongs(char* filename)
{
    int totalTimeOfAllSongs = 0;

    
    return totalTimeOfAllSongs;
}

int findSong(char* filename, char* songName, Song* song)
{
    int songFound = 0;

    

    return songFound;
}
