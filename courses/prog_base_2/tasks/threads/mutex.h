#ifndef MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED

typedef struct mutex_s mutex_t;

mutex_t * mutex_new(void);
void mutex_free(mutex_t * self);

void mutex_lock(mutex_t * self);
void mutex_unlock(mutex_t * self);

#endif // MUTEX_H_INCLUDED
