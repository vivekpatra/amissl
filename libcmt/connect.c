#include <sys/types.h>
#include <netinet/in.h>

#ifdef __amigaos4__
#undef __USE_INLINE__
#include <proto/bsdsocket.h>
#else
#define AMITCP_NEW_NAMES
#include <errno.h>
#include "multitcp.h"
#include <internal/amissl.h>
#endif

#include "libcmt.h"

int
connect(
	int s,
	const struct sockaddr *name,
	int namelen)
{
#ifdef __amigaos4__
  GETISOCKET();
  if(ISocket) return ISocket->connect(s,(struct sockaddr *)name,namelen);
  else return -1;
#else
	GETSTATE();
	switch(state->TCPIPStackType)
	{
		case TCPIP_MLink:{
			int res;
			ObtainSemaphore(&state->MLinkLock->Semaphore);
			res = amitcp_Connect(s,name,namelen);
			ReleaseSemaphore(&state->MLinkLock->Semaphore);
			return res;
			break;}
		case TCPIP_Miami:
		case TCPIP_AmiTCP:
			return amitcp_Connect(s,name,namelen);
			break;
		case TCPIP_IN225:
			return in225_connect(s,(struct sockaddr *)name,namelen);
			break;
		case TCPIP_Termite:
			return termite_connect(s,(struct sockaddr *)name,namelen);
			break;
	}
#endif
}

