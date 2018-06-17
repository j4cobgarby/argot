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
        RESPSTAT_ERROTHER,

        RESPSTAT_STATUS_COUNT
    };
}

