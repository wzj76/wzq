#ifndef __APP_DEF_H__
#define __APP_DEF_H__

#include <map>
#include <deque>
#include <vector>
#include <string>
#include <set>
#include <string>
#include <google/protobuf/stubs/common.h>
#include "datatype.h"
#include "Tools.h"
#include <algorithm>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef bool bit;
typedef unsigned long long ullong;
typedef long long llong;
using google::protobuf::int32;
using google::protobuf::int64;
using namespace std;

#define IsNULL(p)		((p) == NULL)
#define IsNotNULL(p)	((p) != NULL)
#define IsZero(n)		((n) == 0)
#define IsNotZero(n)	((n) != 0)
#define IsGeZero(n)     ((n) >= 0)

#define DB_HOST                "DBHost"                //
#define DB_PORT                "DBPort"                //
#define DB_USERNAME            "DBUserName"               //
#define DB_PASSWORD            "DBPassword"            //
#define DB_NAME                "DBName"                //
#define DB_CONN_NUM            "DBConnNum"              //

#endif








