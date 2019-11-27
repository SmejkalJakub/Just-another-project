#include "symtable.h"


static unsigned long hashFunction(const char *str)
{
    unsigned long h=0;
    const unsigned char *p;

    for(p=(const unsigned char*)str; *p!='\0'; p++)
    {
        h = 65599*h + *p;
    }

    return h % ST_SIZE;
}


void STInit(symTable *STptr)
{
    for(int i = 0; i < ST_SIZE; i++)
    {
        (*STptr)[i] = NULL;
    }
}


symTableItem *STSearch(symTable* STptr, char *key)
{
    for(symTableItem* item = (*STptr)[hashFunction(key)]; item; item = item->next)
    {
        if(strcmp(item->key, key) == 0)
        {
            return item;
        }
    }

    return NULL;
}


symTableItem *STInsert(symTable* STptr, char *key)
{
    symTableItem *newItem = malloc(sizeof(symTableItem));

    if(!newItem)
    {
        return NULL;
    }

    if(!(newItem->key = malloc(strlen(key) + 1)))
    {
        free(newItem);
        return NULL;
    }

    if(!(newItem->params = malloc(sizeof(DS))))
    {
        free(newItem->key);
        free(newItem);
        return NULL;
    }

    DSInit(newItem->params);

    strcpy(newItem->key, key);

    newItem->type = TYPE_NONE;
    newItem->global = false;

    unsigned long index = hashFunction(key);

    newItem->next = (*STptr)[index];
    (*STptr)[index] = newItem;

    return newItem;
}


bool STAddParam(symTableItem *item, int type)
{
    if(type == INT)
    {
        if(!DSAddChar(item->params, 'i'))
        {
            return false;
        }
    }
    else if(type == DOUBLE)
    {
        if(!DSAddChar(item->params, 'f'))
        {
            return false;
        }
    }
    else if(type == STRING)
    {
        if(!DSAddChar(item->params, 's'))
        {
            return false;
        }
    }
    else if(type == BOOL)
    {
        if(!DSAddChar(item->params, 'b'))
        {
            return false;
        }
    }

    return true;
}


void STDeleteItem(symTable *STptr, char *key)
{
    unsigned long index = hashFunction(key);

    symTableItem *actItem = (*STptr)[index];
    symTableItem *prevItem = NULL;

    while(actItem)
    {
        if(strcmp(actItem->key, key) == 0)
        {
            if(prevItem)
            {
                prevItem->next = actItem->next;
            }
            else
            {
                (*STptr)[index] = actItem->next;
            }

            free(actItem->key);
            DSDelete(actItem->params);
            free(actItem->params);
            free(actItem);
        }
    }
}


void STDelete(symTable *STptr)
{
    symTableItem *delItem;
    symTableItem *actItem;

    for(int i = 0; i < ST_SIZE; i++)
    {
        actItem = ((*STptr)[i]);

        while(actItem)
        {
            delItem = actItem;
            actItem = actItem->next;

            free(delItem->key);
            DSDelete(delItem->params);
            free(delItem->params);
            free(delItem);
        }
    }
}
