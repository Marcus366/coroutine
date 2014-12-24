#ifndef __COROUTINE_H__
#define __COROUTINE_H__


#include "context.h"
#include "co_structre.h"


/**************     coroutine interface     *********************/


/**
 * Initialize the coroutine library.
 * It must be called before other function in coroutine library,
 * Otherwise, the program will crash with strange error.
 */
void coroutine_init();


/**
 * Create a new coroutine but not yet run it.
 *
 * @param attr: attributes of coroutine (have no meaning at present).
 * @param start_rtn: function pointer run by coroutine once started.
 * @param arg:  argument of start_rtn.
 *
 * @return: a coroutine context if success, NULL if error.
 */
coroutine_ctx_t* coroutine_create(const void *attr,
    void*(*start_rtn)(void*), void *arg);


/**
 * Run a coroutine with given coroutine id.
 * The caller routine will be suspended at once.
 *
 * @param ctx: context of coroutine which you want to run now.
 */
void coroutine_resume(coroutine_ctx_t *ctx);


/**
 * Pause the current coroutine and schedule to anyone if ready.
 */
void coroutine_yield();


/**
 * Return the current running coroutine context.
 *
 * @return: context related to current coroutine.
 */
coroutine_ctx_t* coroutine_running();


/**************     co_operation interface     *********************/


typedef struct co_ipv4_addr_s co_ipv4_addr_t;
typedef struct co_tcp4_s co_tcp4_t;


/**
 * Initialize a ip version4 address structure.
 *
 * @param addr: the address structure need initializing.
 * @param ip: a ip string with format "xxx.xxx.xxx.xxx" or NULL with INADDR_ANY.
 * @param port: a value related to port.
 *
 * @return: 0 means ok.
 */
int co_ipv4_addr_init(co_ipv4_addr_t *addr, const char *ip, unsigned short port);


/* socket interface
size_t co_send(co_socket_t *sock, const void *buf, size_t len, int flags);
ssize_t co_sendto(int sockfd, const void *buf, size_t len, int flags,
    const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t co_sendmsg(int sockfd, const struct msghdr *msg, int flags);
*/


/* tcp interface */
int co_tcp4_open(co_tcp4_t *tcp);
int co_tcp4_bind(co_tcp4_t *tcp, co_ipv4_addr_t *addr);
int co_tcp4_listen(co_tcp4_t *tcp, int backlog);

int co_tcp4_accept(co_tcp4_t *tcp, co_ipv4_addr_t *addr);
int co_tcp4_connect(co_tcp4_t *tcp, co_ipv4_addr_t *addr);

ssize_t co_tcp4_read(co_tcp4_t *tcp, void *buf, size_t count);
ssize_t co_tcp4_write(co_tcp4_t *tcp, const void *buf, size_t count);


#endif

