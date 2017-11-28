#ifndef DATATYPE_H
#define DATATYPE_H

typedef struct {							//address group inforamtion 
	u16  Adr;
	u8 Len;
	u8 Group;
}ADRLEN_INF;
typedef struct {							//address group inforamtion 
	u32  Adr;
	u32 Len;
	u32 Group;
}ADRLEN_GPSRecordInf;

#endif