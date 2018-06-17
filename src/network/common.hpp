/*
 * =====================================================================================
 *
 *       Filename:  common.hpp
 *
 *    Description:  common declarations
 *
 *        Created:  17/06/18 15:35:45
 *       Compiler:  gcc
 *
 *         Author:  Jacob Garby (), thyself@jacobgarby.co.uk
 *
 * =====================================================================================
 */

namespace argot {
    enum msg_types {
        MSGTYPE_BLANK=0,
        MSGTYPE_SETUP_REQ,
        MSGTYPE_SETUP_RESP,
        MSGTYPE_CHATMSG,
        MSGTYPE_REQUEST_MOTD,

        MSGTYPE_TYPES
    };
    
    enum setup_resp_statuses {
        RESPSTAT_OK=0,
        RESPSTAT_NICKNAME_TAKEN,
        RESPSTAT_FIELD_OMITTED,
        RESPSTAT_INVALIDCHARS,
        RESPSTAT_ERROTHER,

        RESPSTAT_STATUS_COUNT
    };
    
#define PERM_SENDMSG            0b1
#define PERM_KICKNORMAL         0b10
#define PERM_KICKANY            0b100
#define PERM_BANNORMAL          0b1000
#define PERM_BANANY             0b10000
#define PERM_CHANGEMOTD         0b100000
#define PERM_CHANGEMAXCLIENTS   0b1000000
#define PERM_PRIVATEMSG         0b10000000

#define PERMS_DEFAULT 0b10000001 /* SENDMSG and PRIVATEMSG */
#define PERMS_ALL 0xff          /* ALL PERMS */
}

