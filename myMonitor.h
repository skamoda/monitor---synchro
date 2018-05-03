#include "monitor.h"
#include <iostream>
#include <queue>

#define BUFFER_MAX_SIZE 9
#define BUFFER_MIN_SIZE 3

using namespace std;

class myMonitor : Monitor
{
private:
	deque<int> buffer;

	int prod_waiting, even_waiting, odd_waiting;
	Condition produce, even, odd;

public:
	myMonitor();
	void prod(int);
	void even_cons();
	void odd_cons();
};

myMonitor::myMonitor()
{
	prod_waiting = 0;
	even_waiting = 0;
	odd_waiting = 0;
}

myMonitor monitor;

void *producer(void *args)
{
//watek producera - dodaje i usypia

    srand(time(NULL));
	while(1)
	{
        int elem1 = rand() % 50 + 1;
		monitor.prod(elem1);
		sleep(rand()%5 + 1);
	}
}

void *producer2(void *args)
{
//watek producera - dodaje i usypia
    srand(time(NULL));
	while(1)
	{
        int elem2 = rand() % 50 + 1;
		monitor.prod(elem2);
		sleep(2);
	}
}

void *even_consumer(void *args)
{
//watek konsumenta - konsumuje(usuwa) i  usypia
    while(1)
    {
        monitor.even_cons();
        sleep(2);
    }
}

void *odd_consumer(void *args)
{
//watek konsumenta - konsumuje(usuwa) i  usypia
    while(1)
    {
        monitor.odd_cons();
        sleep(rand()%5 + 1);
    }
}

//cialo funkcji producenta
void myMonitor::prod(int element)
{
	enter();

	if(buffer.size() >= BUFFER_MAX_SIZE)
	{
		++prod_waiting;
		wait(produce);
		--prod_waiting;
	}

	//produkcja
	//srand(time(NULL));
	//int elem = rand() % 50 + 1;
	buffer.push_back(element);

	cout << "Elements in buffer: " << endl;
	for(int i = 0; i < buffer.size(); i++)
	{

        cout << buffer.at(i) << endl;
    }

	if(buffer.size() < BUFFER_MAX_SIZE && prod_waiting > 0)
	{
		signal(produce);
	}
	else if(buffer.size() >= BUFFER_MIN_SIZE && (buffer.front() % 2) == 0 && even_waiting > 0)
	{
		signal(even);
	}
	else if(buffer.size() >= BUFFER_MIN_SIZE && (buffer.front() % 2) == 1 && odd_waiting > 0)
	{
		signal(odd);
	}

	leave();
}

//cialo funkcji konsumenta parzystego
void myMonitor::even_cons()
{
    enter();

    if(buffer.size() < BUFFER_MIN_SIZE)
    {
        ++even_waiting;
        wait(even);
        --even_waiting;
    }
    if(buffer.size() >= BUFFER_MIN_SIZE && (buffer.front() % 2) == 1)
    {
        signal(odd);
        wait(even);
    }


    cout << "Parzysty konsumuje: " << buffer.front() << endl;
    buffer.erase(buffer.begin());

    if(buffer.size() < BUFFER_MAX_SIZE)
    {
        signal(produce);
    }

    if(buffer.size() >= BUFFER_MIN_SIZE && (buffer.front() % 2) == 1)
    {
        signal(odd);
    }

    leave();
}

//cialo funkcji konsumenta nieparzystego
void myMonitor::odd_cons()
{
    enter();

    if(buffer.size() < BUFFER_MIN_SIZE)
    {
        ++odd_waiting;
        wait(odd);
        --odd_waiting;
    }
    if(buffer.size() >= BUFFER_MIN_SIZE && (buffer.front() % 2) == 0)
    {
        signal(even);
        wait(odd);
    }

    cout << "Nieparzysty konsumuje: " << buffer.front() << endl;
    buffer.erase(buffer.begin());

    if(buffer.size() < BUFFER_MAX_SIZE)
    {
        signal(produce);
    }

    if(buffer.size() >= BUFFER_MIN_SIZE && (buffer.front() % 2) == 0)
    {
        signal(even);
    }

    leave();
}
