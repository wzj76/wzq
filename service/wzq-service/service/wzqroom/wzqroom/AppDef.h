
#ifndef APP_DEF_H_
#define APP_DEF_H_
#include <map>
#include <deque>
#include <vector>
#include <string>
#include "Tools.h"
#include <google/protobuf/stubs/common.h>
#include "datatype.h"

using google::protobuf::int32;
using google::protobuf::int64;

typedef unsigned char byte;
const int MAX_USER_ID_LEN = 64;
const int SESSION_KEY_LEN = 32;
const int MAX_PLAYERS_PER_TABLE = 9;
const int MAX_COMM_MSG_LEN = 4096;
const int CARDS_NUM = 52;
const int MSG_SEQ_NO_LEN = 18;
const int MAX_ROBOT_IP_LEN = 20;
const int MAX_NICK_NAME_LEN = 80;

const int AWAITING_NOBODY = -1;
const int AWAITING_LE_ONE = -2;

const int TABLE_ACTION_SEAT_ID = 9;

#define AUTO_OPTION_CHECK 1
#define AUTO_OPTION_FOLD 2
#define AUTO_OPTION_CALL_ANY 4

#define IsNULL(p)		((p) == NULL)
#define IsNotNULL(p)	((p) != NULL)
#define IsZero(n)		((n) == 0)
#define IsNotZero(n)	((n) != 0)

#endif //APP_DEF_H_
