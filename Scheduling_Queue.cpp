/*
 * File:   Scheduling_Queue.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 9:34 PM
 */

#include "Scheduling_Queue.h"
/*
Scheduling_Queue::Scheduling_Queue() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    _queue = new std::list<Thread*>();
}

Scheduling_Queue::Scheduling_Queue(const Scheduling_Queue& orig) {
}
*/
Scheduling_Queue::~Scheduling_Queue() {
}

bool Scheduling_Queue::empty() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    return OrderedQueue<Thread*>::empty();
}

Thread* Scheduling_Queue::pop() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    if (!OrderedQueue<Thread*>::empty())
        return OrderedQueue<Thread*>::pop();
    return nullptr;
}

Thread* Scheduling_Queue::remove(Thread* t) {
    return OrderedQueue<Thread*>::remove(t);
}

Thread* Scheduling_Queue::top() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    return OrderedQueue<Thread*>::top();  //(*_queue->begin());
}

void Scheduling_Queue::insert(Thread* t) {
    // INSERT YOUR CODE HERE
    OrderedQueue<Thread*>::push(t);
}
