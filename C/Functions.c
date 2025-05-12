/*
# Names: Elijah Atienza, Dylan Burges
# Description: This program will test the fundamentals of binary text file input and output using only system calls
# Date: 5/11/2025
*/
#pragma warning(disable : 4996)

#include <unistd.h>  // linux I/O header
#include <fcntl.h>  // flag header header
#include <sys/stat.h>  // user permission modes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Song.h"

// From song.h, truncated name for brevity
int FBS = FIELD_BUFFER_SIZE;

// The function returns -1 if there’s a file IO error, or 0 otherwise. 
int writeSongRecord(char* filename, char* name, char* artist, int length)
{
    // If file does not have in directory
    // then file Songs.bin is created.
    int fd = open("Songs.bin", O_WRONLY | O_APPEND | O_CREAT, 0644);

    if (fd == -1) {
        // print error
        write(1, "file open error\n", 16);
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

    // write name 
    write(fd, nameBuf, FBS);

    // write artist
    write(fd, artistBuf, FBS);

    // write length 
    write(fd, lengthBuf, FBS);
 
    // close file
    int ret = close(fd);
    if (ret == -1) {
        write(1, "file close error\n", 17);
        return -1;
    }
    return 0;
}

// The function returns -1 if there’s a file IO error, or 0 otherwise
int readSongRecords(char* filename)
{
    int fd = open("Songs.bin", O_RDONLY);

    if (fd == -1) {
        // print error
        write(1, "file open error\n", 16);
    }

    char nameBuf[FBS]; 
    char artistBuf[FBS];
    char lengthBuf[FBS];

    // read the binary file
    while(1) {

        // read name
        if (read(fd, nameBuf, FBS) <= 0) {
            break;  // End of file
        }
        write(1, "Name: ", 6);
        write(1, nameBuf, FBS);
        write(1, "\n", 1);

        // read artist
        if (read(fd, artistBuf, FBS) <= 0) {
            break;  // End of file
        }
        write(1, "Artist: ", 8);
        write(1, artistBuf, FBS);
        write(1, "\n", 1);


        // read length
        if (read(fd, lengthBuf, FBS) <= 0) {
            break;  // End of file
        }
        write(1, "Length: ", 8);
        write(1, lengthBuf, FBS);
        write(1, "\n\n", 2);

    }

    // close file
    int ret = close(fd);
    if (ret == -1) {
        write(1, "file close error\n", 17);
        return -1;
    }

    return 0;
}


// The function returns the total time of all songs. 
int totalTimeOfSongs(char* filename)
{
    int fd = open("Songs.bin", O_RDONLY);

    if (fd == -1) {
        // print error
        write(1, "file open error\n", 16);
    }

    int totalT = 0;
    char buf[FBS];  

    // make sure im at beginning of binary file
    lseek(fd, 0, SEEK_SET);

    // Keep reading song records until the end of file
    while (1) {   

        // seek to length val
        if (lseek(fd, FBS * 2, SEEK_CUR) == -1) {
            write(1, "seek error\n", 11);
            break;
        }
        
        // read length val to buffer and parse correctly
        if (read(fd, buf, FBS) <= 0) {
            break;  // End of file or error
        }

        // convert to an int and add
        int val = atoi(buf);

        // add every song time here
        totalT += val;
    }

    // close file
    int ret = close(fd);
    if (ret == -1) {
        write(1, "file close error\n", 17);
        return -1;
    }

    return totalT;
}

// The function returns -1 if there’s a file IO error, 1 if the song is found, or 0 if not found.
int findSong(char* filename, char* songName, Song* song)
{
    int fd = open("Songs.bin", O_RDONLY);

    if (fd == -1) {
        // print error
        write(1, "file open error\n", 16);
    }

    char buf[FBS];
    char artistBuf[FBS];
    char lengthBuf[FBS];

    // make sure im at beginning of binary file
    lseek(fd, 0, SEEK_SET);
    
    // Keep reading song records until the end of file
    while (1) {   

        // read song name to buffer and parse correctly
        if (read(fd, buf, FBS) <= 0) {
            // error reading so break and give seek error
            break;
        }; 

        // parse buffer for song name and compare
        if (strcmp(songName, buf) == 0)
        {
            // song found
            // fill out other buffers then add to song struct

            // read artist
            if (read(fd, artistBuf, FBS) <= 0) {
                break;  // End of file or error
            }

            // read length
            if (read(fd, lengthBuf, FBS) <= 0) {
                break;  // End of file or error
            }

            strcpy(song->name, buf);
            strcpy(song->artist, artistBuf);
            strcpy(song->length, lengthBuf);
            
            // close file
            int ret = close(fd);
            if (ret == -1) {
                write(1, "file close error\n", 17);
                return -1;
            }

            // success
            return 1;
        }

        // seek to length val
        if (lseek(fd, FBS * 2, SEEK_CUR) == -1) {
            write(1, "No more room to search\n", 11);
        }

    }

    // close file
    int ret = close(fd);
    if (ret == -1) {
        write(1, "file close error\n", 17);
        return -1;
    }

    // song not found
    return 0;
}

