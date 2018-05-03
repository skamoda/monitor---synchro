#include "myMonitor.h"
#include "monitor.h"
#include <iostream>

int main()
{
    myMonitor *mon = new myMonitor;

    std::cout << "Hello World" << std::endl;

    pthread_t odd_cons, even_cons, prod1, prod2;

    void *ptr = NULL;

    pthread_create(&prod1, NULL, producer, (void*) mon);
    pthread_create(&prod2, NULL, producer2, (void*) mon);
    pthread_create(&even_cons, NULL, even_consumer, (void*) mon);
    pthread_create(&odd_cons, NULL, odd_consumer, (void*) mon);

    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(even_cons, NULL);
    pthread_join(odd_cons, NULL);

    return 0;
}


