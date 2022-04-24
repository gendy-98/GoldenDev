

#include "serviceFn.h"

char time_counter_checker(char *time, char *serial)
{
    char result = 0;
    u_int32_t card_counter;
    /*
    this fn will return 3 bits 1 for time 2 for counter 3 for the counter more than 1

    */
    if (time[6] == strtol(serial + 15, NULL, 10))
    {
        if (time[5] == strtol(serial + 12, NULL, 10))
        {
            if (time[4] == strtol(serial + 9, NULL, 10))
            {
                if (time[2] == strtol(serial + 18, NULL, 10))
                {
                    if (time[1] <= strtol(serial + 21, NULL, 10))
                    {
                        result |= 1;
                    }
                }
                else if (time[2] < strtol(serial + 18, NULL, 10))
                {
                    result |= 1;
                }
            }
            else if (time[4] < strtol(serial + 9, NULL, 10))
            {
                result |= 1;
            }
        }
        else if (time[5] < strtol(serial + 12, NULL, 10))
        {
            result |= 1;
        }
    }
    else if (time[6] < strtol(serial + 15, NULL, 10))
    {
        result |= 1;
    }
    card_counter = strtol(serial + 24, NULL, 10);
    if (card_counter == 0)
    {
        result |= 2;
    }
    if (card_counter == 1)
    {
        result &= ~6;
    }
    if (card_counter > 1)
    {
        result |= 6;
    }
    return result;
}

void replaceAll(char *str, const char *oldWord, const char *newWord)
{
    char *pos, temp[BUFFER_SIZE];
    int index = 0;
    int owlen;

    owlen = strlen(oldWord);


    /*
     * Repeat till all occurrences are replaced. 
     */
    while ((pos = strstr(str, oldWord)) != NULL)
    {
        // Bakup current line
        strcpy(temp, str);

        // Index of current found word
        index = pos - str;

        // Terminate str after word found index
        str[index] = '\0';

        // Concatenate str with new word 
        strcat(str, newWord);
        
        // Concatenate str with remaining words after 
        // oldword found index.
        strcat(str, temp + index + owlen);
    }
}