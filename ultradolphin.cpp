/* Silicon Graphics Inc's SDK for the Nintendo64 Breakdown
 * In other words, a copypasta of all the headers and src from the SDK that do controller stuff.
 * Why has no one looked at the SDK before? It tells you everything...
 */
// header location /ultra/usr/include/PR/
// src location /nintendo/n64kit/nusys/src/nusys-2.06/nusys/

// os_pfs.h - permanent file system

/*
 * Structure for file system
 *
typedef struct {
	int		status;
	OSMesgQueue 	*queue;
	int		channel;
	u8		id[32];
	u8		label[32];
	int		version;
	int		dir_size;
	int		inode_table;		/* block location *
	int		minode_table;		/* mirrioring inode_table *
	int		dir_table;		/* block location *
	int		inode_start_page;	/* page # *
	u8		banks;
	u8		activebank;
} OSPfs;

typedef struct {
	u32	file_size;	/* bytes *
	u32 	game_code;
	u16 	company_code;
	char  	ext_name[4];
	char 	game_name[16];
} OSPfsState;

/* File System size *
#define	OS_PFS_VERSION		0x0200
#define	OS_PFS_VERSION_HI	(OS_PFS_VERSION >> 8)
#define	OS_PFS_VERSION_LO	(OS_PFS_VERSION & 255)

#define PFS_FILE_NAME_LEN       16
#define PFS_FILE_EXT_LEN        4
#define BLOCKSIZE		32		/* bytes *
#define PFS_ONE_PAGE            8		/* blocks *
#define PFS_MAX_BANKS		62

/* File System flag *

#define PFS_READ                0
#define PFS_WRITE               1
#define PFS_CREATE              2

/* File System status *
#define PFS_INITIALIZED		0x1
#define PFS_CORRUPTED		0x2		/* File system was corrupted *
#define PFS_ID_BROKEN		0x4
#define PFS_MOTOR_INITIALIZED	0x8
#define	PFS_GBPAK_INITIALIZED	0x10

/* File System error number *

#define PFS_ERR_NOPACK		1	/* no memory card is plugged or   *
#define PFS_ERR_NEW_PACK        2	/* ram pack has been changed to a *
					/* different one 		  *
#define PFS_ERR_INCONSISTENT    3	/* need to run Pfschecker 	  *
#define PFS_ERR_CONTRFAIL	CONT_OVERRUN_ERROR              
#define PFS_ERR_INVALID         5	/* invalid parameter or file not exist*
#define PFS_ERR_BAD_DATA        6       /* the data read from pack are bad*
#define PFS_DATA_FULL           7	/* no free pages on ram pack      *
#define PFS_DIR_FULL            8	/* no free directories on ram pack*
#define PFS_ERR_EXIST		9	/* file exists 			  *
#define PFS_ERR_ID_FATAL	10	/* dead ram pack *
#define PFS_ERR_DEVICE		11	/* wrong device type*
#define PFS_ERR_NO_GBCART	12 	/* no gb cartridge (64GB-PAK) *
#define PFS_ERR_NEW_GBCART	13 	/* gb cartridge may be changed *

/* file system interface *

extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsRepairId(OSPfs *);
extern s32 osPfsInit(OSMesgQueue *, OSPfs *, int);
extern s32 osPfsReFormat(OSPfs *, OSMesgQueue *, int);
extern s32 osPfsChecker(OSPfs *);
extern s32 osPfsAllocateFile(OSPfs *, u16, u32, u8 *, u8 *, int, s32 *);
extern s32 osPfsFindFile(OSPfs *, u16, u32, u8 *, u8 *, s32 *);
extern s32 osPfsDeleteFile(OSPfs *, u16, u32, u8 *, u8 *);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 osPfsGetLabel(OSPfs *, u8 *, int *);
extern s32 osPfsSetLabel(OSPfs *, u8 *);
extern s32 osPfsIsPlug(OSMesgQueue *, u8 *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern s32 osPfsNumFiles(OSPfs *, s32 *, s32 *);

// os_cont.h - controller interface

/*
 * Structure for controllers
 *
typedef struct {
	u16     type;                   /* Controller Type *
	u8      status;                 /* Controller status *
	u8	errno;
}OSContStatus;

typedef struct {
	u16     button;
	s8      stick_x;		/* -80 <= stick_x <= 80 *
	s8      stick_y;		/* -80 <= stick_y <= 80 *
	u8	errno;
} OSContPad;

typedef struct {
	void    *address;               /* Ram pad Address:  11 bits *
	u8      databuffer[32];         /* address of the data buffer *
	u8      addressCrc;             /* CRC code for address *
	u8      dataCrc;                /* CRC code for data *
	u8	errno;
} OSContRamIo;

/* controller errors *
#define CONT_NO_RESPONSE_ERROR          0x8
#define CONT_OVERRUN_ERROR              0x4
#ifdef _HW_VERSION_1
#define CONT_FRAME_ERROR                0x2
#define CONT_COLLISION_ERROR            0x1
#endif 

/* Controller type *

#define CONT_ABSOLUTE           0x0001
#define CONT_RELATIVE           0x0002
#define CONT_JOYPORT            0x0004
#define CONT_EEPROM		0x8000 // memory pak
#define CONT_EEP16K		0x4000 // bigger memory pak?
#define	CONT_TYPE_MASK		0x1f07
// bytes are flipped from what docs say, silly docs
#define	CONT_TYPE_NORMAL	0x0005 // n64 controller
#define	CONT_TYPE_MOUSE		0x0002 // n64 mouse
#define	CONT_TYPE_VOICE		0x0100 // vru

/* Controller status *

#define CONT_CARD_ON            0x01
#define CONT_CARD_PULL          0x02
#define CONT_ADDR_CRC_ER        0x04
#define CONT_EEPROM_BUSY	0x80

/* Controller error number *

#define	CONT_ERR_NO_CONTROLLER		PFS_ERR_NOPACK 		/* 1 *
#define	CONT_ERR_CONTRFAIL		CONT_OVERRUN_ERROR	/* 4 *
#define	CONT_ERR_INVALID		PFS_ERR_INVALID		/* 5 *
#define	CONT_ERR_DEVICE			PFS_ERR_DEVICE 		/* 11 *
#define	CONT_ERR_NOT_READY		12
#define	CONT_ERR_VOICE_MEMORY		13
#define	CONT_ERR_VOICE_WORD		14
#define	CONT_ERR_VOICE_NO_RESPONSE	15

// os_motor.h - interface for rumble pak

/* Rumble PAK interface *

extern s32 osMotorInit(OSMesgQueue *, OSPfs *, int);
#if	1
#define MOTOR_START		1
#define MOTOR_STOP		0
#define	osMotorStart(x)		__osMotorAccess((x), MOTOR_START)
#define	osMotorStop(x)		__osMotorAccess((x), MOTOR_STOP)
extern s32 __osMotorAccess(OSPfs *, s32);
#else
extern s32 osMotorStop(OSPfs *);
extern s32 osMotorStart(OSPfs *);
#endif

// os_eeprom.h - n64 memory pak interface

/* EEPROM TYPE *

#define EEPROM_TYPE_4K		0x01
#define EEPROM_TYPE_16K		0x02

/* definition for EEPROM *

#define EEPROM_MAXBLOCKS	64
#define EEP16K_MAXBLOCKS	256
#define EEPROM_BLOCK_SIZE	8

// prototypes
extern s32 osEepromProbe(OSMesgQueue *);
extern s32 osEepromRead(OSMesgQueue *, u8, u8 *);
extern s32 osEepromWrite(OSMesgQueue *, u8, u8 *);
extern s32 osEepromLongRead(OSMesgQueue *, u8, u8 *, int);
extern s32 osEepromLongWrite(OSMesgQueue *, u8, u8 *, int);

// os_gbpak.h - interface for transfer pak

typedef struct {
	u16	fixed1;
	u16	start_address;
	u8	nintendo_chr[0x30];
	u8	game_title[16];
	u16	company_code;
	u8	body_code;
	u8	cart_type;
	u8	rom_size;
	u8	ram_size;
	u8	country_code;
	u8	fixed2;
	u8	version;
	u8	isum;
	u16	sum;
} OSGbpakId;

/* definition for 64GB-PAK *

#define OS_GBPAK_POWER          0x01
#define OS_GBPAK_RSTB_DETECTION 0x04
#define OS_GBPAK_RSTB_STATUS    0x08
#define OS_GBPAK_GBCART_PULL    0x40
#define OS_GBPAK_GBCART_ON      0x80

#define	OS_GBPAK_POWER_OFF	0x00 	/* power of 64GB-PAK *
#define	OS_GBPAK_POWER_ON	0x01

#define	OS_GBPAK_ROM_ID_SIZE	0x50	/* ID size of GB cartridge *

/* 64GB-PAK *
extern  s32     osGbpakInit(OSMesgQueue *, OSPfs *, int);
extern  s32     osGbpakPower(OSPfs *, s32);
extern  s32     osGbpakGetStatus(OSPfs *, u8 *);
extern  s32     osGbpakReadWrite(OSPfs *, u16, u16, u8 *, u16);
extern  s32     osGbpakReadId(OSPfs *, OSGbpakId *, u8 *);
extern  s32	osGbpakCheckConnector(OSPfs *, u8 *);

// os_voice.h - interface for the VRU

typedef struct {		/* Voice Recognition System *
	OSMesgQueue	*__mq;		/* SI Message Queue *
	int		__channel;	/* Controller Port # *
	s32		__mode;
	u8		cmd_status;	/* Command Status *
} OSVoiceHandle;

typedef struct {		/* Voice Recognition System *
	u16	warning;
	u16	answer_num;		/* 0...5 *
	u16	voice_level;
	u16	voice_sn;
	u16	voice_time;
	u16	answer[5];
	u16	distance[5];
} OSVoiceData;

/* definition for Voice Recognition System *

#define	VOICE_WARN_TOO_SMALL	0x0400
#define	VOICE_WARN_TOO_LARGE	0x0800
#define	VOICE_WARN_NOT_FIT	0x4000
#define	VOICE_WARN_TOO_NOISY	0x8000

#define	VOICE_STATUS_READY	0
#define	VOICE_STATUS_START	1
#define	VOICE_STATUS_CANCEL	3
#define	VOICE_STATUS_BUSY	5
#define	VOICE_STATUS_END	7

/* Voice Recognition System *
extern s32 osVoiceInit(OSMesgQueue *, OSVoiceHandle *, int);
extern s32 osVoiceCheckWord(u8 *data);
extern s32 osVoiceClearDictionary(OSVoiceHandle *, u8);
extern s32 osVoiceControlGain(OSVoiceHandle *, s32, s32);
extern s32 osVoiceSetWord(OSVoiceHandle *, u8 *);
extern s32 osVoiceStartReadData(OSVoiceHandle *);
extern s32 osVoiceStopReadData(OSVoiceHandle *);
extern s32 osVoiceGetReadData(OSVoiceHandle *, OSVoiceData *);
extern s32 osVoiceMaskDictionary(OSVoiceHandle *, u8 *, int);
extern void osVoiceCountSyllables(u8 *, u32 *);

// nusys.h

/*----------------------------------------------------------------------*
/* SI MANAGER DEFINE							*
/*----------------------------------------------------------------------*
#define	NU_SI_STACK_SIZE		NU_CONT_STACK_SIZE
#define	NU_SI_THREAD_ID			NU_CONT_THREAD_ID
#define NU_SI_THREAD_PRI		NU_CONT_THREAD_PRI
#define NU_SI_MESG_MAX			NU_CONT_MESG_MAX
#define NU_SI_CALLBACK_END		1
#define NU_SI_CALLBACK_CONTINUE		0
#define	NU_SI_SC_MAJOR_NO		0x0000
#define	NU_SI_MSG_MAJOR_NO_MASK		0xff00
#define	NU_SI_MSG_MINOR_NO_MASK		0x00ff

#define	NU_SI_MAJOR_NO_CONT		0x0100
#define	NU_SI_MAJOR_NO_PAK		0x0200
#define	NU_SI_MAJOR_NO_RMB		0x0300
#define	NU_SI_MAJOR_NO_EEPROM		0x0400
#define	NU_SI_MAJOR_NO_GBPAK		0x0500
#define	NU_SI_MAJOR_NO_VRS		0x0600
#define	NU_SI_STOP_MGR_MSG		0x7F00

/*----------------------------------------------------------------------*/
/* CONTROLLER MANEGER DEFINE						*/
/*----------------------------------------------------------------------*
#define NU_CONT_MAXCONTROLLERS		MAXCONTROLLERS
#define	NU_CONT_STACK_SIZE		0x2000
#define	NU_CONT_MESG_MAX		8
#define NU_CONT_THREAD_ID		6
#define NU_CONT_THREAD_PRI		115
#define	NU_CONT_DATA_UNLOCK		0
#define	NU_CONT_DATA_LOCK		1


/*----------------------------------------------------------------------*/
/* Define messages sent to the Controller Manager				*/
/*----------------------------------------------------------------------*
#define	NU_CONT_MSG_BASE		NU_SI_MAJOR_NO_CONT
#define	NU_CONT_RETRACE_MSG		(NU_CONT_MSG_BASE+0)
#define NU_CONT_READ_MSG		(NU_CONT_MSG_BASE+1)
#define NU_CONT_READ_NW_MSG		(NU_CONT_MSG_BASE+2)
#define NU_CONT_QUERY_MSG		(NU_CONT_MSG_BASE+3)


#define	NU_CONT_PAK_MSG_BASE		NU_SI_MAJOR_NO_PAK
#define	NU_CONT_PAK_RETRACE_MSG		(NU_CONT_PAK_MSG_BASE+0)
#define NU_CONT_PAK_OPEN_MSG		(NU_CONT_PAK_MSG_BASE+1)
#define NU_CONT_PAK_FREE_MSG		(NU_CONT_PAK_MSG_BASE+2)
#define NU_CONT_PAK_FOPEN_MSG		(NU_CONT_PAK_MSG_BASE+3)
#define NU_CONT_PAK_READWRITE_MSG	(NU_CONT_PAK_MSG_BASE+4)
#define NU_CONT_PAK_FILEDELETE_MSG	(NU_CONT_PAK_MSG_BASE+5)
#define NU_CONT_PAK_FILESTATE_MSG	(NU_CONT_PAK_MSG_BASE+6)
#define NU_CONT_PAK_FILENUM_MSG		(NU_CONT_PAK_MSG_BASE+7)
#define NU_CONT_PAK_REPAIRID_MSG	(NU_CONT_PAK_MSG_BASE+8)

#define NU_CONT_PAK_MODE_NOCREATE	0
#define NU_CONT_PAK_MODE_CREATE		1
#define	NU_CONT_PAK_TYPE_NONE		0
#define	NU_CONT_PAK_TYPE_PAK		1	/* Controller Pak 	*
#define	NU_CONT_PAK_TYPE_RUMBLE		2	/* Rumble Pak  		*
#define	NU_CONT_PAK_TYPE_GBPAK		3	/* 64GB Pak		*
#define NU_CONT_PAK_READ		PFS_READ
#define	NU_CONT_PAK_WRITE		PFS_WRITE

/*----------------------------------------------------------------------*/
/* Rumble Pak control 							*/
/*----------------------------------------------------------------------*
#define NU_CONT_RMB_STATE_STOP		0x00
#define NU_CONT_RMB_STATE_STOPPING	0x01
#define NU_CONT_RMB_STATE_STOPPED	0x02
#define NU_CONT_RMB_STATE_RUN		0x03
#define NU_CONT_RMB_STATE_FORCESTOP	0x04
#define NU_CONT_RMB_MODE_DISABLE	0x00
#define	NU_CONT_RMB_MODE_ENABLE		0x01
#define NU_CONT_RMB_MODE_AUTORUN	0x02
#define NU_CONT_RMB_MODE_PAUSE		0x80
#define NU_CONT_RMB_AUTO_SEARCH		0x00
#define	NU_CONT_RMB_AUTO_FIND		0x01
#define NU_CONT_RMB_AUTO_SEARCHTIME	(60*5)
#define NU_CONT_RMB_MSG_BASE		NU_SI_MAJOR_NO_RMB
#define NU_CONT_RMB_RETRACE_MSG		(NU_CONT_RMB_MSG_BASE+0)
#define NU_CONT_RMB_CHECK_MSG		(NU_CONT_RMB_MSG_BASE+1)
#define NU_CONT_RMB_START_MSG		(NU_CONT_RMB_MSG_BASE+2)
#define NU_CONT_RMB_STOP_MSG		(NU_CONT_RMB_MSG_BASE+3)	   
#define	NU_CONT_RMB_FORCESTOP_MSG	(NU_CONT_RMB_MSG_BASE+4)
#define	NU_CONT_RMB_FORCESTOPEND_MSG	(NU_CONT_RMB_MSG_BASE+5)

/*----------------------------------------------------------------------*/
/* EEPROM								*/
/*----------------------------------------------------------------------*
#define NU_EEPROM_MSG_BASE		NU_SI_MAJOR_NO_EEPROM
#define NU_EEPROM_RETRACE_MSG		(NU_EEPROM_MSG_BASE+0)
#define NU_EEPROM_CHECK_MSG		(NU_EEPROM_MSG_BASE+1)
#define	NU_EEPROM_READ_MSG		(NU_EEPROM_MSG_BASE+2)
#define NU_EEPROM_WRITE_MSG		(NU_EEPROM_MSG_BASE+3)


/*----------------------------------------------------------------------*/
/* 64GB Pack  Manager							*/
/*----------------------------------------------------------------------*
#define NU_CONT_GBPAK_MSG_BASE		NU_SI_MAJOR_NO_GBPAK
#define NU_CONT_GBPAK_RETRACE_MSG	(NU_CONT_GBPAK_MSG_BASE+0)
#define NU_CONT_GBPAK_OPEN_MSG		(NU_CONT_GBPAK_MSG_BASE+1)
#define NU_CONT_GBPAK_STATUS_MSG	(NU_CONT_GBPAK_MSG_BASE+2)
#define NU_CONT_GBPAK_POWER_MSG		(NU_CONT_GBPAK_MSG_BASE+3)
#define NU_CONT_GBPAK_READID_MSG	(NU_CONT_GBPAK_MSG_BASE+4)
#define NU_CONT_GBPAK_READWRITE_MSG	(NU_CONT_GBPAK_MSG_BASE+5)
#define NU_CONT_GBPAK_CHECKCONNECTOR_MSG (NU_CONT_GBPAK_MSG_BASE+6)

#define NU_CONT_GBPAK_POWER_OFF		OS_GBPAK_POWER_OFF
#define NU_CONT_GBPAK_POWER_ON		OS_GBPAK_POWER_ON
#define	NU_CONT_GBPAK_MBC_RAM_ENABLE_CODE	0x0a
#define	NU_CONT_GBPAK_MBC_RAM_DISABLE_CODE	0x00
#define	NU_CONT_GBPAK_MBC_RAM_REG0_ADDR		0x0000

#define NU_CONT_GBPAK_MBC_REG0_ADDR		0x0000	/* Register 0	*
#define NU_CONT_GBPAK_MBC_REG1_ADDR		0x2100	/* Register 1	*
#define NU_CONT_GBPAK_MBC_REG2_ADDR		0x4000	/* Register 2	*
#define NU_CONT_GBPAK_MBC_REG3_ADDR		0x6000	/* Register 3	*/

/*----------------------------------------------------------------------*/
/*  Voice Recognition System Manager						*/
/*----------------------------------------------------------------------*
#define NU_VRS_MSG_BASE			NU_SI_MAJOR_NO_VRS
#define	NU_VRS_RETRACE_MSG		(NU_VRS_MSG_BASE+0)
#define	NU_VRS_OPEN_MSG			(NU_VRS_MSG_BASE+1)
#define	NU_VRS_CLEAR_DICTIONARY_MSG	(NU_VRS_MSG_BASE+2)
#define	NU_VRS_SET_WORD_MSG		(NU_VRS_MSG_BASE+3)
#define	NU_VRS_MASK_DICTIONARY_MSG	(NU_VRS_MSG_BASE+4)
#define	NU_VRS_START_READ_DATA_MSG	(NU_VRS_MSG_BASE+5)
#define	NU_VRS_GET_READ_DATA_MSG	(NU_VRS_MSG_BASE+6)
#define	NU_VRS_STOP_READ_DATA_MSG	(NU_VRS_MSG_BASE+7)
#define	NU_VRS_CONTROL_GAIN_MSG		(NU_VRS_MSG_BASE+8)

/*----------------------------------------------------------------------*/
/* PI DEFINE								*/
/*----------------------------------------------------------------------*
#define	NU_PI_MESG_NUM			50	/* PI message buffer size *
#define	NU_PI_CART_BLOCK_READ_SIZE	0x4000	/* cart read block size	*/

	/*--------------------------------------*/
	/* PI Common Message 			*/
	/*--------------------------------------*
	typedef struct st_PiOverlaySegment {
		u8* romStart;		/* Segment's ROM start offset          *
		u8* romEnd;			/* Segment's ROM end offset            *
		u8*	ramStart;		/* Segment's CPU start address         *
		u8* textStart;		/* test attribute's DRAM start address *
		u8* textEnd;		/* test attribute's DRAM end address   *
		u8* dataStart;		/* data attribute's DRAM start address *
		u8* dataEnd;		/* data attribute's DRAM end address   *
		u8* bssStart;		/* bss attribute's DRAM start address  *
		u8*	bssEnd;			/* bss attribute's DRAM start address  *
	} NUPiOverlaySegment;

	/*--------------------------------------*/
	/* SI Common  message 			*/
	/*--------------------------------------*
	typedef struct st_SiCommonMesg {
		NUScMsg		mesg;
		OSMesgQueue*	rtnMesgQ;
		s32			error;
		void*		dataPtr;
	} NUSiCommonMesg;

	/*--------------------------------------*/
	/* controller typedef			*/
	/*--------------------------------------*
	typedef struct st_ContRead {
		NUScMsg	mesg;			/* Type of message *
		OSMesgQueue* rtnMesgQ;
		OSContPad*	pad;
	} NUContReadMesg;

	typedef struct st_ContData {
		u16	button;			/* A,B,C,Z,L,R,START,PAD ,button 	*
		s8	stick_x;		/* Range acutally used:   -61 <= stick X <= 61	*
		s8	stick_y;		/* Range actually used:  -63 <= stick Y <= 63	*
		u8	errno;		/* Error				*
		u16	trigger;		/* Button trigger data		*
	} NUContData;

	typedef struct st_ContQuery {
		NUScMsg	mesg;
		OSMesgQueue* rtnMesgQ;
	} NUContQueryMesg;


	typedef struct st_ContPakFile {
		OSPfs	*pfs;
		s32		file_no;
		s32		error;
		u32		type;
	} NUContPakFile;


	typedef struct st_ContPakFreeMesg {
		NUContPakFile*	file;
		s32			size;
	} NUContPakFreeMesg;

	typedef struct st_ContPakFileOpenMesg {
		NUContPakFile*	file;
		u8*			noteName;
		u8*			extName;
		s32			size;
		s32			mode;
	} NUContPakFileOpenMesg;

	typedef struct st_ContPakFileRWMesg {
		NUContPakFile*	file;
		s32			offset;
		s32			size;
		u8			mode;
		u8*			buf;
	} NUContPakFileRWMesg;

	typedef struct st_ContPakFileStateMesg {
		NUContPakFile*	file;
		OSPfsState*		state;
	} NUContPakFileStateMesg;

	typedef struct st_ContPakFileNumMesg {
		NUContPakFile*	file;
		s32*		max_files;
		s32*		used_files;
	} NUContPakFileNumMesg;

	typedef struct st_ContPakIsPlugMesg {
		NUScMsg		mesg;
		OSMesgQueue*	rtnMesgQ;
		s32			error;
		u8			bitPattern;
	} NUContPakIsPlugMesg;

	typedef struct st_ContRmbCtl {
		u16			freq;		/* Vibration frequency *
		u16			frame;
		u16			counter;
		u8			state;
		u8			mode;
		u8			autorun;
		u8			type;
	} NUContRmbCtl;

	typedef struct st_ContRmbMesg {
		u8			contNo;
		s32			error;
		void*		data;
	} NUContRmbMesg;

	typedef struct st_CallBackList {
		struct st_CallBackList* 	next;
		s32(**func)(NUSiCommonMesg*);
		u16				majorNo;
		u8    			funcNum;
	} NUCallBackList;

	typedef struct st_EepromMesg {
		void*		dataPtr;
		s32			error;
		u8			address;
		u16			dataLen;
	} NUEepromMesg;

	typedef struct st_ContGBPakMesg {
		NUContPakFile*	handle;
		s32			data[4];
	} NUContGBPakMesg;

	typedef OSVoiceHandle NUVrsHandle;

	typedef OSVoiceData NUVrsData;

	typedef struct stVrsMesg {
		NUVrsHandle*	vrsHandle;
		s32			data[2];
	} NUVrsMesg;

	/*--------------------------------------*/
	/* controller  Manager function		*/
	/*--------------------------------------*
	extern u8   nuContInit(void);
	extern s32  nuContMgr(void* mesg);
	extern u8   nuContMgrInit(void);
	extern void nuContMgrRemove(void);
	extern void nuContDataLock(void);
	extern void nuContDataUnLock(void);
	extern void nuContDataRead(OSContPad* pad);
	extern void nuContDataReadWait(void);
	extern void nuContDataReadStart(void);
	extern void nuContDataGet(OSContPad* contdata, u32 padno);
	extern void nuContDataGetAll(OSContPad* contdata);
	extern void nuContDataGetEx(NUContData *contdata, u32 padno);
	extern void nuContDataGetExAll(NUContData *contdata);
	extern void nuContReadFuncSet(NUContReadFunc func);
	extern void nuContQueryRead(void);
	extern void nuContDataClose(void);
	extern void nuContDataOpen(void);

	/*--------------------------------------*/
	/* controller pak function		*/
	/*--------------------------------------*
	extern s32  nuContPakMgr(void* mesg_type);
	extern void nuContPakMgrInit(void);
	extern void nuContPakMgrRemove(void);
	extern void nuContPakJisToN64(u8* src, u8* dest, u32 len);
	extern void nuContPakN64ToJis(u8* src, u8* dest, u32 len);
	extern s32  nuContPakOpen(NUContPakFile *file, u32 contNo);
	extern s32  nuContPakGetFree(NUContPakFile *file);
	extern void nuContPakCodeSet(u8* companyCode, u8* gameCode);
	extern s32  nuContPakRepairId(NUContPakFile *file);
	extern s32  nuContPakFileOpen(NUContPakFile *file,
		u8* noteName, u8* extName, u32 mode, s32 size);
	extern s32  nuContPakFileOpenJis(NUContPakFile *file,
		u8* noteName, u8* extName, u32 mode, s32 size);
	extern s32  nuContPakFileReadWrite(NUContPakFile *file,
		s32 offset, s32 size, u8* buf, u32 mode);
	extern s32  nuContPakFileDelete(NUContPakFile *file, u8* noteName, u8* extName);
	extern s32  nuContPakFileDeleteJis(NUContPakFile *file, u8* noteName, u8* extName);
	extern s32  nuContPakFileState(NUContPakFile *file, OSPfsState* state);
	extern s32  nuContPakFileNum(NUContPakFile *file,
		s32 *max_files, s32 *used_files);
	extern s32 nuContPakFileFread(NUContPakFile *file, s32 offset, s32 size, u8* buf);

	/*--------------------------------------*/
	/* rumbler pak functions			*/
	/*--------------------------------------*
	extern void nuContRmbMgrInit(void);
	extern s32  nuContRmbMgr(void* mesg);
	extern void nuContRmbMgrRemove(void);
	extern void nuContRmbStart(u32 contNo, u16 freq, u16 frame);
	extern void nuContRmbStop(u32 contNo);
	extern s32  nuContRmbCheck(u32 contNo);
	extern void nuContRmbModeSet(u32 contNo, u8 mode);
	extern void nuContRmbForceStop(void);
	extern void nuContRmbForceStopEnd(void);
	extern void nuContRmbSearchTimeSet(u32 frame);

	/*--------------------------------------*/
	/* 64GB Pack functions			*/
	/*--------------------------------------*
	extern s32 nuContGBPakMgr(void* mesg);
	extern void nuContGBPakMgrInit(void);
	extern void nuContGBPakMgrRemove(void);
	extern s32 nuContGBPakOpen(NUContPakFile* handle, s32 contNo);
	extern s32 nuContGBPakGetStatus(NUContPakFile* handle, u8* status);
	extern s32 nuContGBPakReadID(NUContPakFile* handle, OSGbpakId* id, u8* status);
	extern s32 nuContGBPakReadWrite(NUContPakFile* handle, u16 flag, u16 address, u8* buffer, u16 size);
	extern s32 nuContGBPakPower(NUContPakFile* handle, s32 flag);
	extern s32 nuContGBPakCheckConnector(NUContPakFile* handle, u8* status);
	extern s32 nuContGBPakFread(NUContPakFile* handle, u16 address, u8* buffer, u16 size);

	extern s32 nuContGBPakFwrite(NUContPakFile* handle, u16 address, u8* buffer, u16 size);
	extern s32 nuContGBPakRegWrite(NUContPakFile* handle, u16 addr, u8 data);

	/*--------------------------------------*/
	/* eeprom functions			*/
	/*--------------------------------------*
	extern void nuEepromMgrInit(void);
	extern s32  nuEepromMgr(void* mesg);
	extern s32  nuEepromCheck(void);
	extern void nuEepromMgrRemove(void);
	extern s32 nuEepromRead(u8 address, u8* buffer, u16 len);
	extern s32 nuEepromWrite(u8 address, u8* buffer, u16 len);


	/*--------------------------------------*/
	/* pi functions				*/
	/*--------------------------------------*
	extern void nuPiInit(void);
	extern void nuPiReadRom(u32 rom_addr, void* buf_ptr, u32 size);
	extern void nuPiInitSram(void);
	extern void nuPiInitDDrom(void);
	extern void nuPiReadWriteSram(u32 addr, void* buf_ptr, u32 size, s32 flag);
	extern void nuPiReadRomOverlay(NUPiOverlaySegment* segment);

	/*--------------------------------------*/
	/* si functions				*/
	/*--------------------------------------*
	extern u8 nuSiMgrInit(void);
	extern void nuSiCallBackAdd(NUCallBackList* list);
	extern void nuSiCallBackRemove(NUCallBackList* list);
	extern s32 nuSiSendMesg(NUScMsg mesg, void* dataPtr);
	extern void nuSiSendMesgNW(NUScMsg mesg, void* dataPtr);
	extern void nuSiMgrStop(void);
	extern void nuSiMgrRestart(void);
	/*--------------------------------------*/
	/* si functions				*/
	/*--------------------------------------*
	extern s32 nuVrsMgr(void *mesg);
	extern void nuVrsMgrInit(void);
	extern s32 nuVrsOpen(NUVrsHandle* handle, s32 contNo);
	extern s32 nuVrsClearDictionary(NUVrsHandle* handle, u8 words);
	extern s32 nuVrsSetWord(NUVrsHandle* handle, u8* word);
	extern s32 nuVrsStartReadData(NUVrsHandle* handle);
	extern s32 nuVrsGetReadData(NUVrsHandle* handle, NUVrsData* data);
	extern s32 nuVrsStopReadData(NUVrsHandle* handle);
	extern s32 nuVrsControlGain(NUVrsHandle* handle, s32 analog, s32 digital);
	extern s32 nuVrsMaskDictionary(NUVrsHandle* handle, u8* maskpattern, s32 size);

/*----------------------------------------------------------------------*/
/*	nuContDataGet - Obtains Controller data			*/
/*	Obtains read data from the Controller Manager.		*/
/*									*/
/*	IN:	*contpad  	Pointer to the Controller structure			*/
/* 		cont_no  	Controller number				*/
/*----------------------------------------------------------------------*
#if 0
#define	nuContDataGet(contpad, cont_no)					\
{									\
    bcopy(&nuContData[cont_no], contpad, sizeof(OSContPad));		\
}

/*----------------------------------------------------------------------*/
/*	nuContDataGetAll - Obtains Controller data			*/
/*	Obtains read data from the Controller Manager.			*/
/*	Copies all 4 buffers.			*/
/*	IN:	*contpad  Pointer to the Controller structure		*/
/* 		cont_no  Controller number				*/
/*----------------------------------------------------------------------*
#define nuContDataGetAll(contpad)					\
{									\
    bcopy(&nuContData[0], contpad, sizeof(OSContPad)*MAXCONTROLLERS);\
}
#endif
/*----------------------------------------------------------------------*/
/*	nuContPakFileFind - Search node			*/
/*----------------------------------------------------------------------*
#define nuContPakFileFind(file, nodeName, extName)			\
    nuContPakFileOpen(file, nodeNname, extName, NU_CONT_PAK_NOCREAT, 0)\


/*----------------------------------------------------------------------*/
/*	nuContPakFileFindJis - Search node					*/
/*----------------------------------------------------------------------*
#define nuContPakFileFindJis(file, nodeName, extName)			\
    nuContPakFileOpenJis(file, nodeNname, extName, NU_CONT_PAK_NOCREAT, 0)\

/*----------------------------------------------------------------------*/
/*	nuContPakFileRead - Read note				*/
/*----------------------------------------------------------------------*
#define nuContPakFileRead(file, offset, size, buf)			\
    nuContPakFileReadWrite(file, offset, size, buf, PFS_READ)		\

/*----------------------------------------------------------------------*/
/*	nuContPakFileWrite - Read note				*/
/*----------------------------------------------------------------------*
#define nuContPakFileWrite(file, offset, size, buf)			\
    nuContPakFileReadWrite(file, offset, size, buf, PFS_WRITE)		\

/*----------------------------------------------------------------------*/
/*	nuContGBPakRead - Read from the GB cartridge			*/
/*----------------------------------------------------------------------*
#define nuContGBPakRead(file, address, buf, size)			\
    nuContGBPakReadWrite(file, OS_READ, address, buf, size)

/*----------------------------------------------------------------------*/
/*	nuContGBPakWrite - Write to the GB cartridge		*/
/*----------------------------------------------------------------------*
#define nuContGBPakWrite(file, address, buf, size)			\
    nuContGBPakReadWrite(file, OS_WRITE, address, buf, size)

/*----------------------------------------------------------------------*/
/*	nuPiReadSram - Read from SRAM				*/
/*----------------------------------------------------------------------*
#define nuPiReadSram(addr, buf_ptr, size)				\
	nuPiReadWriteSram(addr, buf_ptr, size, OS_READ)

/*----------------------------------------------------------------------*/
/*	nuPiWriteSram - Write to SRAM				*/
/*----------------------------------------------------------------------*
#define nuPiWriteSram(addr, buf_ptr, size)				\
    nuPiReadWriteSram(addr, buf_ptr, size, OS_WRITE)

/*----------------------------------------------------------------------*/
/*	Define voice recognition macros					*/
/*----------------------------------------------------------------------*
#define	nuVrsCheckWord		osVoiceCheckWord
#define	nuVrsCountSyllables	osVoiceCountSyllables
