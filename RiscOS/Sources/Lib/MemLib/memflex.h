/* ExtrasLib:MemFlex.h */

/* Like RiscOSLib:flex.h, but can use dynamic areas
 * (K) All Rites Reversed - Copy What You Like
 *
 * Authors:
 *      Peter Hartley       <peter@ant.co.uk>
 */


#ifndef ExtrasLib_MemFlex_h
#define ExtrasLib_MemFlex_h

/*---------------------------------------------------------------------------*
 * A flex anchor.                                                            *
 * This is a pointer to your pointer to your data, so it should really be    *
 * a void**, but because of C's casting behaviour, this definition makes it  *
 * easier to do things like MemFlex_Alloc( &pSpriteArea, size )              *
 *---------------------------------------------------------------------------*/
typedef void *flex_ptr;

/*---------------------------------------------------------------------------*
 * To link with the debugging version of the Mem* routines,                  *
 * compile with -DMEM_DEBUG on the command line.                             *
 *---------------------------------------------------------------------------*/
#ifdef MEM_DEBUG
void MemFlex__Check( flex_ptr anchor );
void MemFlex_Dump(void *f);
#else
#define MemFlex__Check(__x)
#define MemFlex_Dump(__x)
#endif

/*---------------------------------------------------------------------------*
 * Initialise the MemFlex system                                             *
 * Must be called before any MemFlex *or MemHeap* routine                    *
 * Pass NULL as the area name only if you don't want a dynamic area even on  *
 * a Risc PC -- MemFlex falls back automatically to using application space  *
 * on older machines.                                                        *
 *---------------------------------------------------------------------------*/
os_error *MemFlex_Initialise2( const char *pDynamicAreaName );

/*---------------------------------------------------------------------------*
 * Non-dynamic-area version of Initialise                                    *
 * For backward compatibility                                                *
 *---------------------------------------------------------------------------*/
#define MemFlex_Initialise(__x) MemFlex_Initialise2( NULL )

/*---------------------------------------------------------------------------*
 * Allocate, or re-allocate, a block                                         *
 * If allocating for the first time, *anchor must be NULL. anchor, on the    *
 * other hand, must never be NULL.                                           *
 *---------------------------------------------------------------------------*/
os_error *MemFlex_Alloc( flex_ptr anchor, int size );

/*---------------------------------------------------------------------------*
 * Change the size of a block by adding space in the middle (or at the end)  *
 * Copes with by being negative as follows:                                  *
 *                                                                           *
 *      Contents before  MidExtend call  Contents after                      *
 *      0123456789       at=4, by=2      0123xx456789                        *
 *      0123456789       at=4, by=-2     01456789                            *
 *---------------------------------------------------------------------------*/
#define   memflex_ATTHEEND (-1)
os_error *MemFlex_MidExtend( flex_ptr anchor, int at, int by );

/*---------------------------------------------------------------------------*
 * Free a block                                                              *
 * Also writes *anchor to NULL                                               *
 *---------------------------------------------------------------------------*/
os_error *MemFlex_Free( flex_ptr anchor );

/*---------------------------------------------------------------------------*
 * Return the size of a block                                                *
 *---------------------------------------------------------------------------*/
int       MemFlex_Size( flex_ptr anchor );

/*---------------------------------------------------------------------------*
 * Move a flex block's anchor                                                *
 * If *dst is not NULL on entry, the flex block it anchors is freed.         *
 * Sets *src to NULL if successful.                                          *
 *---------------------------------------------------------------------------*/
os_error *MemFlex_MoveAnchor( flex_ptr dst, flex_ptr src );

/*---------------------------------------------------------------------------*
 * Return the total amount of free space in the system                       *
 *---------------------------------------------------------------------------*/
int       MemFlex_TotalFree( void );

/*---------------------------------------------------------------------------*
 * Is the flex area in a (Risc PC) dynamic area?                             *
 *---------------------------------------------------------------------------*/
BOOL      MemFlex_Dynamic( void );

/*---------------------------------------------------------------------------*
 * Make the flex area shrinkable (or unshrinkable)                           *
 * Default is shrinkable. If bShrinkable==TRUE then this call also does any  *
 * shrinking needed.                                                         *
 *---------------------------------------------------------------------------*/
os_error *MemFlex_Shrinkable( BOOL bShrinkable );

/*---------------------------------------------------------------------------*
 * MemFlex_budge                                                             *
 * If you register this with _kernel_register_slotextend, it will be called  *
 * by the C library when malloc fails. Do NOT do this if MemFlex is using a  *
 * dynamic area (it's also unnecessary). MemFlex_budge calls MemHeap_malloc. *
 *---------------------------------------------------------------------------*/
int MemFlex_budge( int n, void **a );
int MemFlex_dont_budge( int n, void **a );

    /*==============================================*
     *   Definitions for Risc_OSLib compatibility   *
     *==============================================*/

/*---------------------------------------------------------------------------*
 * These calls can be freely mixed with the above -- however, calls to the   *
 * "genuine" flex routines in Risc_OSLib cannot. To ensure nobody ends up    *
 * calling the old stuff, put ExtrasLib before Risc_OSLib in your "link"     *
 * command line. Or use DeskLib :-)                                          *
 *---------------------------------------------------------------------------*/

#define flex_init(__x)          (void)MemFlex_Initialise(NULL)

#define flex_alloc(a,n)         (*((void**)a)=NULL,MemFlex_Alloc(a,n)==NULL)
#define flex_free(a)            (void)MemFlex_Free(a)
#define flex_size               MemFlex_Size
#define flex_extend(a,n)        (MemFlex_Alloc(a,n)==NULL)
#define flex_midextend(a,at,by) (MemFlex_MidExtend(a,at,by)==NULL)
#define flex_budge              MemFlex_budge
#define flex_dont_budge         MemFlex_dont_budge


    /*================================================*
     *   WARNINGS on the use of MemFlex and MemHeap   *
     *================================================*/

/* (1) Putting the flex and heap areas into dynamic areas makes things work
 *     much better -- for instance, the kernel can do stack extension freely in
 *     the application space without having to worry about other areas above
 *     the malloc arena. For this reason, I'd recommend that in development
 *     versions of applications, dynamic areas are NOT used, unless you're
 *     targeting new machines only:
 *
 *      #if your_applications_debug_flag
 *         Error_CheckFatal( MemFlex_Initialise2(NULL) );
 *      #else
 *         Error_CheckFatal( MemFlex_Initialise2( "My application's files" ) );
 *      #endif
 *
 *     Doing this can make many bugs show up on your development machine that
 *     would only otherwise show up when some poor punter runs your application
 *     on an older machine!
 *
 * (2) Some particularly gnarly applications -- for instance, ones using the
 *     various coroutine libraries available for Risc OS -- allocate blocks in
 *     the heap that will be used as stack space. If you do this, bear in mind
 *     that dynamic areas are allocated outside the bottom 64Mb on Risc PCs --
 *     26-bit code cannot be executed in them! This only matters if you use
 *     routines like _kernel_swi and os_swix, which build tiny bits of
 *     trampoline code on the stack and then call them. By far the best way
 *     round this is *never* to use the *abominations* _kernel_swi and os_swix,
 *     and always use assembler SWI veneers -- either writing your own or using
 *     a library like DeskLib or OSLib which includes the ones you need.
 */

#endif
