#ifndef __COROUTINE_H__
#define __COROUTINE_H__


typedef long coroutine_t;


/**
 * Create a new coroutine but not yet run it.
 *
 * @cidp: an out argument of attached coroutine id if success.
 * @attr: attributes of coroutine (have no meaning at present).
 * @start_rtn: function pointer run by coroutine once started.
 * @arg:  argument of start_rtn.
 *
 * @return: 0 if success, 1 if error.
 */
int coroutine_create(coroutine_t *cidp, const void *attr,
    void*(*start_rtn)(void*), void *arg);


/**
 * Run a coroutine with given coroutine id.
 * The caller routine will be suspended at once.
 *
 * @cid: id of coroutine which you want to run now.
 */
void coroutine_resume(coroutine_t cid);


/**
 * Pause the current coroutine.
 */
void coroutine_yield();


coroutine_t coroutine_self();


int coroutine_equal(coroutine_t lhs, coroutine_t rhs);


#endif

