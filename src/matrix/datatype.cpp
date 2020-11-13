#include <cstring>
#include "datatype.h"

Data_Type name_to_dt(const char *name)
{
    if (!strcmp(name, "double"))
        return Data_Type::dt_double;
    else if (!strcmp(name, "single"))
        return Data_Type::dt_single;
    else if (!strcmp(name, "int8"))
        return Data_Type::dt_int8;
    else if (!strcmp(name, "int16"))
        return Data_Type::dt_int16;
    else if (!strcmp(name, "int32"))
        return Data_Type::dt_int32;
    else if (!strcmp(name, "int64"))
        return Data_Type::dt_int64;
    else if (!strcmp(name, "uint8"))
        return Data_Type::dt_uint8;
    else if (!strcmp(name, "uint16"))
        return Data_Type::dt_uint16;
    else if (!strcmp(name, "uint32"))
        return Data_Type::dt_uint32;
    else if (!strcmp(name, "uint64"))
        return Data_Type::dt_uint64;
    else
        return Data_Type::dt_wrong;
}
