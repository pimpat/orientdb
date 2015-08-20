#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "hiorient.h"
#include "swap_endian.h"
#include "reqmsg.h"

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 2424

#define DB_NAME "datamodel_xml"
#define MAX_CONTENT_SIZE (10000)

int connectSocket(DTPacket* dtPacket) {
    int sockfd;
    struct sockaddr_in serv_addr;
    //  testPacket
//    Packet GPacket;
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
    read(sockfd, &dtPacket->GPacket, 256);
    
    printf("Connect Socket OK\n");
    
    return sockfd;
}

int connectServer(DTPacket* dtPacket) {
    int ret, size;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_CONNECT;
    *(int *)dtPacket->GPacket.ssid = -1;
    size = reqConnectMsg(dtPacket->GPacket.msg, NULL, NULL, 28, NULL, "root", "pimpat");
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5+sizeof(int));
    if (ret<0 || dtPacket->GPacket.opType!=0) return 1;
    
    // copy ssid
    memcpy(dtPacket->GPacket.ssid, dtPacket->GPacket.msg, 4);
    int ssid = *(int *)dtPacket->GPacket.ssid;
    swapEndian(&ssid, INT);
    printf("ssid: %d\n", ssid);
    
    //  for version 28
    int token;
    ret = read(dtPacket->Sockfd, &token, 4);
    
    return 0;
}

int disconnectServer(DTPacket* dtPacket) {
    int ret, size;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = 5;
    size = reqDBCloseMsg(dtPacket->GPacket.msg);
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    return 0;
}

int createDatabase(char *dbName, DTPacket* dtPacket) {
    int ret, size;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_DB_CREATE;
    size = reqDBCreateMsg(dtPacket->GPacket.msg, dbName, "graph", "plocal");
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5+0);
    if (ret<0 || dtPacket->GPacket.opType!=0) return 1;
    
    printf("CREATE DATABASE '%s' plocal: OK!\n", dbName);
    return 0;
}

int createClass(char *myclass, char *extend, DTPacket* dtPacket) {
    int ret;
    char command[256];
    sprintf(command, "CREATE CLASS %s EXTENDS %s", myclass, extend);
    ret = sendCommand(command,dtPacket);
    if (ret!=0) return 1;
    
    printf("%s: OK!\n", command);
    return 0;
}

int prepareDB(DTPacket* dtPacket) {
    int ret;
    
    dtPacket->Sockfd = connectSocket(dtPacket);
    if (dtPacket->Sockfd < 0)
        return dtPacket->Sockfd;
    printf("connectSocket..PASS\n");
    
    ret = connectServer(dtPacket);
    if (ret!=0)
        return 1;
    printf("connectServer...PASS\n");
    
    ret = createDatabase(DB_NAME, dtPacket);
    if (ret!=0)
        return 1;
    printf("createDB...PASS\n");
    
    /* Edge toUser */
    ret = createClass("toUser","E",dtPacket);
    if (ret!=0) return 1;
    
    /* Edge toCategory */
    ret = createClass("toCategory","E",dtPacket);
    if (ret!=0) return 1;
    
    /* Edge toState */
    ret = createClass("toState","E",dtPacket);
    if (ret!=0) return 1;
    
    /* Edge toTask */
    ret = createClass("toTask","E",dtPacket);
    if (ret!=0) return 1;
    
    /* Edge toSubTask */
    ret = createClass("toSubTask","E",dtPacket);
    if (ret!=0) return 1;
    
    /* DeleteNode */
    ret = createClass("DeleteNode","V",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY DeleteNode.userID STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DeleteNode.userID STRING: OK!\n");
    
    /* Edge toDelete */
    ret = createClass("toDelete","E",dtPacket);
    if (ret!=0) return 1;
    
    /* Edge toDeletedData */
    ret = createClass("toDeletedData","E",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY toDeletedData.dataID STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY toDeletedData.dataID STRING: OK!\n");
    
    /* Data */
    ret = createClass("Data","V",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY Data.dataID STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.dataID STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY Data.dataType INTEGER",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.dataType INTEGER: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY Data.dataName STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.dataName STRING: OK!\n");
    
//    ret = sendCommand("CREATE PROPERTY Data.attachmentFTOLinks EMBEDDEDLIST STRING");
//    if (ret!=0) return 1;
//    //printf("CREATE PROPERTY Data.attachmentFTOLinks EMBEDDEDLIST STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY Data.chatRoom STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY Data.chatRoom STRING: OK!\n");
    
    /* Edge toDataHolder */
    ret = createClass("toDataHolder","E",dtPacket);
    if (ret!=0) return 1;
    
    /* DataHolder */
    ret = createClass("DataHolder","V",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY DataHolder.versionKeeped INTEGER",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataHolder.versionKeeped INTEGER: OK!\n");
    
    /* Edge toDataContent */
    ret = createClass("toDataContent","E",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY toDataContent.type STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY toDataContent.type STRING: OK!\n");
    
    /* DataContent */
    ret = createClass("DataContent","V",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY DataContent.SHA256hashCode STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataContent.SHA256hashCode STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY DataContent.timeStamp STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataContent.timeStamp STRING: OK!\n");
    
    ret = sendCommand("CREATE PROPERTY DataContent.isDiff BOOLEAN",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY DataContent.isDiff BOOLEAN: OK!\n");
    
    //  minusPatch
    ret = sendCommand("CREATE PROPERTY DataContent.minus_schemaCode INTEGER",dtPacket);
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.minus_byteCount INTEGER",dtPacket);
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.minus_data STRING",dtPacket);
    if (ret!=0) return 1;
    
    //  plusPatch
    ret = sendCommand("CREATE PROPERTY DataContent.plus_schemaCode INTEGER",dtPacket);
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.plus_byteCount INTEGER",dtPacket);
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.plus_data STRING",dtPacket);
    if (ret!=0) return 1;
    
    //  fullContent
    ret = sendCommand("CREATE PROPERTY DataContent.full_schemaCode INTEGER",dtPacket);
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.full_byteCount INTEGER",dtPacket);
    if (ret!=0) return 1;
    ret = sendCommand("CREATE PROPERTY DataContent.full_data STRING",dtPacket);
    if (ret!=0) return 1;
    
    /* Edge toContent */
    ret = createClass("toContent","E",dtPacket);
    if (ret!=0) return 1;
    
    ret = sendCommand("CREATE PROPERTY toContent.type STRING",dtPacket);
    if (ret!=0) return 1;
    //printf("CREATE PROPERTY toContent.type STRING: OK!\n");
    
    /* Edge toDataHolder_fromDC */
    ret = createClass("toDataHolder_fromDC","E",dtPacket);
    if (ret!=0) return 1;
    
    //  alter class
    ret = sendCommand("ALTER DATABASE CUSTOM useLightweightEdges=false",dtPacket);
    if (ret!=0) return 1;
    printf("ALTER DATABASE CUSTOM useLightweightEdges=false: OK!\n",dtPacket);
    
    disconnectServer(dtPacket);
    close(dtPacket->Sockfd);
    
    printf("prepareDB...PASS\n");
    return 0;
}

int openDatabase(char *dbName, DTPacket* dtPacket) {
    printf("Opening Database...\n");
    int ret, size, i;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_DB_OPEN;
    *(int *)dtPacket->GPacket.ssid = -1;
    size = reqDBOpenMsg(dtPacket->GPacket.msg, NULL, NULL, 28, NULL, dbName, "graph", "root", "pimpat");
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5+sizeof(int));
    printf("resSize: %d\n", ret);
    printf("resCode: %d\n", dtPacket->GPacket.opType);
    if (ret<0 || dtPacket->GPacket.opType!=0) return 1;
    
    // copy ssid
    memcpy(dtPacket->GPacket.ssid, dtPacket->GPacket.msg, 4);
    int ssid = *(int *)dtPacket->GPacket.ssid;
    swapEndian(&ssid, INT);
    printf("ssid: %d\n", ssid);
    
    //  for version 28
    int token;
    ret = read(dtPacket->Sockfd, &token, 4);
    
    //  copy num-of-clusters
    short nCluster;
    ret = read(dtPacket->Sockfd, &nCluster, sizeof(short));
    swapEndian(&nCluster, SHORT);
    printf("num-of-clusters: %hd\n", nCluster);
    
    for (i=0; i<nCluster; i++) {
        char name[128];
        //char type[32];
        short id;//, dataSeg;
        
        //  read info classes
        read(dtPacket->Sockfd, &size, sizeof(int));
        swapEndian(&size, INT);
        read(dtPacket->Sockfd, name, size);
        name[size] = '\0';
        read(dtPacket->Sockfd, &id, sizeof(short));
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
    
    read(dtPacket->Sockfd, &size, sizeof(int));
    swapEndian(&size, INT);
    if (size > 0) {
        char clusterConfig[size];
        read(dtPacket->Sockfd, clusterConfig, size);
        //clusterConfig[size]='\0';
    }
    
    read(dtPacket->Sockfd, &size, sizeof(int));
    swapEndian(&size, INT);
    if (size > 0) {
        char orientRelease[size+1];
        read(dtPacket->Sockfd, orientRelease, size);
        orientRelease[size] = '\0';
        printf("%s\n", orientRelease);
    }
    return 0;
}

int sendCommand(char *command, DTPacket* dtPacket) {
    int ret, size;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_COMMAND;
    size = reqCommandMsg(dtPacket->GPacket.msg, "c", command);
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5+256);
    //printf("opType:%d\n", GPacket.opType);
    if (ret<0 || dtPacket->GPacket.opType!=0)
        return 1;
    else
        return 0;
}

int createVertex(char *command, short* cltid, long* rid, DTPacket* dtPacket){
    int ret, size;
    //  testPacket
//    Packet GPacket;
    int in_size;
    dtPacket->GPacket.opType = REQUEST_COMMAND;
    size = reqCommandMsg(dtPacket->GPacket.msg, "c", command);
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5+1+2+1);
    if (ret<0 || dtPacket->GPacket.opType!=0){
        //printf("createVertex..FAILED\n");
        return 1;
    }
    else{
        ret = read(dtPacket->Sockfd, &*cltid, 2);
        if (ret<0) return 1;
        swapEndian(&*cltid, SHORT);
        
        ret = read(dtPacket->Sockfd, &*rid, 8);
        if (ret<0) return 1;
        swapEndian(&*rid, LONG);
        
        printf("@rid #%d:%lu\n",*cltid,*rid);
        read(dtPacket->Sockfd, &in_size, 4);
        read(dtPacket->Sockfd, &in_size, 4);
        swapEndian(&in_size, INT);
        //printf("in_size: %d\n",in_size);
        
        ret = read(dtPacket->Sockfd,&dtPacket->GPacket.msg,in_size+1);
        if (ret<0) return 1;
        return 0;
    }
}

char* getContent(char *query, DTPacket* dtPacket) {
    int i, ret, size, total;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(dtPacket->GPacket.msg, "q", query);
//    printf("\nQUERY: %s\n",query);
//    printf("\nMSG: %d\n",GPacket.opType);
//    printf("\nLEN: %d\n",strlen(GPacket.msg));
//    printf("SIZE: %d\n",size);
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5);
    printf("opType:%d\n", dtPacket->GPacket.opType);
    if (ret<0 || dtPacket->GPacket.opType!=0) {
        printf("getContent error\n");
        return NULL;
    }
    
    read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 1);
    read(dtPacket->Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
    
    if(total==0)
        return NULL;
    
    char* str = (char*)malloc(sizeof(char)*MAX_CONTENT_SIZE);
    if(total==1){
        for(i=0;i<total;i++){
            read(dtPacket->Sockfd, &size, 4);
            swapEndian(&size, INT);
            printf("size: %d\n",size);
            if(size==0){
                free(str);
                return NULL;
            }
            read(dtPacket->Sockfd, str, size);
            str[size]='\0';
            printf("msg: %s\n",str);
            read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
            printf("[result from getContent]---------------\n",i);
        }
        return str;
    }
    else{
        char* all_str = (char*)malloc(sizeof(char)*MAX_CONTENT_SIZE);
        for(i=0;i<total;i++){
            read(dtPacket->Sockfd, &size, 4);
            swapEndian(&size, INT);
            printf("size: %d\n",size);
            read(dtPacket->Sockfd, str, size);
            str[size]='\0';
            strcat(all_str,"#");
            strcat(all_str,str);
            printf("msg: %s\n",str);
            read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
            printf("[result from getContent]---------------\n",i);
        }
        free(str);
        printf("all_str(len): %d\n",strlen(all_str));
        return all_str;
    }
}

char** getArrayRid(char* query, DTPacket* dtPacket){
    int i, ret, size, total;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(dtPacket->GPacket.msg, "q", query);
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", dtPacket->GPacket.opType);
    if (ret<0 || dtPacket->GPacket.opType!=0) {
        printf("FAILED >> getArrayRid\n");
        return NULL;
    }
    
    read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 1);
    read(dtPacket->Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
    
    if(total==0)
        return NULL;
    
    char **result_rid = (char**)malloc(sizeof(char*)*total+1);
    
    for(i=0;i<total;i++){
        read(dtPacket->Sockfd, &size, 4);
        swapEndian(&size, INT);
        printf("[%d]--------------------------------------------------\n",i);
        printf("size: %d\t",size);
        result_rid[i] = (char*)malloc(sizeof(char)*size-3);
        
        //  read content
        read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 4);
        read(dtPacket->Sockfd, &dtPacket->GPacket.msg, size-4);
        dtPacket->GPacket.msg[size-4]='\0';
        printf("msg: %s\n", dtPacket->GPacket.msg);
        sprintf(result_rid[i],"%s",dtPacket->GPacket.msg);
        //printf("len: %d\n",strlen(result_rid[i]));
        printf("myrid: %s\n",result_rid[i]);
        read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
    }
    result_rid[i] = NULL;
    
    //    for(i=0;result_rid[i]!=NULL;i++) {
    //        printf("TEST_rid: %s\n",result_rid[i]);
    //    }
    
    return result_rid;
}

char** getArrayDistinctRid(char* query, DTPacket* dtPacket){
    int i, ret, size, total;
    //  testPacket
//    Packet GPacket;
    dtPacket->GPacket.opType = REQUEST_COMMAND;
    size = reqQueryMsg(dtPacket->GPacket.msg, "q", query);
    ret = write(dtPacket->Sockfd, &dtPacket->GPacket, 5+size);
    
    ret = read(dtPacket->Sockfd, &dtPacket->GPacket, 5);
    //printf("ret:%d\n", ret);
    printf("opType:%d\n", dtPacket->GPacket.opType);
    if (ret<0 || dtPacket->GPacket.opType!=0) {
        printf("FAILED >> getArrayRid\n");
        return NULL;
    }
    
    read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 1);
    read(dtPacket->Sockfd, &total, 4);
    swapEndian(&total, INT);
    printf("total: %d\n",total);    // total record
    read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
    
    if(total==0)
        return NULL;
    
    char **result_rid = (char**)malloc(sizeof(char*)*total+1);
    
    for(i=0;i<total;i++){
        read(dtPacket->Sockfd, &size, 4);
        swapEndian(&size, INT);
        printf("[%d]--------------------------------------------------\n",i);
        printf("size: %d\t",size);
        result_rid[i] = (char*)malloc(sizeof(char)*size-3);
        
        //  read content
        read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 4);
        read(dtPacket->Sockfd, &dtPacket->GPacket.msg, size-4);
        dtPacket->GPacket.msg[size-4]='\0';
        printf("msg: %s\n", dtPacket->GPacket.msg);
        sprintf(result_rid[i],"%s",dtPacket->GPacket.msg+5);
        //printf("len: %d\n",strlen(result_rid[i]));
        printf("myrid: %s\n",result_rid[i]);
        read(dtPacket->Sockfd, &dtPacket->GPacket.msg, 2+1+2+8+4);
    }
    result_rid[i] = NULL;
    
    return result_rid;
}

/*
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
*/

void varintParse(int64_t in, uint8_t *buffer){
	uint64_t tmp = (in << 1) ^ (in >> 63);
	// printf("%016llX\n", tmp);
	
	int i = 0;
	do { 
		buffer[i++] = (tmp & 0x7f) | 0x80;
		tmp >>= 7;
	} while(tmp > 0);
	buffer[i - 1] &= 0x7f;
}

int64_t varintUnparse(uint8_t *buffer){
	int64_t ans = 0;
	short i = 0;
	do {
		ans |= (buffer[i] << (i * 7));
	}while(buffer[i++] & 0x80);
	int64_t signOffset = (ans & 0x1) << 63;
	ans = ((ans ^ (signOffset >> 63)) | signOffset) >> 1;
	return ans;

}

void printAsciiVarint(uint8_t *buffer){
	char tmp[21];
	char *writer = tmp;
	int i = 0;
	do{
		writer += sprintf(writer, "%02hhX", buffer[i]);
	}while(buffer[i++] & 0x80);
	printf("%s\n", tmp);
}

