
//  transport
#include <zmq.h>
#include <assert.h>
#include "zhelpers.h"
#include <transport.h>
//#include <jansson.h>

#include <string>
//#include <iostream>
//#include <cstdlib>
//#include <cstdio>
//#include <fstream>

//#include "reqmsg.h"
//#include "swap_endian.h"
//#include "diff_match_patch.h"
//#include "ezxml.h"
//#include "hiorient.h"
#include "datamodel.h"

/* test function */
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
    
    test_setNewData();
    return 0;
    
//    testWeb();
//    return 0;
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
//    prepareDB(&dtPacket);
//-------------------------------------------------------------------------

//    Sockfd = connectSocket();
    dtPacket.Sockfd = connectSocket(&dtPacket);
    if (dtPacket.Sockfd < 0){
        printf ("error connectSocket\n");
    }
    
//    ret = openDatabase(DB_NAME);
    ret = _openDatabase(DB_NAME,&dtPacket);
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
//    createVertex(sql, &_cltid, &_rid);
//    printf("@rid #%d:%lu\n",_cltid,_rid);
    
    Data* dt = *data;
    int i=0;
    
    char* strGen = genString(dt);
    printf("\n%s\n",strGen);
    
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
//    Data** arr_dt = queryAllUsersFromData("E4DEA39D5E7646918E07B414C2CC0671",&dtPacket);
//    for(i=0;arr_dt[i]!=NULL;i++){
//        printf("id-arrdt[%d]: %s\n",i,arr_dt[i]->dataID);
//        printf("name-arrdt[%d]: %s\n",i,arr_dt[i]->dataName);
//        printf("type-arrdt[%d]: %d\n",i,arr_dt[i]->dataType);
//        freeData(arr_dt[i]);
//    }
//    free(arr_dt);
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
    disconnectServer(&dtPacket);
    close(dtPacket.Sockfd);
}

void testWeb(){
    int ret;
    DTPacket dtPacket;
    dtPacket.Sockfd = connectSocket(&dtPacket);
    if (dtPacket.Sockfd < 0){
        printf ("error connectSocket\n");
    }
    ret = _openDatabase(DB_NAME,&dtPacket);
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
        char *str = s_recv(responder);
        //        printf("str: %s\n",str);
        
        int numtype;
        char *token = strtok(str,":");
        numtype = atoi(token);
        
        //  create new data
        if(numtype == 1){
            printf("[#%d create new data]----------------------\n",numtype);
            
            char msg[10] = "";
            char new_uuid[100] = "";
            token = strtok(NULL,":");
            int casetype = atoi(token);
            token = strtok(NULL,":");
            char *Name = strdup(token);
            printf("%s\n", Name);
            free(str);
            
            char * org_uuid = (char *)malloc(33*sizeof(char));
            char * user_uuid = (char *)malloc(33*sizeof(char));
            char * category_uuid = (char *)malloc(33*sizeof(char));
            char * state_uuid = (char *)malloc(33*sizeof(char));
            char * task_uuid = (char *)malloc(33*sizeof(char));
            char * subtask_uuid = (char *)malloc(33*sizeof(char));
            
            if (casetype == 1){
                const char *dID = createOrg(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(org_uuid, dID);
                strcat(msg, "Org");
                printf("---%s\n", org_uuid);
                free((char*)dID);
            }
            else if (casetype == 2){
                const char *dID = createUser(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(user_uuid, dID);
                strcat(msg, "User");
                printf("---%s\n", org_uuid);
                printf("---%s\n", user_uuid);
                addUser2OrgByID(org_uuid, user_uuid, &dtPacket);
                free((char*)dID);
            }
            else if (casetype == 3){
                const char *dID = createCategory(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(category_uuid, dID);
                strcat(msg, "Category");
                printf("---%s\n", user_uuid);
                printf("---%s\n", category_uuid);
                addCategory2UserByID(user_uuid, category_uuid, &dtPacket);
                free((char*)dID);
            }
            else if (casetype == 4){
                const char *dID = createState(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(state_uuid, dID);
                strcat(msg, "State");
                printf("---%s\n", category_uuid);
                printf("---%s\n", state_uuid);
                addState2CategoryByID(category_uuid, state_uuid, &dtPacket);
                free((char*)dID);
            }
            else if (casetype == 5){
                const char *dID = createTask(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(task_uuid, dID);
                strcat(msg, "Task");
                printf("---%s\n", state_uuid);
                printf("---%s\n", task_uuid);
                addTask2StateByID(state_uuid, task_uuid, &dtPacket);
                free((char*)dID);
            }
            else if (casetype == 6){
                const char *dID = createSubTask(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(subtask_uuid, dID);
                strcat(msg, "Subtask");
                printf("---%s\n", task_uuid);
                printf("---%s\n", subtask_uuid);
                addSubTask2TaskByID(task_uuid, subtask_uuid, &dtPacket);
                free((char*)dID);
            }
            else if (casetype == 7){
                const char *dID = createTask(Name, newSchema, &dtPacket);
                strcat(new_uuid, dID);
                strcpy(task_uuid, dID);
                strcat(msg, "Task");
                printf("---%s\n", category_uuid);
                printf("---%s\n", task_uuid);
                addTask2CategoryByID(category_uuid, task_uuid, &dtPacket);
                free((char*)dID);
            }
            
            //            if (casetype == 1){
            //                strcat(new_uuid, "A49A6E33D8884D8CB4BA9AE217F077E9");
            //                strcat(msg, "Org");
            //            }
            //            else if (casetype == 2){
            //                strcat(new_uuid, "613CC870FA9A4ECABE5E42E3EE66DF15");
            //                strcat(msg, "User");
            //            }
            //            else if (casetype == 3){
            //                strcat(new_uuid, "A57CA09C802745289C9959EA43D1CF2F");
            //                strcat(msg, "Category");
            //            }
            //            else if (casetype == 4){
            //                strcat(new_uuid, "E8E045F1C7BF4F3280E3912AE29FA9B8");
            //                strcat(msg, "State");
            //            }
            //            else if (casetype == 5){
            //                strcat(new_uuid, "236A19345EFB4A16936A9562BAA21D7D");
            //                strcat(msg, "Task");
            //            }
            //            else if (casetype == 6){
            //                strcat(new_uuid, "ADE6EA3EDC1D4119A9B66AE3B7705F0A");
            //                strcat(msg, "Subtask");
            //            }
            //            else if (casetype == 7){
            //                strcat(new_uuid, "A2C75A6CFCAD4B9986CEFEE70B97330C");
            //                strcat(msg, "Task");
            //            }
            
            Data* dData = queryDataByID(new_uuid, &dtPacket);
            printf("--->%s\n", dData->dataID);
            printf("--->%s\n", dData->dataName);
            printf("--->%d\n", dData->dataType);
            printf("--->%s\n", dData->chatRoom);
            printf("----------\n");
            printf("-->%d\n", dData->content->versionKeeped);
            printf("----------\n");
            printf("SHA : %s\n", dData->content->lastestCommon->SHA256hashCode);
            printf("Stamp : %s\n", dData->content->lastestCommon->timeStamp);
            printf("isDiff : %d\n", dData->content->lastestCommon->isDiff);
            printf("----------\n");
            
            char* strDT = genString(dData);
            char rep_str[10000];
            sprintf(rep_str, "1:%s:%s", msg, strDT);
            printf("%s\n", rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free(org_uuid);
            free(user_uuid);
            free(category_uuid);
            free(state_uuid);
            free(task_uuid);
            free(subtask_uuid);
            free(dData);
            free(Name);
            
        }
        //  edit and save
        else if(numtype == 2){
            printf("[#%d edit+save]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *id = strdup(token);
            token = strtok(NULL,":");
            char *content = strdup(token);
            free(str);
            
            saveNewDataContentByID(id, content, &dtPacket);
            
            Data* dData = queryDataByID(id, &dtPacket);
            printf("--->%s\n", dData->dataID);
            printf("--->%s\n", dData->dataName);
            printf("--->%d\n", dData->dataType);
            printf("--->%s\n", dData->chatRoom);
            printf("----------\n");
            printf("-->%d\n", dData->content->versionKeeped);
            printf("----------\n");
            printf("SHA : %s\n", dData->content->lastestCommon->SHA256hashCode);
            printf("Stamp : %s\n", dData->content->lastestCommon->timeStamp);
            printf("isDiff : %d\n", dData->content->lastestCommon->isDiff);
            printf("----------\n");
            
            char* strDT = genString(dData);
            char* msg = genMsg(dData);
            
            char rep_str[10000];
            sprintf(rep_str, "1:%s:%s", msg, strDT);
            printf("%s\n", rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free(id);
            free(content);
            //            free(strDT);
            //            free(msg);
            freeData(dData);
        }
        //  delete
        else if(numtype == 3){
            printf("[#%d delete]----------------------\n",numtype);
            
            //  deleteObj
            free(str);
            s_send(responder, "test");
            printf("\n\n");
        }
        //  query data
        else if(numtype == 4){
            printf("[#%d query]----------------------\n",numtype);
            
            //char uuid[100] = "ADE6EA3EDC1D4119A9B66AE3B7705F0A";
            token = strtok(NULL,":");
            char *id = strdup(token);
            free(str);
            printf(">>>>> Query function <<<<<\n");
            
            // Org** ddt = queryOrgFromData(uuid, &dtPacket);
            // User** ddt = queryAllUsersFromData(uuid, &dtPacket);
            // Category** ddt = queryAllCategoriesFromData(uuid,&dtPacket);
            // State** ddt = queryAllStatesFromData(uuid, &dtPacket);
            // Task** ddt = queryAllTasksFromData(uuid, &dtPacket);
            // SubTask** ddt = queryAllSubTasksFromData(uuid, &dtPacket);
            // for (int i = 0; ddt[i] != NULL; ++i)
            // {
            //     printf("------------- %d ---------------\n", i);
            //     printf("DataID: %s\n", ddt[i]->dataID);
            //     printf("DataNane: %s\n", ddt[i]->dataName);
            //     printf("DataType: %d\n", ddt[i]->dataType);
            //     printf("--------------------------------\n");
            
            // }
            
            Data *data = queryDataByID(id, &dtPacket);
            char* strDT = genString(data);
            char* msg = genMsg(data);
            printf("-=-=-=-=-=- %s\n", msg);
            char rep_str[10000];
            sprintf(rep_str, "1:%s:%s", msg, strDT);
            printf(">>>>> Finished Query <<<<<\n");
            printf("%s\n", rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free(id);
            //            free(strDT);
            //            free(msg);
            freeData(data);
        }
        //  clone
        else if(numtype == 5){
            printf("[#%d clone]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *id = strdup(token);
            printf("id : %s\n", id);
            
            //---- sendMsg ------------------------------------------------------------
            printf("\n---- sendMsg ----\n");
            //            char msg[256];
            
            token = strtok(NULL,":");
            char *mySender = strdup(token);
            printf("sender: %s\n",mySender);
            
            token = strtok(NULL,":");
            char *myTarget = strdup(token);
            printf("target: %s\n",myTarget);
            
            //            sprintf(msg,"%s",id);
            
            free(str);
            
            //---- query data ---------------------
            Data* dt = queryDataByID(id,&dtPacket);
            char* strPack = NULL;
            if(dt != NULL){
                strPack = buildStringFromData(dt);
            }
            //-------------------------------------
            
            TPUserID sender;
            TPUserID target;
            
            memset(&sender,0,sizeof(sender));
            strcpy((char*)sender,mySender);
            
            memset(&target,0,sizeof(target));
            strcpy((char*)target,myTarget);
            
            if(dt!=NULL){
                message.msg = strPack;
                message.bytesCount = (int)strlen(strPack);
            }
            else{
                message.msg = id;
                message.bytesCount = (int)strlen(id);
            }
            
            printf("msg: %s\nbytes: %d\n",message.msg,message.bytesCount);
            //  fixed
            message.msgType = TPTransportMsg::TP_MSG_TYPE_CHAT;
            //  TPTransportMsg::
            ret = TPSendToUser(context, &sender, &target, &message);
            if (ret != 0) {
                fprintf(stderr, "Client sendToUser return (%d)\n", ret);
            }
            //-------------------------------------------------------------------------
            
            char rep_str[100]="5:clone success";
            strcat(rep_str,":");
            strcat(rep_str,id);
            strcat(rep_str,":");
            strcat(rep_str,mySender);
            printf("rep_str: %s\n",rep_str);
            s_send(responder, rep_str);
            printf("\n\n");
            
            if(dt!=NULL){
                freeData(dt);
                free(strPack);
            }
            free(id);
            //            free(user);
            free(mySender);
            free(myTarget);
        }
        //  login
        else if(numtype == 6){
            printf("[#%d login]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *usrname = strdup(token);
            free(str);
            printf("username(login): %s\n",usrname);
            
            TPUserID userID;
            memset(&userID,0,sizeof(userID));
            strcpy((char*)userID,usrname);
            
            ret = TPLoginUser(context, &userID);
            if (ret != 0) {
                fprintf(stderr, "Client loginUser return (%d)\n", ret);
            }
            
            char rep_str[100]="6:login success";
            strcat(rep_str,":");
            strcat(rep_str,usrname);
            s_send(responder, rep_str);
            printf("\n\n");
            
            free(usrname);
        }
        //  logout
        else if(numtype==0){
            printf("[#%d logout]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *usrname = strdup(token);
            free(str);
            
            printf("username(logout): %s\n",usrname);
            
            TPUserID userID;
            memset(&userID,0,sizeof(userID));
            strcpy((char*)userID,usrname);
            
            ret = TPLogoutUser(context, &userID);
            if (ret != 0) {
                fprintf(stderr, "Client logoutUser return (%d)\n", ret);
            }
            
            char rep_str[100]="0:logout success";
            strcat(rep_str,":");
            strcat(rep_str,usrname);
            s_send(responder,rep_str);
            printf("\n\n");
            
            free(usrname);
        }
        //  read msg
        else if(numtype==10){
            printf("[#%d readMsg]----------------------\n",numtype);
            
            token = strtok(NULL,":");
            char *myUserID = strdup(token);
            free(str);
            
            char myMsg[400]="10";
            printf("username(req): %s\n",myUserID);
            
            char* id;
            
            TPUserID userID;
            TPTransportMsg *message;
            memset(&userID,0,sizeof(userID));
            strcpy((char*)userID,myUserID);
            
            if (strcmp((char *)&userID, "NULL") != 0) {
                message = TPMsgGet(context, &userID, TPTransportMsg::TP_MSG_TYPE_CHAT);
            }
            else {
                message = TPMsgGet(context, NULL, TPTransportMsg::TP_MSG_TYPE_CHAT);
            }
            if (message != NULL) {
                message->msg[message->bytesCount] = '\0';
                
                //---- create data --------------------
                Data* dt = buildDatafromString(message->msg);
                if(dt != NULL){
                    addData2Data(NULL, dt, -1, &dtPacket);
                }
                //-------------------------------------
                
                printf("cloned msg: %s\n",message->msg);
                //                printf("%s: %s\n", message->from, message->msg);
                //                printf("'%s'->'%s': '%s' @%lu\n", message->from, message->to, message->msg, message->timestamp);
                strcat(myMsg,":");
                strcat(myMsg,myUserID);
                strcat(myMsg,":");
                strcat(myMsg,message->from);
                strcat(myMsg,":");
                strcat(myMsg,message->to);
                strcat(myMsg,":");
                //                strcat(myMsg,message->msg);
                token = strtok(message->msg,":");
                printf("cloned id: %s\n",id);
                id = strdup(token);
                strcat(myMsg,id);
                //                strcat(myMsg,":");
                //
                //                char tstamp[20]="";
                //                sprintf(tstamp,"%lu",message->timestamp);
                //                //                printf("time: @%lu\n",message->timestamp);
                //                strcat(myMsg,tstamp);
                
                TPMsgFree(message);
                
                printf("send this str: %s\n",myMsg);
                s_send(responder,myMsg);
                free(id);
            } else {
                printf("not found message\n");
                strcat(myMsg,":");
                strcat(myMsg,myUserID);
                strcat(myMsg,":none");
                s_send(responder,myMsg);
            }
            printf("\n\n");
            
            free(myUserID);
        }
        else{
            printf("error numtype\n");
        }
        
    }
}

