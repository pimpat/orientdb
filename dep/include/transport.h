#ifndef __TRANSPORT_H_INCLUDED__
#define __TRANSPORT_H_INCLUDED__

#ifdef __APPLE__
    #include <uuid/uuid.h>
#else
    #include <uuid.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __cplusplus

extern "C" {
    
#endif

typedef struct TPTransportCTX TPTransportCTX;
typedef struct TPRouterCTX TPRouterCTX;
typedef struct TPConfigStruct TPConfigStruct;

typedef uuid_t TPDeviceID;
typedef uuid_t TPUserID;
typedef uuid_t TPGroupID;

// definition of transport message
#ifdef __cplusplus
typedef class TPMsgType TPMsgType;
#else
typedef enum TPMsgType TPMsgType;
#endif
typedef struct {
    enum TPMsgType {
        TP_MSG_TYPE,
        TP_MSG_TYPE_CHAT,
        TP_MSG_TYPE_CWDATA,
        TP_MSG_TYPE_CWCONTROL,
        TP_MSG_TYPE_CHATCONTROL,
    } msgType;
    unsigned int bytesCount;
    char *msg;
    char from[16];
    char to[16];
    time_t timestamp;
} TPTransportMsg;

/// Client
// 1) In order to use any transport, you need to init it first.
int TPTransportInit(TPTransportCTX **context, TPDeviceID *deviceID, struct sockaddr_in ipAddr, in_port_t port2);
int TPTransportInitFromConfig(TPTransportCTX **context, TPConfigStruct *config);
int TPTransportInitFromConfigFile(TPTransportCTX **context, const char *path);

// 2) authentication: before start communcation from any client,
// we need to authenticated our client first.
// we can authenticated multiple time at once
// need to see current backend of what is possible
int TPLoginUser(TPTransportCTX *context, TPUserID *userID);
int TPLogoutUser(TPTransportCTX *context, TPUserID *userID);

// 2.1) list presence user
typedef struct {
    int userLen;
    struct TPUserStatus {
        TPUserID userID;
        enum { TP_STATUS_OFFLINE, TP_STATUS_ONLINE } status;
    } *users;
} TPArrayUser;
/* TPListAllUser
 * return result code
 * context[in]:
 * arrayUser[out]: result, struct TPArrayUser
 */
int TPListAllUser(TPTransportCTX *context, TPArrayUser **arrayUser);
void TPArrayUserFree(TPArrayUser *arrayUser);

// 2.2) list group
/* TPListGroup
 * return number of group
 * context[in]:
 * userID[in]: user that get list group
 * groupIDs[out]: result, array of groupID
 */
int TPListGroup(TPTransportCTX *context, TPUserID *userID, TPGroupID **groupIDs);
int TPGroupIDsFree(TPGroupID *groupIDs);
/*
 * TPListMemberInGroup
 * return result code
 * context[in]:
 * userID[in]: user that list group
 * groupID[in]: 
 * arrayUser[out]: result, strutc TPArrayUser
 */
int TPListMemberInGroup(TPTransportCTX *context, TPUserID *userID, TPGroupID *groupID, TPArrayUser **arrayUser);

// 3) start using transport
int TPGroupNew(TPTransportCTX *tContext, TPGroupID *groupRef);
int TPGroupJoin(TPTransportCTX *context, TPUserID *userID, TPGroupID *groupRef);
int TPGroupLeave(TPTransportCTX *context, TPUserID *userID, TPGroupID *groupRef);
// send message
int TPSendToClient(TPTransportCTX *context, TPDeviceID *deviceID, TPTransportMsg *message);
int TPSendToUser(TPTransportCTX *context, TPUserID *myUserID, TPUserID *targetUserID, TPTransportMsg *message);
int TPSendToGroup(TPTransportCTX *context, TPUserID *userID, TPGroupID *groupRef, TPTransportMsg *message);
int TPSendToSelf(TPTransportCTX *context, TPTransportMsg *message);
int TPSendTagToSelf(TPTransportCTX *context, TPTransportMsg *message, unsigned int tag);

// pick up message in queue with user id, and message type
TPTransportMsg* TPMsgGet(TPTransportCTX *context, TPUserID *myUserID, TPTransportMsg::TPMsgType msgType);
int TPMsgGetFromDB(TPTransportCTX *tContext, TPGroupID *groupRef, time_t timestamp, int limit, TPTransportMsg ***arrayResult);
int TPMsgGetFromDBWithSender(TPTransportCTX *context, void *refID, int refIDSize, void *senderID, int senderIDSize, time_t timestamp, int limit, TPTransportMsg ***arrayResult);
void TPMsgFree(TPTransportMsg* msg);
int TPMsgCallbackSet(TPTransportCTX *context, TPTransportMsg::TPMsgType msgType, void (*callback)(const TPTransportMsg *message));

long TPGetOffsetTime(TPTransportCTX *context);

// 4) close transport
int TPTransportClose(TPTransportCTX *context);
// 4.1) reset
int TPTransportSocketReset(TPTransportCTX *context);
int TPTransportSocketPause(TPTransportCTX *context);
int TPTransportSocketResume(TPTransportCTX *context);

// use in FTO-log-transport
int TPTagFuncCallSet(TPTransportCTX *tContext, unsigned int (*callbackFucntion)(TPTransportMsg *message), unsigned int tag);
void TPTagFuncCallRemove(unsigned int tag);
int TPSendTagToClient(TPTransportCTX *context, TPDeviceID *deviceID, TPTransportMsg *message, unsigned int tag);
int TPSendTagToGroup(TPTransportCTX *context, TPUserID *userID, TPGroupID *groupRef, TPTransportMsg *message, unsigned int tag);

/// Router
int TPRouterStart(TPRouterCTX **router, TPDeviceID *deviceID, struct sockaddr_in ipAddr, const char *dbDir);
int TPRouterStartFromConfigFile(TPRouterCTX **router, const char *path);
int TPRouterStop(TPRouterCTX *router);

typedef enum {
    TP_CONFIG,
    TP_CONFIG_ID,
    TP_CONFIG_IPADDR,
    TP_CONFIG_ROUTERIPADDR,
    TP_CONFIG_DBDIR,
    TP_CONFIG_PORTPOOL,
    TP_CONFIG_UUID,
    TP_CONFIG_SHADOWUSER
} TPConfigOption;
int TPConfigStructInit(TPConfigStruct **config);
void TPConfigStructFree(TPConfigStruct *config);
int TPConfigSet(TPConfigStruct *config, TPConfigOption configOption, const void *configVal, int configLen);
int TPConfigGet(TPTransportCTX *context, TPConfigOption configOption, void *configVal, int *configLen);

int TPGetShadowFromRouter(TPTransportCTX *context);
int TPMsgGetFromDBByRange(TPTransportCTX *context, void *refID, int refIDSize, time_t start ,time_t end, int limit, TPTransportMsg ***arrayResult);
int TPGetMsgFromShadow(TPTransportCTX *tcontext, void *refID,int refIDsize,time_t start_time,time_t end_time,int limit,TPTransportMsg **arrayMsg);
int TPSendToUserAndCopyToSelf(TPTransportCTX *tContext, TPUserID *myUserID, TPUserID *userID, TPTransportMsg *message);
int TPSendToUserSelf(TPTransportCTX *tContext, TPUserID *myUserID, TPUserID *userID, TPTransportMsg *message);
#ifdef __cplusplus
}
#endif

#endif
