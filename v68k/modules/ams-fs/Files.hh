/*
	Files.hh
	--------
*/

#ifndef FILES_HH
#define FILES_HH

struct FileParam;
struct IOParam;
struct VolumeParam;
struct WDPBRec;

typedef short (*Open_ProcPtr)( short trap_word : __D1, FileParam* pb : __A0 );
typedef short (*IO_ProcPtr  )( short trap_word : __D1, IOParam*   pb : __A0 );

extern Open_ProcPtr old_Open;
extern IO_ProcPtr   old_Close;
extern IO_ProcPtr   old_Read;
extern IO_ProcPtr   old_Write;

void initialize();

short GetVol_patch( short trap_word : __D1, WDPBRec* pb : __A0 );

short FlushVol_patch( short trap_word : __D1, VolumeParam* pb : __A0 );

short Create_patch( short trap_word : __D1, FileParam* pb : __A0 );

short Open_patch  ( short trap_word : __D1, IOParam* pb : __A0 );
short OpenRF_patch( short trap_word : __D1, IOParam* pb : __A0 );
short Read_patch  ( short trap_word : __D1, IOParam* pb : __A0 );
short Write_patch ( short trap_word : __D1, IOParam* pb : __A0 );

short Close_patch( short trap_word : __D1, IOParam* pb : __A0 );

#endif
