#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

//#include "zmq.h"
//#include "zhelpers.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>

#include <uuid/uuid.h>
//#include <jansson.h>
#include "reqmsg.h"
#include "swap_endian.h"
#include "diff_match_patch.h"
#include "ezxml.h"

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 2424

#define DB_NAME "datamodel_xml"
#define MAX_SQL_SIZE (10000)
#define MAX_DIFF_SIZE (1000)
#define MAX_CONTENT_SIZE (10000)
#define VER_KEEPED (5)

#define TRUE 1
#define FALSE 0

using namespace std;

int Sockfd;
typedef struct Packet {
    char opType;
    char ssid[4];
    char msg[20000];
} Packet;
Packet GPacket;

//typedef struct Addr{
//    short cltid;
//    long rid;
//} Addr;
//Addr addr;

//----------------------------------

//typedef struct ChatRef{
//    const char* chatRefURL;
//} ChatRef;

typedef struct CWTimeStamp{
    char* timeStampCode;
} CWTimeStamp;

typedef char* Text;
typedef char Schema;

typedef struct ObjectBinary {
    int schemaCode;
    int byteCount;
    char* data;
} ObjectBinary;

typedef struct DataHolder DataHolder;
typedef int t_bool;
typedef struct DataContent{
    DataHolder* dataHd;
    DataContent* nextVersion;
    DataContent* preVersion;
    char* SHA256hashCode;
    CWTimeStamp* timeStamp;
    ObjectBinary* plusPatch;
    ObjectBinary* minusPatch;
    ObjectBinary* fullContent;
    t_bool isDiff;
} DataContent;

struct DataHolder{
    DataContent* lastestCommon;
    DataContent* head;
    DataContent* current;
    int versionKeeped;
};

typedef struct Data{
    const char* dataID;
    char* dataName;
    int dataType;
    DataHolder* content;
//    Text* attachmentFTOLinks;
    char* chatRoom;
} Data;

typedef Data Org;
typedef Data User;
typedef Data Category;
typedef Data State;
typedef Data Task;
typedef Data SubTask;

typedef int ReturnErr;

extern Data* mydata;
Data* mydata = NULL;

enum DATATYPE {
    _data,
    _subTask,
    _task,
    _state,
    _category,
    _user,
    _org
};

enum EDGETYPE {
    _toUser,
    _toCategory,
    _toState,
    _toTask,
    _toSubTask,
};

enum TAGNAME {
    _attachmentFTOLinks,
    _content,
    _book_id,
    _author,
    _title,
    _genre,
    _price
};

/* binary protocol */
int connectSocket();
int connectServer();
int disconnectServer();
int createDatabase(char *dbName);
int createClass(char *myclass, char *extend);
int prepareDB();
int openDatabase(char *dbName);
int sendCommand(char *command);
int queryRecord(char *query);
int loadRecord(short clusterId, long clusterPos);
int createVertex(char *command, short* cltid, long* rid);

char* getRid(char *query);
char* getContent(char *query);
char** getArrayRid(char* query);
char** getArrayDistinctRid(char* query);

/* datamodel function */
const char* stringUUID();
const char* createNewData(int dataType);                // global Data
const char* _createNewData(Data** data, int dataType);  // local Data
DataHolder* createNewDataHolder();
ObjectBinary* createNewObjBinary(char* data);
ObjectBinary* _createNewObjBinary(char* data, int dataType);
int countDataContent(Data* data);

void setDataName(Data* data, char* dataName);
void setChatRoom(Data* data, char* chatRoom);
char* getDataName(Data* data);
char* getChatRoom(Data* data);

ReturnErr setDataNameByID(char* dataID, char* dataName);
ReturnErr setChatRoomByID(char* dataID, char* chatRoom);
char* getDataNameByID(char* dataID);
char* getChatRoomByID(char* dataID);

ObjectBinary* getDataContent(Data* data);
ObjectBinary* getDataContentLastestCommon(Data* data);
ObjectBinary* getDataContentByID(char* dataID);
ObjectBinary* getDataContentLastestCommonByID(char* dataID);

Text getDiffDataAtHead(Data* data);
Text getDiffDataAtLastestCommon(Data* data);
//Text getDiffDataAtHeadByID(char* dataID);
//Text getDiffDataAtLastestCommonByID(char* dataID);

ObjectBinary* getContentNextVer(Data* data);
ObjectBinary* getContentPreVer(Data* data);
//ObjectBinary* getContentNextVerByID(char* dataID);
//ObjectBinary* getContentPreVerByID(char* dataID);

Text getDiffDataNextVer(Data* data);
Text getDiffDataPreVer(Data* data);
//Text getDiffDataNextVerByID(char* dataID);
//Text getDiffDataPreVerByID(char* dataID);

Text getDataContentWithTag(Data* data, char* tagName, char* id);
//Text getDataContentWithTagByID(char* dataID, char* tagName);

ReturnErr setNewDataContent(Data* data, ObjectBinary* content);
ReturnErr setNewDataDiffWithTag(Data* data, char* tagName, char* id, Text diff);

Text getTagContentWithName(ObjectBinary* fullContent, char* tagName, char* id);
Text getTagContent(ObjectBinary* fullContent, int tagNameEnum, char* id);
ReturnErr setTagContent(ObjectBinary* fullContent, Text newTagContent, char* tagName, char* id);

Text* getAttachmentFTOLinks(Data* data);

const char* createOrg(Text orgName, Schema* orgSchema);
const char* createUser(Text userName, Schema* userSchema);
const char* createCategory(Text categoryName, Schema* categorySchema);
const char* createState(Text stateName, Schema* stateSchema);
const char* createTask(Text taskName, Schema* taskSchema);
const char* createSubTask(Text subTaskName, Schema* subTaskSchema);
const char* createData(Text dataName, Schema* dataSchema, int dType);

//Text getDataContentForKey(Schema* schema, uuid_t objID, char* keyName);

ReturnErr addUser2OrgByID(char* orgID, char* userID);
ReturnErr addCategory2UserByID(char* userID, char* categoryID);
ReturnErr addState2CategoryByID(char* categoryID, char* stateID);
ReturnErr addTask2StateByID(char* stateID, char* taskID);
ReturnErr addTask2CategoryByID(char* categoryID, char* taskID);
ReturnErr addSubTask2TaskByID(char* taskID, char* subTaskID);
ReturnErr addData2DataByID(char* f_dataID, char* t_dataID, int dType);

ReturnErr addUser2Org(char* orgID, User* user);
ReturnErr addCategory2User(char* userID, Category* category);
ReturnErr addState2Category(char* categoryID, State* state);
ReturnErr addTask2State(char* stateID, Task* task);
ReturnErr addTask2Category(char* categoryID, Task* task);
ReturnErr addSubTask2Task(char* taskID, SubTask *subTask);
ReturnErr addData2Data(char* dataID, Data* data, int dType);

ReturnErr removeUserFromOrg(char* orgID, char* userID);
ReturnErr removeCategoryFromUser(char* userID, char* categoryID);
ReturnErr removeStateFromCategory(char* categoryID, char* stateID);
ReturnErr removeTaskFromState(char* stateID, char* taskID);
ReturnErr removeTaskFromCategory(char* categoryID, char* taskID);
ReturnErr removeSubTaskFromTask(char* taskID, char* subTaskID);
ReturnErr removeDataFromData(char* f_dataID, char* t_dataID);

Org** queryOrgFromData(char* dataID);
User** queryAllUsersFromData(char* dataID);
Category** queryAllCategoriesFromData(char* dataID);
State** queryAllStatesFromData(char* dataID);
Task** queryAllTasksFromData(char* dataID);
SubTask** queryAllSubTasksFromData(char* dataID);
Data** queryDataFromData(char* dataID, int dType);

Data* queryDataByRid(char* rid);
Data* queryDataByID(char* dataID);

t_bool isObjectOwnedByUser(char* userID, char* objID);
t_bool isObjectUnderCategory(char* categoryID, char* objID);
t_bool isObjectUnderState(char* stateID, char* objID);
t_bool isObjectUnderTask(char* taskID, char* objID);
t_bool isObjectUnderData(char* dataID, char* objID);

ReturnErr deleteObj(char* userID, char* upperID, char* objID);
ReturnErr flushTrash(char* userID);

/* diff-patch function */
string getDiff(char* old_str, char* new_str);
string getPatch(char* str, char* str_patch);

void test_setNewData();
void test_getData(Data** data);
void testCRUD(Data** data);
void freeData(Data* data);
void freeObjBinary(ObjectBinary* obj);

std::string& replace(std::string& s, const std::string& from, const std::string& to);
int main() {
    /*
    string s;
    char test_str[]="\"123'4'6\"";
    s.assign(test_str,strlen(test_str));
    std::string str(s);
    printf("before: %s\n",test_str);
    
    string result = replace(s,"'","\\'");
    result = replace(result,"\"","\\\"");
    printf("replace: %s\n",result.c_str());
    */
    test_setNewData();
    return 0;
}

std::string& replace(std::string& s, const std::string& from, const std::string& to)
{
    if(!from.empty())
        for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}

int connectSocket() {
    int sockfd;
    struct sockaddr_in serv_addr;
    
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if ((sockfd) < 0) {
        printf("Error: Could net create socket\n");
        return sockfd;
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("inet_pton error occured\n");
        return -1;
    }
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error: Connect Failed\n");
        return -1;
    }
    read(sockfd, &GPacket, 256);
    
    printf("Connect Socket OK\n");
    
    return sockfd;
}

int connectServer() {
    int ret, size;
    GPacket.opType = REQUEST_CONNECT;
    *(int *)GPacket.ssid = -1;
    size = reqConnectMsg(GPacket.msg, NULL, NULL, 28, NULL, "root", "pimpat");
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5+sizeof(int));
    if (ret<0 || GPacket.opType!=0) return 1;
    
    // copy ssid
    memcpy(GPacket.ssid, GPacket.msg, 4);
    int ssid = *(int *)GPacket.ssid;
    swapEndian(&ssid, INT);
    printf("ssid: %d\n", ssid);
    
    //  for version 28
    int token;
    ret = read(Sockfd, &token, 4);
    
    return 0;
}

int disconnectServer() {
    int ret, size;
    GPacket.opType = 5;
    size = reqDBCloseMsg(GPacket.msg);
    ret = write(Sockfd, &GPacket, 5+size);
    return 0;
}

int createDatabase(char *dbName) {
    int ret, size;
    GPacket.opType = REQUEST_DB_CREATE;
    size = reqDBCreateMsg(GPacket.msg, dbName, "graph", "plocal");
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5+0);
    if (ret<0 || GPacket.opType!=0) return 1;
    
    printf("CREATE DATABASE '%s' plocal: OK!\n", dbName);
    return 0;
}

int createClass(char *myclass, char *extend) {
    int ret;
    char command[256];
    sprintf(command, "CREATE CLASS %s EXTENDS %s", myclass, extend);
    ret = sendCommand(command);
    if (ret!=0) return 1;
    
    printf("%s: OK!\n", command);
    return 0;
}

int createRecord(int cltId, char* recContent){
    int ret, size;
    GPacket.opType = REQUEST_RECORD_CREATE;
    size = reqRecCreateMsg(GPacket.msg, cltId, recContent);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5+0);
    if (ret<0 || GPacket.opType!=0) return 1;
    
    short clusterId;
    read(Sockfd, &clusterId, sizeof(short));
    swapEndian(&clusterId, SHORT);
    printf("clusterId: %d\n", clusterId);
    
    long clusterPos;
    read(Sockfd, &clusterPos, sizeof(long));
    swapEndian(&clusterPos, LONG);
    printf("clusterPos: %d\n", clusterPos);
    
    int recVersion;
    read(Sockfd, &recVersion, sizeof(int));
    swapEndian(&recVersion, INT);
    printf("record-version: %d\n", recVersion);
    
    int count_of_changes;
    read(Sockfd, &count_of_changes, sizeof(int));
    swapEndian(&count_of_changes, INT);
    printf("count_of_changes: %d\n", count_of_changes);
    
    return 0;
}

int prepareDB() {
    int ret;
    
    Sockfd = connectSocket();
    if (Sockfd < 0) return Sockfd;
    printf("connectSocket..PASS\n");
    ret = connectServer();
    if (ret!=0) return 1;
    printf("connectServer...PASS\n");
    
    ret = createDatabase(DB_NAME);
    if (ret!=0) return 1;
    printf("createDB...PASS\n");
    
    /* Edge toUser */
    ret = createClass("toUser","E");
    if (ret!=0) return 1;
    
    /* Edge toCategory */
    ret = createClass("toCategory","E");
    if (ret!=0) return 1;
    
    /* Edge toState */
    ret = createClass("toState","E");
    if (ret!=0) return 1;
    
    /* Edge toTask */
    ret = createClass("toTask","E");
    if (ret!=0) return 1;
    
    /* Edge toSubTask */
    ret = createClass("toSubTask","E");
    if (ret!=0) return 1;
    
    /* DeleteNode */
    ret = createClass("DeleteNode","V");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY DeleteNode.userID STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DeleteNode.userID STRING: OK!\n");
    
    /* Edge toDelete */
    ret = createClass("toDelete","E");
    if (ret!=0) return 1;
    
    /* Edge toDeletedData */
    ret = createClass("toDeletedData","E");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY toDeletedData.dataID STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY toDeletedData.dataID STRING: OK!\n");
    
    /* Data */
    ret = createClass("Data","V");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY Data.dataID STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.dataID STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY Data.dataType INTEGER");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.dataType INTEGER: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY Data.dataName STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.dataName STRING: OK!\n");
    
//    ret = sendCommand("CREATE PROPERTY Data.attachmentFTOLinks EMBEDDEDLIST STRING");
//    if (ret!=0) return 1;
//    //printf("CREATE PROPERTY Data.attachmentFTOLinks EMBEDDEDLIST STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY Data.chatRoom STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.chatRoom STRING: OK!\n");
    
    /* Edge toDataHolder */
    ret = createClass("toDataHolder","E");
    if (ret!=0) return 1;
    
    /* DataHolder */
    ret = createClass("DataHolder","V");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY DataHolder.versionKeeped INTEGER");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataHolder.versionKeeped INTEGER: OK!\n");
    
    /* Edge toDataContent */
    ret = createClass("toDataContent","E");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY toDataContent.type STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY toDataContent.type STRING: OK!\n");
    
    /* DataContent */
    ret = createClass("DataContent","V");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY DataContent.SHA256hashCode STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataContent.SHA256hashCode STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY DataContent.timeStamp STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataContent.timeStamp STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY DataContent.isDiff BOOLEAN");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataContent.isDiff BOOLEAN: OK!\n");
    
    //  minusPatch
    ret = sendCommand("CREATE PROPERTY DataContent.minus_schemaCode INTEGER");
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.minus_byteCount INTEGER");
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.minus_data STRING");
    if (ret!=0) return 1;
    
    //  plusPatch
    ret = sendCommand("CREATE PROPERTY DataContent.plus_schemaCode INTEGER");
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.plus_byteCount INTEGER");
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.plus_data STRING");
    if (ret!=0) return 1;
    
    //  fullContent
    ret = sendCommand("CREATE PROPERTY DataContent.full_schemaCode INTEGER");
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.full_byteCount INTEGER");
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.full_data STRING");
    if (ret!=0) return 1;
    
    /* Edge toContent */
    ret = createClass("toContent","E");
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY toContent.type STRING");
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY toContent.type STRING: OK!\n");
    
    /* Edge toDataHolder_fromDC */
    ret = createClass("toDataHolder_fromDC","E");
    if (ret!=0) return 1;
    
    //  alter class
    ret = sendCommand("ALTER DATABASE CUSTOM useLightweightEdges=false");
    if (ret!=0) return 1;
    printf("ALTER DATABASE CUSTOM useLightweightEdges=false: OK!\n");
    
    disconnectServer();
    close(Sockfd);
    
    printf("prepareDB...PASS\n");
    return 0;
}

int openDatabase(char *dbName) {
    printf("Opening Database...\n");
    int ret, size, i;
    GPacket.opType = REQUEST_DB_OPEN;
    *(int *)GPacket.ssid = -1;
    size = reqDBOpenMsg(GPacket.msg, NULL, NULL, 28, NULL, dbName, "graph", "root", "pimpat");
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5+sizeof(int));
    printf("resSize: %d\n", ret);
    printf("resCode: %d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0) return 1;
    
    // copy ssid
    memcpy(GPacket.ssid, GPacket.msg, 4);
    int ssid = *(int *)GPacket.ssid;
    swapEndian(&ssid, INT);
    printf("ssid: %d\n", ssid);
    
    //  for version 28
    int token;
    ret = read(Sockfd, &token, 4);
    
    //  copy num-of-clusters
    short nCluster;
    ret = read(Sockfd, &nCluster, sizeof(short));
    swapEndian(&nCluster, SHORT);
    printf("num-of-clusters: %hd\n", nCluster);
    
    for (i=0; i<nCluster; i++) {
        char name[128];
        //char type[32];
        short id;//, dataSeg;
        
        //  read info classes
        read(Sockfd, &size, sizeof(int));
        swapEndian(&size, INT);
        read(Sockfd, name, size);
        name[size] = '\0';
        read(Sockfd, &id, sizeof(short));
        swapEndian(&id, SHORT);
        
        //        read(Sockfd, &size, sizeof(int));
        //        swapEndian(&size, INT);
        //        read(Sockfd, type, size);
        //        type[size] = '\0';
        //        read(Sockfd, &dataSeg, sizeof(short));
        //        swapEndian(&dataSeg, SHORT);
        //        printf("name:'%s'\t id:%hd\t type:%s\t dataSeg:%hd\n", name, id, type, dataSeg);
        
        //  for version 28
        printf("name:'%s'\t id:%hd\n", name, id);
    }
    
    read(Sockfd, &size, sizeof(int));
    swapEndian(&size, INT);
    if (size > 0) {
        char clusterConfig[size];
        read(Sockfd, clusterConfig, size);
        //clusterConfig[size]='\0';
    }
    
    read(Sockfd, &size, sizeof(int));
    swapEndian(&size, INT);
    if (size > 0) {
        char orientRelease[size+1];
        read(Sockfd, orientRelease, size);
        orientRelease[size] = '\0';
        printf("%s\n", orientRelease);
    }
    return 0;
}

int sendCommand(char *command) {
    int ret, size;
    GPacket.opType = REQUEST_COMMAND;
    size = reqCommandMsg(GPacket.msg, "c", command);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5+256);
    //printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0)
        return 1;
    else
        return 0;
}

int queryRecord(char *query) {
    int i, ret, size, total;
    GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(GPacket.msg, "q", query);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0) return 1;
    
    read(Sockfd, &GPacket.msg, 1);
    read(Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    //read(Sockfd, &GPacket.msg, 1+4+2+1+2+8+4);
    
    for(i=0;i<total;i++){
        read(Sockfd, &size, 4);
        swapEndian(&size, INT);
        printf("[%d]--------------------------------------------------\n",i);
        printf("size: %d\t",size);
        
        //  read content
        read(Sockfd, GPacket.msg, size);
        GPacket.msg[size]='\0';
        printf("msg: %.*s\n",size,GPacket.msg);
        //printf("len: %d\n", strlen(GPacket.msg));
        read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    }
    
    return 0;
}

int loadRecord(short clusterId, long clusterPos) {
    int ret, size;
    GPacket.opType = REQUEST_RECORD_LOAD;
    size = reqRecLoadMsg(GPacket.msg, clusterId, clusterPos);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0) return 1;
    
    char payloadStatus;
    read(Sockfd, &payloadStatus, sizeof(char));
    printf("status:%hhu\n", payloadStatus);
    
    char recType;
    read(Sockfd, &recType, sizeof(char));
    printf("record-type: %c\n", recType);
    
    int recVersion;
    read(Sockfd, &recVersion, sizeof(int));
    swapEndian(&recVersion, INT);
    printf("record-version: %d\n", recVersion);
    
    //  read content
    read(Sockfd, &size, sizeof(int));
    swapEndian(&size, INT);
    printf("size: %d\n",size);
    
    char content[size+1];
    read(Sockfd, content, size);
    content[size] = '\0';
    printf("content:'%s'\n", content);
    
    return 0;
}

int createVertex(char *command, short* cltid, long* rid){
    int ret, size;
    int in_size;
    GPacket.opType = REQUEST_COMMAND;
    size = reqCommandMsg(GPacket.msg, "c", command);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5+1+2+1);
    if (ret<0 || GPacket.opType!=0){
        //printf("createVertex..FAILED\n");
        return 1;
    }
    else{
        ret = read(Sockfd, &*cltid, 2);
        if (ret<0) return 1;
        swapEndian(&*cltid, SHORT);
        
        ret = read(Sockfd, &*rid, 8);
        if (ret<0) return 1;
        swapEndian(&*rid, LONG);
        
        printf("@rid #%d:%lu\n",*cltid,*rid);
        read(Sockfd, &in_size, 4);
        read(Sockfd, &in_size, 4);
        swapEndian(&in_size, INT);
        //printf("in_size: %d\n",in_size);
        
        ret = read(Sockfd,&GPacket.msg,in_size+1);
        if (ret<0) return 1;
        return 0;
    }
}

char* getRid(char *query) {
    int i, ret, size, total;
    GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(GPacket.msg, "q", query);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0){
        printf("getRid error\n");
        return NULL;
    }
    
    read(Sockfd, &GPacket.msg, 1);
    read(Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    //read(Sockfd, &GPacket.msg, 1+4+2+1+2+8+4);
    
    if(total==0){
        printf("FAILED >> data not found\n");
        return NULL;
    }
    
    char myResult[20];
    char* str;
    char* token;
    char delim[2] = "#";
    //char* str = (char*)malloc(sizeof(char)*10);   (fix)
    
    for(i=0;i<total;i++){
        read(Sockfd, &size, 4);
        swapEndian(&size, INT);
        //printf("[%d]--------------------------------------------------\n",i);
        //printf("size: %d\t",size);
        
        //  read content
        read(Sockfd, GPacket.msg, size);
        GPacket.msg[size]='\0';
        sprintf(myResult,"%s",GPacket.msg);
        printf("size: %lu\t",strlen(myResult));
        printf("msg: %s\n", myResult);
        token = strtok(myResult, delim);
        token = strtok(NULL, delim);
        //printf("result_token: %s\n", token);
        str = strdup(token);
        //sprintf(str,"%s",token);  (fix)
        read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    }
    printf("@rid #%s\n",str);
    return str;
}

char* getContent(char *query) {
    int i, ret, size, total;
    GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(GPacket.msg, "q", query);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5);
    printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0) {
        printf("getContent error\n");
        return NULL;
    }
    
    read(Sockfd, &GPacket.msg, 1);
    read(Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    
    if(total==0)
        return NULL;
    
    char* str = (char*)malloc(sizeof(char)*MAX_CONTENT_SIZE);
    if(total==1){
        for(i=0;i<total;i++){
            read(Sockfd, &size, 4);
            swapEndian(&size, INT);
            printf("size: %d\n",size);
            read(Sockfd, str, size);
            str[size]='\0';
            printf("msg: %s\n",str);
            read(Sockfd, &GPacket.msg, 2+1+2+8+4);
            printf("[result from getContent]---------------\n",i);
        }
        return str;
    }
    else{
        char* all_str = (char*)malloc(sizeof(char)*MAX_CONTENT_SIZE);
        for(i=0;i<total;i++){
            read(Sockfd, &size, 4);
            swapEndian(&size, INT);
            printf("size: %d\n",size);
            read(Sockfd, str, size);
            str[size]='\0';
            strcat(all_str,"#");
            strcat(all_str,str);
            printf("msg: %s\n",str);
            read(Sockfd, &GPacket.msg, 2+1+2+8+4);
            printf("[result from getContent]---------------\n",i);
        }
        free(str);
        printf("all_str(len): %d\n",strlen(all_str));
        return all_str;
    }
}

char** getArrayRid(char* query){
    int i, ret, size, total;
    GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(GPacket.msg, "q", query);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0) {
        printf("FAILED >> getArrayRid\n");
        return NULL;
    }
    
    read(Sockfd, &GPacket.msg, 1);
    read(Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    
    if(total==0)
        return NULL;
    
    char **result_rid = (char**)malloc(sizeof(char*)*total+1);
    
    for(i=0;i<total;i++){
        read(Sockfd, &size, 4);
        swapEndian(&size, INT);
        printf("[%d]--------------------------------------------------\n",i);
        printf("size: %d\t",size);
        result_rid[i] = (char*)malloc(sizeof(char)*size-3);
        
        //  read content
        read(Sockfd, &GPacket.msg, 4);
        read(Sockfd, &GPacket.msg, size-4);
        GPacket.msg[size-4]='\0';
        printf("msg: %s\n", GPacket.msg);
        sprintf(result_rid[i],"%s",GPacket.msg);
        //printf("len: %d\n",strlen(result_rid[i]));
        printf("myrid: %s\n",result_rid[i]);
        read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    }
    result_rid[i] = NULL;
    
    //    for(i=0;result_rid[i]!=NULL;i++) {
    //        printf("TEST_rid: %s\n",result_rid[i]);
    //    }
    
    return result_rid;
}

char** getArrayDistinctRid(char* query){
    int i, ret, size, total;
    GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(GPacket.msg, "q", query);
    ret = write(Sockfd, &GPacket, 5+size);
    
    ret = read(Sockfd, &GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", GPacket.opType);
    if (ret<0 || GPacket.opType!=0) {
        printf("FAILED >> getArrayRid\n");
        return NULL;
    }
    
    read(Sockfd, &GPacket.msg, 1);
    read(Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    
    if(total==0)
        return NULL;
    
    char **result_rid = (char**)malloc(sizeof(char*)*total+1);
    
    for(i=0;i<total;i++){
        read(Sockfd, &size, 4);
        swapEndian(&size, INT);
        printf("[%d]--------------------------------------------------\n",i);
        printf("size: %d\t",size);
        result_rid[i] = (char*)malloc(sizeof(char)*size-3);
        
        //  read content
        read(Sockfd, &GPacket.msg, 4);
        read(Sockfd, &GPacket.msg, size-4);
        GPacket.msg[size-4]='\0';
        printf("msg: %s\n", GPacket.msg);
        sprintf(result_rid[i],"%s",GPacket.msg+5);
        //printf("len: %d\n",strlen(result_rid[i]));
        printf("myrid: %s\n",result_rid[i]);
        read(Sockfd, &GPacket.msg, 2+1+2+8+4);
    }
    result_rid[i] = NULL;
    
    return result_rid;
}

//----------------------------------------------------------------------------------------------------------

const char *stringUUID(){
    char hex_char [] = "0123456789ABCDEF";
    char *uuidstr = (char*) malloc (sizeof (uuid_t) * 2 + 1);
    
    uuid_t uuid;
    uuid_generate (uuid);
    //    int i;
    //    for (i = 0; i < sizeof uuid; i ++) {
    //        printf("%02x ", uuid[i]);
    //    }
    //    printf("\n\n");
    int byte_nbr;
    for (byte_nbr = 0; byte_nbr < sizeof (uuid_t); byte_nbr++) {
        uuidstr [byte_nbr * 2 + 0] = hex_char [uuid [byte_nbr] >> 4];
        uuidstr [byte_nbr * 2 + 1] = hex_char [uuid [byte_nbr] & 15];
    }
    uuidstr[32]='\0';
    return uuidstr;
}

const char* createNewData(int dataType){
    mydata = (Data*)malloc(sizeof(Data));
    mydata->dataID = stringUUID();
    printf("dataID: %s\n",mydata->dataID);
    mydata->dataName = NULL;
    mydata->dataType = dataType;
    mydata->content = createNewDataHolder();
//    mydata->attachmentFTOLinks = NULL;
    mydata->chatRoom = NULL;
    return mydata->dataID;
}

const char* _createNewData(Data** data, int dataType){
    *data = (Data*)malloc(sizeof(Data));
    Data* dt = *data;
    dt->dataID = stringUUID();
    printf("dataID: %s\n",dt->dataID);
    dt->dataName = NULL;
    dt->dataType = dataType;
    dt->content = createNewDataHolder();
//    dt->attachmentFTOLinks = NULL;
    dt->chatRoom = NULL;
    return dt->dataID;
}

DataHolder* createNewDataHolder(){
    DataHolder* dh = (DataHolder*)malloc(sizeof(DataHolder));
    dh->versionKeeped = VER_KEEPED;
    dh->head = NULL;
    dh->lastestCommon = NULL;
    dh->current = NULL;
    return dh;
}

ObjectBinary* createNewObjBinary(char* data){
    ObjectBinary* obj = (ObjectBinary*)malloc(sizeof(ObjectBinary));
    obj->schemaCode = 0;    // fix
    obj->byteCount = (int)strlen(data);
    obj->data = strdup(data);
    return obj;
}

ObjectBinary* _createNewObjBinary(char* data, int dataType){
    ObjectBinary* obj = (ObjectBinary*)malloc(sizeof(ObjectBinary));
    obj->schemaCode = dataType;
    obj->byteCount = (int)strlen(data);
    obj->data = strdup(data);
    return obj;
}

int countDataContent(Data* data){
//    printf("\n'countDataContent'\n");
    DataContent* dc = data->content->lastestCommon;
    if(dc==NULL){
        ("\n!!! Not found DataContent !!!\n");
    }

    int i=1;
    while(dc!=NULL){
//        printf("dc-SHA: %s\n",dc->SHA256hashCode);
        i++;
        if(dc->nextVersion!=NULL)
            dc = dc->nextVersion;
        else{
            break;
        }
    }
//    printf("i: %d\n",i);
    return i;
}

void setDataName(Data* data, char* dataName){
    if(data->dataName == NULL){
        data->dataName = strdup(dataName);
    }
    else{
        free(data->dataName);
        data->dataName = strdup(dataName);
    }
}
char* getDataName(Data* data){
    if(data->dataName != NULL){
        char* result = strdup(data->dataName);
        return result;
    }
    else{
        return NULL;
    }
}

void setChatRoom(Data* data, char* chatRoom){
    if(data->chatRoom == NULL){
        data->chatRoom = strdup(chatRoom);
    }
    else{
        free(data->chatRoom);
        data->chatRoom = strdup(chatRoom);
    }
}

char* getChatRoom(Data* data){
    if(data->chatRoom != NULL){
        char* result = strdup(data->chatRoom);
        return result;
    }
    else{
        return NULL;
    }
}

ReturnErr setDataNameByID(char* dataID, char* dataName){
    char sql[MAX_SQL_SIZE];
    printf("--------------------------------------------------[update_dataName]\n");
    sprintf(sql,"update Data set dataName='%s' where dataID='%s'",dataName,dataID);
    int ret = sendCommand(sql);
    if (ret!=0){
        printf("setDataNameByID..FAILED(update_dataName)\n");
        return -1;
    }
    return 0;
}

ReturnErr setChatRoomByID(char* dataID, char* chatRoom){
    char sql[MAX_SQL_SIZE];
    printf("--------------------------------------------------[update_chatRoom]\n");
    sprintf(sql,"update Data set chatRoom='%s' where dataID='%s'",chatRoom,dataID);
    int ret = sendCommand(sql);
    if (ret!=0){
        printf("setChatRoomByID..FAILED(update_chatRoom)\n");
        return -1;
    }
    return 0;
}

char* getDataNameByID(char* dataID){
    char sql[MAX_SQL_SIZE];
    printf("--------------------------------------------------[get_dataName]\n");
    sprintf(sql,"select dataName from Data where dataID='%s'",dataID);
    char* result = getContent(sql);
    printf("result: %s\n",result);
    if(result == NULL){
        return NULL;
    }
    char *token, *dataName;
    token=strtok(result, "\"");
    token=strtok(NULL, "\"");
//    printf("token: %s\n",token);
    dataName = strdup(token);
    free(result);
    return dataName;
}

char* getChatRoomByID(char* dataID){
    char sql[MAX_SQL_SIZE];
    printf("--------------------------------------------------[get_chatRoom]\n");
    sprintf(sql,"select chatRoom from Data where dataID='%s'",dataID);
    char* result = getContent(sql);
    printf("result: %s\n",result);
    if(result == NULL){
        return NULL;
    }
    char *token, *chatRoom;
    token=strtok(result, "\"");
    token=strtok(NULL, "\"");
//    printf("token: %s\n",token);
    if(token != NULL){
        chatRoom = strdup(token);
        free(result);
        return chatRoom;
    }
    else{
        printf("'%s' not found chatRoom\n",dataID);
        free(result);
        return NULL;
    }
}

ObjectBinary* getDataContent(Data* data){
    if(data->content->head != NULL){
        ObjectBinary* obj_h = data->content->head->fullContent;
//        ObjectBinary* result = createNewObjBinary(obj_h->data);
        ObjectBinary* result = _createNewObjBinary(obj_h->data,data->dataType);
        return result;
    }
    else
        return NULL;
}

ObjectBinary* getDataContentLastestCommon(Data* data){
    //  for test
    //    data->content->lastestCommon = data->content->lastestCommon->nextVersion;
    //    data->content->lastestCommon->preVersion = NULL;
    if(data->content->head != NULL){
        if(data->content->lastestCommon->fullContent != NULL && data->content->lastestCommon->plusPatch == NULL){
//            ObjectBinary* result = createNewObjBinary(data->content->lastestCommon->fullContent->data);
            ObjectBinary* result = _createNewObjBinary(data->content->lastestCommon->fullContent->data, data->dataType);
            return result;
        }
        else{
            char* init_str = strdup(data->content->head->fullContent->data);
            string s;
            DataContent *mydc, *prev_mydc;
            for(mydc=data->content->head->preVersion;mydc!=NULL;mydc=prev_mydc){
                prev_mydc = mydc->preVersion;
                s = getPatch(init_str, mydc->minusPatch->data);
                free(init_str);
                init_str = strdup(s.c_str());
            }
//            ObjectBinary* result = createNewObjBinary(init_str);
            ObjectBinary* result = _createNewObjBinary(init_str,data->dataType);
            free(init_str);
            return result;
        }
    }
    else
        return NULL;
}

ObjectBinary* getDataContentByID(char* dataID){
    printf(">> getDataContentByID\n");
    char sql[MAX_SQL_SIZE];
    
    //  check edge toDeletedData exist
    printf("--------------------------------------------------[check_toDeletedData_exist]\n");
    sprintf(sql,"select count(*) from (select expand(out('toDeletedData')) from DeleteNode) where dataID = '%s'",dataID);
    printf("SQL: %s\n",sql);
    char* result = getContent(sql);
    char* token = strtok(result,":");
    token = strtok(NULL,"l");
    printf("count: %s\n\n",token);
    int count = atoi(token);
    free(result);
    
    if(count!=0){
        return NULL;
    }
    
    printf("--------------------------------------------------[get_DataContent]\n");
    sprintf(sql,"select full_schemaCode,full_byteCount,full_data from (select expand(out('toDataHolder').outE('toDataContent')[type='head'].inV()) from Data where dataID='%s')",dataID);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    printf("\nresult: %s\n",result);
    if(result==NULL){
        printf("'%s' not found fullContent\n",dataID);
        return NULL;
    }
    else{
        string init;
        init.assign(result);
        printf("before: %s\n",result);
        string res = replace(init,"\\'","'");
        res = replace(res,"\\\"","\"");
        printf("\n------------------------------------------------------------------------------------------------------------\n\n");
        printf("replace: %s\n",res.c_str());
        printf("\n------------------------------------------------------------------------------------------------------------\n\n");
        
        free(result);
        result = strdup(res.c_str());
        
        ObjectBinary* obj =(ObjectBinary*)malloc(sizeof(ObjectBinary));
        
        char *token, *br, *pt_data;
        //  schemaCode
        token = strtok_r(result,":",&br);
        token = strtok_r(NULL,",",&br);
        printf("schemaCode: %s\n",token);
        obj->schemaCode = atoi(token);
        
        //  byteCount
        token = strtok_r(NULL,":",&br);
        token = strtok_r(NULL,",",&br);
        printf("bytecount: %s\n",token);
        obj->byteCount = atoi(token);
        
        printf("br: %s\n",br);
        pt_data = br+11;
        printf("pt_data: %s\n",pt_data);
        
        int b_count = atoi(token);
        printf("b_count: %d\n",b_count);
        
        obj->data = (char*)calloc(b_count+1,sizeof(char));
        memcpy(obj->data,pt_data,b_count);
        printf("obj_data: %s\n",obj->data);
        
        free(result);
        return obj;
    }
}

ObjectBinary* getDataContentLastestCommonByID(char* dataID){
    printf(">> getDataContentLastestCommonByID\n");
    char sql[MAX_SQL_SIZE];
    
    //  check edge toDeletedData exist
    printf("--------------------------------------------------[check_toDeletedData_exist]\n");
    sprintf(sql,"select count(*) from (select expand(out('toDeletedData')) from DeleteNode) where dataID = '%s'",dataID);
    printf("SQL: %s\n",sql);
    char* result = getContent(sql);
    char* token = strtok(result,":");
    token = strtok(NULL,"l");
    printf("count: %s\n\n",token);
    int count = atoi(token);
    free(result);
    
    if(count!=0){
        return NULL;
    }

    printf("--------------------------------------------------[get_DataContent]\n");
    Data* dt = queryDataByID(dataID);
    if(dt==NULL){
        return NULL;
    }
    else{
        ObjectBinary* obj = getDataContentLastestCommon(dt);
        freeData(dt);
        return obj;
    }
}

Text getDiffDataAtHead(Data* data){
    if(data->content->head != NULL && data->content->head->plusPatch != NULL){
        char* result = strdup(data->content->head->plusPatch->data);
        return result;
    }
    else
        return NULL;
}

Text getDiffDataAtLastestCommon(Data* data){
    //  for test
    //    data->content->lastestCommon = data->content->lastestCommon->nextVersion;
    if(data->content->lastestCommon != NULL && data->content->lastestCommon->plusPatch != NULL){
        char* result = strdup(data->content->lastestCommon->plusPatch->data);
        return result;
    }
    else
        return NULL;
}

ObjectBinary* getContentNextVer(Data* data){
    if(data->content->current != NULL && data->content->current->nextVersion != NULL){
        if(data->content->current->nextVersion->fullContent!= NULL){
//            ObjectBinary* result = createNewObjBinary(data->content->current->nextVersion->fullContent->data);
            ObjectBinary* result = _createNewObjBinary(data->content->current->nextVersion->fullContent->data,data->dataType);
            return result;
        }
        else{
            DataContent* dc = data->content->current;
            int i=0;
            while(dc!=NULL){
                i++;
                dc = dc->nextVersion;
            }
            i=i-1;
            
            char* init_str = strdup(data->content->head->fullContent->data);
            string s;
            DataContent *mydc, *prev_mydc;
            for(mydc=data->content->head->preVersion;mydc!=NULL;mydc=prev_mydc){
                if(i==0)
                    break;
                prev_mydc = mydc->preVersion;
                s = getPatch(init_str, mydc->minusPatch->data);
                free(init_str);
                init_str = strdup(s.c_str());
                i--;
            }
//            ObjectBinary* result = createNewObjBinary(init_str);
            ObjectBinary* result = _createNewObjBinary(init_str,data->dataType);
            free(init_str);
            return result;
        }
    }
    else{
        printf("\n*** Now current is pointing to head\n");
        return NULL;
    }
}

ObjectBinary* getContentPreVer(Data* data){
    if(data->content->current != NULL && data->content->current->preVersion != NULL){
        if(data->content->current->preVersion->fullContent!= NULL){
//            ObjectBinary* result = createNewObjBinary(data->content->current->preVersion->fullContent->data);
            ObjectBinary* result = _createNewObjBinary(data->content->current->preVersion->fullContent->data,data->dataType);
            return result;
        }
        else{
            DataContent* dc = data->content->current;
            int i=0;
            while(dc!=NULL){
                i++;
                dc = dc->nextVersion;
            }
//            printf("i: %d\n",i);
            
            char* init_str = strdup(data->content->head->fullContent->data);
            string s;
            DataContent *mydc, *prev_mydc;
            for(mydc=data->content->head->preVersion;mydc!=NULL;mydc=prev_mydc){
                if(i==0)
                    break;
                prev_mydc = mydc->preVersion;
                s = getPatch(init_str, mydc->minusPatch->data);
                free(init_str);
                init_str = strdup(s.c_str());
                i--;
            }
//            ObjectBinary* result = createNewObjBinary(init_str);
            ObjectBinary* result = _createNewObjBinary(init_str,data->dataType);
            free(init_str);
            return result;
        }
    }
    else{
        printf("\n*** Now current is pointing to lastestCommon\n");
        return NULL;
    }
}

Text getDiffDataNextVer(Data* data){
    if(data->content->current != NULL && data->content->current->nextVersion != NULL){
        char* result = strdup(data->content->current->nextVersion->plusPatch->data);
        return result;
    }
    else{
        printf("\n*** Now current is pointing to head\n");
        return NULL;
    }
}

Text getDiffDataPreVer(Data* data){
    if(data->content->current != NULL && data->content->current->preVersion != NULL){
        if(data->content->current->preVersion->plusPatch != NULL){
            char* result = strdup(data->content->current->preVersion->plusPatch->data);
            return result;
        }
        else{
            printf("\n*** PreVersion is initial xml\n");
            return NULL;
        }
    }
    else{
        printf("\n*** Now current is pointing to lastestCommon\n");
        return NULL;
    }
}

Text getDataContentWithTag(Data* data, char* tagName, char* id){
    char* full_xml = strdup(data->content->head->fullContent->data);
    ezxml_t root = ezxml_parse_str(full_xml,strlen(full_xml));
    ezxml_t partial_data = ezxml_get(root,tagName,-1);
    
    if(partial_data==NULL) {
        printf("FAILED >> incorrect tag\n");
        free(full_xml);
        ezxml_free(root);
        return NULL;
    }
    
    ezxml_t link;
    char* link_id;
    int exist = 0;
    
    if(strcmp(tagName,"attachmentFTOLinks")==0){
        for(link = ezxml_child(partial_data, "link");link;link=link->next){
            link_id = (char*)ezxml_attr(link, "id");
            printf("link_id: %s\n",link_id);
            if(strcmp(link_id,id)==0){
                exist=1;
                break;
            }
        }
        if(exist==1){
            partial_data = link;
        }
        else{
            printf("FAILED >> incorrect id\n");
            free(full_xml);
            ezxml_free(root);
            return NULL;
        }
    }
    
    printf("val: %s\n",partial_data->txt);
    char* result = strdup(partial_data->txt);
    free(full_xml);
    ezxml_free(root);
    return result;
}

ReturnErr setNewDataContent(Data* data, ObjectBinary* content){
    int total = countDataContent(data);
    printf("\n[ DATA #%d ]\n",total);
    if(data->content->head==NULL && data->content->lastestCommon==NULL && data->content->current==NULL){
        DataContent* dc0 = (DataContent*)malloc(sizeof(DataContent));
        
        DataHolder* dh = data->content;
        dh->lastestCommon = dc0;
        dh->head = dc0;
        dh->current = dc0;
        
        dc0->preVersion = NULL;
        dc0->nextVersion = NULL;
        
        //  dc0 ------------------------------------------------------------
        dc0->dataHd = dh;
        dc0->SHA256hashCode = strdup("default_hashcode");
        dc0->timeStamp = NULL;
        dc0->isDiff = FALSE;
        
        //  dc0->fullContent
        ObjectBinary* obj0 = (ObjectBinary*)malloc(sizeof(ObjectBinary));
        dc0->fullContent = obj0;
        obj0->schemaCode = content->schemaCode;
        obj0->data = strdup(content->data);
        obj0->byteCount = content->byteCount;
        
        //  dc0->minusPatch
        dc0->minusPatch = NULL;
        
        //  dc0->plusPatch
        dc0->plusPatch = NULL;
        
        printf("full: %s\n",obj0->data);
        return obj0->byteCount;
    }
    else{
        DataContent* dc0 = data->content->head;                             //  old
        DataContent* dc1 = (DataContent*)malloc(sizeof(DataContent));       //  new
        
        DataHolder* dh = data->content;
        dh->head = dc1;
        dh->current = dc1;
        
        printf("old: %s\n",dc0->fullContent->data);
        printf("new: %s\n",content->data);
        
        string s_plus = getDiff(dc0->fullContent->data,content->data);
        string s_minus = getDiff(content->data,dc0->fullContent->data);
        printf("s_plus: %s\n",s_plus.c_str());
        printf("s_minus: %s\n",s_minus.c_str());
        
        dc0->nextVersion = dc1;
        dc1->preVersion = dc0;
        dc1->nextVersion = NULL;
        
        //  dc0 ------------------------------------------------------------
        dc0->isDiff = TRUE;
        
        //  dc0->fullContent
        free(dc0->fullContent->data);
        free(dc0->fullContent);
        dc0->fullContent = NULL;
        
        //  dc0->minusPatch
        ObjectBinary* obj0_minus = (ObjectBinary*)malloc(sizeof(ObjectBinary));
        dc0->minusPatch = obj0_minus;
        obj0_minus->schemaCode = content->schemaCode;
        obj0_minus->data = strdup(s_minus.c_str());
        obj0_minus->byteCount = (int)strlen(obj0_minus->data);
        
        //  dc0->plusPatch (not change)

        //  dc1 ------------------------------------------------------------
        dc1->dataHd = dh;
        dc1->SHA256hashCode = strdup("default_hashcode");
        dc1->timeStamp = NULL;
        dc1->isDiff = FALSE;
        
        //  dc1->fullContent
        ObjectBinary* obj1_full = (ObjectBinary*)malloc(sizeof(ObjectBinary));
        dc1->fullContent = obj1_full;
        obj1_full->schemaCode = content->schemaCode;
        obj1_full->data = strdup(content->data);
        obj1_full->byteCount = content->byteCount;
        
        //  dc1->minusPatch
        dc1->minusPatch = NULL;
        
        //  dc1->plusPatch
        ObjectBinary* obj1_plus = (ObjectBinary*)malloc(sizeof(ObjectBinary));
        dc1->plusPatch = obj1_plus;
        obj1_plus->schemaCode = content->schemaCode;
        obj1_plus->data = strdup(s_plus.c_str());
        obj1_plus->byteCount = (int)strlen(obj1_plus->data);
        
        printf("\n--- Check old/new data ---\n");
        if(dc0->minusPatch != NULL)
            printf("minus_old: %s\n",dc0->minusPatch->data);
        if(dc0->plusPatch != NULL)
            printf("plus_old: %s\n",dc0->plusPatch->data);
        if(dc0->fullContent != NULL)
            printf("full_old: %s\n",dc0->fullContent->data);
        if(dc1->minusPatch != NULL)
            printf("minus_new: %s\n",dc1->minusPatch->data);
        if(dc1->plusPatch != NULL)
            printf("plus_new: %s\n",dc1->plusPatch->data);
        if(dc1->fullContent != NULL)
            printf("full_new: %s\n",dc1->fullContent->data);
        
        return obj1_plus->byteCount;
    }
}

ReturnErr setNewDataDiffWithTag(Data* data, char* tagName, char* id, Text diff){
    printf("\n--- setNewDataDiffWithTag ---\n");
    DataContent* dc_old = data->content->head;
    char* old_xml = strdup(dc_old->fullContent->data);
    
    ezxml_t root = ezxml_parse_str(old_xml,strlen(old_xml));
    ezxml_t link, partial_data;

    int exist = 0;
    char *link_id, *val_after, *new_xml;
    string s;
    
    if(strcmp(tagName,"attachmentFTOLinks")==0){
        //  find tag "attachmentFTOLinks"
        partial_data = ezxml_get(root,tagName,-1);
        for(link = ezxml_child(partial_data, "link");link;link=link->next){
            link_id = (char*)ezxml_attr(link, "id");
            printf("link_id: %s\n",link_id);
            if(strcmp(link_id,id)==0){
                exist = 1;
                break;
            }
        }
        if(exist==1){
            printf("val_before: %s\n",link->txt);
            printf("val_diff: %s\n",diff);
            
            s = getPatch(link->txt,diff);
            val_after = strdup(s.c_str());
            printf("val_after: %s\n",val_after);
            
            ezxml_set_txt(link,val_after);
            new_xml = ezxml_toxml(root);
            printf("new_xml: %s\n", new_xml);
        }
        else{
            link = ezxml_add_child(partial_data,"link",0);
            ezxml_set_attr(link,"id",id);
            ezxml_set_txt(link, diff);
            new_xml = ezxml_toxml(root);
            val_after = NULL;
            printf("val_diff(full): %s\n",diff);
            printf("new_xml: %s\n", new_xml);
        }
    }
    else{
        partial_data = ezxml_get(root,tagName,-1);
        
        if(partial_data==NULL) {
            printf("FAILED >> incorrect tag\n");
            free(old_xml);
            ezxml_free(root);
            return -1;
        }
        
        printf("val_before: %s\n",partial_data->txt);
        printf("val_diff: %s\n",diff);
        
        s = getPatch(partial_data->txt,diff);
        val_after = strdup(s.c_str());
        printf("val_after: %s\n",val_after);
        
        ezxml_set_txt(partial_data,val_after);
        new_xml = ezxml_toxml(root);
        printf("new_xml: %s\n", new_xml);
    }

//    ObjectBinary* objB = createNewObjBinary(new_xml);
    ObjectBinary* objB = _createNewObjBinary(new_xml,data->dataType);
    int ret = setNewDataContent(data, objB);

    free(objB->data);
    free(objB);
    free(old_xml);
    if(val_after != NULL)
        free(val_after);
    free(new_xml);
    ezxml_free(root);
    return ret;
}

Text getTagContentWithName(ObjectBinary* fullContent, char* tagName, char* id){
    char* full_xml = strdup(fullContent->data);
    ezxml_t root = ezxml_parse_str(full_xml,strlen(full_xml));
    ezxml_t partial_data = ezxml_get(root,tagName,-1);
    
    if(partial_data==NULL) {
        printf("FAILED >> incorrect tag\n");
        free(full_xml);
        ezxml_free(root);
        return NULL;
    }
    
    ezxml_t link;
    char* link_id;
    int exist = 0;
    
    if(strcmp(tagName,"attachmentFTOLinks")==0){
        for(link = ezxml_child(partial_data, "link");link;link=link->next){
            link_id = (char*)ezxml_attr(link, "id");
            printf("link_id: %s\n",link_id);
            if(strcmp(link_id,id)==0){
                exist=1;
                break;
            }
        }
        if(exist==1){
            partial_data = link;
        }
        else{
            printf("FAILED >> incorrect id\n");
            free(full_xml);
            ezxml_free(root);
            return NULL;
        }
    }
    
    printf("val: %s\n",partial_data->txt);
    char* result = strdup(partial_data->txt);
    free(full_xml);
    ezxml_free(root);
    return result;
}

Text getTagContent(ObjectBinary* fullContent, int tagNameEnum, char* id){
    char* full_xml = strdup(fullContent->data);
    ezxml_t root = ezxml_parse_str(full_xml,strlen(full_xml));
    ezxml_t partial_data;

    switch(tagNameEnum){
        case _attachmentFTOLinks:
            partial_data = ezxml_get(root,"attachmentFTOLinks",-1);
            break;
        case _content:
            partial_data = ezxml_get(root,"content",-1);
            break;
        case _book_id:
            partial_data = ezxml_get(root,"book_id",-1);
            break;
        case _author:
            partial_data = ezxml_get(root,"author",-1);
            break;
        case _title:
            partial_data = ezxml_get(root,"title",-1);
            break;
        case _genre:
            partial_data = ezxml_get(root,"genre",-1);
            break;
        case _price:
            partial_data = ezxml_get(root,"price",-1);
            break;
        default:
            partial_data = NULL;
            break;
    }
    
    if(partial_data==NULL) {
        printf("FAILED >> incorrect tag\n");
        free(full_xml);
        ezxml_free(root);
        return NULL;
    }
    
    ezxml_t link;
    char* link_id;
    int exist = 0;
    
    if(tagNameEnum==_attachmentFTOLinks){
        for(link = ezxml_child(partial_data, "link");link;link=link->next){
            link_id = (char*)ezxml_attr(link, "id");
            printf("link_id: %s\n",link_id);
            if(strcmp(link_id,id)==0){
                exist=1;
                break;
            }
        }
        if(exist==1){
            partial_data = link;
        }
        else{
            printf("FAILED >> incorrect id\n");
            free(full_xml);
            ezxml_free(root);
            return NULL;
        }
    }
    
    printf("val: %s\n",partial_data->txt);
    char* result = strdup(partial_data->txt);
    free(full_xml);
    ezxml_free(root);
    return result;
}

ReturnErr setTagContent(ObjectBinary* fullContent, Text newTagContent, char* tagName, char* id){
    char* full_xml = strdup(fullContent->data);
    ezxml_t root = ezxml_parse_str(full_xml,strlen(full_xml));
    
    ezxml_t link, partial_data;
    
    int exist = 0;
    char *link_id, *val_after, *new_xml;
    string s;
    
    if(strcmp(tagName,"attachmentFTOLinks")==0){
        //  find tag "attachmentFTOLinks"
        partial_data = ezxml_get(root,tagName,-1);
        for(link = ezxml_child(partial_data, "link");link;link=link->next){
            link_id = (char*)ezxml_attr(link, "id");
            printf("link_id: %s\n",link_id);
            if(strcmp(link_id,id)==0){
                exist = 1;
                break;
            }
        }
        if(exist==1){
            ezxml_set_txt(link,newTagContent);
        }
        else{
            link = ezxml_add_child(partial_data,"link",0);
            ezxml_set_attr(link,"id",id);
            ezxml_set_txt(link, newTagContent);
        }
    }
    else{
        partial_data = ezxml_get(root,tagName,-1);
        
        if(partial_data==NULL) {
            printf("FAILED >> incorrect tag\n");
            free(full_xml);
            ezxml_free(root);
            return -1;
        }
        
        ezxml_set_txt(partial_data,newTagContent);
    }
    new_xml = ezxml_toxml(root);
    printf("new_xml: %s\n", new_xml);
    
    free(fullContent->data);
    fullContent->data = new_xml;
    fullContent->byteCount = strlen(new_xml);
    
    free(full_xml);
    ezxml_free(root);
    return fullContent->byteCount;
}

Text* getAttachmentFTOLinks(Data* data){
    char* full_xml = strdup(data->content->head->fullContent->data);
    ezxml_t root = ezxml_parse_str(full_xml,strlen(full_xml));
    ezxml_t partial_data = ezxml_get(root,"attachmentFTOLinks",-1);
    
    ezxml_t link;
    char* link_id;
    int count = 0;
    
    for(link = ezxml_child(partial_data, "link");link;link=link->next){
        link_id = (char*)ezxml_attr(link, "id");
        printf("link_id: %s\n",link_id);
        count++;
    }
    
    if(count==0){
        printf("not found link in TAG 'attachmentFTOLinks'\n");
        free(full_xml);
        ezxml_free(root);
        return NULL;
    }
    
    Text* att_link = (Text*)malloc((count+1)*sizeof(Text));
    int i=0;
    for(link = ezxml_child(partial_data, "link");link;link=link->next){
//        link_id = (char*)ezxml_attr(link, "id");
        printf("link_txt: %s\n",link->txt);
        att_link[i] = strdup(link->txt);
        i++;
    }
    att_link[i]=NULL;
    
    free(full_xml);
    ezxml_free(root);
    return att_link;
}

const char* createOrg(Text orgName, Schema* orgSchema){
    const char* uuidstr = createData(orgName,orgSchema,_org);
    return uuidstr;
}

const char* createUser(Text userName, Schema* userSchema){
    const char* uuidstr = createData(userName,userSchema,_user);
    return uuidstr;
}

const char* createCategory(Text categoryName, Schema* categorySchema){
    const char* uuidstr = createData(categoryName,categorySchema,_category);
    return uuidstr;
}

const char* createState(Text stateName, Schema* stateSchema){
    const char* uuidstr = createData(stateName,stateSchema,_state);
    return uuidstr;
}

const char* createTask(Text taskName, Schema* taskSchema){
    const char* uuidstr = createData(taskName,taskSchema,_task);
    return uuidstr;
}

const char* createSubTask(Text subTaskName, Schema* subTaskSchema){
    const char* uuidstr = createData(subTaskName,subTaskSchema,_subTask);
    return uuidstr;
}

const char* createData(Text dataName, Schema* dataSchema, int dType){
    //  schema isn't used
    const char* uuidstr = stringUUID();
    printf("uuid: %s\n",uuidstr);
    int ret;
    char sql[MAX_SQL_SIZE];
    
    short dt_cltid, dh_cltid, dl_cltid, dc_cltid;
    long dt_rid, dh_rid, dl_rid, dc_rid;
    
    //  Data
    printf("--------------------------------------------------[create_vertex_Data]\n");
    // chatRoom use setChatRoomByID
    sprintf(sql,"CREATE VERTEX Data set dataID='%s', dataType=%d, dataName='%s'",uuidstr,dType,dataName);
    ret = createVertex(sql,&dt_cltid,&dt_rid);
    if (ret!=0){
        printf("createData..FAILED(vertex_Data)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  DataHolder
    printf("--------------------------------------------------[create_vertex_DataHolder]\n");
    sprintf(sql,"CREATE VERTEX DataHolder set versionKeeped=%d",VER_KEEPED);
    ret = createVertex(sql,&dh_cltid,&dh_rid);
    if (ret!=0){
        printf("createData..FAILED(vertex_DataHolder)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  toDataHolder
    printf("--------------------------------------------------[create_edge_toDataHolder]\n");
    sprintf(sql,"CREATE EDGE toDataHolder from #%d:%lu to #%d:%lu",dt_cltid,dt_rid,dh_cltid,dh_rid);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("createData..FAILED(edge_toDataHolder)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  Delete (for User only)
    if(dType==_user){
        printf("--------------------------------------------------[create_vertex_DeleteNode]\n");
        sprintf(sql,"CREATE VERTEX DeleteNode set userID='%s'",uuidstr);
        ret = createVertex(sql,&dl_cltid,&dl_rid);
        if (ret!=0){
            printf("createData..FAILED(vertex_DeleteNode)\n");
            free((char*)uuidstr);
            return NULL;
        }
        
        //  toDelete
        printf("--------------------------------------------------[create_edge_toDelete]\n");
        sprintf(sql,"CREATE EDGE toDelete from #%d:%lu to #%d:%lu",dt_cltid,dt_rid,dl_cltid,dl_rid);
        ret = sendCommand(sql);
        if (ret!=0){
            printf("createData..FAILED(edge_toDelete)\n");
            free((char*)uuidstr);
            return NULL;
        }
    }
    
    //  DataContent
    printf("--------------------------------------------------[create_vertex_DataContent]\n");
    sprintf(sql,"CREATE VERTEX Datacontent set isDiff=%d, SHA256hashCode='%s', full_schemaCode=%d, full_byteCount=%d, full_data='%s'",FALSE,"h-a-s-h",dType,strlen(dataSchema),dataSchema);
//    printf("SQL: %s\n",sql);
    ret = createVertex(sql,&dc_cltid,&dc_rid);
    if (ret!=0){
        printf("createData..FAILED(vertex_DataContent)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  toDataContent(head)
    printf("--------------------------------------------------[create_edge_toDataContent_head]\n");
    sprintf(sql,"CREATE EDGE toDataContent from #%d:%lu to #%d:%lu set type='head'",dh_cltid,dh_rid,dc_cltid,dc_rid);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("createData..FAILED(edge_toDataContent_head)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  toDataContent(current)
    printf("--------------------------------------------------[create_edge_toDataContent_current]\n");
    sprintf(sql,"CREATE EDGE toDataContent from #%d:%lu to #%d:%lu set type='current'",dh_cltid,dh_rid,dc_cltid,dc_rid);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("createData..FAILED(edge_toDataContent_current)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  toDataContent(lastestCommon)
    printf("--------------------------------------------------[create_edge_toDataContent_lastestCommon]\n");
    sprintf(sql,"CREATE EDGE toDataContent from #%d:%lu to #%d:%lu set type='lastestCommon'",dh_cltid,dh_rid,dc_cltid,dc_rid);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("createData..FAILED(edge_toDataContent_lastestCommon)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    //  toDataHolder_fromDC
    printf("--------------------------------------------------[create_edge_toDataHolder_fromDC]\n");
    sprintf(sql,"CREATE EDGE toDataHolder_fromDC from #%d:%lu to #%d:%lu",dc_cltid,dc_rid,dh_cltid,dh_rid);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("createData..FAILED(edge_toDataHolder_fromDC)\n");
        free((char*)uuidstr);
        return NULL;
    }
    
    return uuidstr;
}

ReturnErr addUser2OrgByID(char* orgID, char* userID){
    int ret = addData2DataByID(orgID, userID, _toUser);
    return ret;
}

ReturnErr addCategory2UserByID(char* userID, char* categoryID){
    int ret = addData2DataByID(userID, categoryID, _toCategory);
    return ret;
}

ReturnErr addState2CategoryByID(char* categoryID, char* stateID){
    int ret = addData2DataByID(categoryID, stateID, _toState);
    return ret;
}

ReturnErr addTask2StateByID(char* stateID, char* taskID){
    int ret = addData2DataByID(stateID, taskID, _toTask);
    return ret;
}

ReturnErr addTask2CategoryByID(char* categoryID, char* taskID){
    int ret = addData2DataByID(categoryID, taskID, _toTask);
    return ret;
}

ReturnErr addSubTask2TaskByID(char* taskID, char* subTaskID){
    int ret = addData2DataByID(taskID, subTaskID, _toSubTask);
    return ret;
}

ReturnErr addData2DataByID(char* f_dataID, char* t_dataID, int dType){
    int ret;
    char sql[MAX_SQL_SIZE];
    
    switch(dType){
        case _toUser:
            printf("--------------------------------------------------[create_edge_toUser]\n");
            sprintf(sql,"create edge toUser from (select from Data where dataID='%s') to (select from Data where dataID='%s')",f_dataID,t_dataID);
            break;
        case _toCategory:
            printf("--------------------------------------------------[create_edge_toCategory]\n");
            sprintf(sql,"create edge toCategory from (select from Data where dataID='%s') to (select from Data where dataID='%s')",f_dataID,t_dataID);
            break;
        case _toState:
            printf("--------------------------------------------------[create_edge_toState]\n");
            sprintf(sql,"create edge toState from (select from Data where dataID='%s') to (select from Data where dataID='%s')",f_dataID,t_dataID);
            break;
        case _toTask:
            printf("--------------------------------------------------[create_edge_toTask]\n");
            sprintf(sql,"create edge toTask from (select from Data where dataID='%s') to (select from Data where dataID='%s')",f_dataID,t_dataID);
            break;
        case _toSubTask:
            printf("--------------------------------------------------[create_edge_toSubTask]\n");
            sprintf(sql,"create edge toSubTask from (select from Data where dataID='%s') to (select from Data where dataID='%s')",f_dataID,t_dataID);
            break;
        default:
            return -1;
            break;
    }
    
    ret = sendCommand(sql);
    if (ret!=0){
        printf("addData2DataByID..FAILED\n");
        return -1;
    }
    
    return 0;
}

ReturnErr addUser2Org(char* orgID, User* user){
    int ret = addData2Data(orgID, user, _toUser);
    return ret;
}

ReturnErr addCategory2User(char* userID, Category* category){
    int ret = addData2Data(userID, category, _toCategory);
    return ret;
    
}

ReturnErr addState2Category(char* categoryID, State* state){
    int ret = addData2Data(categoryID, state, _toState);
    return ret;
}

ReturnErr addTask2State(char* stateID, Task* task){
    int ret = addData2Data(stateID, task, _toTask);
    return ret;
}

ReturnErr addTask2Category(char* categoryID, Task* task){
    int ret = addData2Data(categoryID, task, _toTask);
    return ret;
}

ReturnErr addSubTask2Task(char* taskID, SubTask *subTask){
    int ret = addData2Data(taskID, subTask, _toSubTask);
    return ret;
}

ReturnErr addData2Data(char* dataID, Data* data, int dType){
    int ret;
    char sql[MAX_SQL_SIZE];
    
    short dt_cltid, dh_cltid, dl_cltid;
    long dt_rid, dh_rid, dl_rid;
    
    //  Data
    printf("--------------------------------------------------[create_vertex_Data]\n");
    sprintf(sql,"CREATE VERTEX Data set dataID='%s', dataType=%d, dataName='%s', chatRoom='%s'",data->dataID,data->dataType,data->dataName,data->chatRoom);
    ret = createVertex(sql,&dt_cltid,&dt_rid);
    if (ret!=0){
        printf("addData2Data..FAILED(vertex_Data)\n");
        return -1;
    }
    
    //  DataHolder
    printf("--------------------------------------------------[create_vertex_DataHolder]\n");
    sprintf(sql,"CREATE VERTEX DataHolder set versionKeeped=%d",data->content->versionKeeped);
    ret = createVertex(sql,&dh_cltid,&dh_rid);
    if (ret!=0){
        printf("addData2Data..FAILED(vertex_DataHolder)\n");
        return -1;
    }
    
    //  toDataHolder
    printf("--------------------------------------------------[create_edge_toDataHolder]\n");
    sprintf(sql,"CREATE EDGE toDataHolder from #%d:%lu to #%d:%lu",dt_cltid,dt_rid,dh_cltid,dh_rid);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("addData2Data..FAILED(edge_toDataHolder)\n");
        return -1;
    }
    
    //  Delete (for User only)
    if(dType==_toUser){
        printf("--------------------------------------------------[create_vertex_DeleteNode]\n");
        sprintf(sql,"CREATE VERTEX DeleteNode set userID='%s'",data->dataID);
        ret = createVertex(sql,&dl_cltid,&dl_rid);
        if (ret!=0){
            printf("addData2Data..FAILED(vertex_DeleteNode)\n");
            return -1;
        }
        
        //  toDelete
        printf("--------------------------------------------------[create_edge_toDelete]\n");
        sprintf(sql,"CREATE EDGE toDelete from #%d:%lu to #%d:%lu",dt_cltid,dt_rid,dl_cltid,dl_rid);
        ret = sendCommand(sql);
        if (ret!=0){
            printf("addData2Data..FAILED(edge_toDelete)\n");
            return -1;
        }
    }
    
    //  DataContent
    int count = countDataContent(data);
    count--;
    short dc_cltid[count];
    long dc_rid[count];
//    printf("count: %d\n",count);
    int i=0;
    DataContent *dc, *next_dc;
    for(dc=data->content->lastestCommon;dc!=NULL;dc=next_dc){
        printf("--------------------------------------------------[create_vertex_DataContent #%d]\n",i);
        if(dc->fullContent != NULL && dc->minusPatch == NULL && dc->plusPatch == NULL){
            printf("case1\n");
            
            string init;
            init.assign(dc->fullContent->data,strlen(dc->fullContent->data));
            printf("before: %s\n",dc->fullContent->data);
            string result = replace(init,"'","\\'");
            result = replace(result,"\"","\\\"");
            printf("replace: %s\n",result.c_str());
            
            sprintf(sql,"CREATE VERTEX Datacontent set isDiff=%d, SHA256hashCode='%s', full_schemaCode=%d, full_byteCount=%d, full_data='%s'",dc->isDiff,dc->SHA256hashCode,dc->fullContent->schemaCode,dc->fullContent->byteCount,result.c_str());
        }
        else if(dc->plusPatch != NULL && dc->minusPatch != NULL && dc->fullContent == NULL){
            printf("case2\n");
            
            string init;
            init.assign(dc->plusPatch->data,strlen(dc->plusPatch->data));
            printf("before: %s\n",dc->plusPatch->data);
            string result = replace(init,"'","\\'");
            result = replace(result,"\"","\\\"");
            printf("replace: %s\n",result.c_str());
            
            string init2;
            init2.assign(dc->minusPatch->data,strlen(dc->minusPatch->data));
            printf("before: %s\n",dc->minusPatch->data);
            string result2 = replace(init2,"'","\\'");
            result2 = replace(result2,"\"","\\\"");
            printf("replace: %s\n",result2.c_str());
            
            sprintf(sql,"CREATE VERTEX Datacontent set isDiff=%d, SHA256hashCode='%s', plus_schemaCode=%d, plus_byteCount=%d, plus_data='%s', minus_schemaCode=%d, minus_byteCount=%d, minus_data='%s'",dc->isDiff,dc->SHA256hashCode,dc->plusPatch->schemaCode,dc->plusPatch->byteCount,result.c_str(),dc->minusPatch->schemaCode,dc->minusPatch->byteCount,result2.c_str());
        }
        else if(dc->plusPatch != NULL && dc->fullContent != NULL && dc->minusPatch == NULL){
            printf("case3\n");
            string init;
            init.assign(dc->fullContent->data,strlen(dc->fullContent->data));
            printf("before: %s\n",dc->fullContent->data);
            string result = replace(init,"'","\\'");
            result = replace(result,"\"","\\\"");
            printf("replace: %s\n",result.c_str());
            
            //  normal
            sprintf(sql,"CREATE VERTEX Datacontent set isDiff=%d, SHA256hashCode='%s', plus_schemaCode=%d, plus_byteCount=%d, plus_data='%s', full_schemaCode=%d, full_byteCount=%d, full_data='%s'",dc->isDiff,dc->SHA256hashCode,dc->plusPatch->schemaCode,dc->plusPatch->byteCount,dc->plusPatch->data,dc->fullContent->schemaCode,dc->fullContent->byteCount,result.c_str());
            
            //  json format
//            sprintf(sql,"CREATE VERTEX Datacontent content {'isDiff':%d, 'plus_schemaCode':%d, 'plus_byteCount':%d, 'plus_data':'%s', 'full_schemaCode':%d, 'full_byteCount':%d, 'full_data':'%s'}",dc->isDiff,dc->plusPatch->schemaCode,dc->plusPatch->byteCount,dc->plusPatch->data,dc->fullContent->schemaCode,dc->fullContent->byteCount,dc->fullContent->data);
        }
        else{
            //  minusPatch
            printf("case4\n");
            
            string init;
            init.assign(dc->minusPatch->data,strlen(dc->minusPatch->data));
            printf("before: %s\n",dc->minusPatch->data);
            string result = replace(init,"'","\\'");
            result = replace(result,"\"","\\\"");
            printf("replace: %s\n",result.c_str());
            
            sprintf(sql,"CREATE VERTEX Datacontent set isDiff=%d, SHA256hashCode='%s', minus_schemaCode=%d, minus_byteCount=%d, minus_data='%s'",dc->isDiff,dc->SHA256hashCode,dc->minusPatch->schemaCode,dc->minusPatch->byteCount,result.c_str());
        }
        printf("SQL: %s\n",sql);
        ret = createVertex(sql,&dc_cltid[i],&dc_rid[i]);
        if (ret!=0){
            printf("addData2Data..FAILED(vertex_DataContent)\n");
            return -1;
        }
        printf("@rid #%d:%lu\n",dc_cltid[i],dc_rid[i]);
        next_dc = dc->nextVersion;
        i++;
    }
    
    int head = count-1;
    
    //  head/lastestCommon/current
    printf("--------------------------------------------------[create_edge_toDataContent_head]\n");
    sprintf(sql,"CREATE EDGE toDataContent from #%d:%lu to #%d:%lu set type='head'",dh_cltid,dh_rid,dc_cltid[count-1],dc_rid[count-1]);
//    printf("SQL: %s\n",sql);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("addData2Data..FAILED(edge_toDataContent_head)\n");
        return -1;
    }

    printf("--------------------------------------------------[create_edge_toDataContent_current]\n");
    sprintf(sql,"CREATE EDGE toDataContent from #%d:%lu to #%d:%lu set type='current'",dh_cltid,dh_rid,dc_cltid[count-1],dc_rid[count-1]);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("addData2Data..FAILED(edge_toDataContent_current)\n");
        return -1;
    }

    printf("--------------------------------------------------[create_edge_toDataContent_lastestCommon]\n");
    sprintf(sql,"CREATE EDGE toDataContent from #%d:%lu to #%d:%lu set type='lastestCommon'",dh_cltid,dh_rid,dc_cltid[0],dc_rid[0]);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("addData2Data..FAILED(edge_toDataContent_lastestCommon)\n");
        return -1;
    }
    
    //  next/pre/toDataHolder_fromDC
    for(i=0;i<count;i++){
        printf("--------------------------------------------------[create_edge_toDataHolder_fromDC #%d]\n",i);
        sprintf(sql,"CREATE EDGE toDataHolder_fromDC from #%d:%lu to #%d:%lu",dc_cltid[i],dc_rid[i],dh_cltid,dh_rid);
        ret = sendCommand(sql);
        if (ret!=0){
            printf("addData2Data..FAILED(edge_toDataHolder_fromDC)\n");
            return -1;
        }
        
        // pre
        if(i != 0){
            printf("--------------------------------------------------[create_edge_toContent_Pre]\n");
            sprintf(sql,"CREATE EDGE toContent from #%d:%lu to #%d:%lu set type='pre'",dc_cltid[i],dc_rid[i],dc_cltid[i-1],dc_rid[i-1]);
            ret = sendCommand(sql);
            if (ret!=0){
                printf("addData2Data..FAILED(edge_toContent_Pre)\n");
                return -1;
            }
        }
        // next
        if(i != count-1){
            printf("--------------------------------------------------[create_edge_toContent_Next]\n");
            sprintf(sql,"CREATE EDGE toContent from #%d:%lu to #%d:%lu set type='next'",dc_cltid[i],dc_rid[i],dc_cltid[i+1],dc_rid[i+1]);
            ret = sendCommand(sql);
            if (ret!=0){
                printf("addData2Data..FAILED(edge_toContent_Next)\n");
                return -1;
            }
        }
    }
    
    // create Edge from Data to Data
    switch(dType){
        case _toUser:
            printf("--------------------------------------------------[create_edge_toUser]\n");
            sprintf(sql,"create edge toUser from (select from Data where dataID='%s') to #%d:%lu",dataID,dt_cltid,dt_rid);
            break;
        case _toCategory:
            printf("--------------------------------------------------[create_edge_toCategory]\n");
            sprintf(sql,"create edge toCategory from (select from Data where dataID='%s') to #%d:%lu",dataID,dt_cltid,dt_rid);
            break;
        case _toState:
            printf("--------------------------------------------------[create_edge_toState]\n");
            sprintf(sql,"create edge toState from (select from Data where dataID='%s') to #%d:%lu",dataID,dt_cltid,dt_rid);
            break;
        case _toTask:
            printf("--------------------------------------------------[create_edge_toTask]\n");
            sprintf(sql,"create edge toTask from (select from Data where dataID='%s') to #%d:%lu",dataID,dt_cltid,dt_rid);
            break;
        case _toSubTask:
            printf("--------------------------------------------------[create_edge_toSubTask]\n");
            sprintf(sql,"create edge toSubTask from (select from Data where dataID='%s') to #%d:%lu",dataID,dt_cltid,dt_rid);
            break;
        default:
            return -1;
            break;
    }
//    printf("SQL: %s\n",sql);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("addData2Data..FAILED(edge_toData)\n");
        return -1;
    }
    
    return 0;
}

ReturnErr removeUserFromOrg(char* orgID, char* userID){
    int ret = removeDataFromData(orgID, userID);
    return ret;
}

ReturnErr removeCategoryFromUser(char* userID, char* categoryID){
    int ret = removeDataFromData(userID, categoryID);
    return ret;
}

ReturnErr removeStateFromCategory(char* categoryID, char* stateID){
    int ret = removeDataFromData(categoryID, stateID);
    return ret;
}

ReturnErr removeTaskFromState(char* stateID, char* taskID){
    int ret = removeDataFromData(stateID, taskID);
    return ret;
}

ReturnErr removeTaskFromCategory(char* categoryID, char* taskID){
    int ret = removeDataFromData(categoryID, taskID);
    return ret;
}

ReturnErr removeSubTaskFromTask(char* taskID, char* subTaskID){
    int ret = removeDataFromData(taskID, subTaskID);
    return ret;
}

ReturnErr removeDataFromData(char* f_dataID, char* t_dataID){
    char sql[MAX_SQL_SIZE];
    printf("--------------------------------------------------[delete_edge(from..to)]\n");
    sprintf(sql,"DELETE EDGE from (select from Data where dataID='%s') to (select from Data where dataID='%s')",f_dataID,t_dataID);
    int ret = sendCommand(sql);
    if (ret!=0){
        printf("removeDataFromData..FAILED(delete_edge(from..to))\n");
        return -1;
    }
    return 0;
}

ReturnErr deleteObj(char* userID, char* upperID, char* objID){
    char sql[MAX_SQL_SIZE];

    //  delete edge
    printf("--------------------------------------------------[delete_edge(from..to)]\n");
    sprintf(sql,"DELETE EDGE from (select from Data where dataID='%s') to (select from Data where dataID='%s')",upperID,objID);
    int ret = sendCommand(sql);
    if (ret!=0){
        printf("deleteObj..FAILED(delete_edge(from..to))\n");
        return -1;
    }
    
    //  create edge + set dataID
    printf("--------------------------------------------------[create_edge_toDeletedData]\n");
    sprintf(sql,"create edge toDeletedData from (select from DeleteNode where userID='%s') to (select from Data where dataID='%s') set  dataID='%s'",userID,objID,upperID);
    ret = sendCommand(sql);
    if (ret!=0){
        printf("deleteObj..FAILED(create_edge_toDeletedData)\n");
        return -1;
    }
    return 0;
}

ReturnErr flushTrash(char* userID){
    char sql[MAX_SQL_SIZE];
    
    //  delete edge toDeletedData
    printf("--------------------------------------------------[delete_edge_toDeletedData(by userID)]\n");
    
    sprintf(sql,"delete edge from (select from DeleteNode where userID='%s')",userID);
    int ret = sendCommand(sql);
    if (ret!=0){
        printf("deleteObj..FAILED(delete_edge(from..to))\n");
        return -1;
    }
    return 0;
}

Org** queryOrgFromData(char* dataID){
    Org** org = queryDataFromData(dataID, _org);
    return org;
}

User** queryAllUsersFromData(char* dataID){
    User** user = queryDataFromData(dataID, _user);
    return user;
}

Category** queryAllCategoriesFromData(char* dataID){
    Category** cat = queryDataFromData(dataID, _category);
    return  cat;
}

State** queryAllStatesFromData(char* dataID){
    State** state = queryDataFromData(dataID, _state);
    return  state;
}

Task** queryAllTasksFromData(char* dataID){
    Task** task = queryDataFromData(dataID, _task);
    return task;
}

SubTask** queryAllSubTasksFromData(char* dataID){
    SubTask** subTask = queryDataFromData(dataID, _subTask);
    return subTask;
}

Data** queryDataFromData(char* dataID, int dType){
    char sql[MAX_SQL_SIZE];
    
    switch(dType){
        case _org:
            printf("--------------------------------------------------[get @rid_org]\n");
            break;
        case _user:
            printf("--------------------------------------------------[get @rid_user]\n");
            break;
        case _category:
            printf("--------------------------------------------------[get @rid_category]\n");
            break;
        case _state:
            printf("--------------------------------------------------[get @rid_state]\n");
            break;
        case _task:
            printf("--------------------------------------------------[get @rid_task]\n");
            break;
        case _subTask:
            printf("--------------------------------------------------[get @rid_subTask]\n");
            break;
        default:
            return NULL;
            break;
            
    }

    sprintf(sql,"select distinct(rid) from (select expand($f) let $a = (select @rid from (select expand(in().in().in().in().in()) from (select from Data where dataID='%s')) where dataType=%d), $c = (select @rid from (select expand(in().in().in()) from (select from Data where dataID='%s')) where dataType=%d), $d = (select @rid from (select expand(in().in()) from (select from Data where dataID='%s')) where dataType=%d), $e = (select @rid from (select expand(in()) from (select from Data where dataID='%s')) where dataType=%d), $f = unionall($a, $b, $c, $d, $e))",dataID,dType,dataID,dType,dataID,dType,dataID,dType,dataID,dType);
    printf("SQL: %s\n",sql);
    
    int i,j;
    char** result_rid = getArrayDistinctRid(sql);
    if(result_rid==NULL){
        printf("no result_rid\n");
        return NULL;
    }
    else{
        for(i=0;result_rid[i]!=NULL;i++)
            printf("result_rid[%d]: %s\n",i,result_rid[i]);
    }
    
    sprintf(sql,"select distinct(rid) from (select expand($l) let $h = (select @rid from (select expand(in()) from (select expand(in().in().in().in()) from (select from Data where dataID='%s')) where @class='DeleteNode')), $i = (select @rid from (select expand(in()) from (select expand(in().in().in()) from (select from Data where dataID='%s')) where @class='DeleteNode')), $j = (select @rid from (select expand(in()) from (select expand(in().in()) from (select from Data where dataID='%s')) where @class='DeleteNode')), $k = (select @rid from (select expand(in()) from (select expand(in()) from (select from Data where dataID='%s')) where @class='DeleteNode')), $l = unionall($h, $i, $j, $k))",dataID,dataID,dataID,dataID);
    printf("SQL: %s\n",sql);
    
    char** dup_rid = getArrayDistinctRid(sql);
    
    if(dup_rid!=NULL){
        //  show duplicate rid
        for(i=0;dup_rid[i]!=NULL;i++)
            printf("dup_rid[%d]: %s\n",i,dup_rid[i]);
        // filter rid
        for(i=0;dup_rid[i]!=NULL;i++){
            for(j=0;result_rid[j]!=NULL;j++){
                if(strcmp(result_rid[j],dup_rid[i])==0){
                    sprintf(result_rid[j],"%s","-");
                    break;
                }
            }
        }
    }
    else{
        printf("no dup_id\n");

    }
    
    int count=0;
    //  show filtered id
    for(j=0;result_rid[j]!=NULL;j++){
        if(strcmp(result_rid[j],"-")!=0){
            printf("filter_rid: %s\n",result_rid[j]);
            count++;
        }
    }
    printf("\nnum(distinct+filter): %d\n",count);
    if(count==0){
        printf("no result_rid\n");
        return NULL;
    }
    
    Data** dt = (Data**)malloc(sizeof(Data*)*(count+1));
//    for(i=0;i<count;i++){
//        dt[i] = (Data*)malloc(sizeof(Data));
//    }
    for(j=0;result_rid[j]!=NULL;j++){
        if(strcmp(result_rid[j],"-")!=0){
            dt[j] = queryDataByRid(result_rid[j]);
        }
    }
//    printf("j: %d\n",j);
    dt[count]=NULL;
    
    if(dup_rid!=NULL){
        for(i=0;dup_rid[i]!=NULL;i++){
            free(dup_rid[i]);
        }
        free(dup_rid);
    }
    
    if(result_rid!=NULL){
        for(i=0;result_rid[i]!=NULL;i++){
            free(result_rid[i]);
        }
        free(result_rid);
    }
    
    return dt;
//    return NULL;
}
                          
Data* queryDataByRid(char* rid){
    char sql[MAX_SQL_SIZE];
    char* token;
    
    //  Data
    printf("--------------------------------------------------[get_info_Data]\n");
    sprintf(sql,"SELECT dataID,dataName,dataType,chatRoom from %s",rid);
    printf("SQL: %s\n",sql);
    char* result = getContent(sql);
    
    if(result==NULL){
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    printf("--- Data ---\n");
    Data* dt = (Data*)malloc(sizeof(Data));
    
    token = strtok(result,"\"");
    token = strtok(NULL,"\"");
    printf("dataID: %s\n",token);
    dt->dataID = strdup(token);
    
    token = strtok(NULL,"\"");
    token = strtok(NULL,"\"");
    printf("dataName: %s\n",token);
    dt->dataName = strdup(token);
    
    token = strtok(NULL,":");
    token = strtok(NULL,",");
    printf("dataType: %s\n",token);
    dt->dataType = atoi(token);
    
    token = strtok(NULL,"\"");
    if(token!=NULL){
        token = strtok(NULL,"\"");
        printf("chatRoom: %s\n\n",token);
        dt->chatRoom = strdup(token);
    }
    else{
        dt->chatRoom = NULL;
    }
    
    free(result);
    
    //  DataHolder
    printf("--------------------------------------------------[get_info_DataHolder]\n");
    sprintf(sql,"SELECT @rid,versionKeeped from (select expand(out('toDataHolder')) from %s)",rid);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    
    if(result==NULL){
        free((char*)dt->dataID);
        free(dt->dataName);
        if(dt->chatRoom != NULL)
            free(dt->chatRoom);
        free(dt);
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    printf("--- DataHolder ---\n");
    DataHolder* dh = (DataHolder*)malloc(sizeof(DataHolder));
    dt->content = dh;
    
    token = strtok(result,":");
    token = strtok(NULL,",");
    printf("dh_@rid: %s\n",token);
    char* rid_dh = strdup(token);
    
    token = strtok(NULL,":");
    token = strtok(NULL,",");
    printf("versionKeeped: %s\n\n",token);
    dh->versionKeeped = atoi(token);
    dh->head = NULL;
    dh->lastestCommon = NULL;
    dh->current = NULL;
    
    free(result);
    
    //  count DataContent
    printf("--------------------------------------------------[get_count_DataContent]\n");
    sprintf(sql,"select count(*) from (select expand(in('toDataHolder_fromDC')) from %s)",rid_dh);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    
    if(result==NULL){
        free(rid_dh);
        free(dh);
        free((char*)dt->dataID);
        free(dt->dataName);
        if(dt->chatRoom != NULL)
            free(dt->chatRoom);
        free(dt);
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    token = strtok(result,":");
    token = strtok(NULL,"l");
    printf("count: %s\n\n",token);
    int count = atoi(token);
    
    free(result);
    
    //  next/pre/dataHd
    //  isDiff SHA256hashCode timeStamp(NULL) minus/plus/full
    
    //  DataContent
    printf("--------------------------------------------------[get_DataContent]\n");
    sprintf(sql,"select isDiff,SHA256hashCode,timeStamp,plus_schemaCode,plus_byteCount,plus_data,minus_schemaCode,minus_byteCount,minus_data,full_schemaCode,full_byteCount,full_data from (select expand(in('toDataHolder_fromDC')) from %s)",rid_dh);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    
    if(result==NULL){
        free(rid_dh);
        free(dh);
        free((char*)dt->dataID);
        free(dt->dataName);
        if(dt->chatRoom != NULL)
            free(dt->chatRoom);
        free(dt);
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    DataContent** dc = (DataContent**)malloc(sizeof(DataContent*)*(count+1));
    int i;
    for(i=0;i<count;i++){
        dc[i] = (DataContent*)malloc(sizeof(DataContent));
    }
    dh->head = dc[count-1];
    dh->lastestCommon = dc[0];
    dh->current = dc[count-1];
    dc[count] = NULL;
    
    string init;
    init.assign(result);
    printf("before: %s\n",result);
    string res = replace(init,"\\'","'");
    res = replace(res,"\\\"","\"");
    printf("\n------------------------------------------------------------------------------------------------------------\n\n");
    printf("replace: %s\n",res.c_str());
    printf("\n------------------------------------------------------------------------------------------------------------\n\n");
    
    free(result);
    result = strdup(res.c_str());
    
    char* tmp_tok = strtok(result,"#");
    char **dc_str = (char**)malloc(sizeof(char*)*count);
    
    for(i=0;i<count;i++){
        printf("--- DataContent[%d] ---\n",i);
        printf("tmp_tok: %s\n",tmp_tok);
        //        printf("len: %d\n",strlen(tmp_tok));
        
        dc_str[i]= strdup(tmp_tok);
        printf("\ndc_str: %s\n",dc_str[i]);
        
        dc[i]->timeStamp = NULL;
        dc[i]->minusPatch = NULL;
        dc[i]->plusPatch = NULL;
        dc[i]->fullContent = NULL;
        dc[i]->dataHd = dh;
        
        if(i!=0){
            dc[i]->preVersion = dc[i-1];
        }
        if(i!=count-1){
            dc[i]->nextVersion = dc[i+1];
        }
        
        tmp_tok = strtok(NULL, "#");
    }
    dc[0]->preVersion = NULL;
    dc[count-1]->nextVersion = NULL;
    free(result);
    
    char* br;
    char* pt_data;
    
    for(i=0;i<count;i++){
        printf("\n[%d]\n",i);
        
        //  isDiff
        token = strtok_r(dc_str[i],":",&br);
        token = strtok_r(NULL,",",&br);
        printf("isDiff: %s\n",token);
        
        if(strcmp(token,"true")==0){
            dc[i]->isDiff = TRUE;
        }
        else{
            dc[i]->isDiff = FALSE;
        }
        
        //  SHA256hashCode
        token = strtok_r(NULL,"\"",&br);
        token = strtok_r(NULL,"\"",&br);
        printf("SHA256hashCode: %s\n",token);
        dc[i]->SHA256hashCode = strdup(token);
        
        //  timeStamp(NULL)
        
        //  minusPatch/plusPatch/fullContent
        result = br+1;
        printf("result: %s\n",result);
        token = strtok_r(result, ":", &br);
        
        while(token!=NULL){
            printf("key: %s\n",token);
            if(token == NULL)
                break;
            if(strcmp(token,"minus_schemaCode")==0){
                printf("\n[minus]\n");
                dc[i]->minusPatch = (ObjectBinary*)malloc(sizeof(ObjectBinary));
                
                //  schemaCode
                token = strtok_r(NULL,",",&br);
                printf("schemaCode: %s\n",token);
                dc[i]->minusPatch->schemaCode = atoi(token);
                
                //  byteCount
                token = strtok_r(NULL,":",&br);
                token = strtok_r(NULL,",",&br);
                printf("bytecount: %s\n",token);
                dc[i]->minusPatch->byteCount = atoi(token);
                
                printf("br: %s\n",br);
                pt_data = br+12;
                
                int b_count = atoi(token);
                dc[i]->minusPatch->data = (char*)calloc(b_count+1,sizeof(char));
                memcpy(dc[i]->minusPatch->data,pt_data,b_count);
                printf("dc_data: %s\n",dc[i]->minusPatch->data);
                //                strcat(dc[i]->minusPatch->data,"1");
                //                printf("len: %d\n",strlen(dc[i]->minusPatch->data));
                //                printf("--- %c\n",dc[i]->minusPatch->data[b_count]);
                pt_data = br+12+b_count+1;
                if(pt_data[0] == '\0'){
                    printf("data is NULL\n");
                    break;
                }
                else{
                    pt_data = br+11+b_count+2;
                    printf("pt_data: %s\n",pt_data);
                    token = strtok_r(pt_data,":",&br);
                }
            }
            //            token = NULL;
            else if(strcmp(token,"plus_schemaCode")==0){
                printf("\n[plus]\n");
                dc[i]->plusPatch = (ObjectBinary*)malloc(sizeof(ObjectBinary));
                
                //  schemaCode
                token = strtok_r(NULL,",",&br);
                printf("schemaCode: %s\n",token);
                dc[i]->plusPatch->schemaCode = atoi(token);
                
                //  byteCount
                token = strtok_r(NULL,":",&br);
                token = strtok_r(NULL,",",&br);
                printf("bytecount: %s\n",token);
                dc[i]->plusPatch->byteCount = atoi(token);
                
                printf("br: %s\n",br);
                pt_data = br+11;
                printf("pt_data: %s\n",pt_data);
                
                int b_count = atoi(token);
                printf("b_count: %d\n",b_count);
                
                dc[i]->plusPatch->data = (char*)calloc(b_count+1,sizeof(char));
                memcpy(dc[i]->plusPatch->data,pt_data,b_count);
                printf("dc_data: %s\n",dc[i]->plusPatch->data);
                pt_data = br+11+b_count+1;
                if(pt_data[0] == '\0'){
                    printf("data is NULL\n");
                    break;
                }
                else{
                    pt_data = br+11+b_count+2;
                    printf("pt_data: %s\n",pt_data);
                    token = strtok_r(pt_data,":",&br);
                }
            }
            //            if(strcmp(token,"full_schemaCode")==0){
            else{
                printf("\n[full]\n");
                dc[i]->fullContent = (ObjectBinary*)malloc(sizeof(ObjectBinary));
                
                //  schemaCode
                token = strtok_r(NULL,",",&br);
                printf("schemaCode: %s\n",token);
                dc[i]->fullContent->schemaCode = atoi(token);
                
                //  byteCount
                token = strtok_r(NULL,":",&br);
                token = strtok_r(NULL,",",&br);
                printf("bytecount: %s\n",token);
                dc[i]->fullContent->byteCount = atoi(token);
                
                printf("br: %s\n",br);
                pt_data = br+11;
                printf("pt_data: %s\n",pt_data);
                
                int b_count = atoi(token);
                printf("b_count: %d\n",b_count);
                
                dc[i]->fullContent->data = (char*)calloc(b_count+1,sizeof(char));
                memcpy(dc[i]->fullContent->data,pt_data,b_count);
                printf("dc_data: %s\n",dc[i]->fullContent->data);
                pt_data = br+11+b_count+1;
                if(pt_data[0] == '\0'){
                    printf("data is NULL\n");
                    break;
                }
                else{
                    pt_data = br+11+b_count+2;
                    printf("pt_data: %s\n",pt_data);
                }
            }
        }
        free(dc_str[i]);
    }
    free(rid_dh);
    free(dc_str);
    free(dc);
    return dt;
}

Data* queryDataByID(char* dataID){
    char sql[MAX_SQL_SIZE];
    char* token;
    
    //  Data
    printf("--------------------------------------------------[get_info_Data]\n");
    sprintf(sql,"SELECT dataID,dataName,dataType,chatRoom from Data where dataID='%s'",dataID);
    printf("SQL: %s\n",sql);
    char* result = getContent(sql);
    
    if(result==NULL){
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    printf("--- Data ---\n");
    Data* dt = (Data*)malloc(sizeof(Data));
    
    token = strtok(result,"\"");
    token = strtok(NULL,"\"");
    printf("dataID: %s\n",token);
    dt->dataID = strdup(token);
    
    token = strtok(NULL,"\"");
    token = strtok(NULL,"\"");
    printf("dataName: %s\n",token);
    dt->dataName = strdup(token);
    
    token = strtok(NULL,":");
    token = strtok(NULL,",");
    printf("dataType: %s\n",token);
    dt->dataType = atoi(token);
    
    token = strtok(NULL,"\"");
    if(token!=NULL){
        token = strtok(NULL,"\"");
        printf("chatRoom: %s\n\n",token);
        dt->chatRoom = strdup(token);
    }
    else{
        dt->chatRoom = NULL;
    }

    free(result);
    
    //  DataHolder
    printf("--------------------------------------------------[get_info_DataHolder]\n");
    sprintf(sql,"SELECT @rid,versionKeeped from (select expand(out('toDataHolder')) from Data where dataID='%s')",dataID);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    
    if(result==NULL){
        free((char*)dt->dataID);
        free(dt->dataName);
        if(dt->chatRoom != NULL)
            free(dt->chatRoom);
        free(dt);
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    printf("--- DataHolder ---\n");
    DataHolder* dh = (DataHolder*)malloc(sizeof(DataHolder));
    dt->content = dh;
    
    token = strtok(result,":");
    token = strtok(NULL,",");
    printf("dh_@rid: %s\n",token);
    char* rid_dh = strdup(token);
    
    token = strtok(NULL,":");
    token = strtok(NULL,",");
    printf("versionKeeped: %s\n\n",token);
    dh->versionKeeped = atoi(token);
    dh->head = NULL;
    dh->lastestCommon = NULL;
    dh->current = NULL;
    
    free(result);
    
    //  count DataContent
    printf("--------------------------------------------------[get_count_DataContent]\n");
    sprintf(sql,"select count(*) from (select expand(in('toDataHolder_fromDC')) from %s)",rid_dh);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    
    if(result==NULL){
        free(rid_dh);
        free(dh);
        free((char*)dt->dataID);
        free(dt->dataName);
        if(dt->chatRoom != NULL)
            free(dt->chatRoom);
        free(dt);
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    token = strtok(result,":");
    token = strtok(NULL,"l");
    printf("count: %s\n\n",token);
    int count = atoi(token);
    
    free(result);
    
    //  next/pre/dataHd
    //  isDiff SHA256hashCode timeStamp(NULL) minus/plus/full
    
    //  DataContent
    printf("--------------------------------------------------[get_DataContent]\n");
    sprintf(sql,"select isDiff,SHA256hashCode,timeStamp,plus_schemaCode,plus_byteCount,plus_data,minus_schemaCode,minus_byteCount,minus_data,full_schemaCode,full_byteCount,full_data from (select expand(in('toDataHolder_fromDC')) from %s)",rid_dh);
    printf("SQL: %s\n",sql);
    result = getContent(sql);
    
    if(result==NULL){
        free(rid_dh);
        free(dh);
        free((char*)dt->dataID);
        free(dt->dataName);
        if(dt->chatRoom != NULL)
            free(dt->chatRoom);
        free(dt);
        return NULL;
    }
    printf("result: %s\n\n\n\n\n",result);
    DataContent** dc = (DataContent**)malloc(sizeof(DataContent*)*(count+1));
    int i;
    for(i=0;i<count;i++){
        dc[i] = (DataContent*)malloc(sizeof(DataContent));
    }
    dh->head = dc[count-1];
    dh->lastestCommon = dc[0];
    dh->current = dc[count-1];
    dc[count] = NULL;
    
    string init;
    init.assign(result);
    printf("before: %s\n",result);
    string res = replace(init,"\\'","'");
    res = replace(res,"\\\"","\"");
    printf("\n------------------------------------------------------------------------------------------------------------\n\n");
    printf("replace: %s\n",res.c_str());
    printf("\n------------------------------------------------------------------------------------------------------------\n\n");
    
    free(result);
    result = strdup(res.c_str());
    
    char* tmp_tok = strtok(result,"#");
    char **dc_str = (char**)malloc(sizeof(char*)*count);

    for(i=0;i<count;i++){
        printf("--- DataContent[%d] ---\n",i);
        printf("tmp_tok: %s\n",tmp_tok);
//        printf("len: %d\n",strlen(tmp_tok));
        
        dc_str[i]= strdup(tmp_tok);
        printf("\ndc_str: %s\n",dc_str[i]);

        dc[i]->timeStamp = NULL;
        dc[i]->minusPatch = NULL;
        dc[i]->plusPatch = NULL;
        dc[i]->fullContent = NULL;
        dc[i]->dataHd = dh;

        if(i!=0){
            dc[i]->preVersion = dc[i-1];
        }
        if(i!=count-1){
            dc[i]->nextVersion = dc[i+1];
        }
        
        tmp_tok = strtok(NULL, "#");
    }
    dc[0]->preVersion = NULL;
    dc[count-1]->nextVersion = NULL;
    free(result);

    char* br;
    char* pt_data;

    for(i=0;i<count;i++){
        printf("\n[%d]\n",i);
        
        //  isDiff
        token = strtok_r(dc_str[i],":",&br);
        token = strtok_r(NULL,",",&br);
        printf("isDiff: %s\n",token);
        
        if(strcmp(token,"true")==0){
            dc[i]->isDiff = TRUE;
        }
        else{
            dc[i]->isDiff = FALSE;
        }

        //  SHA256hashCode
        token = strtok_r(NULL,"\"",&br);
        token = strtok_r(NULL,"\"",&br);
        printf("SHA256hashCode: %s\n",token);
        dc[i]->SHA256hashCode = strdup(token);
        
        //  timeStamp(NULL)
        
        //  minusPatch/plusPatch/fullContent
        result = br+1;
        printf("result: %s\n",result);
        token = strtok_r(result, ":", &br);
        
        while(token!=NULL){
            printf("key: %s\n",token);
            if(token == NULL)
                break;
            if(strcmp(token,"minus_schemaCode")==0){
                printf("\n[minus]\n");
                dc[i]->minusPatch = (ObjectBinary*)malloc(sizeof(ObjectBinary));
                
                //  schemaCode
                token = strtok_r(NULL,",",&br);
                printf("schemaCode: %s\n",token);
                dc[i]->minusPatch->schemaCode = atoi(token);
                
                //  byteCount
                token = strtok_r(NULL,":",&br);
                token = strtok_r(NULL,",",&br);
                printf("bytecount: %s\n",token);
                dc[i]->minusPatch->byteCount = atoi(token);
                
                printf("br: %s\n",br);
                pt_data = br+12;
                
                int b_count = atoi(token);
                dc[i]->minusPatch->data = (char*)calloc(b_count+1,sizeof(char));
                memcpy(dc[i]->minusPatch->data,pt_data,b_count);
                printf("dc_data: %s\n",dc[i]->minusPatch->data);
//                strcat(dc[i]->minusPatch->data,"1");
//                printf("len: %d\n",strlen(dc[i]->minusPatch->data));
//                printf("--- %c\n",dc[i]->minusPatch->data[b_count]);
                pt_data = br+12+b_count+1;
                if(pt_data[0] == '\0'){
                    printf("data is NULL\n");
                    break;
                }
                else{
                    pt_data = br+11+b_count+2;
                    printf("pt_data: %s\n",pt_data);
                    token = strtok_r(pt_data,":",&br);
                }
            }
//            token = NULL;
            else if(strcmp(token,"plus_schemaCode")==0){
                printf("\n[plus]\n");
                dc[i]->plusPatch = (ObjectBinary*)malloc(sizeof(ObjectBinary));
                
                //  schemaCode
                token = strtok_r(NULL,",",&br);
                printf("schemaCode: %s\n",token);
                dc[i]->plusPatch->schemaCode = atoi(token);
                
                //  byteCount
                token = strtok_r(NULL,":",&br);
                token = strtok_r(NULL,",",&br);
                printf("bytecount: %s\n",token);
                dc[i]->plusPatch->byteCount = atoi(token);
                
                printf("br: %s\n",br);
                pt_data = br+11;
                printf("pt_data: %s\n",pt_data);
                
                int b_count = atoi(token);
                printf("b_count: %d\n",b_count);
                
                dc[i]->plusPatch->data = (char*)calloc(b_count+1,sizeof(char));
                memcpy(dc[i]->plusPatch->data,pt_data,b_count);
                printf("dc_data: %s\n",dc[i]->plusPatch->data);
                pt_data = br+11+b_count+1;
                if(pt_data[0] == '\0'){
                    printf("data is NULL\n");
                    break;
                }
                else{
                    pt_data = br+11+b_count+2;
                    printf("pt_data: %s\n",pt_data);
                    token = strtok_r(pt_data,":",&br);
                }
            }
//            if(strcmp(token,"full_schemaCode")==0){
            else{
                printf("\n[full]\n");
                dc[i]->fullContent = (ObjectBinary*)malloc(sizeof(ObjectBinary));
                
                //  schemaCode
                token = strtok_r(NULL,",",&br);
                printf("schemaCode: %s\n",token);
                dc[i]->fullContent->schemaCode = atoi(token);
                
                //  byteCount
                token = strtok_r(NULL,":",&br);
                token = strtok_r(NULL,",",&br);
                printf("bytecount: %s\n",token);
                dc[i]->fullContent->byteCount = atoi(token);
                
                printf("br: %s\n",br);
                pt_data = br+11;
                printf("pt_data: %s\n",pt_data);
                
                int b_count = atoi(token);
                printf("b_count: %d\n",b_count);
                
                dc[i]->fullContent->data = (char*)calloc(b_count+1,sizeof(char));
                memcpy(dc[i]->fullContent->data,pt_data,b_count);
                printf("dc_data: %s\n",dc[i]->fullContent->data);
                pt_data = br+11+b_count+1;
                if(pt_data[0] == '\0'){
                    printf("data is NULL\n");
                    break;
                }
                else{
                    pt_data = br+11+b_count+2;
                    printf("pt_data: %s\n",pt_data);
                }
            }
        }
        free(dc_str[i]);
    }
    free(rid_dh);
    free(dc_str);
    free(dc);
    return dt;
}

t_bool isObjectOwnedByUser(char* userID, char* objID){
    t_bool bl = isObjectUnderData(userID,objID);
    return bl;
}

t_bool isObjectUnderCategory(char* categoryID, char* objID){
    t_bool bl = isObjectUnderData(categoryID,objID);
    return bl;
}

t_bool isObjectUnderState(char* stateID, char* objID){
    t_bool bl = isObjectUnderData(stateID,objID);
    return bl;
}

t_bool isObjectUnderTask(char* taskID, char* objID){
    t_bool bl = isObjectUnderData(taskID,objID);
    return bl;
}

t_bool isObjectUnderData(char* dataID, char* objID){
    char sql[MAX_SQL_SIZE];
    printf("--------------------------------------------------[get_count_path]\n");
    
    //  for ordb 2.0
    sprintf(sql,"select count(*) from (traverse in() from (select from Data where dataID='%s')) where dataID = '%s'",objID,dataID);
    
    //  for ordb 2.1
//    sprintf(sql,"select count(*) from(select expand(path) from(select shortestPath((select from Data where dataID='%s'),(select from Data where dataID='%s'),'IN') as path))",objID,dataID);
    printf("SQL: %s\n",sql);
    char* result = getContent(sql);
    if(result==NULL){
        printf("not found dataID/objID\n");
        return FALSE;
    }
    printf("result: %s\n",result);
    char* token = strtok(result,":");
    token = strtok(NULL,"l");
    printf("count: %s\n",token);
    
    if(strcmp(token,"0")!=0){
        printf("object is under data.\n");
        free(result);
        return TRUE;
    }
    else{
        printf("object isn't under data.\n");
        free(result);
        return FALSE;
    }
}

string getDiff(char* old_str, char* new_str){
    diff_match_patch<wstring> dmp;
    string s;
    wstring strPatch;
    wstring str1 = wstring(new_str,new_str+strlen(new_str));
    //    wcout << str1 << endl;
    wstring str0 = wstring(old_str,old_str+strlen(old_str));
    //    wcout << str0 << endl;
    strPatch = dmp.patch_toText(dmp.patch_make(str0, str1));
    s.assign(strPatch.begin(),strPatch.end());
    //    wcout << strPatch << endl;
    return s;
}

string getPatch(char* str, char* str_patch){
    diff_match_patch<wstring> dmp;
    string s;
    wstring strResult;
    strResult = wstring(str,str+strlen(str));
    pair<wstring, vector<bool> > out
    = dmp.patch_apply(dmp.patch_fromText(wstring(str_patch,str_patch+strlen(str_patch))), strResult);
    strResult = out.first;
    return s.assign(strResult.begin(),strResult.end());
}

void test_setNewData(){
    const char* uuid_data;
    Data* _mydata;
    uuid_data = _createNewData(&_mydata, _category);
    
    printf("uuid: %s\n",_mydata->dataID);
    printf("dataType: %d\n",_mydata->dataType);
    printf("ver_keeped: %d\n\n",_mydata->content->versionKeeped);
    
    char content0[] = "<root><attachmentFTOLinks></attachmentFTOLinks><book_id>bk100</book_id><author>Gambardella, Matthew</author><title>XML Developer's Guide</title><genre>Computer</genre><price>44.95</price></root>";
    char content1[] = "<root><attachmentFTOLinks></attachmentFTOLinks><book_id>bk100</book_id><author>Pim</author><title>XML Developer's Guide</title><genre>Computer</genre><price>44.95</price></root>";
    char content2[] = "<root><attachmentFTOLinks><link id='1'>www.xyz.com</link><link id='2'>www.123.com</link></attachmentFTOLinks><book_id>bk100</book_id><author>Pim</author><title>XML Developer's Guide</title><genre>Computer</genre><price>44.95</price></root>";
    
    ObjectBinary* obj0 = _createNewObjBinary(content0,_mydata->dataType);
    ObjectBinary* obj1 = _createNewObjBinary(content1,_mydata->dataType);
    ObjectBinary* obj2 = _createNewObjBinary(content2,_mydata->dataType);
    
    setNewDataContent(_mydata,obj0);
    setNewDataContent(_mydata,obj1);
//    printf("\n\n\n\n--- test_getData[1] --\n");
//    _mydata->content->current = _mydata->content->lastestCommon;
//    test_getData(&_mydata);
    setNewDataContent(_mydata,obj2);
    
    string diff = getDiff("www.xyz.com", "www.abc.com");
    char* res_diff = strdup(diff.c_str());
    printf("res_diff: %s\n\n",res_diff);
    
    setNewDataDiffWithTag(_mydata, "attachmentFTOLinks", "1", res_diff);
    setNewDataDiffWithTag(_mydata, "attachmentFTOLinks", "3", "www.qaz.com");
    
    string s = getDiff("44.95", "1000");
    char* diff_price = strdup(s.c_str());
    
    setNewDataDiffWithTag(_mydata, "price", NULL, diff_price);
    setNewDataDiffWithTag(_mydata, "titel", NULL, "hello");
    
    setDataName(_mydata, "myTask2");
    printf("dataName: %s\n",_mydata->dataName);
    setChatRoom(_mydata, "chat-room");
    printf("chatRoom: %s\n",_mydata->chatRoom);

    free(obj0->data);
    free(obj0);
    free(obj1->data);
    free(obj1);
    free(obj2->data);
    free(obj2);
    
    free(res_diff);
    free(diff_price);
    
//    printf("\n\n\n\n--- test_getData[2] --\n");
//    _mydata->content->current = _mydata->content->lastestCommon;
//    test_getData(&_mydata);

    printf("\n\n\n\n--- test_CRUD --\n");
    testCRUD(&_mydata);
    
    /* free Data */
    int i=0;
    DataContent *mydc, *next_mydc;
    for(mydc=_mydata->content->lastestCommon;mydc!=NULL;mydc=next_mydc){
        printf("-- %d --\n",i);
        next_mydc = mydc->nextVersion;
        if(mydc->SHA256hashCode != NULL)
            free(mydc->SHA256hashCode);
        if(mydc->timeStamp != NULL)
            free(mydc->timeStamp->timeStampCode);
        if(mydc->minusPatch != NULL){
            printf("minus: %s\n",mydc->minusPatch->data);
            free(mydc->minusPatch->data);
            free(mydc->minusPatch);
        }
        if(mydc->plusPatch != NULL){
            printf("plus: %s\n",mydc->plusPatch->data);
            printf("len: %d\n",strlen(mydc->plusPatch->data));
            printf("sizeof: %d\n",sizeof(mydc->plusPatch->data));
            free(mydc->plusPatch->data);
            free(mydc->plusPatch);
        }
        if(mydc->fullContent != NULL){
            printf("full: %s\n",mydc->fullContent->data);
            free(mydc->fullContent->data);
            free(mydc->fullContent);
        }
        free(mydc);
        i++;
    }
    free(_mydata->content);
    free((char*)_mydata->dataID);
    free(_mydata->dataName);
    free(_mydata->chatRoom);
    free(_mydata);
}

void test_getData(Data** data){
    Data* dt = *data;
    
//  getAttachmentFTOLinks -----------------------------------------------
//    Text* arr_link = getAttachmentFTOLinks(dt);
//    int i;
//    for(i=0;arr_link[i]!=NULL;i++){
//        printf("link[%d]: %s\n",i,arr_link[i]);
//        free(arr_link[i]);
//    }
//    free(arr_link);
//-----------------------------------------------------------------------
    
//  setTagContent -------------------------------------------------------
//    ObjectBinary* obj0 = (ObjectBinary*)malloc(sizeof(ObjectBinary));
//    obj0->schemaCode = dt->content->head->fullContent->schemaCode;
//    obj0->byteCount = dt->content->head->fullContent->byteCount;
//    obj0->data = strdup(dt->content->head->fullContent->data);
//    
//    int count_bytes = setTagContent(obj0,"1234567","attachmentFTOLinks","2");
//    printf("schemaCode: %d\n",obj0->schemaCode);
//    printf("byteCount: %d\n",obj0->byteCount);
//    printf("data: %s\n",obj0->data);
//    free(obj0->data);
//    free(obj0);
//-----------------------------------------------------------------------
    
//  getTagContent + getTagContentWithName -------------------------------
//    char* res1 = getTagContent(dt->content->head->fullContent,_attachmentFTOLinks,"4");
//    printf("res(enum): %s\n",res1);
//    free(res1);
//    char* res2 = getTagContentWithName(dt->content->head->fullContent,"attachmentFTOLinks","4");
//    printf("res(tagName): %s\n",res2);
//    free(res2);
//-----------------------------------------------------------------------
    
//  getDataContentWithTag -----------------------------------------------
//    char* val = getDataContentWithTag(dt, "attachmentFTOLinks", "2");
//    printf("val: %s\n",val);
//    free(val);
//-----------------------------------------------------------------------
    
//  getDiffDataPreVer + NextVer -----------------------------------------
//    char* diff_pre = getDiffDataPreVer(dt);
//    if(diff_pre != NULL){
//        printf("diff_pre: %s\n",diff_pre);
//        free(diff_pre);
//    }
//    char* diff_next = getDiffDataNextVer(dt);
//    if(diff_next != NULL){
//        printf("diff_next: %s\n",diff_next);
//        free(diff_next);
//    }
//-----------------------------------------------------------------------
    
//  getContentPreVer + NextVer ------------------------------------------
//    ObjectBinary* obj_pv = getContentPreVer(dt);
//    ObjectBinary* obj_nv = getContentNextVer(dt);
//    if(obj_pv != NULL){
//        printf("schemaCode_pv: %d\n",obj_pv->schemaCode);
//        printf("byteCount_pv: %d\n",obj_pv->byteCount);
//        printf("data_pv: %s\n",obj_pv->data);
//        free(obj_pv->data);
//        free(obj_pv);
//    }
//    if(obj_nv != NULL){
//        printf("schemaCode_nv: %d\n",obj_nv->schemaCode);
//        printf("byteCount_nv: %d\n",obj_nv->byteCount);
//        printf("data_nv: %s\n",obj_nv->data);
//        free(obj_nv->data);
//        free(obj_nv);
//    }
//-----------------------------------------------------------------------
    
//  getDiffDataAtHead + LastestCommon -----------------------------------
//    printf("\n--- getDiffDataAtHead + LastestCommon ---\n");
//    char* diff_h = getDiffDataAtHead(dt);
//    char* diff_l = getDiffDataAtLastestCommon(dt);
//    printf("diff_h: %s\n",diff_h);
//    printf("diff_l: %s\n",diff_l);
//    free(diff_h);
//    free(diff_l);
//-----------------------------------------------------------------------
    
//  getDataContent + LastestCommon --------------------------------------
//    ObjectBinary *obj_h = getDataContent(dt);
//    ObjectBinary *obj_l = getDataContentLastestCommon(dt);
//    printf("\n--- getDataContent ---\n");
//    printf("schemaCode: %d\n",obj_h->schemaCode);
//    printf("byteCount: %d\n",obj_h->byteCount);
//    printf("data: %s\n",obj_h->data);
//    
//    printf("\n--- getDataContentLastestCommon ---\n");
//    printf("schemaCode: %d\n",obj_l->schemaCode);
//    printf("byteCount: %d\n",obj_l->byteCount);
//    printf("data: %s\n",obj_l->data);
//    free(obj_h->data);
//    free(obj_l->data);
//    free(obj_h);
//    free(obj_l);
//-----------------------------------------------------------------------

}

void testCRUD(Data** data){
//----[1]------------------------------------------------------------------
//    prepareDB();
//-------------------------------------------------------------------------
    int ret;
    Sockfd = connectSocket();
    if (Sockfd < 0){
        printf ("error connectSocket\n");
    }
    
    ret = openDatabase(DB_NAME);
    if (ret!=0) {
        printf ("error openDatabase\n");
    }
    
//    char sql[]="create vertex Data set dataName='document', dataID='123456'";
//    short _cltid;
//    long _rid;
//    createVertex(sql, &_cltid, &_rid);
//    printf("@rid #%d:%lu\n",_cltid,_rid);
    
//----[2]------------------------------------------------------------------
/*
    Schema test_schema[]="<root><attachmentFTOLinks></attachmentFTOLinks><book_id></book_id><author></author><title></title><genre></genre><price></price></root>";
    
    const char* uuid_user = createUser("Pimpat", test_schema);
    const char* uuid_user2 = createUser("Tanapon", test_schema);
    const char* uuid_cat = createCategory("Pics", test_schema);
    const char* uuid_cat2 = createCategory("test-Category", test_schema);
    const char* uuid_task = createTask("myTask", test_schema);
    const char* uuid_state = createState("myState", test_schema);
    const char* uuid_state2 = createState("myState2", test_schema);
    
    //  Pimpat --> Pics
    addData2DataByID((char*)uuid_user, (char*)uuid_cat, _toCategory);
    //  Tanapon --> Pics
    addData2DataByID((char*)uuid_user2, (char*)uuid_cat, _toCategory);
    //  Tanapon --> test-Category
    addData2DataByID((char*)uuid_user2, (char*)uuid_cat2, _toCategory);
    //  Pics --> myTask
    addTask2CategoryByID((char*)uuid_cat, (char*)uuid_task);
    //  test-Category --> myState
    addState2CategoryByID((char*)uuid_cat2,(char*)uuid_state);
    //  test-Category --> myState2
    addState2CategoryByID((char*)uuid_cat2,(char*)uuid_state2);
    //  myState2 --> myTask
    addTask2StateByID((char*)uuid_state2,(char*)uuid_task);
    //  myState2 --> myTask2
    addData2Data((char*)uuid_state2,dt,_toTask);
 
    //  delete myState
    deleteObj((char*)uuid_user2, (char*)uuid_cat2, (char*)uuid_state);
*/
//-------------------------------------------------------------------------
    Data* dt = *data;
    int i=0;

//----[3]------------------------------------------------------------------
//    setDataNameByID("A99B27E341CA424D84FADCCB5B856910","Tanapon");
//    setChatRoomByID("A99B27E341CA424D84FADCCB5B856910", "chat-room2");
//-------------------------------------------------------------------------
    
//----[4]------------------------------------------------------------------
//    Data** arr_dt = queryAllUsersFromData("E4DEA39D5E7646918E07B414C2CC0671");
//    for(i=0;arr_dt[i]!=NULL;i++){
//        printf("id-arrdt[%d]: %s\n",i,arr_dt[i]->dataID);
//        printf("name-arrdt[%d]: %s\n",i,arr_dt[i]->dataName);
//        printf("type-arrdt[%d]: %d\n",i,arr_dt[i]->dataType);
//        freeData(arr_dt[i]);
//    }
//    free(arr_dt);
//-------------------------------------------------------------------------
    
//----[5]------------------------------------------------------------------
//    //  test-Category --> myState2 ??
//    t_bool bl1 = isObjectUnderData("2603169373904B9FBF05F72620D70F3D","A1C49651099946C7B2F9CD0B70092797");
//    //  myState2 --> test-Category ??
//    t_bool bl2 = isObjectUnderData("A1C49651099946C7B2F9CD0B70092797","2603169373904B9FBF05F72620D70F3D");
//    printf("bl1: %d\n",bl1);
//    printf("bl2: %d\n",bl2);
//-------------------------------------------------------------------------
    
//    const char* uuid_org = createOrg("Throughwave",test_schema);
//    addUser2OrgByID((char*)uuid_org,"A99B27E341CA424D84FADCCB5B856910");

//----[6]------------------------------------------------------------------
//    Data* q_data = queryDataByID("AB461924401F4B98B3DBB183CA9FEA50");
//    Data* q_data = queryDataByID("2603169373904B9FBF05F72620D70F3D");
//    printf("\n--- query Data ---\n");
//    printf("dataID: %s\n", q_data->dataID);
//    printf("dataName: %s\n", q_data->dataName);
//    printf("dataType: %d\n", q_data->dataType);
//    printf("chatRoom: %s\n", q_data->chatRoom);
//    printf("versionKeeped: %d\n", q_data->content->versionKeeped);
//    
//    if(q_data->content->head->fullContent != NULL)
//        printf("head: %s\n", q_data->content->head->fullContent->data);
//    if(q_data->content->lastestCommon->minusPatch != NULL)
//        printf("last: %s\n", q_data->content->lastestCommon->minusPatch->data);
//    if(q_data->content->lastestCommon->fullContent != NULL)
//        printf("last: %s\n", q_data->content->lastestCommon->fullContent->data);
//    
//    int count_dc = countDataContent(q_data);
//    printf("count_dc: %d\n",count_dc-1);
//    
//    /* free Data */
//    freeData(q_data);
//-------------------------------------------------------------------------

//----[7]------------------------------------------------------------------
//    ObjectBinary *obj_h = getDataContentByID("AB461924401F4B98B3DBB183CA9FEA50");
//    ObjectBinary *obj_l = getDataContentLastestCommonByID("AB461924401F4B98B3DBB183CA9FEA50");
//    printf("\n--- getDataContentByID ---\n");
//    printf("schemaCode: %d\n",obj_h->schemaCode);
//    printf("byteCount: %d\n",obj_h->byteCount);
//    printf("data: %s\n",obj_h->data);
//
//    printf("\n--- getDataContentLastestCommonByID ---\n");
//    printf("schemaCode: %d\n",obj_l->schemaCode);
//    printf("byteCount: %d\n",obj_l->byteCount);
//    printf("data: %s\n",obj_l->data);
//    freeObjBinary(obj_h);
//    freeObjBinary(obj_l);
//-------------------------------------------------------------------------
    
//----[8]------------------------------------------------------------------
//    char* name = getDataNameByID("4EC579D4402740A19D1DADA9542D38E5");
//    char* chat = getChatRoomByID("4EC579D4402740A19D1DADA9542D38E5");
//    char* chat = getChatRoomByID("A1C49651099946C7B2F9CD0B70092797");
//    printf("name: %s\n",name);
//    printf("chat: %s\n",chat);
//    free(name);
//    free(chat);
//-------------------------------------------------------------------------
    
//    for(i=0;i<count_dc-1;i++){
//        ObjectBinary* obj_nv = getContentNextVer(q_data);
//        if(obj_nv != NULL){
//            printf("schemaCode_nv: %d\n",obj_nv->schemaCode);
//            printf("byteCount_nv: %d\n",obj_nv->byteCount);
//            printf("data_nv: %s\n",obj_nv->data);
//            free(obj_nv->data);
//            free(obj_nv);
//            
//        }
//        if(i != count_dc-1)
//            q_data->content->current = q_data->content->current->preVersion;
//    }

//    addState2CategoryByID("2603169373904B9FBF05F72620D70F3D", "C27C43B2CDD74AADB9A9096EFFCFB7BE");
//    addTask2StateByID("A1C49651099946C7B2F9CD0B70092797", "4EC579D4402740A19D1DADA9542D38E5");
    
//----[9]------------------------------------------------------------------
//    //  test-Category -x-> myState
//    deleteObj("8B8462EC0C8A4519B05A9D9DB788F13F","2603169373904B9FBF05F72620D70F3D", "C27C43B2CDD74AADB9A9096EFFCFB7BE");
//    //  myState2 -x-> myTask2
//    deleteObj("8B8462EC0C8A4519B05A9D9DB788F13F","A1C49651099946C7B2F9CD0B70092797", "4EC579D4402740A19D1DADA9542D38E5");
//-------------------------------------------------------------------------
    
//----[10]------------------------------------------------------------------
//    flushTrash("8B8462EC0C8A4519B05A9D9DB788F13F");
//-------------------------------------------------------------------------
    disconnectServer();
    close(Sockfd);
}

void freeData(Data* data){
    int i=0;
    DataContent *mydc, *next_mydc;
    for(mydc=data->content->lastestCommon;mydc!=NULL;mydc=next_mydc){
        printf("-- %d --\n",i);
        next_mydc = mydc->nextVersion;
        if(mydc->SHA256hashCode != NULL)
            free(mydc->SHA256hashCode);
        if(mydc->timeStamp != NULL)
            free(mydc->timeStamp->timeStampCode);
        if(mydc->minusPatch != NULL){
            printf("minus: %s\n",mydc->minusPatch->data);
            free(mydc->minusPatch->data);
            free(mydc->minusPatch);
        }
        if(mydc->plusPatch != NULL){
            printf("plus: %s\n",mydc->plusPatch->data);
            free(mydc->plusPatch->data);
            free(mydc->plusPatch);
        }
        if(mydc->fullContent != NULL){
            printf("full: %s\n",mydc->fullContent->data);
            free(mydc->fullContent->data);
            free(mydc->fullContent);
        }
        free(mydc);
        i++;
    }
    
    free(data->content);
    free((char*)data->dataID);
    if(data->dataName!=NULL)
        free(data->dataName);
    if(data->chatRoom!=NULL)
        free(data->chatRoom);
    free(data);
}

void freeObjBinary(ObjectBinary* obj){
    free(obj->data);
    free(obj);
}

