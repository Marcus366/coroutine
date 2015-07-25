#ifndef __TEST_H__
#define __TEST_H__


typedef struct test_entry_s test_entry_t;
struct test_entry_s {
    void  (*init)(test_entry_t*);
    int   (*func)();

    const char   *name;
    int           code;
    char          log[64];

    struct test_entry_s *next;
};


void test_init(test_entry_t *entry);


#define TEST(__name__)                              \
    int test_##__name__();                          \
    /* declear the test entry */                    \
    test_entry_t test_entry_##__name__ = {          \
        test_init, test_##__name__,                 \
        #__name__, 0, {0}, NULL                     \
    };                                              \
    int test_##__name__()


#define ASSERT(boolexpr, abortstr)              \
    if (!(boolexpr)) {                          \
        exit(-1);                               \
    }


#endif

