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

// The function returns -1 if there’s a file IO error, or 0 otherwise. 
int writeSongRecord(char* filename, char* name, char* artist, int length)
{
    // open file successfully before writing
    FILE* file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("ERROR: File won't open.\n");
        return -1;
    }
    fprintf(file, "%s\n%s\n%d\n", name, artist, length);
    // close file
    fclose(file);
    return 0;
}

// The function returns -1 if there’s a file IO error, or 0 otherwise
int readSongRecords(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR: File won't open.\n");
        return -1;
    }
    // Buffer for each field
    char nameBuf[FBS];
    char artistBuf[FBS];
    char lengthBuf[FBS];
    // make sure all reads are succesful before continuing
    while(fgets(nameBuf, FBS, file))
    {
        if ( !fgets(artistBuf, FBS, file) ||
             !fgets(lengthBuf, FBS, file))
        {
            break;
        }
        
        nameBuf[strcspn(nameBuf, "\n")] = 0;
        artistBuf[strcspn(artistBuf, "\n")] = 0;
        int length = atoi(lengthBuf);

        printf("Name: %s\nArtist: %s\nLength: %d\n", nameBuf, artistBuf, length);
    }
    
    return 0;
}


// The function returns the total time of all songs. 
int totalTimeOfSongs(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return -1;
    }
    int totalT = 0;
    char buf[FBS];
    

    // Keep reading song records until the end of file
    while (fgets(buf, FBS, file))  // Read name (skip)
    {
        // Read artist (skip)
        if (!fgets(buf, FBS, file)) {
            break;
        }

        // Read length
        if (!fgets(buf, FBS, file)) {
            break;
        }

        // add every song time here
        totalT += atoi(buf);
    }
    fclose(file);
    return totalT;
}

// The function returns -1 if there’s a file IO error, 1 if the song is found, or 0 if not found.
int findSong(char* filename, char* songName, Song* song)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Err opening file.\n");
        return -1;
    }

    char nameBuf[FBS];
    char artistBuf[FBS];
    char lengthBuf[FBS];

    while (fgets(nameBuf, FBS, file))
    {
        // confirm recordis full
        if (!fgets(artistBuf, FBS, file) || !fgets(lengthBuf, FBS, file))
        {
            break;
        }
        // Remove newline characters
        nameBuf[strcspn(nameBuf, "\n")] = 0;
        artistBuf[strcspn(artistBuf, "\n")] = 0;

        // Strcmp for curr vs target song
        if (strcmp(nameBuf, songName) == 0)
        {
            // song found
            strcpy(song->name, nameBuf);
            strcpy(song->artist, artistBuf);
            strcpy(song->length, lengthBuf);
            
            fclose(file);
            // success
            return 1;
        }
    }

    fclose(file);
    return 0;
}

