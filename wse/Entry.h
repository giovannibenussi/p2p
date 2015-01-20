#ifndef _ENTRY_h
#define _ENTRY_h
#include "../glob.h"
#include <iostream>

using namespace std;


/* Class that represents an entry in Rcache or local Cache */

class Entry {

    public:
        long priority;
        string key;
        BIGNUM *hashValue;

        bool isReplicated;
        bool flag;
        int size;

        // new parameters to handle WSE entries
        int version;
        long int timeOut;

        // parameters to handle TTL of entries
        // bool valid;

        int frequency;

        long int getTimeOut() {
            return timeOut;
        }

        ~Entry() {
            BN_free(hashValue);
        }

        Entry(double _priority, string _key, BIGNUM *_hashValue,  int _version, int _size, long int time) {
            priority = _priority;
            key.assign(_key);
            hashValue = BN_dup(_hashValue);
            frequency = 0;
            size = _size;
            flag = true;
            // valid=true;
            isReplicated = false;
            timeOut = time;
            version = _version;
        }

        // when the entry is used is updated (higher priority)
        void update(long timestamps) {
            //      cout << "updateEntry" << endl;
            //     fflush(stdout);
            priority = timestamps;
        }


        void updateFrequency() {
            frequency = frequency + 1;
        }



        // checks it the ttl is still valid
        bool old(long timestamps) {
            if (timestamps > this->timeOut) {
                //   valid = false;
                return true;
            } else
                return false;
        }

        // This simulates that a new entry from the WSE is recovered with a new TTL
        // void updateValidation(long int _timeOut, int _version)
        // {
        //    valid=true;
        //    this->timeOut = _timeOut;
        //    this->version = _version;
        // }

        // helps when sending an entry to other peer
        Entry *clone() {
            Entry *tmp = new Entry(this->priority,
                                   this->key,
                                   this->hashValue,
                                   //     this->timeOut,
                                   this->version,
                                   this->size,
                                   this->timeOut);

            //  tmp->valid = this->valid;
            //tmp->frequency = this->frequency;
            //  timeOut = STATIC_TIMEOUT;
            return tmp;
        }

        void reset() {
            frequency = 0;
        }

};

#endif
