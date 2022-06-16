#include <stdio.h>
#include <signal.h>
#include <sys/ucontext.h>

extern void trap_and_check (void);

static void
sig_handler (int sig, siginfo_t *info, void *ucontext)
{
    ucontext_t *uc = ucontext;
    int i;

    for (i = 0; i < 32; i++)
        uc->uc_mcontext.__gregs[i] = 0xdeadbeefdeadbeefUL;

    uc->uc_mcontext.__pc += 4;
}

int
main (int argc, char *argv[])
{
    struct sigaction act = { 0 };

    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_SIGINFO;

    sigaction (SIGTRAP, &act, NULL);

    trap_and_check ();

    return 0;
}
