/*
 * File:   Process.cpp
 * Author: <preencher>
 *
 * Created on August 31, 2015, 7:49 AM
 */

#include "Debug.h"
#include "Process.h"
#include "Thread.h"
#include "System.h"
#include "Simulator.h"
#include "Scheduling_Queue.h" // no desespero

Process::Process(unsigned int parentId) {
    this->_entity = Simulator::getInstance()->getEntity(); // simulation purposes only
    this->_entity->setPointer(this); // simulation purposes only
    
    this->_idInfo._processId = Process::getNewId();
    this->_idInfo._parentId = parentId;
    this->_idInfo._priority = Simulator::generate_uniform_distribution(0, 10);
    // random memory info about the new process
    this->_memInfo._segments = new std::list<MemorySegment*>();
    unsigned int numSegments = Simulator::generate_uniform_distribution(2, 5);
    for (unsigned int i = 0; i < numSegments; i++) {
        MemorySegment* segment = new MemorySegment(Simulator::generate_uniform_distribution(250 / (numSegments + 3), 250 / numSegments));
        _memInfo._segments->insert(_memInfo._segments->begin(), segment);
    }    
    // INSERT YOUR CODE HERE
    // ...
}
Process::Process(const Process& orig) {
}

Process::~Process() {
    
}

unsigned int Process::getParentId() const {
    return _idInfo._parentId;
}

unsigned int Process::getId() const {
    return _idInfo._processId;
}

/**
 * Processos chegarão ao sistema quando for invocada a chamada static Process* Process::exec(), que precisa
 * ser implementada. A implementação dessa chamada de sistema deve criar um Process, alocar memória para ele invocando
 * a chamada de sistema Partition* MemoryManagement::alloc(Process* process), inicializar seus atributos, colocá-lo na lista
 * de processos no sistema e criar uma thread (correspondente a "main") para esse processo invocando a chamada
 * static Thread* Thread::thread_create(Process* parent). O metodo deve retornar o processo criado.
 **/
Process* Process::exec() { /*static*/
    Debug::cout(Debug::Level::trace, "Process::exec()");
    // INSERT YOUR CODE HERE (just complete what is missing... easy this time...)

    // criar um Process
    Process* newProcess = new Process(0);

    // alocar memória para ele
    unsigned int size = 0;
    for(std::list<MemorySegment*>::iterator i = newProcess->_memInfo._segments->begin(); i != newProcess->_memInfo._segments->end(); i++)
        size += (*i)->getSize();
    Partition* part = System::memoryManager()->allocateMemory(size);
    newProcess->_memInfo._partition = part;
        
    // inicializar seus atributos
    //

    // colocá-lo na lista de processos no sistema
    Process::getProcessesList()->push_back(newProcess);

    // criar uma thread (correspondente a "main") para esse processo
    Thread::thread_create(newProcess);
    return newProcess;
}

/**
 * Processos serão finalizados quando for invocada a chamada static void Process::exit(int status = 0), que precisa ser
 * implementada. A implementação dessa chamada deve desalocar a memória do processo invocando
 * void MemoryManager::deallocateMemory(Partition* partition), excluir todas as threads desse processo, excluir o processo (destruir
 * o objeto Process), invocar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() e então
 * o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando
 * void Dispatcher::dispatch(Thread* previous, Thread* next).
 **/
void Process::exit(int status) { /*static*/
    Debug::cout(Debug::Level::trace, "Process::exit(" + std::to_string(status) + ")");
    
    if (Thread::running() != nullptr) {
        Process* proc = Thread::running()->getProcess();
        System::memoryManager()->deallocateMemory(proc->_memInfo._partition);
        Process::getProcessesList()->remove(proc);
        // Segmentation fault is a bitch...
        HQueue<Thread*>* threads = new HQueue<Thread*>();
        // criar lista e adicionar elas na lista depois excluir
        for (std::list<Thread*>::iterator it = Thread::getThreadsList()->begin(); it != Thread::getThreadsList()->end(); it++) {
            if ((*it)->getProcess() ==  proc) 
                threads->push((*it));
        }
         
        while (!threads->empty()) {
            Thread *t = threads->pop();
            if (t->_state == Thread::State::READY)
                System::scheduler()->remove(t);
            delete t;
        }
        delete threads;
        delete proc;
    }
    Thread::chooseAndDispatch();
}
