/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "driver_sd.h"
/* Definitions of physical drive number for each drive */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
    if(BSP_SD_GetCardState() == MSD_OK)
    {
        stat &= ~STA_NOINIT;
    }
    
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

    if(BSP_SD_Init() == MSD_OK)
    {
        stat &= ~STA_NOINIT;
    }
    else
    {
        stat = STA_NOINIT;
    }
    return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_ERROR;
    uint32_t timeout = 100000;

    res = BSP_SD_ReadBlocks((uint32_t *)buff, sector, count, SD_DATATIMEOUT);
    if(res==MSD_OK)
    {
        while(BSP_SD_GetCardState()!= MSD_OK)
        {
            if (timeout-- == 0)
            {
                return RES_ERROR;
            }
        }
        res = RES_OK;
    }
    else
    {
        res = RES_ERROR;
    }
    
    return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_ERROR;
    uint32_t timeout = 100000;

    res = BSP_SD_WriteBlocks((uint32_t *)buff, sector, count, SD_DATATIMEOUT);
    if(res==MSD_OK)
    {
        while(BSP_SD_GetCardState()!= MSD_OK)
        {
            if (timeout-- == 0)
            {
                return RES_ERROR;
            }
        }    
        res = RES_OK;
    }
    else
    {
        res = RES_ERROR;
    }
    
    return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_ERROR;

    switch(cmd)
    {
        case CTRL_SYNC :
        {
            res = RES_OK;
            break;
        }
        case GET_SECTOR_COUNT:
        {
            res = RES_OK;
            *(DWORD*)buff = sd_cardinfo.LogBlockNbr;
            break;
        }
        case GET_SECTOR_SIZE:
        {
            res = RES_OK;
            *(DWORD*)buff = sd_cardinfo.LogBlockSize;
            break;
        }
        case GET_BLOCK_SIZE:
        {
            res = RES_OK;
            *(DWORD*)buff = sd_cardinfo.LogBlockSize;
            break;
        }
        default:
        {
            res = RES_PARERR;
            break;
        }
    }

	return res;
}

DWORD get_fattime(void)
{
    
    return 0;
}
