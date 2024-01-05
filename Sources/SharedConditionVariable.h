#ifndef _CONDITIONVARIABLE_
#define _CONDITIONVARIABLE_

#include <string>

class SharedConditionVariable
{
public:
    SharedConditionVariable();
    ~SharedConditionVariable();

    pthread_cond_t* get();

private:
    pthread_cond_t* conditionVariable_;
    int id_;
};

#endif