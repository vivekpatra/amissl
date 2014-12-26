#if defined(__amigaos4__)
#define __USE_INLINE__
#endif

#include <proto/exec.h>
#include <proto/amisslmaster.h>
#include <libraries/amisslmaster.h>
#include <stdio.h>

#if defined(__amigaos4__)
#define GETINTERFACE(iface, base)	(iface = (APTR)GetInterface((struct Library *)(base), "main", 1L, NULL))
#define DROPINTERFACE(iface)			(DropInterface((struct Interface *)iface), iface = NULL)
#else
#define GETINTERFACE(iface, base)	TRUE
#define DROPINTERFACE(iface)
#endif

int main(void)
{
	struct Library *AmiSSLMasterBase;
  #if defined(__amigaos4__)
  struct AmiSSLMasterIFace *IAmiSSLMaster;
  #endif

#if 1
{
	printf("single base test\n");
	AmiSSLMasterBase = OpenLibrary("amisslmaster.library", 0);
	printf("base %08lx\n", AmiSSLMasterBase);
	if(AmiSSLMasterBase != NULL &&
     GETINTERFACE(IAmiSSLMaster, AmiSSLMasterBase))
	{
		printf("checking base %08lx...", AmiSSLMasterBase);
    #if defined(__amigaos4__)
		printf(" iface %08lx...", IAmiSSLMaster);
    #endif
		InitAmiSSLMaster(AMISSL_V097g, TRUE);
		printf(" done\n");
    DROPINTERFACE(IAmiSSLMaster);
		CloseLibrary(AmiSSLMasterBase);
	}
	printf("single base test finished\n");
}
#endif

#if 1
{
	struct Library *base1;
	struct Library *base2;
  #if defined(__amigaos4__)
  struct AmiSSLMasterIFace *ibase1;
  struct AmiSSLMasterIFace *ibase2;
  #endif

	printf("multi base test\n");
	base1 = OpenLibrary("amisslmaster.library", 0);
	printf("base1 %08lx\n", base1);
	base2 = OpenLibrary("amisslmaster.library", 0);
	printf("base2 %08lx\n", base2);

	if(base1 != NULL && base2 != NULL &&
     GETINTERFACE(ibase1, base1) &&
     GETINTERFACE(ibase2, base2))
	{
		AmiSSLMasterBase = base1;
    IAmiSSLMaster = ibase1;
		printf("checking base %08lx...", AmiSSLMasterBase);
    #if defined(__amigaos4__)
		printf(" iface %08lx...", IAmiSSLMaster);
    #endif
		InitAmiSSLMaster(AMISSL_V097g, TRUE);
		printf(" done\n");

		AmiSSLMasterBase = base2;
    IAmiSSLMaster = ibase2;
		printf("checking base %08lx...", AmiSSLMasterBase);
    #if defined(__amigaos4__)
		printf(" iface %08lx...", IAmiSSLMaster);
    #endif
		InitAmiSSLMaster(AMISSL_V2, FALSE);
		printf(" done\n");

		AmiSSLMasterBase = base1;
    IAmiSSLMaster = ibase1;
		printf("checking base %08lx...", AmiSSLMasterBase);
    #if defined(__amigaos4__)
		printf(" iface %08lx...", IAmiSSLMaster);
    #endif
		InitAmiSSLMaster(AMISSL_V097g, TRUE);
		printf(" done\n");

		AmiSSLMasterBase = base2;
    IAmiSSLMaster = ibase2;
		printf("checking base %08lx...", AmiSSLMasterBase);
    #if defined(__amigaos4__)
		printf(" iface %08lx...", IAmiSSLMaster);
    #endif
		InitAmiSSLMaster(AMISSL_V2, FALSE);
		printf(" done\n");
	}

	if(base1 != NULL)
  {
    DROPINTERFACE(ibase1);
		CloseLibrary(base1);
  }

	if(base2 != NULL)
  {
    DROPINTERFACE(ibase2);
		CloseLibrary(base2);
  }
	printf("multi base test finished\n");
}
#endif

	return 0;
}
