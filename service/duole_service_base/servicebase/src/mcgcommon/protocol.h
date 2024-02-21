#ifndef __PROTOCOL_H_20131002__
#define __PROTOCOL_H_20131002__

#include "datatype.h"

typedef struct tagHEADER
{
	DWORD dwType;		
	DWORD dwLength;		
	DWORD dwSeqID;	
}HEADER, *PHEADER;

#define ID_REQ					0x00000000
#define ID_ACK					0x02000000
#define ID_NTF					0x04000000
#define ID_SEND                 0x08000000
#define ID_MSK                  0x00ffffff

#define	ID_MSGBASE_SYSTEM						0x00000000		
#define	ID_MSGBASE_SYSTEM_MAX					0x000000FF
#endif //__PROTOCOL_H_20131002__
