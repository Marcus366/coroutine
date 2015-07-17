#ifndef __COROUTINE_H__
#define __COROUTINE_H__


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "context.h"


/**************     crt interface     *********************/


/**
 * Initialize the crt library.
 *
 * It must be called before other functions in crt library.
 * Otherwise, the program will crash with unknown error.
 */
void crt_init();


/**
 * Create a new crt but not yet run it.
 *
 * @param attr: attributes of crt (have no meaning at present).
 * @param start_rtn: function pointer run by crt once started.
 * @param arg:  argument of start_rtn.
 *
 * @return: a crt context if success, NULL if error.
 */
crt_ctx_t* crt_create(const void *attr,
    void(*start_rtn)(void*), void *arg);


/**
 * Run a crt with given crt context.
 * The caller routine will be suspended at once.
 *
 * @param ctx: context of crt which you want to run now.
 */
void crt_resume(crt_ctx_t *ctx);


/**
 * Pause the current crt and schedule to anyone if ready.
 */
void crt_yield();


/**
 * Return the current running crt context.
 *
 * @return: context related to current crt.
 */
crt_ctx_t* crt_running();


/**************     co_operation interface     *********************/


/**
 * Initialize a ip version4 address structure.
 *
 * @param addr: the address structure need initializing.
 * @param ip: a ip string with format "xxx.xxx.xxx.xxx" or NULL with INADDR_ANY.
 * @param port: a value related to port.
 *
 * @return: 0 means ok.
 */
int co_ip4_addr_init(struct sockaddr_in *addr, const char *ip, unsigned short port);
int co_ip6_addr_init(struct sockaddr_in6 *addr, const char *ip, unsigned short port);


/* socket interface
size_t co_send(co_socket_t *sock, const void *buf, size_t len, int flags);
ssize_t co_sendto(int sockfd, const void *buf, size_t len, int flags,
    const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t co_sendmsg(int sockfd, const struct msghdr *msg, int flags);
*/


/* tcp interface */
/*
int co_tcp4_open(co_tcp_t *tcp);
int co_tcp6_open(co_tcp_t *tcp);

int co_tcp_bind(co_tcp_t *tcp, const struct sockaddr *addr);
int co_tcp_listen(co_tcp_t *tcp, int backlog);
int co_tcp_accept(co_tcp_t *tcp, struct sockaddr *addr, socklen_t *len);

int co_tcp_connect(co_tcp_t *tcp, struct sockaddr *addr);

ssize_t co_tcp_read(co_tcp_t *tcp, void *buf, size_t count);
ssize_t co_tcp_write(co_tcp_t *tcp, const void *buf, size_t count);
*/


/* file interface */
int co_file_open(const char *pathname, int flags, ... /* mode_t */);

ssize_t co_read(int fd, void *buf, size_t count);
ssize_t co_write(int fd, const void *buf, size_t count);

ssize_t co_pread(int fd, void *buf, size_t count, off_t offset_t);
ssize_t co_pwrite(int fd, void *buf, size_t count, off_t offset_t);


/* pipe interface */
int co_pipe_open(int pipefd[2]);
int co_pipe2_open(int pipefd[2], int flags);


#endif

