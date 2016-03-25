#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

// types
typedef struct thread_s thread_t;
typedef void * (*thread_func_t)(void *);

// helper functions
thread_t * thread_getCurrentThread(void);
void thread_exit(void * retval);

// constructors
thread_t * thread_create(thread_func_t func);
thread_t * thread_create_args(thread_func_t func, void * args);
// destructor
void thread_free(thread_t * self);

// methods
int thread_join(const thread_t * self);
int thread_join_result(const thread_t * self, void ** result);
int thread_terminate(const thread_t * self);
int thread_equals(const thread_t * self, const thread_t * obj);

#endif // THREAD_H_INCLUDED
