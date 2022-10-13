/******************************************************************************/
/*
  Author  - icemesh
*/
/******************************************************************************/

#include "textdb.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int ReadFile(const char* file)
{
	FILE* handle = fopen( file, "rb" );
	if(!handle)
	{
		printf("Could not open %s! - Does it even exist?\n", file);
		return -1;
	}
	else
	{
		fseek(handle ,0x0, SEEK_END);
		int totalSize = ftell(handle);
		fseek(handle, 0x0, SEEK_SET);
		uint8_t* pData = (uint8_t*)malloc(totalSize);
		if(!pData)
		{
			printf("!!! Failed to allocate %d bytes for file\n", totalSize);
			fclose(handle);
			return -1;
		}
		else
		{
			fread(pData, totalSize, 0x1, handle);
			fclose(handle);
			int32_t numEntries = *(int32_t*)pData;
			char fname[64];
			snprintf(fname, 64, "%s.txt", file);
			FILE* dumpHandle = fopen(fname, "w");
			if(dumpHandle)
			{
				fprintf( dumpHandle, "%i text entries\n", numEntries);
				pData+=0x4;
				char* pText = (char*)(pData + (numEntries * 0x10));
				uint8_t* pEntry = NULL;
				if(numEntries != 0x0)
				{
					int32_t counter = 0x0;
					uint64_t textId = 0x0;
					uint64_t offset = 0x0;
					do{
						pEntry = pData + (counter * 0x10);
						textId = *(uint64_t*)pEntry;
						offset = *(uint64_t*)(pEntry+0x8);
						fprintf(dumpHandle, "%.16llX: %s\n", textId, (pText+offset) );
						counter++;
					}while(counter < numEntries);
				}
				fclose(dumpHandle);
				pData-=0x4;
			}
			free(pData);
			return 0;
		}
	}	
}
