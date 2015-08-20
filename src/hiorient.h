
#ifdef __cplusplus
extern "C" {
#endif
typedef struct Packet {
    char opType;
    char ssid[4];
    char msg[20000];
} Packet;
//Packet GPacket;

typedef struct DTPacket {
    Packet GPacket;
    int Sockfd;
} DTPacket;

int connectSocket(DTPacket* dtPacket);
int connectServer(DTPacket* dtPacket);
int disconnectServer(DTPacket* dtPacket);
int createDatabase(char *dbName, DTPacket* dtPacket);
int createClass(char *myclass, char *extend, DTPacket* dtPacket);
int prepareDB(DTPacket* dtPacket);
int openDatabase(char *dbName, DTPacket* dtPacket);
int sendCommand(char *command, DTPacket* dtPacket);
int createVertex(char *command, short* cltid, long* rid, DTPacket* dtPacket);
char* getContent(char *query, DTPacket* dtPacket);
char** getArrayRid(char* query, DTPacket* dtPacket);
char** getArrayDistinctRid(char* query, DTPacket* dtPacket);

void varintParse(int64_t in, uint8_t *buffer);
int64_t varintUnparse(uint8_t *buffer);
void printAsciiVarint(uint8_t *buffer);
#ifdef __cplusplus
}
#endif
