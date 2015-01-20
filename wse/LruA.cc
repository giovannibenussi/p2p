#include "../auxiliar/Util.h"
#include "LruA.h"


Answer * LRUA::check(string hashValue) {
    for (cache_entry_pointer = cache.begin(); cache_entry_pointer != cache.end(); cache_entry_pointer++) {
        if ( ((*cache_entry_pointer)->getKey()).compare(hashValue) == 0) {
            (*cache_entry_pointer)->update(timestamps);
            return (*cache_entry_pointer);
        }
    }
    return NULL;
}

/*Answer * LRUA::check(string hashValue)
{
    list<Answer *>::iterator iter;
    for (iter = cache.begin(); iter != cache.end(); iter ++)
    {
        string tmp = (*iter)->getKey();
        if (tmp.compare(hashValue) == 0)
        {
            (*iter)->update(timestamps);
            return (*iter);
        }
    }
    return NULL;
}*/

void LRUA::cacheErase(Answer * a) {
    vector<Answer *>::iterator iter;
    for (iter = cache.begin(); iter != cache.end(); iter ++) {
        string tmp = (*iter)->getKey();
        if (tmp.compare(a->key) == 0) {
            cache.erase(iter);
            break;
        }
    }
}

void LRUA::insertAnswer(Answer * a) {
    if (!this->isFreeSpace (a->size)) {
        this->createSpace(a->size);
    }

    ptr = a;
    pq.push(ptr);
    freeCache -= a->size;
    cache.push_back( a);
}

bool LRUA::isFreeSpace( int size ) {
    if ( freeCache >= size )
        return true;
    return false;
}

//---------------------------------------------------

void LRUA::createSpace( int size ) {
    do {
        ptr = pq.top();
        pq.pop();
        freeCache += ptr->size;
        cacheErase( ptr );
        delete ptr ;
    } while ( freeCache < size );
}

