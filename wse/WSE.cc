#include "WSE.h"

void WSE::inner_body(void) {

    unsigned int number_of_queries = 0;
    while (1) {
        if (queue_in.empty() )
            passivate();
        // ASSERT(!queue_in.empty());

        cout << "------------ NUEVO MENSAJE EN WSE" << endl;

        Message *mes = message_stack.back();
        MessageWSE *m = mes->GetMessagePointer();

        // MessageWSE *m = queue_in.front();

        // queue_in.pop_front();
        message_stack.pop_back();

        m->setAnswer(WSE::getVersion(m->getKey(), m->getQuery()));

        cout << "............. WSE -> PEER" << endl;
        this->SendMessage(new Message(this->GetId(), this->GetType(), mes->GetIdFrom(), mes->GetTypeFrom(), time(), m));

        /*switch ( m->source ) {
            case PEER:

                //           m->setAnswer(WSE::getVersion(m->getQuery()));

                m->setAnswer(WSE::getVersion(m->getKey(), m->getQuery()));

                (*(m->src))->receiveWSE(m);


                if ( (*(m->src))->idle() && !(*(m->src))->get_busy() )
                    (*(m->src))->activateAfter( current() );
                break;
            case USER:
                //chequea en la Rcache si esta la query, sino la inserta

                //     m->setAnswer(WSE::getVersion(m->getQuery()));
                cout << "Antes:   " << *(m->getQuery()) << endl;
                m->setAnswer(WSE::getVersion(m->getKey(), m->getQuery()));
                this->SendMessage(a);
                cout << "Despues: " << *(m->getQuery()) << endl;

                cout << "USER QUERY WSE" << endl;
                break;

            default: cout << "ERROR: WSE Type o msg" << endl;
        }*/
        number_of_queries++;
        if(number_of_queries % INTERVALO_MEDIR_CACHE_HITS == 0){
            // cout << (double)this->cache_hits << endl;
        }
    }
}

int WSE::getVersion(string s, BIGNUM * b)
{
    int ttl;
    Answer * a = ANSWERS->check(s);
    if (a != NULL)
    {
        // cout << "!= NULL" << endl;
        if (a->getTimeOut() < this->time())
        {
            cout << "\tHIT" << endl;
            this->cache_hits++;
            ttl = (this->time()) + TTL_WSE;
            // TTL es igual a new random number
            a->setNewVersion(ttl);
            //version +1
        }
    }
    else
    {
        // TTL es igual a new random number y version = 1
        ttl = (this->time()) + TTL_WSE;
        Answer * tmp = new Answer(this->time(), s, b, ttl, 1, SIZE_ANSWER);
        a = tmp;
        ANSWERS->insertAnswer(tmp);
    }
    return a->getVersion();
}


// returns the version of the query answer
/*
int WSE::getVersion(int* b)
{
int ttl;


   Answer* a= ANSWERS->check(b);

   if (a!=NULL)
   {
        if(a->getTimeOut()<this->time())
    {
            ttl = (this->time())+ randomTTL();
             // TTL es igual a new random number
            a->setNewVersion(ttl);
            //version +1
        }
  }
  else
  {
     // TTL es igual a new random number y version = 1
     ttl= (this->time()) +randomTTL();
     Answer *tmp=new Answer(this->time(),  b,ttl,1,SIZE_ANSWER);
     a=tmp;
     ANSWERS->insertAnswer(tmp);
  }

 return a->getVersion();
}
*/
// TODO
long int WSE::randomTTL() {
    long int r = rand();

    return r;
}
