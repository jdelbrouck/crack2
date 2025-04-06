#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings

//Trim a trailing newline char if found
void trim(char str[])
{
    int len = strlen(str);
    if (str[len-1] == '\n')
    {
        str[len-1] = '\0';
    }
}

// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!
char * tryWord(char * plaintext, char * hashFilename)
{
    // Hash the plaintext
    char *hashtext = md5(plaintext, strlen(plaintext));

    // Open the hash file
    FILE *in = fopen(hashFilename, "r");
    if (!in)
    {
        printf("Can't open %s for reading\n", hashFilename);
        exit(1);
    }

    // Loop through the hash file, one line at a time.
    char hashline[HASH_LEN];
    int matchfound = 0;
    while (fgets(hashline, HASH_LEN, in) != NULL)
    {
        // Attempt to match the hash from the file to the
        // hash of the plaintext.

        if (!strcmp(hashtext, hashline))
        {
            matchfound = 1;
        }
    }

    // If there is a match, you'll return the hash.
    // If not, return NULL.

    // Before returning, do any needed cleanup:
    //   Close files?
    fclose(in);
    //   Free memory?
    // This breaks the ftn, so I'm assuming it makes more sense to treat this like md5 ftn and free it in the body
    //free(hashtext);

    // Modify this line so it returns the hash
    // that was found, or NULL if not found.
    if (matchfound == 1)
    {
        return hashtext;
    }
    else
    {
        free(hashtext); //not passing this out of the function so free it here
        return NULL;
    }
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // These two lines exist for testing. When you have
    // tryWord working, it should display the hash for "hello",
    // which is 5d41402abc4b2a76b9719d911017c592.
    // Then you can remove these two lines and complete the rest
    // of the main function below.
    //char *found = tryWord("hello", "hashes00.txt");
    //printf("%s %s\n", found, "hello");


    // Open the dictionary file for reading.
    FILE *in = fopen(argv[2], "r");
    if (!in)
    {
        fprintf(stderr, "Can't open %s for reading\n", argv[2]);
        exit(1);
    }

    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hash_file.
    char word[PASS_LEN];
    int cracks = 0;
    while (fgets(word, PASS_LEN, in) != NULL)
    {
        trim(word); //remove the newline character pulled in by fgets if applicable
        char *match = tryWord(word, argv[1]);

        // If we got a match, display the hash and the word. For example:
        //   5d41402abc4b2a76b9719d911017c592 hello

        if (match)
        {
            printf("%s %s\n", match, word);
            cracks++;
        }
        
        free(match); //free this before we lose the address when the loop resets
    }
    
    // Close the dictionary file.
    fclose(in);

    // Display the number of hashes that were cracked.
    printf("%d passwords cracked\n", cracks);
    
    // Free up any malloc'd memory? // handled inside the loop/functions
}

