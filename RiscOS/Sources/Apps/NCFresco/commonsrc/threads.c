/* -*-c-*- */

#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>

#include "kernel.h"

#include "threads.h"
#include "jmpbuf.h"

#include "memwatch.h"
#include "myassert.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef STBWEB_ROM
#define STBWEB_ROM 0
#endif

/* Cross-compilation of this is too machine specific. */
#ifdef RISCOS

extern void sutil_get_relocation_offsets(int *stack_limit);

#define STACK_LIMIT_OFFSET	0x230
#define STACK_MAGIC		0xf60690ff
static jmp_buf return_point;
static thread running_thread = 0;

static int tmp_argc;
static char **tmp_argv;
static main_function tmp_mf;

extern void thread_exit(int rc)
{
    IMGDBG(("thrd%p: in thread_exit\n", running_thread));
    running_thread->rc = rc;
    running_thread->halt_point = "<DEAD via exit>";

    longjmp(return_point, thread_DEAD);
}

extern void thread_starter(void)
{
    int rc;

#if DEBUG >= 3
    fprintf(stderr, "Thread starter called\n");
#endif

    rc = (tmp_mf)(tmp_argc, tmp_argv);

#if DEBUG >= 3
    fprintf(stderr, "Thread main returned\n");
#endif

    IMGDBG(("thrd%p: main() returned\n", running_thread));

#ifdef RISCOS
    running_thread->rc = rc;
    running_thread->halt_point = "<DEAD via main return>";
#endif

    longjmp(return_point, thread_DEAD);
}

thread thread_start(main_function fn, int argc, char **argv, int stack_size)
{
    thread new;
    my_stack_chunk *sc = (my_stack_chunk *) _kernel_current_stack_chunk();

#if DEBUG >= 3
    fprintf(stderr, "Thread start entered\n");
#endif

    /* Allocate stack space */
    new = mm_malloc(sizeof(thread_struct));
#if DEBUG >= 3
    fprintf(stderr, "Got a thread structure\n");
#endif

    new->stack = malloc(stack_size + STACK_LIMIT_OFFSET);
    /* NOT mm_malloc as this MUST be in application space, not a dynamic
     * area, in case someone is misguided enough to call os_swi or _kernel_swi
     */

    if ( !new->stack )      /* quite possible */
    {
        mm_free( new );
        IMGDBG(("thread_start: malloc FAILED\n"));
        return NULL;
    }

#if DEBUG >= 3
    fprintf(stderr, "Got a stack\n");
#endif

    /* Fix up stack headers/footers */
    *(new->stack) = *sc;
    new->stack->sc.sc_mark = STACK_MAGIC;
    new->stack->sc.sc_next = 0;
    new->stack->sc.sc_prev = 0;
    new->stack->sc.sc_size = stack_size + STACK_LIMIT_OFFSET; /* SJM: added STACK_LIMIT_OFFSET to make MemCheck work */
    new->stack->sc.sc_deallocate = 0;

#if STBWEB_ROM
    /* insert the C relocation offset stuff at the bottom of the stack */
    sutil_get_relocation_offsets((int *)((char *)(new->stack) + sizeof(_kernel_stack_chunk)));
#endif

#if DEBUG >= 3
    fprintf(stderr, "Stack header filled\n");
#endif

    /* Fix up jump buffer */

    memset(new->exec_point, 0, sizeof(jmp_buf));
    new->exec_point[jmpbuf_lr] = (int) &thread_starter;
    new->exec_point[jmpbuf_sp] = ((int) (long) new->stack) + STACK_LIMIT_OFFSET + stack_size;
    new->exec_point[jmpbuf_sl] = ((int) (long) new->stack) + STACK_LIMIT_OFFSET;

#if DEBUG >= 3
    fprintf(stderr, "Long jump buffer filled\n");
#endif

    tmp_mf = fn;
    tmp_argc = argc;
    tmp_argv = argv;

    IMGDBG(("thrd%p: starting\n",new));

    if ((new->status = (thread_status) setjmp(return_point)) == 0)
    {
#if DEBUG >= 3
	fprintf(stderr, "Return point set jump succeeded\n");
	fprintf(stderr, "Return address values fp=0x%08x, sp=0x%08x, sl=0x%08x, lr=0x%08x.\n",
	       return_point[jmpbuf_fp], return_point[jmpbuf_sp],
	       return_point[jmpbuf_sl], return_point[jmpbuf_lr] );
#endif

	new->exec_point[jmpbuf_lr] &= ~3;
	new->exec_point[jmpbuf_lr] |= (return_point[jmpbuf_lr]  & 3);

        IMGDBG(("running_thread = %p\n", new));

	running_thread = new;
	longjmp(new->exec_point, 1);
#if DEBUG >= 3
	fprintf(stderr, "Thread start long jump returned\n");
#endif

        IMGDBG(("running_thread = 0\n"));
	running_thread = 0;
    }

    return new;
}

thread thread_run(thread t)
{
    IMGDBG(("thrd%p: being run\n",t));
    if ((t->status = (thread_status) setjmp(return_point)) == 0)
    {
        IMGDBG(( "running_thread = %p\n", t ));
	running_thread = t;
	longjmp(t->exec_point, 1);

	IMGDBG(( "running_thread = 0\n" ));
	running_thread = 0;
    }

    return t;
}

void thread_destroy(thread t)
{
    /* Free up thread information */
    _kernel_stack_chunk *sc, *sc2;
    MemCheck_checking checking = MemCheck_SetChecking(0, 0);

    IMGDBG(("thrd%p: being destroyed\n", t ));

    sc = (_kernel_stack_chunk*) t->stack;

    while (sc)
    {
	sc2 = sc->sc_next;

	if (sc->sc_deallocate)
	    sc->sc_deallocate(sc);

	sc = sc2;
    }

    MemCheck_RestoreChecking(checking);

    free((void*)t->stack);      /* NOT mm_free as this was allocated with
                                 * (SharedCLibrary's) malloc
                                 */

    mm_free(t);

    if ( t == running_thread )
    {
        IMGDBG(( "running_thread = 0\n" ));
        running_thread = 0;
    }
}

void thread_wait(char *s)
{
    IMGDBG(("thrd%p: wait(%s)\n", running_thread, s ));
    running_thread->halt_point = s;

    if (setjmp(running_thread->exec_point) == 0)
    {
	longjmp(return_point, thread_ALIVE);
    }
}

#endif /* RISCOS */
