/*
 * File:   Scheduling_Queue.h
 * Author: cacian
 *
 * Created on October 9, 2015, 9:34 PM
 */

#ifndef SCHEDULING_QUEUE_H
#define	SCHEDULING_QUEUE_H

#include <queue>

#include "Thread.h"
/**************************************************************
 *               Custom Queue BEGIN                           *
 **************************************************************/

#define E_EMPTY_QUEUE -1

template <typename T>
class Element {
	private:
		T data; // this should be a pointer but I have no time and is too risky to change this now
		Element<T>* next;
		Element<T>* prior;
	public:
		Element(T& data, Element<T>* next, Element<T>* prior): data(data), next(next), prior(prior) { }
		Element(T& data): data(data), next(nullptr), prior(nullptr) { }
		virtual ~Element() {
			if (this->next != nullptr)
				this->next->setPrior(this->prior);
			if (this->prior != nullptr)
				this->prior->setNext(this->next);	
		} 
		T getData() {
			return this->data;	
		}
		void setData(T &data) {
			this->data = data;
		}
		Element<T>* getNext() {
			return this->next;	
		}
		void setNext(Element<T>* next) {
			this->next = next;
		}
		Element<T>* getPrior() {
			return this->prior;	
		}
		void setPrior(Element<T>* prior) {
			this->prior = prior;
		}
};

template <typename T>
class HQueue {
	protected:
		Element<T>* head;
		Element<T>* tail;
		unsigned int size;
		/* This method can be overridden in subclasses, 
		 * thus making it possible to insert elements in a different order.
		 */
		virtual Element<T>* insert(T& data) {
			return nullptr;// always insert in the end of the queue
		}
		
	public:
		HQueue() {
			this->head= nullptr;
			this->tail= nullptr;
			this->size= 0;	
		}
        virtual ~HQueue() {
            Element<T>* i = this->head;
            while (i != this->tail) {
                i= i->getNext();
            }
            delete this->tail;
        }
        T top () const {
            return this->head->getData();
        }
		unsigned int getSize() {
			return this->size;
		}
		T remove (T& item) {
			Element<T>* i= this->head;
			T result, a;
			while (i != nullptr) {
				a = i->getData();
				if (a == item) {
					result = i->getData();
					if (i == this->head)
						this->head = i->getNext(); 
					if (i == this->tail)
						this->tail = i->getPrior();
					this->size--;
					delete i;
					return result;
				}
				i = i->getNext();
			}
			return nullptr;
		}
		void push(T &data) {
			Element<T>* e = new Element<T>(data);
			if (this->size == 0) {
				this->head = e;
				this->tail = e;
			}			
			else {
				Element<T>* next = insert(data);
				if (next == nullptr) {
					e->setPrior(this->tail);
					this->tail->setNext(e);
					this->tail = e;	
				} else {
					if (next == this->head)
						this->head = e;
					e->setNext(next);
					e->setPrior(next->getPrior());
					next->setPrior(e);
					if (e->getPrior() != nullptr)
						e->getPrior()->setNext(e);
				}
			}
			this->size++;
		}
		T pop () {
			if (this->size == 0)
				throw E_EMPTY_QUEUE;
			T r= this->head->getData();
			Element<T>* h = this->head;
			this->head= h->getNext();
			delete h; 
			this->size--; 
			if (size == 0)
				this->tail = nullptr; 
			return r;
		}
		bool empty() {
			return this->size == 0;
		}		
};
template<typename T>
class OrderedQueue: public HQueue<T> {
	protected:
		Element<T>* insert(T& data) {
			Element<T>* e = HQueue<T>::head;
			if (data > HQueue<T>::tail->getData())
				return nullptr;
			while ((e != nullptr) && (data > e->getData())) {
				e = e->getNext();
			}
			return e;
		}
		
	public:
		OrderedQueue(): HQueue<T>() {}
};
/**************************************************************
 *               Custom Queue END                             *
 **************************************************************/

/**
 * I chose to use a custom queue because it's easier to sort it, in this case I'm sorting  
 * the queue as items are inserted in it, which gives me O(n-1) in the worst case  
 */
class Scheduling_Queue : private OrderedQueue<Thread*> { // private because I don't wanna mess the pre existent methods
public:
    Scheduling_Queue(): OrderedQueue<Thread*>() {}
    Scheduling_Queue(const Scheduling_Queue& orig) {}
    virtual ~Scheduling_Queue();
public:
    bool empty();
    void insert(Thread* t);
    Thread* remove(Thread* t);
    Thread* pop(); // remove the top (head) element
    Thread* top(); // returns the top (head) element
private:
    // INSERT YOUR CODE HERE
    // ...
    //std::list<Thread*>* _queue; /* change or delete if you want. It is just a suggestion */
};

#endif	/* SCHEDULING_QUEUE_H */

