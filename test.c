#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/ucontext.h>

#define ALIGN_UP(addr, align) ((addr + (typeof (addr))align - 1) & ~((typeof (addr))align - 1))
#define __NSIG_WORDS (ALIGN_UP ((_NSIG - 1), ULONG_WIDTH) / ULONG_WIDTH)
#define __NSIG_BYTES (__NSIG_WORDS * (ULONG_WIDTH / UCHAR_WIDTH))

extern void trap_and_check (void);
extern void syscall_and_check (int sysnum, ...);

static pthread_t thread_main;

static void
sig_handler (int sig, siginfo_t *info, void *ucontext)
{
    ucontext_t *uc = ucontext;
    int i;

    for (i = 0; i < 32; i++)
        uc->uc_mcontext.__gregs[i] = 0xdeadbeefdeadbeefUL;

    uc->uc_mcontext.__pc += 4;
}

static void *
thread_handler (void *data)
{
    usleep (1000);

    pthread_kill (thread_main, SIGTRAP);
}

static void
sigreturn_test (void)
{
    trap_and_check ();
}

static void
sigsuspend_test (void)
{
    pthread_t thread_emiter;
    sigset_t mask;

    sigemptyset (&mask);

    thread_main = pthread_self ();
    pthread_create (&thread_emiter, NULL, thread_handler, NULL);

    syscall_and_check (__NR_rt_sigsuspend, &mask, __NSIG_BYTES);
}

static void
read_test (void)
{
    pthread_t thread_emiter;
    char buf[512];

    thread_main = pthread_self ();
    pthread_create (&thread_emiter, NULL, thread_handler, NULL);

    syscall_and_check (__NR_read, 0, buf, sizeof (buf));
}

int
main (int argc, char *argv[])
{
    struct sigaction act = { 0 };

    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction (SIGTRAP, &act, NULL);

    if (argc < 2) {
        fprintf (stderr, "%s [ sigreturn | sigsuspend | read ]\n", argv[0]);
        return -1;
    }

    if (strcmp (argv[1], "sigreturn") == 0) {
        sigreturn_test ();
    } else if (strcmp (argv[1], "sigsuspend") == 0) {
        sigsuspend_test ();
    } else {
        read_test ();
    }

    return 0;
}
