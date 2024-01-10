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
    pthread_condattr_t condattr_;
    pthread_cond_t* conditionVariable_;
    int id_;
};

#endif