#define PROTO_AMISSL_H // Don't include amissl protos

//#define DEBUG
#include <internal/debug.h>

#include <string.h>
#include <stdlib.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/locale.h>
#include <proto/utility.h>

#include <dos/var.h>
#if defined(__amigaos4__)
#include <dos/obsolete.h>
#endif
#include <utility/tagitem.h>
#include <exec/memory.h>

#include <openssl/crypto.h>
#include <openssl/lhash.h>
#include <amissl/tags.h>

#include <internal/amissl.h>
#include <internal/amissl_compiler.h>
#include <libraries/amissl.h>

#include <clib/amissl_protos.h>
#define NO_MTCP_PROTOS
#ifdef __GNUC__
#include "../libcmt/libcmt.h"
#include "../libcmt/multitcp.h"
#else
#include "/libcmt/libcmt.h"
#include "/libcmt/multitcp.h"
#endif
#include "amisslinit.h"

#ifdef __amigaos4__
struct AmiSSLIFace;
#define __BASE_OR_IFACE	struct AmiSSLIFace *Self
#else
#define __BASE_OR_IFACE	struct Library *Self
#endif
#define __BASE_OR_IFACE_VAR	Self

#ifdef __amigaos4__
struct Library *IntuitionBase = NULL;
struct IntuitionIFace *IIntuition = NULL;
struct Library *LocaleBase = NULL;
struct LocaleIFace *ILocale = NULL;
struct Library *UtilityBase = NULL;
struct UtilityIFace *IUtility = NULL;
#else
#define XMKSTR(x) #x
#define MKSTR(x)  XMKSTR(x)

const char *FullVersion = "\0$VER: amissl_v" MKSTR(VERSIONNAME) ".library " MKSTR(VERSION) "." MKSTR(AMISSLREVISION) " (" MKSTR(AMISSLDATE) ") " MKSTR(LIBCPU) " version\r\n";

struct ExecBase *SysBase = NULL;
struct DosLibrary *DOSBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;
#if !defined(__MORPHOS__)
struct LocaleBase *LocaleBase = NULL;
#else
struct Library *LocaleBase = NULL;
#endif
struct Library *UtilityBase = NULL;
#endif

struct SignalSemaphore __mem_cs;
LONG GMTOffset;
void *__pool;

struct SignalSemaphore *lock_cs = NULL; /* This needs to be dynamically allocated since it takes up too much near data */
static struct SignalSemaphore AMISSL_COMMON_DATA openssl_cs;// = {NULL};
static LONG AMISSL_COMMON_DATA SemaphoreInitialized = 0;
static struct HashTable * AMISSL_COMMON_DATA thread_hash = NULL;
static ULONG AMISSL_COMMON_DATA LastThreadGroupID = 0;
static ULONG ThreadGroupID = 0;
static ULONG clock_base;
static long SSLVersionApp = 0;

clock_t clock(void)
{
	struct DateStamp ds;
	ULONG clock_curr;

	DateStamp(&ds);
	clock_curr = ((ULONG)ds.ds_Tick + TICKS_PER_SECOND * 60 * ((ULONG)ds.ds_Minute + 24 * 60 * (ULONG)ds.ds_Days))
		         * CLOCKS_PER_SEC / TICKS_PER_SECOND;

	return(clock_curr - clock_base);
}

#include <proto/intuition.h>
#include <intuition/intuition.h>
#include <internal/amissl.h>

#if !defined(__amigaos4__)
// required for clib2's math init/exit functions
void __show_error(const char * message)
{
  struct EasyStruct ErrReq;

  ErrReq.es_StructSize   = sizeof(struct EasyStruct);
  ErrReq.es_Flags        = 0;
  ErrReq.es_Title        = (STRPTR)"AmiSSL/OpenSSL internal error";
  ErrReq.es_TextFormat   = (STRPTR)message;
  ErrReq.es_GadgetFormat = (STRPTR)"Ok";

  // Open an Easy Requester
  EasyRequestArgs(NULL, &ErrReq, NULL, NULL);
}

void exit(UNUSED int rc)
{
}
#endif

AMISSL_STATE *CreateAmiSSLState(void)
{
	unsigned long pid;
	AMISSL_STATE *ret;

	ObtainSemaphore(&openssl_cs);

  pid = (unsigned long)FindTask(NULL);
	ret = (AMISSL_STATE *)malloc(sizeof(AMISSL_STATE));

	if (ret != NULL)
	{
		//kprintf("Allocating new state for %08lx\n",pid);
		ret->pid = pid;
		ret->errno = 0;
		ret->errno_ptr = &ret->errno;
		ret->socket_errno_initialized = 0;
		ret->getenv_var = 0;
		ret->stack = 0;
		ret->SocketBase = NULL;
#ifdef __amigaos4__
		ret->ISocket = NULL;
		ret->ISocketPtr = NULL;
#endif
		ret->ThreadGroupID = ThreadGroupID;

		if(!h_insert(thread_hash, pid, ret))
		{
			free(ret);
			ret = NULL;
		}
	}

	ReleaseSemaphore(&openssl_cs);

	return ret;
}
#ifdef __amigaos4__

#define SB_ObtainSemaphore  ObtainSemaphore
#define SB_ReleaseSemaphore ReleaseSemaphore
#define SB_FindTask         FindTask
#define SB_AllocVec(s,t)    AllocVecTags(s, AVT_Type, MEMF_SHARED, TAG_DONE)
#define SB_FreeVec          FreeVec

#else

#define SB_ObtainSemaphore  ObtainSemaphore
#define SB_ReleaseSemaphore ReleaseSemaphore
#define SB_FindTask         FindTask
#define SB_AllocVec(s,t)    AllocVec(s, t)
#define SB_FreeVec          FreeVec

#endif

STDARGS AMISSL_STATE *GetAmiSSLState(void)
{
	AMISSL_STATE *ret;

	SB_ObtainSemaphore(&openssl_cs);
	ret = (AMISSL_STATE *)h_find(thread_hash, (long)SB_FindTask(NULL));
	//kprintf("Looked up state %08lx for %08lx\n",ret,pid);
	SB_ReleaseSemaphore(&openssl_cs);

	return ret;
}

STDARGS void SetAmiSSLerrno(int err)
{
	AMISSL_STATE *p = GetAmiSSLState();
	*p->errno_ptr = err;
}

STDARGS int GetAmiSSLerrno(void)
{
	AMISSL_STATE *p = GetAmiSSLState();
	return *p->errno_ptr;
}

struct CRYPTO_dynlock_value
{
  struct SignalSemaphore lock_cs;
};

static struct CRYPTO_dynlock_value *amigaos_dyn_create_function(UNUSED const char *file, UNUSED int line)
{
  struct CRYPTO_dynlock_value *value;

#if defined(__amigaos4__)
	if((value = AllocVecTags(sizeof(*value), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE)))
#else
	if((value = AllocVec(sizeof(*value), MEMF_CLEAR)))
#endif
  {
    InitSemaphore(&value->lock_cs);
  }

  return value;
}

static void amigaos_dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l,
                                      UNUSED const char *file, UNUSED int line)
{
  if(mode & CRYPTO_LOCK)
    ObtainSemaphore(&l->lock_cs);
  else
    ReleaseSemaphore(&l->lock_cs);
}

static void amigaos_dyn_destroy_function(struct CRYPTO_dynlock_value *l,
                                         UNUSED const char *file, UNUSED int line)
{
  InitSemaphore(&l->lock_cs);
	FreeVec(l);
}

static void amigaos_locking_callback(int mode, int type, UNUSED const char *file, UNUSED int line)
{
	if(mode & CRYPTO_LOCK)
		ObtainSemaphore(&(lock_cs[type]));
	else
		ReleaseSemaphore(&(lock_cs[type]));
}

static void amigaos_threadid_callback(CRYPTO_THREADID *id)
{
	ObtainSemaphore(&openssl_cs);
  CRYPTO_THREADID_set_pointer(id, (void*)FindTask(NULL));
	ReleaseSemaphore(&openssl_cs);
}

static void ThreadGroupStateCleanup(UNUSED long Key, AMISSL_STATE *a)
{
	if (a->ThreadGroupID == ThreadGroupID)
	{
		kprintf("- Cleaning up state %08lx for %08lx (group %lu)\n", a, a->pid, a->ThreadGroupID);
		h_delete(thread_hash, a->pid);
		free(a);
	}
}

static void *h_allocfunc(long size)
{
	return SB_AllocVec(size,MEMF_ANY);
}

static void h_freefunc(void *mem)
{
	SB_FreeVec(mem);
}

void InternalInitAmiSSL(UNUSED struct AmiSSLInitStruct *amisslinit)
{
  /* nothing */
}

LIBPROTO(InitAmiSSLA, LONG, REG(a6, __BASE_OR_IFACE), REG(a0, struct TagItem *tagList))
{
	AMISSL_STATE *state;
	LONG err;

	if((state = CreateAmiSSLState()))
	{
		int *errno_ptr;

		state->SocketBase = (APTR)GetTagData(AmiSSL_SocketBase, (int)NULL, tagList);

#ifdef __amigaos4__
		state->ISocket = (struct SocketIFace *)GetTagData(AmiSSL_ISocket, (int)NULL, tagList);
		state->ISocketPtr = (struct SocketIFace **)GetTagData(AmiSSL_ISocketPtr, (ULONG)NULL, tagList);
		state->IAmiSSL = Self;
		state->AmiSSLBase = ((struct Interface *)Self)->Data.LibBase;

		/* When ISocket[Ptr] is supplied, there is no need to specify SocketBase.
		 * This combination would confuse the code below which thinks that it
		 * needs to GetInterface if there is a SocketBase and also drops it later.
		 */
		if (state->ISocket || state->ISocketPtr)
			state->SocketBase = NULL;

		if (state->ISocketPtr)
			state->ISocket = NULL; /* This is unneeded, ISocket should never be accessed directly */
		else
			state->ISocketPtr = &state->ISocket;
#else
		state->AmiSSLBase = Self;
#endif

#ifdef __amigaos4__
		if(state->SocketBase)
		{ // This means we are beeing called from a 68k program and we need to get the ppc interface to the library ourselves
			if((*state->ISocketPtr = (struct SocketIFace *)GetInterface(state->SocketBase,"main",1,NULL)))
			{
				// All is good. Now we can make socket calls as if everything was ppc
			}
			else
			{
				// Ouch, we are using a 68k stack without an interface. Not much to do for now...
				return 1; // Error
			}
		}
		kprintf("SocketBase: %08lx\n",state->SocketBase);
		kprintf("ISocket: %08lx (ISocket address: %08lx)\n",state->ISocket,&state->ISocket);
		kprintf("ISocketPtr: %08lx\n",state->ISocketPtr);
#else
		state->TCPIPStackType = (LONG)GetTagData(AmiSSL_SocketBaseBrand, TCPIP_AmiTCP, tagList);
		state->MLinkLock = (APTR)GetTagData(AmiSSL_MLinkLock, (int)NULL, tagList);
#endif

		if((errno_ptr = (int *)GetTagData(AmiSSL_ErrNoPtr, (int)NULL, tagList)))
			state->errno_ptr = errno_ptr;

		initialize_socket_errno();

		SSLVersionApp = GetTagData(AmiSSL_SSLVersionApp, 0, tagList);

		err = 0;
	}
	else
	{
		err = 1;
	}

	return(err);
}

LONG CleanupAmiSSLA(UNUSED struct TagItem *tagList)
{
	AMISSL_STATE *state;

	if((state = GetAmiSSLState()))
	{
#ifdef __amigaos4__
		if(state->SocketBase && state->ISocketPtr && *state->ISocketPtr)
		{
			DropInterface((struct Interface *)*state->ISocketPtr);
			state->ISocketPtr = NULL;
		}
#endif

		ObtainSemaphore(&openssl_cs);
		h_delete(thread_hash, state->pid);
		ReleaseSemaphore(&openssl_cs);

		free(state);
	}

	return(0);
}

#ifdef __amigaos4__
LIBPROTOVA(InitAmiSSL, LONG, REG(a6, __BASE_OR_IFACE), ...)
{
	__gnuc_va_list ap;
	struct TagItem *tags;

	__builtin_va_start(ap, Self);
	tags = va_getlinearva(ap, struct TagItem *);
	__builtin_va_end(ap);

	return CALL_LFUNC(InitAmiSSLA, tags);
}

LIBPROTOVA(CleanupAmiSSL, LONG, REG(a6, __BASE_OR_IFACE), ...)
{
	__gnuc_va_list ap;
	struct TagItem *tags;

	__builtin_va_start(ap, Self);
	tags = va_getlinearva(ap, struct TagItem *);
	__builtin_va_end(ap);

	return LIB_CleanupAmiSSLA(Self, tags);
}
#endif

void AmiSSLAbort(void)
{
	OpenSSLDie("unknown", 0, "abort() or similar function called");
}

void openlog(void) {}
void closelog(void) {}
void syslog(UNUSED int priority, UNUSED const char *message, ...) {}

LIBPROTO(__UserLibCleanup, void, REG(a6, UNUSED __BASE_OR_IFACE))
{
	traceline();

	if (thread_hash)
	{
		kprintf("Performing unfreed states cleanup for %08lx (group %lu)\n", FindTask(NULL), ThreadGroupID);
		ObtainSemaphore(&openssl_cs);
		h_doall(thread_hash, (void (*)(long, void *))ThreadGroupStateCleanup);
		ReleaseSemaphore(&openssl_cs);
	}
	else
		kprintf("No thread_hash\n");

#ifdef __amigaos4__
	DropInterface((struct Interface *)ILocale);
	DropInterface((struct Interface *)IUtility);
	DropInterface((struct Interface *)IIntuition);
#endif

	CloseLibrary((struct Library *)LocaleBase);
	CloseLibrary((struct Library *)UtilityBase);
	CloseLibrary((struct Library *)IntuitionBase);
#ifndef __amigaos4__
	CloseLibrary((struct Library *)DOSBase);
#endif

	CRYPTO_set_locking_callback(NULL);

	FreeVec(lock_cs);

#ifdef __amigaos4__
	if(__pool)
		FreeSysObject(ASOT_MEMPOOL, __pool);
#else
	if(__pool)
		DeletePool(__pool);
#endif
}

LIBPROTO(__UserLibExpunge, void, REG(a6, UNUSED __BASE_OR_IFACE))
{
	traceline();
}

LIBPROTO(__UserLibInit, int, REG(a6, __BASE_OR_IFACE))
{
	int err = 1; /* Assume error condition */

#ifdef __amigaos4__
	InitSemaphore(&__mem_cs);
	InitSemaphore(&openssl_cs);
#else
	SysBase = *(struct ExecBase **)4;
#endif

	kprintf("Calling user lib init\n");

	if (!thread_hash)
	{
		Forbid();

		if(!SemaphoreInitialized)
		{
			InitSemaphore(&openssl_cs);
			SemaphoreInitialized = TRUE;
		}

		Permit();

		ObtainSemaphore(&openssl_cs);

		if (!thread_hash)
			thread_hash = h_new(7, h_allocfunc,h_freefunc);

		ReleaseSemaphore(&openssl_cs);
	}

	ObtainSemaphore(&openssl_cs);

	ThreadGroupID = ++LastThreadGroupID;
	kprintf("Thread group ID: %lu\n", ThreadGroupID);

	ReleaseSemaphore(&openssl_cs);

#ifdef __amigaos4__
	if ((__pool = AllocSysObjectTags(ASOT_MEMPOOL, ASOPOOL_MFlags, MEMF_PRIVATE, ASOPOOL_Puddle, 8192, ASOPOOL_Threshold, 4096, ASOPOOL_Name, "AmiSSL", TAG_DONE))
	    && (lock_cs = AllocVecTags(CRYPTO_num_locks() * sizeof(*lock_cs), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_DONE)))
#else
	if ((__pool = CreatePool(MEMF_ANY, 8192, 4096))
	    && (lock_cs = AllocVec(CRYPTO_num_locks() * sizeof(*lock_cs), MEMF_CLEAR)))
#endif
	{
		struct Locale *locale;
		struct DateStamp ds;
		int i;

		for (i=0; i<CRYPTO_num_locks(); i++)
		{
			InitSemaphore(&lock_cs[i]);
		}

		InitSemaphore(&__mem_cs);

    // set static locks callbacks
		CRYPTO_set_locking_callback((void (*)())amigaos_locking_callback);
    CRYPTO_THREADID_set_callback(amigaos_threadid_callback);

    // set dynamic locks callbacks
    CRYPTO_set_dynlock_create_callback(amigaos_dyn_create_function);
    CRYPTO_set_dynlock_lock_callback(amigaos_dyn_lock_function);
    CRYPTO_set_dynlock_destroy_callback(amigaos_dyn_destroy_function);

#ifdef __amigaos4__
		if ((IntuitionBase = OpenLibrary("intuition.library", 50))
            && (UtilityBase = OpenLibrary("utility.library", 50))
			&& (LocaleBase = OpenLibrary("locale.library", 50))
			&& (IIntuition = (struct IntuitionIFace *)GetInterface(IntuitionBase,"main",1,NULL))
			&& (IUtility = (struct UtilityIFace *)GetInterface(UtilityBase,"main",1,NULL))
			&& (ILocale = (struct LocaleIFace *)GetInterface(LocaleBase,"main",1,NULL))
			&& (locale = OpenLocale(NULL)))
#else
		if ((DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 37))
		    && (IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 36))
            && (UtilityBase = OpenLibrary("utility.library", 37))
			&& (LocaleBase = (struct LocaleBase *)OpenLibrary("locale.library", 38))
			&& (locale = OpenLocale(NULL)))
#endif
		{
			DateStamp(&ds);
			clock_base = ((ULONG)ds.ds_Tick + TICKS_PER_SECOND * 60 * ((ULONG)ds.ds_Minute + 24 * 60 * (ULONG)ds.ds_Days))
						 * CLOCKS_PER_SEC / TICKS_PER_SECOND;

			GMTOffset = locale->loc_GMTOffset;

			CloseLocale(locale);
			err = 0;
		}
	}

	kprintf("Userlib res: %d\n",err);

	if (err != 0)
		CALL_LFUNC_NP(__UserLibCleanup, Self);

	return(err);
}
