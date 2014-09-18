#ifndef AMISSLMASTER_INTERFACE_DEF_H
#define AMISSLMASTER_INTERFACE_DEF_H

/*
** This file was machine generated by idltool 53.9.
** Do not edit
*/ 

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#ifdef __cplusplus
#ifdef __USE_AMIGAOS_NAMESPACE__
namespace AmigaOS {
#endif
extern "C" {
#endif

struct AmiSSLMasterIFace
{
	struct InterfaceData Data;

	uint32 APICALL (*Obtain)(struct AmiSSLMasterIFace *Self);
	uint32 APICALL (*Release)(struct AmiSSLMasterIFace *Self);
	void APICALL (*Expunge)(struct AmiSSLMasterIFace *Self);
	struct Interface * APICALL (*Clone)(struct AmiSSLMasterIFace *Self);
	LONG APICALL (*InitAmiSSLMaster)(struct AmiSSLMasterIFace *Self, LONG APIVersion, LONG UsesOpenSSLStructs);
	struct Library * APICALL (*OpenAmiSSL)(struct AmiSSLMasterIFace *Self);
	void APICALL (*CloseAmiSSL)(struct AmiSSLMasterIFace *Self);
	struct Library * APICALL (*OpenAmiSSLCipher)(struct AmiSSLMasterIFace *Self, LONG Cipher);
	void APICALL (*CloseAmiSSLCipher)(struct AmiSSLMasterIFace *Self, struct Library * CipherBase);
};

#ifdef __cplusplus
}
#ifdef __USE_AMIGAOS_NAMESPACE__
}
#endif
#endif

#endif /* AMISSLMASTER_INTERFACE_DEF_H */
