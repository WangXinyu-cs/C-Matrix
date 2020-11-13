#ifndef _DATATYPE_H_
#define _DATATYPE_H_

enum Data_Type
{
    dt_wrong,
	dt_double,
	dt_single,
	dt_int8,
	dt_int16,
	dt_int32,
	dt_int64,
	dt_uint8,
	dt_uint16,
	dt_uint32,
	dt_uint64
};

extern Data_Type name_to_dt(const char *);

#endif
