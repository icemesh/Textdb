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
			printf("!!! Failed to allocate 0x%08X bytes for file\n", totalSize);
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
				char* pText = (char*)(pData + (numEntries << 0x3));
				uint8_t* pEntry = NULL;
				if(numEntries != 0x0)
				{
					int32_t counter = 0x0;
					uint32_t textId = 0x0;
					uint32_t offset = 0x0;
					do{
						pEntry = pData + (counter << 0x3);
						textId = *(uint32_t*)pEntry;
						offset = *(uint32_t*)(pEntry+0x4);
						fprintf(dumpHandle, "%08X: %s\n", textId, (pText+offset) );
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
