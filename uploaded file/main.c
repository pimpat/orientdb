//
//  main.c
//  Datamodel_XML
//
//  Created by Pimpat on 9/3/2558 BE.
//  Copyright (c) 2558 Pimpat. All rights reserved.
//

//  transport
#include <zmq.h>
#include <assert.h>
#include "zhelpers.h"
#include <transport.h>

#include "datamodel.h"

/* test function */
void testNewCreateVertex();
void test_setNewData();
void test_getData(Data** data);
void testCRUD(Data** data);
void testWeb();

int getIdFromConfig(TPTransportCTX *context, char *configFile, char *id) {
    TPConfigGet(context, TP_CONFIG_UUID, id, NULL);
    if (strcmp(id, "") == 0) {
        TPConfigGet(context, TP_CONFIG_ID, id, NULL);
    }
    return 0;
}

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
    
    /*
     char x[15] = "1:20:300:4000";
     char y[10] = "a:b:c";
     char* tmp;
     int i;
     char *ptr, *ptr2;
     char *res2;
     
     char* res1 = strtok_r(x,":",&ptr);
     while(res1!=NULL){
     printf("res: %s\n",res1);
     res1 = strtok_r(NULL,":",&ptr);
     //        res1 = strtok(NULL,":");
     
     tmp = strdup(y);
     res2 = strtok_r(tmp,":",&ptr2);
     //        char *res2 = strtok(tmp,":");
     while(res2!=NULL){
     //            printf("res(in): %s\n",res2);
     res2 = strtok_r(NULL,":",&ptr2);
     }
     free(tmp);
     }
     */
    
    //    test_setNewData();
    //    return 0;
    
    testWeb();
    return 0;
}

void testNewCreateVertex(){
    DTPacket dtPacket;
    dtPacket.Sockfd = ORD_ConnectSocket(&dtPacket);
    if (dtPacket.Sockfd < 0){
        printf ("error connectSocket\n");
    }
    int ret = ORD_OpenDatabase(DB_NAME,&dtPacket);
    if (ret!=0) {
        printf ("error openDatabase\n");
    }
    
    OrientDataGroup newData;
    newData.dataNumber = 4;
    newData.data = (OrientData *)malloc(sizeof(OrientData) * 4);
    
    newData.data[0].dataType = _STRING;
    newData.data[0].fieldName = (char *)malloc(sizeof(char) * 129);
    sprintf(newData.data[0].fieldName, "dataID");
    newData.data[0].dataPnt = malloc(sizeof(char) * 50);
    sprintf((char *)newData.data[0].dataPnt, "D20306F796524D87878DF5C17A33C94D");
    
    newData.data[1].dataType = _STRING;
    newData.data[1].fieldName = (char *)malloc(sizeof(char) * 129);
    sprintf(newData.data[1].fieldName, "chatRoom");
    newData.data[1].dataPnt = malloc(sizeof(char) * 30);
    sprintf((char *)newData.data[1].dataPnt, "Gradeiei");
    
    newData.data[2].dataType = _STRING;
    newData.data[2].fieldName = (char *)malloc(sizeof(char) * 129);
    sprintf(newData.data[2].fieldName, "dataName");
    newData.data[2].dataPnt = malloc(sizeof(char) * 30);
    sprintf((char *)newData.data[2].dataPnt, "sas");
    
    newData.data[3].dataType = _DOUBLE;
    newData.data[3].fieldName = (char *)malloc(sizeof(char) * 129);
    sprintf(newData.data[3].fieldName, "dataType");
    newData.data[3].dataPnt = malloc(sizeof(int));
    *(double *)(newData.data[3].dataPnt) = 1.23;
    
    short clustid;
    long rid;
    ORD_CreateVertex_B("data", &clustid, &rid, &dtPacket, &newData);
    printf("create %hd#%ld\n", clustid, rid);
    ORD_CloseDatabase(&dtPacket);
    ORD_DisconnectServer(&dtPacket);
    close(dtPacket.Sockfd);
    
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
    
    char* res_diff = getDiff("www.xyz.com", "www.abc.com");
    //char* res_diff = strdup(diff.c_str());
    printf("res_diff: %s\n\n",res_diff);
    
    setNewDataDiffWithTag(_mydata, "attachmentFTOLinks", "1", res_diff);
    setNewDataDiffWithTag(_mydata, "attachmentFTOLinks", "3", "www.qaz.com");
    
    char* diff_price = getDiff("44.95", "1000");
    //char* diff_price = strdup(s.c_str());
    
    setNewDataDiffWithTag(_mydata, "price", NULL, diff_price);
    setNewDataDiffWithTag(_mydata, "titel", NULL, "hello");
    
    setDataName(_mydata, "_myTask2");
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
    int ret;
    DTPacket dtPacket;
    
    //----[1]------------------------------------------------------------------
    //    ORD_PrepareDB(&dtPacket);
    //-------------------------------------------------------------------------
    
    //    Sockfd = ORD_ConnectSocket();
    dtPacket.Sockfd = ORD_ConnectSocket(&dtPacket);
    if (dtPacket.Sockfd < 0){
        printf ("error connectSocket\n");
    }
    
    //    ret = openDatabase(DB_NAME);
    ret = ORD_OpenDatabase(DB_NAME,&dtPacket);
    if (ret!=0) {
        printf ("error openDatabase\n");
    }
    
    printf("\n\nsockfd: %d\n",dtPacket.Sockfd);
    int ssid = *(int*)dtPacket.GPacket.ssid;
    swapEndian(&ssid, INT);
    printf("sessionid: %d\n\n",ssid);
    
    //    char sql[]="create vertex Data set dataName='document', dataID='123456'";
    //    short _cltid;
    //    long _rid;
    //     _ORD_CreateVertex_C(sql, &_cltid, &_rid);
    //    printf("@rid #%d:%lu\n",_cltid,_rid);
    
    Data* dt = *data;
    int i=0;
    
    //    char* strGen = genString(dt);
    //    printf("\n%s\n",strGen);
    
    char* r = ORD_GetContent("select * from #19:2", &dtPacket);
    //    char* r = ORD_GetContent("select from (select expand(out_toTask) from #19:2)", &dtPacket);
    //    char* r = ORD_GetContent("select * from (traverse * from #21:2 while $depth <=1) where $depth =1 and @class<>'toDataHolder'", &dtPacket);
    printf("MY RESULT...\n%s\n",r);
    free(r);
    
    //----[test new functions]-------------------------------------------------
    Edge** arr_edge = getAllEdgeForThisObject("2F9101248BFF49039F915225C87407C5", &dtPacket);
    printf("\n--- getAllEdgeForThisObject ---\n");
    if(arr_edge!=NULL){
        for(i=0;arr_edge[i]!=NULL;i++){
            printf("edge_rid[%d]: %s\n", i, arr_edge[i]->edgeRid);
            printf("edge_name[%d]: %s\n", i, arr_edge[i]->edgeName);
            //            free(arr_edge[i]->edgeRid);
            //            free(arr_edge[i]->edgeName);
            //            free(arr_edge[i]);
            freeEdge(arr_edge[i]);
        }
        free(arr_edge);
    }
    
    Data* my_data = getObjectPointByEdge("#15:0", &dtPacket);
    printf("\n--- getObjectPointByEdge ---\n");
    displayObjectContent(my_data);
    //    printf("dataID: %s\n", my_data->dataID);
    //    printf("dataName: %s\n", my_data->dataName);
    //    printf("dataType: %d\n", my_data->dataType);
    //    printf("chatRoom: %s\n", my_data->chatRoom);
    //    printf("versionKeeped: %d\n", my_data->content->versionKeeped);
    //
    //    if(my_data->content->head->fullContent != NULL)
    //        printf("head: %s\n", my_data->content->head->fullContent->data);
    //    if(my_data->content->lastestCommon->minusPatch != NULL)
    //        printf("last(minus): %s\n", my_data->content->lastestCommon->minusPatch->data);
    //    if(my_data->content->lastestCommon->fullContent != NULL)
    //        printf("last(full): %s\n", my_data->content->lastestCommon->fullContent->data);
    freeData(my_data);
    //-------------------------------------------------------------------------
    
    //----[2]------------------------------------------------------------------[ts]
    /*
     Schema test_schema[]="<root><attachmentFTOLinks></attachmentFTOLinks><book_id></book_id><author></author><title></title><genre></genre><price></price></root>";
     
     const char* uuid_user = createUser("_Pimpat", test_schema,&dtPacket);
     const char* uuid_user2 = createUser("_Tanapon", test_schema,&dtPacket);
     const char* uuid_cat = createCategory("_Pics", test_schema,&dtPacket);
     const char* uuid_cat2 = createCategory("_test-Category", test_schema,&dtPacket);
     const char* uuid_task = createTask("_myTask", test_schema,&dtPacket);
     const char* uuid_state = createState("_myState", test_schema,&dtPacket);
     const char* uuid_state2 = createState("_myState2", test_schema,&dtPacket);
     
     //  Pimpat --> Pics
     addData2DataByID((char*)uuid_user, (char*)uuid_cat, _toCategory,&dtPacket);
     //  Tanapon --> Pics
     addData2DataByID((char*)uuid_user2, (char*)uuid_cat, _toCategory,&dtPacket);
     //  Tanapon --> test-Category
     addData2DataByID((char*)uuid_user2, (char*)uuid_cat2, _toCategory,&dtPacket);
     //  Pics --> myTask
     addTask2CategoryByID((char*)uuid_cat, (char*)uuid_task,&dtPacket);
     //  test-Category --> myState
     addState2CategoryByID((char*)uuid_cat2,(char*)uuid_state,&dtPacket);
     //  test-Category --> myState2
     addState2CategoryByID((char*)uuid_cat2,(char*)uuid_state2,&dtPacket);
     //  myState2 --> myTask
     addTask2StateByID((char*)uuid_state2,(char*)uuid_task,&dtPacket);
     //  myState2 --> myTask2
     addData2Data((char*)uuid_state2,dt,_toTask,&dtPacket);
     
     //  delete myState
     deleteObj((char*)uuid_user2, (char*)uuid_cat2, (char*)uuid_state,&dtPacket);
     */
    //-------------------------------------------------------------------------
    //    addTask2CategoryByID("2603169373904B9FBF05F72620D70F3D", "4EC579D4402740A19D1DADA9542D38E5",&dtPacket);
    //    addTask2StateByID("A1C49651099946C7B2F9CD0B70092797","4EC579D4402740A19D1DADA9542D38E5",&dtPacket);
    //    addData2DataByID("8B8462EC0C8A4519B05A9D9DB788F13F","AB7E6634A8ED4F37895982BF9F0D6196",_toCategory,&dtPacket);
    //    addTask2CategoryByID("AB7E6634A8ED4F37895982BF9F0D6196","4EC579D4402740A19D1DADA9542D38E5",&dtPacket);
    
    
    //----[3]------------------------------------------------------------------[ts]
    //    setDataNameByID("9956F26257E54272A33902A1E4309D42","DST",&dtPacket);
    //    setChatRoomByID("9956F26257E54272A33902A1E4309D42", "chat-room2",&dtPacket);
    //-------------------------------------------------------------------------
    
    //----[4]------------------------------------------------------------------[ts]
    //        Data** arr_dt = queryAllUsersFromData("E4DEA39D5E7646918E07B414C2CC0671",&dtPacket);
    //        for(i=0;arr_dt[i]!=NULL;i++){
    //            printf("id-arrdt[%d]: %s\n",i,arr_dt[i]->dataID);
    //            printf("name-arrdt[%d]: %s\n",i,arr_dt[i]->dataName);
    //            printf("type-arrdt[%d]: %d\n",i,arr_dt[i]->dataType);
    //            freeData(arr_dt[i]);
    //        }
    //        free(arr_dt);
    //-------------------------------------------------------------------------
    
    //----[5]------------------------------------------------------------------[ts]
    //    //  test-Category --> myState2 ??
    //    t_bool bl1 = isObjectUnderData("2603169373904B9FBF05F72620D70F3D","A1C49651099946C7B2F9CD0B70092797",&dtPacket);
    //    //  myState2 --> test-Category ??
    //    t_bool bl2 = isObjectUnderData("A1C49651099946C7B2F9CD0B70092797","2603169373904B9FBF05F72620D70F3D",&dtPacket);
    //    printf("bl1: %d\n",bl1);
    //    printf("bl2: %d\n",bl2);
    //-------------------------------------------------------------------------
    //    Schema test_schema[]="<root><attachmentFTOLinks></attachmentFTOLinks><book_id></book_id><author></author><title></title><genre></genre><price></price></root>";
    //    const char* uuid_org = createOrg("Exxon",test_schema);
    //    addUser2OrgByID("12F5B1ED18BF470AB0F22A5CE571D379","8025B5A5A68F4285AF58FEEB0A32CFFA",&dtPacket);
    
    //----[6]------------------------------------------------------------------[ts]
    //    Data* q_data = queryDataByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    ////    Data* q_data = queryDataByID("2603169373904B9FBF05F72620D70F3D",&dtPacket);
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
    //        printf("last(minus): %s\n", q_data->content->lastestCommon->minusPatch->data);
    //    if(q_data->content->lastestCommon->fullContent != NULL)
    //        printf("last(full): %s\n", q_data->content->lastestCommon->fullContent->data);
    //
    //    int count_dc = countDataContent(q_data);
    //    printf("count_dc: %d\n",count_dc-1);
    //
    //    char* strPack = buildStringFromData(q_data);
    //    Data* data_str = buildDatafromString(strPack);
    //    ObjectBinary *obj_l = getDataContentLastestCommon(data_str);
    //    if(obj_l != NULL){
    //        printf("\n--- test(getLast->buildDatafromString) ---\n");
    //        printf("schemaCode: %d\n",obj_l->schemaCode);
    //        printf("byteCount: %d\n",obj_l->byteCount);
    //        printf("data: %s\n",obj_l->data);
    //        freeObjBinary(obj_l);
    //    }
    //    free(strPack);
    //    freeData(data_str);
    //
    //    /* free Data */
    //    freeData(q_data);
    //-------------------------------------------------------------------------
    
    //----[7]------------------------------------------------------------------[ts]
    //    ObjectBinary *obj_h = getDataContentByID("9956F26257E54272A33902A1E4309D42",&dtPacket);
    //    ObjectBinary *obj_l = getDataContentLastestCommonByID("9956F26257E54272A33902A1E4309D42",&dtPacket);
    //    printf("\n--- getDataContentByID ---\n");
    //    if(obj_h != NULL){
    //        printf("schemaCode: %d\n",obj_h->schemaCode);
    //        printf("byteCount: %d\n",obj_h->byteCount);
    //        printf("data: %s\n",obj_h->data);
    //        freeObjBinary(obj_h);
    //    }
    //
    //    if(obj_l != NULL){
    //        printf("\n--- getDataContentLastestCommonByID ---\n");
    //        printf("schemaCode: %d\n",obj_l->schemaCode);
    //        printf("byteCount: %d\n",obj_l->byteCount);
    //        printf("data: %s\n",obj_l->data);
    //        freeObjBinary(obj_l);
    //    }
    //-------------------------------------------------------------------------
    
    //----[8]------------------------------------------------------------------[ts]
    //    char* name = getDataNameByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    //    char* chat = getChatRoomByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    ////    char* chat = getChatRoomByID("4EC579D4402740A19D1DADA9542D38E5",&dtPacket);
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
    
    //----[9]------------------------------------------------------------------[ts]
    //    //  test-Category -x-> myState
    //    deleteObj("F911F887F326479EB27A0C4DBF351FBC","AB7E6634A8ED4F37895982BF9F0D6196", "C9C53A6C14AF4C29BD814AC6200A7400",&dtPacket);
    //    //  myState2 -x-> myTask2
    //    deleteObj("F911F887F326479EB27A0C4DBF351FBC","9ABA072021954B6C8124CA23D3F4529E", "8919AE50442E4C80A18A8820E99A9DDA",&dtPacket);
    //-------------------------------------------------------------------------
    
    //----[10]-----------------------------------------------------------------[ts]
    //    flushTrash("F911F887F326479EB27A0C4DBF351FBC",&dtPacket);
    //-------------------------------------------------------------------------
    
    //----[11]-----------------------------------------------------------------[ts]
    //    char* diff_h = getDiffDataAtHeadByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    //    printf("diff_h: %s\n",diff_h);
    //    free(diff_h);
    //
    //    char* diff_l = getDiffDataAtLastestCommonByID("2603169373904B9FBF05F72620D70F3D",&dtPacket);
    //    if(diff_l != NULL){
    //        printf("diff_l: %s\n",diff_l);
    //        free(diff_l);
    //    }
    //-------------------------------------------------------------------------
    
    //----[12]-----------------------------------------------------------------[ts]
    //    ObjectBinary *obj_p = getContentPreVerByID("9956F26257E54272A33902A1E4309D42",&dtPacket);
    //    if(obj_p != NULL){
    //        printf("schemaCode: %d\n",obj_p->schemaCode);
    //        printf("byteCount: %d\n",obj_p->byteCount);
    //        printf("data: %s\n",obj_p->data);
    //        freeObjBinary(obj_p);
    //    }
    //    ObjectBinary *obj_n = getContentNextVerByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    //-------------------------------------------------------------------------
    
    //----[13]-----------------------------------------------------------------[ts]
    //    char* plus_p = getDiffDataPreVerByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    //    if(plus_p != NULL){
    //        printf("plus_pre: %s\n", plus_p);
    //        free(plus_p);
    //    }
    //    char* plus_n = getDiffDataNextVerByID("AB461924401F4B98B3DBB183CA9FEA50",&dtPacket);
    //    if(plus_n != NULL){
    //        printf("plus_next: %s\n", plus_n);
    //        free(plus_n);
    //    }
    //-------------------------------------------------------------------------
    
    //----[14]-----------------------------------------------------------------[ts]
    //    char* value = getDataContentWithTagByID("9956F26257E54272A33902A1E4309D42","author",NULL,&dtPacket);
    //    printf("value: %s\n",value);
    //    free(value);
    //-------------------------------------------------------------------------
    
    //----[15]-----------------------------------------------------------------[ts]
    //    char new_xml2[]="<root><attachmentFTOLinks></attachmentFTOLinks><book_id></book_id><author>Pimpat Teo</author><title>C-coding-book2</title><genre>computer</genre><price>200</price></root>";
    //    int x = saveNewDataContentByID("9956F26257E54272A33902A1E4309D42",new_xml2,&dtPacket);
    ////    int x = saveNewDataContentByID((char*)uuid_org,new_xml2);
    //-------------------------------------------------------------------------
    
    //----[16]-----------------------------------------------------------------[ts]
    //    int y = removeTaskFromState("9ABA072021954B6C8124CA23D3F4529E","50CFEBD64BFA49A1976B5E7A46691A7A",&dtPacket);
    //-------------------------------------------------------------------------
    
    //    Org** org = queryOrgFromData("E4DEA39D5E7646918E07B414C2CC0671",&dtPacket);
    
    //----[17]-----------------------------------------------------------------[ts]
    //    char** list = getOrgID(&dtPacket);
    ////    char** list = getCatFromUserID("8B8462EC0C8A4519B05A9D9DB788F13F",&dtPacket);
    ////    char** list = getTaskFromCatID("2603169373904B9FBF05F72620D70F3D", &dtPacket);
    //    printf("\n--- test list ---\n");
    //    for(i=0;list[i]!=NULL;i++){
    //        printf("list[%d]: %s\n",i,list[i]);
    //        free(list[i]);
    //    }
    //    free(list);
    //-------------------------------------------------------------------------
    
    //---- test(getLast->buildDatafromString) ---------------------------------
    //    char* myDiff = readcontent("/Users/pimpat/Desktop/datamodel_xml/diff_content.txt");
    //    printf("\ns:\n: %s\n",myDiff);
    //    printf("\nlen: %d\n",strlen(myDiff));
    //
    //    Data* data_str = buildDatafromString(myDiff);
    //    printf("dataID: %s\n",data_str->dataID);
    //    printf("dataName: %s\n",data_str->dataName);
    //    ObjectBinary *obj_l = getDataContentLastestCommon(data_str);
    //    if(obj_l != NULL){
    //        printf("\n--- test(getLast->buildDatafromString) ---\n");
    //        printf("schemaCode: %d\n",obj_l->schemaCode);
    //        printf("byteCount: %d\n",obj_l->byteCount);
    //        printf("data: %s\n",obj_l->data);
    //        freeObjBinary(obj_l);
    //    }
    //    freeData(data_str);
    //    free(myDiff);
    //-------------------------------------------------------------------------
    ORD_DisconnectServer(&dtPacket);
    close(dtPacket.Sockfd);
}

void testWeb(){
    printf("testWeb..\n\n");
    
    int ret;
    DTPacket dtPacket;
    dtPacket.Sockfd = ORD_ConnectSocket(&dtPacket);
    if (dtPacket.Sockfd < 0){
        printf ("error connectSocket\n");
    }
    ret = ORD_OpenDatabase(DB_NAME,&dtPacket);
    if (ret!=0) {
        printf ("error openDatabase\n");
    }
    printf("\n\nsockfd: %d\n",dtPacket.Sockfd);
    int ssid = *(int*)dtPacket.GPacket.ssid;
    swapEndian(&ssid, INT);
    printf("sessionid: %d\n\n",ssid);
    
    //---- transport ----------------------------------------------------------
    
    void *context2 = zmq_ctx_new ();
    void *responder = zmq_socket (context2, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:6666");
    assert (rc == 0);
    
    TPTransportCTX *context = NULL;
    TPTransportMsg message;
    char configFile[256];
    char deviceID[64];
    
    //  set config
    sprintf(configFile,"%s","/Users/pimpat/Desktop/Transport2/TransportLayer/install/config/config1.conf");
    ret = TPTransportInitFromConfigFile(&context, configFile);
    if (ret == 0) {
        getIdFromConfig(context, configFile, deviceID);
        
        char path[256];
        system("mkdir -p /Users/pimpat/Desktop/Transport2/TransportLayer/install/chat_history");
        sprintf(path, "/Users/pimpat/Desktop/Transport2/TransportLayer/install/chat_history/%s.db", deviceID);
    }
    else {
        fprintf(stderr, "Client startClient return (%d)\n", ret);
    }
    //-------------------------------------------------------------------------
    Schema newSchema[]="<root><attachmentFTOLinks></attachmentFTOLinks><book_id></book_id><author></author><title></title><genre></genre><price></price></root>";
    
    while (1) {
        printf("\nin while loop..\n");
        char *str = s_recv(responder);
        //        printf("str: %s\n",str);
        
        int numtype;
        char *token = strtok(str,":");
        numtype = atoi(token);
        
        //  MARK:create new data
        if(numtype == 1){
            printf("[#%d create new data]----------------------\n",numtype);
            
            const char* dtID;
            
            token = strtok(NULL,":");
            int casetype = atoi(token);
            
            token = strtok(NULL,":");
            char *dtName = strdup(token);
            printf("dtName: %s\n", dtName);
            
            token = strtok(NULL,":");
            char *upperID = strdup(token);
            printf("upperID: %s\n", upperID);
            
            if (casetype == 1){
                dtID = createOrg(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
            }
            else if (casetype == 2){
                dtID = createUser(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
                addUser2OrgByID(upperID, (char*)dtID, &dtPacket);
            }
            else if (casetype == 3){
                dtID = createCategory(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
                addCategory2UserByID(upperID, (char*)dtID, &dtPacket);
            }
            else if (casetype == 4){
                dtID = createState(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
                addState2CategoryByID(upperID, (char*)dtID, &dtPacket);
            }
            else if (casetype == 5){
                dtID = createTask(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
                addTask2StateByID(upperID, (char*)dtID, &dtPacket);
            }
            else if (casetype == 6){
                dtID = createSubTask(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
                addSubTask2TaskByID(upperID, (char*)dtID, &dtPacket);
            }
            else if (casetype == 7){
                dtID = createTask(dtName, newSchema, &dtPacket);
                printf("dtID: %s\n",dtID);
                addTask2CategoryByID(upperID, (char*)dtID, &dtPacket);
            }
            
            Data* dData = queryDataByID((char*)dtID, &dtPacket);
            
            //  check data
            printf("dataID: %s\n", dData->dataID);
            printf("dataName: %s\n", dData->dataName);
            printf("dataType: %d\n", dData->dataType);
            printf("chatRoom: %s\n", dData->chatRoom);
            printf("--------------------\n");
            printf("versionKeeped: %d\n", dData->content->versionKeeped);
            printf("--------------------\n");
            printf("SHA256hashCode: %s\n", dData->content->lastestCommon->SHA256hashCode);
            printf("timeStamp: %s\n", dData->content->lastestCommon->timeStamp);
            printf("isDiff: %d\n", dData->content->lastestCommon->isDiff);
            printf("--------------------\n");
            
            char* msg = genMsg(dData);
            char rep_str[100];
            sprintf(rep_str, "1:%s:%s:%s", msg, dData->dataID, dData->dataName);
            printf("%s\n", rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free((char*)dtID);
            free(dtName);
            if(upperID!=NULL){
                free(upperID);
            }
            freeData(dData);
            free(msg);
            free(str);
        }
        
        //  MARK:edit and save
        else if(numtype == 2){
            printf("[#%d edit+save]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *dtID = strdup(token);
            printf("dtID: %s\n",dtID);
            
            token = strtok(NULL,":");
            char *content = strdup(token);
            printf("content: %s\n",content);
            
            saveNewDataContentByID(dtID, content, &dtPacket);
            
            Data* dData = queryDataByID(dtID, &dtPacket);
            
            //  check data
            printf("dataID: %s\n", dData->dataID);
            printf("dataName: %s\n", dData->dataName);
            printf("dataType: %d\n", dData->dataType);
            printf("chatRoom: %s\n", dData->chatRoom);
            printf("--------------------\n");
            printf("versionKeeped: %d\n", dData->content->versionKeeped);
            printf("--------------------\n");
            printf("SHA256hashCode: %s\n", dData->content->lastestCommon->SHA256hashCode);
            printf("timeStamp: %s\n", dData->content->lastestCommon->timeStamp);
            printf("isDiff: %d\n", dData->content->lastestCommon->isDiff);
            printf("--------------------\n");
            
            char* strDT = genString(dData);
            char* msg = genMsg(dData);
            
            char rep_str[10000];
            sprintf(rep_str, "5:%s:%s", msg, strDT);
            printf("%s\n", rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free(dtID);
            free(content);
            freeData(dData);
            free(strDT);
            free(msg);
            free(str);
        }
        
        //  MARK:delete
        else if(numtype == 3){
            printf("[#%d delete]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *objID = strdup(token);
            printf("objID: %s\n",objID);
            
            token = strtok(NULL,":");
            char *upperID = strdup(token);
            printf("upperID: %s\n",upperID);
            
            token = strtok(NULL,":");
            char *userID = strdup(token);
            printf("userID: %s\n",userID);
            
            ReturnErr chk = deleteObj(userID, upperID, objID, &dtPacket);
            if (chk == 0){
                char rep_str[100];
                sprintf(rep_str, "6:%s:%s", "Delete success", objID);
                printf("%s\n", rep_str);
                s_send(responder, rep_str);
            }
            else{
                s_send(responder, "404:Delete failed");
            }
            printf("\n\n");
            
            free(objID);
            free(upperID);
            free(userID);
            free(str);
        }
        
        //  MARK:query data
        else if(numtype == 4){
            printf("[#%d query]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *dtID = strdup(token);
            printf("dtID: %s\n",dtID);
            
            Data *data = queryDataByID(dtID, &dtPacket);
            
            if (data != NULL){
                char* msg = genMsg(data);
                char rep_str[100];
                sprintf(rep_str, "1:%s:%s:%s", msg, data->dataID, data->dataName);
                printf("%s\n", rep_str);
                s_send(responder, rep_str);
                
                freeData(data);
                free(msg);
            }
            else{
                printf("not found data from '%d'!!!\n",dtID);
                s_send(responder, "404:not found data");
            }
            printf("\n\n");
            
            free(dtID);
            free(str);
        }
        
        //  MARK:clone
        else if(numtype == 5){
            printf("[#%d clone]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *dtID = strdup(token);
            printf("dtID : %s\n", dtID);
            
            //---- sendMsg ------------------------------------------------------------
            printf("\n---- sendMsg ----\n");
            
            token = strtok(NULL,":");
            char *mySender = strdup(token);
            printf("sender: %s\n",mySender);
            
            token = strtok(NULL,":");
            char *myTarget = strdup(token);
            printf("target: %s\n",myTarget);
            
            //---- query data ---------------------
            Data* dt = queryDataByID(dtID,&dtPacket);
            char* strPack = NULL;
            if(dt != NULL){
                strPack = buildStringFromData(dt);
            }
            //-------------------------------------
            
            TPUserID sender;
            TPUserID target;
            
//            memset(&sender,0,sizeof(sender));
//            strcpy((char*)sender,mySender);
//            
//            memset(&target,0,sizeof(target));
//            strcpy((char*)target,myTarget);
            
            if (TPUsernameToUserID(context, mySender, &sender) != 0)
                return;
            if (TPUsernameToUserID(context, myTarget, &target) != 0)
                return;
            char uuid_str_sender[37];
            uuid_unparse_lower(sender, uuid_str_sender);
            printf("> senderID: %s\n", uuid_str_sender);
            
            char uuid_str_target[37];
            uuid_unparse_lower(target, uuid_str_target);
            printf("> targetID: %s\n", uuid_str_target);
            
            if(dt!=NULL){
                message.msg = strPack;
                message.bytesCount = (int)strlen(strPack);
            }
            else{
                message.msg = "failed";
                message.bytesCount = strlen("failed");
            }
            
            printf("msg: %s\nbytes: %d\n",message.msg,message.bytesCount);
            message.msgType = TP_MSG_TYPE_CHAT;
            //            message.msgType = TPTransportMsg::TP_MSG_TYPE_CHAT;
            //  TPTransportMsg::
            ret = TPSendToUser(context, &sender, &target, &message);
            if (ret != 0) {
                fprintf(stderr, "Client sendToUser return (%d)\n", ret);
            }
            //-------------------------------------------------------------------------
            
            char rep_str[100]="3:";
            if(dt!=NULL){
                strcat(rep_str,"clone data success:");
                strcat(rep_str,dtID);
                strcat(rep_str,":");
                strcat(rep_str,mySender);
            }
            else{
                strcat(rep_str,message.msg);
                strcat(rep_str,":");
                strcat(rep_str,dtID);
                strcat(rep_str,":");
                strcat(rep_str,mySender);
            }
            
            printf("rep_str: %s\n",rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            if(dt!=NULL){
                freeData(dt);
                free(strPack);
            }
            
            free(dtID);
            free(mySender);
            free(myTarget);
            free(str);
        }
        
        //  MARK:login
        else if(numtype == 6){
            printf("[#%d login]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *usrname = strdup(token);
            printf("username(login): %s\n",usrname);
            
            TPUserID userID;
//            memset(&userID,0,sizeof(userID));
//            strcpy((char*)userID,usrname);
            
//            ret = TPLoginUser(context, &userID);
            ret = TPLoginUser(context, usrname, NULL, &userID);
            if (ret != 0) {
                fprintf(stderr, "Client loginUser return (%d)\n", ret);
            }
            
            char rep_str[100]="2:login success:";
            strcat(rep_str,usrname);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free(usrname);
            free(str);
        }
        
        //  MARK:logout
        else if(numtype==0){
            printf("[#%d logout]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *usrname = strdup(token);
            printf("username(logout): %s\n",usrname);
            
            TPUserID userID;
//            memset(&userID,0,sizeof(userID));
//            strcpy((char*)userID,usrname);
            TPUsernameToUserID(context, usrname, &userID);
            
            ret = TPLogoutUser(context, &userID);
            if (ret != 0) {
                fprintf(stderr, "Client logoutUser return (%d)\n", ret);
            }
            
            char rep_str[100]="0:logout success:";
            strcat(rep_str,usrname);
            s_send(responder,rep_str);
            printf("\n\n");
            
            free(usrname);
            free(str);
        }
        
        //  MARK:read msg
        else if(numtype==10){
            printf("[#%d read msg]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *myUserID = strdup(token);
            printf("username(req): %s\n",myUserID);
            
            char* dtID;
            char myMsg[400]="10:";
            
            TPUserID userID;
            TPTransportMsg *message;
//            memset(&userID,0,sizeof(userID));
//            strcpy((char*)userID,myUserID);
            
            if (TPUsernameToUserID(context, myUserID, &userID) != 0)
                return;
            char uuid_str[37];
            uuid_unparse_lower(userID, uuid_str);
            printf("> userID: %s\n", uuid_str);
            if (strcmp((char *)&userID, "NULL") != 0) {
                //                message = TPMsgGet(context, &userID, TPTransportMsg::TP_MSG_TYPE_CHAT);
                message = TPMsgGet(context, &userID, TP_MSG_TYPE_CHAT);
            }
            else {
                //                message = TPMsgGet(context, NULL, TPTransportMsg::TP_MSG_TYPE_CHAT);
                message = TPMsgGet(context, NULL, TP_MSG_TYPE_CHAT);
            }
            
            if (message != NULL) {
//                message->msg[message->bytesCount] = '\0';
                
                //---- create data --------------------
                Data* dt = buildDatafromString(message->msg);
                if(dt != NULL){
                    addData2Data(NULL, dt, -1, &dtPacket);
                }
                //-------------------------------------
                
                printf("cloned msg(all): %s\n",message->msg);
                
                char fromName[64];
                char toName[64];
                if (TPUserIDToUsername(context, (TPUserID *)message->from, fromName) != 0) {
                    strcpy(fromName, message->from);
                }
                if (TPUserIDToUsername(context, (TPUserID *)message->to, toName) != 0) {
                    strcpy(toName, message->to);
                }
                
                strcat(myMsg,myUserID);
                strcat(myMsg,":");
                strcat(myMsg,fromName);
                strcat(myMsg,":");
                strcat(myMsg,toName);
                strcat(myMsg,":");
                token = strtok(message->msg,":");
                dtID = strdup(token);
                printf("cloned id: %s\n",dtID);
                strcat(myMsg,dtID);
                
                printf("send this str: %s\n",myMsg);
                s_send(responder,myMsg);
                free(dtID);
            } else {
                printf("not found message\n");
                strcat(myMsg,myUserID);
                strcat(myMsg,":none");
                s_send(responder,myMsg);
            }
            printf("\n\n");
            
            TPMsgFree(message);
            free(myUserID);
            free(str);
        }
        
        //  MARK:get datalists
        else if(numtype == 7){
            printf("[#%d get datalists]----------------------\n",numtype);
            
            char** list;
            char strlists[10000] = "";
            char rep_str[10000];
            
            token = strtok(NULL,":");
            int casetype = atoi(token);
            
            token = strtok(NULL,":");
            char *dtID = strdup(token);
            printf("dtID : %s\n", dtID);
            
            if (casetype == 1){
                list = getUserFromOrgID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:User:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:Userlists are NULL");
                }
                
                
            }
            else if (casetype == 2){
                list = getCatFromUserID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:Category:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:Categorylists are NULL");
                }
            }
            else if (casetype == 3){
                list = getStateFromCatID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:State:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:Statelists are NULL");
                }
            }
            else if (casetype == 4){
                list = getTaskFromStateID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:Task:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:Tasklists are NULL");
                }
            }
            else if (casetype == 5){
                list = getSubTaskFromTaskID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:Subtask:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:Subtasklists are NULL");
                }
            }
            else if (casetype == 6){
                list = getTaskFromCatID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:TaskfromCat:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:TaskfromCat are NULL");
                }
            }
            else if (casetype == 7){
                list = getSubTaskFromTaskID(dtID, &dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:SubtaskfromCat:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:SubtaskfromCat are NULL");
                }
            }
            else if (casetype == 8){
                list = getOrgID(&dtPacket);
                if (list != NULL){
                    printf("\n--- test list ---\n");
                    for(int i=0;list[i]!=NULL;i++){
                        printf("list[%d]: %s\n",i,list[i]);
                        strcat(strlists, list[i]);
                        strcat(strlists, ":");
                        free(list[i]);
                    }
                    free(list);
                    
                    sprintf(rep_str, "4:Org:%s", strlists);
                    printf("%s\n", rep_str);
                    s_send(responder, rep_str);
                }
                else{
                    s_send(responder, "404:Orglists are NULL");
                }
            }
            printf("\n\n");
            
            if(dtID != NULL){
                free(dtID);
            }
            free(str);
        }
        
        //  MARK:query data from datalists
        else if(numtype == 8){
            printf("[#%d query data from datalists]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *dtID = strdup(token);
            printf("dtID : %s\n", dtID);
            
            Data *Qdata = queryDataByID(dtID, &dtPacket);
            if (Qdata != NULL){
                char* strDT = genString(Qdata);
                char* msg = genMsg(Qdata);
                
                char rep_str[10000];
                sprintf(rep_str, "5:%s:%s", msg, strDT);
                printf("%s\n", rep_str);
                s_send(responder, rep_str);
                
                free(strDT);
                free(msg);
            }
            else{
                s_send(responder, "404:not found data");
            }
            printf("\n\n");
            
            free(dtID);
            free(str);
        }
        
        //  MARK:add Edge
        else if(numtype == 9){
            printf("[#%d add edge]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            int casetype = atoi(token);
            
            token = strtok(NULL,":");
            char *Rdata = strdup(token);
            
            token = strtok(NULL,":");
            char *Ldata = strdup(token);
            
            if (casetype == 1){
                int chk = addUser2OrgByID(Ldata, Rdata, &dtPacket);
                if (chk == 0){
                    s_send(responder, "404:add User -> Org success");
                }
                else{
                    s_send(responder, "404:add User -> Org error!");
                }
            }
            else if (casetype == 2){
                int chk = addCategory2UserByID(Ldata, Rdata, &dtPacket);
                if (chk == 0){
                    s_send(responder, "404:add Category -> User success");
                }
                else{
                    s_send(responder, "404:add Category -> User error!");
                }
            }
            else if (casetype == 3){
                int chk = addState2CategoryByID(Ldata, Rdata, &dtPacket);
                if (chk == 0){
                    s_send(responder, "404:add State -> Category success");
                }
                else{
                    s_send(responder, "404:add State -> Category error!");
                }
            }
            else if (casetype == 4){
                int chk = addTask2StateByID(Ldata, Rdata, &dtPacket);
                if (chk == 0){
                    s_send(responder, "404:add Task -> State success");
                }
                else{
                    s_send(responder, "404:add Task -> State error!");
                }
            }
            else if (casetype == 5){
                int chk = addSubTask2TaskByID(Ldata, Rdata, &dtPacket);
                if (chk == 0){
                    s_send(responder, "404:add Subtask -> Task success");
                }
                else{
                    s_send(responder, "404:add Subtask -> Task error!");
                }
            }
            else if (casetype == 6){
                int chk = addTask2CategoryByID(Ldata, Rdata, &dtPacket);
                if (chk == 0){
                    s_send(responder, "404:add Task -> Category success");
                }
                else{
                    s_send(responder, "404:add Task -> Category error!");
                }
            }
            printf("\n\n");
            
            free(Rdata);
            free(Ldata);
            free(str);
        }
        
        else{
            printf("error numtype\n");
        }
    }
}

