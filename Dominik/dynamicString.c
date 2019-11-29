#include "dynamicString.h"


void DSInit(DS *dynamicStr)
{
    dynamicStr->str = NULL;
    dynamicStr->maxLength = 0;
    dynamicStr->actIndex = 0;
}


bool DSAddChar(DS *dynamicStr, char character)
{
    if(dynamicStr->maxLength - dynamicStr->actIndex <= 1)
    {
        dynamicStr->maxLength += CHUNK;
        dynamicStr->str = realloc(dynamicStr->str, dynamicStr->maxLength);

        if(!dynamicStr->str)
        {
            return false;
        }
    }

    dynamicStr->str[dynamicStr->actIndex++] = character;
    dynamicStr->str[dynamicStr->actIndex] = '\0';

    return true;
}


bool DSAddStr(DS *dynamicStr, char *str)
{
    while(*str != '\0')
    {
        if(!DSAddChar(dynamicStr, *str))
        {
            return false;
        }

        str++;
    }

    /*unsigned int strLength = strlen(str);

    if(dynamicStr->actIndex + strLength >= dynamicStr->maxLength)
    {
        dynamicStr->maxLength = dynamicStr->actIndex + strLength + 1;
        dynamicStr->str = realloc(dynamicStr->str, dynamicStr->maxLength);

        if(!dynamicStr->str)
        {
            return false;
        }
    }

    strcpy(dynamicStr->str + dynamicStr->actIndex, str);

    dynamicStr->actIndex += strLength;*/

    return true;
}


bool DSCopyStr(DS *dest, DS *src)
{
    if(src->actIndex >= dest->maxLength)
    {
        dest->maxLength = src->actIndex + 1;
        dest->str = realloc(dest->str, dest->maxLength);

        if(!dest->str)
        {
            return false;
        }
    }

    strcpy(dest->str, src->str);

    dest->actIndex = src->actIndex;

    return true;
}


int DSCmpToDS(DS *dynamicStr1, DS *dynamicStr2)
{
    return strcmp(dynamicStr1->str, dynamicStr2->str);
}


int DSCmpToStr(DS *dynamicStr, char *str)
{
    return strcmp(dynamicStr->str, str);
}


char *DSGetStr(DS *dynamicStr)
{
    return dynamicStr->str;
}


unsigned int DSGetLength(DS *dynamicStr)
{
    return dynamicStr->actIndex;
}


void DSClear(DS *dynamicStr)
{
    if(dynamicStr)
    {
        dynamicStr->actIndex = 0;
        dynamicStr->str[0] = '\0';
    }
}


void DSDelete(DS *dynamicStr)
{
    if(dynamicStr)
    {
        free(dynamicStr->str);
        DSInit(dynamicStr);
    }
}
