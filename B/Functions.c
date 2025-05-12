/*
# Names: Elijah Atienza, Dylan Burges
# Description: This program will test the fundamentals of binary text file input and output
# Assignment Letter: B
# Date: 5/11/2025
*/
#pragma warning(disable : 4996)

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
    FILE* file = fopen(filename, "ab");
    if (file == NULL)
    {
        printf("ERROR: File won't open or create.\n");
        return -1;
    }

    char nameBuf[FBS]; 
    char artistBuf[FBS];
    char lengthBuf[FBS];

    // set lengthBuf to 0 then print length var in ASCI to the buffer.
    memset(lengthBuf, 0, FBS);
    snprintf(lengthBuf, sizeof(lengthBuf), "%d", length);

    // once i attach a null terminating character then I can have a buffer of "FBS"
    // size written to the file which looks like what they have in the example binary file

    // add null terminators to correctly parse in reading function
    strncpy(nameBuf, name, sizeof(nameBuf) - 1);
    nameBuf[strlen(name)] = '\0';

    strncpy(artistBuf, artist, sizeof(artistBuf) - 1);
    artistBuf[strlen(artist)] = '\0';

    // write name (the "+ 1" accounts for null terminator)
    fwrite(nameBuf, sizeof(char), FBS, file);

    // write artist
    fwrite(artistBuf, sizeof(char), FBS, file);

    // write length 
    fwrite(lengthBuf, sizeof(char), FBS, file);
 
    // close file
    fclose(file);
    return 0;
}

// The function returns -1 if there’s a file IO error, or 0 otherwise
int readSongRecords(char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("ERROR: File won't open.\n");
        return -1;
    }

    char nameBuf[FBS];
    char artistBuf[FBS];
    char lengthBuf[FBS];

    // read the binary file
    while(1) {

        // read name
        if (fread(nameBuf, sizeof(char), FBS, file) == 0) {
            break;  // End of file or error
        }
        printf("Name: %s\n", nameBuf);

        // read artist
        if (fread(artistBuf, sizeof(char), FBS, file) == 0) {
            break;  // End of file or error
        }
        printf("Artist: %s\n", artistBuf);

        // read length
        if (fread(lengthBuf, sizeof(char), FBS, file) == 0) {
            break;  // End of file or error
        }
        printf("Length: %d\n\n", atoi(lengthBuf));
    }

    fclose(file);

    return 0;
}


// The function returns the total time of all songs. 
int totalTimeOfSongs(char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return -1;
    }

    int totalT = 0;
    char buf[FBS];  

    // make sure im at beginning of binary file
    fseek(file, 0, SEEK_SET);
    

    // Keep reading song records until the end of file
    while (1) {   

        // seek to length val
        if (fseek(file, FBS * 2, SEEK_CUR) != 0) {
            printf("End of line or error detected while seeking\n");
        }
        
        // read length val to buffer and parse correctly
        if (fread(buf, sizeof(char), FBS, file) == 0) {
            break;  // End of file or error
        }

        // convert to an int and add
        int val = atoi(buf);

        // add every song time here
        totalT += val;
    }

    fclose(file);
    return totalT;
}

// The function returns -1 if there’s a file IO error, 1 if the song is found, or 0 if not found.
int findSong(char* filename, char* songName, Song* song)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Err opening file.\n");
        return -1;
    }

    char buf[FBS];
    char artistBuf[FBS];
    char lengthBuf[FBS];

    // make sure im at beginning of binary file
    fseek(file, 0, SEEK_SET);
    
    // Keep reading song records until the end of file
    while (1) {   

        // read song name to buffer and parse correctly
        if (fread(buf, sizeof(char), FBS, file) != FBS) {
            printf("No more room to search for songs\n");
            break;
        }; 

        // parse buffer for song name and compare
        if (strcmp(songName, buf) == 0)
        {
            // song found
            // fill out other buffers then add to song struct

            // read artist
            if (fread(artistBuf, sizeof(char), FBS, file) == 0) {
                break;  // End of file or error
            }

            // read length
            if (fread(lengthBuf, sizeof(char), FBS, file) == 0) {
                break;  // End of file or error
            }

            strcpy(song->name, buf);
            strcpy(song->artist, artistBuf);
            strcpy(song->length, lengthBuf);
            
            fclose(file);

            // success
            return 1;
        }

        // seek to length val
        if (fseek(file, FBS * 2, SEEK_CUR) != 0) {
            printf("End of line or error detected while seeking\n");
        }

    }
    // song not found
    fclose(file);
    return 0;
}

