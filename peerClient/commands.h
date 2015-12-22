#ifndef COMMANDS
#define COMMANDS
#include <iostream>
#include <stdint.h>
#include <string.h>

typedef uint8_t UNSIGNED_CHARACTER;
typedef int32_t SIGNED_INT;
typedef std::string STRING;
using namespace std;

//serialize deserialize typeDef
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#define SWAP32(a) ( \
    ((a & 0x000000FF) << 24) | \
    ((a & 0x0000FF00) <<  8) | \
    ((a & 0x00FF0000) >>  8) | \
    ((a & 0xFF000000) >> 24))



#define SERIALIZE_8(tmp, getMethod, buf, bytes) \
    tmp = getMethod; \
    memcpy(buf+bytes, &tmp, sizeof(tmp)); \
    bytes += sizeof(tmp);

#define SERIALIZE_32(tmp, getMethod, buf, bytes) \
    tmp = getMethod;\
    memcpy(buf+bytes, &tmp, sizeof(tmp));\
    bytes += sizeof(tmp);

#define SERIALIZE_STRING(tmp, getMethod, buf, bytes, stringSize ) \
    tmp = getMethod; \
    stringSize = tmp.size(); \
    memcpy(buf+bytes, &stringSize, sizeof(stringSize));\
    bytes += sizeof(stringSize);    \
    memcpy(buf+bytes, tmp.c_str(), tmp.size()); \
    bytes += tmp.size();

#define DESERIALIZE_8(tmp, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    setMethod;\
    offset  += sizeof(tmp);

#define DESERIALIZE_32(tmp, tmpData, setMethod, buf, offset)\
    memcpy(&tmp, buf + offset, sizeof(tmp));\
    tmpData = tmp; \
    setMethod;\
    offset  += sizeof(tmp);

#define DESERIALIZE_STRING(tmpS1, setMethod, buf, offset, stringSize)\
    memcpy(&stringSize,buf + offset, sizeof(stringSize)); \
    offset += sizeof(stringSize); \
    tmpS1.assign(buf + offset, stringSize);\
    setMethod;\
    offset+=tmpS1.size();



namespace CMD {
    enum commands
    {
        INFORM_AND_UPDATE = 1, //for informing the server
        QUERY_FOR_CONTENT,  //for sending query to server for a file
        ACK_CONTENT_FROM_SERVER,    //ACK for Query output
        CLIENT_REQUEST, //HTTP GET,Request other peer for file
        CLIENT_CONFIMATION_FOR_UPLOADING, //HTTP 400,transient peer sends confirmation of uploading
        CLIENT_FILE_NOT_FOUND, //HTTP 404,if file not available at transisent peer server
        FRAGEMENT_FILE_DATA,    //for sending fragmented file data
        EXIT_BY_CLIENT  //for exiting from directory server
     };
    class ClientConfirmation
    {
        private:
            STRING _fileName;
            SIGNED_INT _totalNoOfPackets;
         public:
            ClientConfirmation() { }
            ClientConfirmation(char *buf) ;
            ClientConfirmation(STRING fileName,  SIGNED_INT totalNoOfPackets)
            {
                _fileName = fileName;
                _totalNoOfPackets = totalNoOfPackets;
            }
            int serialize(char *buf);
            STRING getFileName() { return _fileName; }
            SIGNED_INT getTotalNoOfPackets() { return _totalNoOfPackets; }
            void setFileName(STRING fileName) { _fileName = fileName; }
            void setTotalNoOfPackets(SIGNED_INT totalNoOfPackets) { _totalNoOfPackets = totalNoOfPackets; }
            void dump();

    };
    class ClientFileNotFound
    {
        private:
            STRING _fileName;
         public:
            ClientFileNotFound() { }
            ClientFileNotFound(char *buf) ;
            ClientFileNotFound(STRING fileName)
            {
                _fileName = fileName;
            }
            int serialize(char *buf);
            STRING getFileName() { return _fileName; }
            void setFileName(STRING fileName) { _fileName = fileName; }
            void dump();

    };
    class ClientRequest
    {
    private:
        STRING _fileName;
        STRING _userName;
        STRING _ip;
        SIGNED_INT _port;
     public:
        ClientRequest() { }
        ClientRequest(char *buf);
        ClientRequest(STRING fileName, STRING userName, STRING ip, SIGNED_INT port)
        {
            _fileName = fileName;
            _userName = userName;
            _ip = ip;
            _port = port;
        }
        int serialize(char *buf);
        STRING getFileName() { return _fileName; }
        STRING getUserName() { return _userName; }
        STRING getIp() { return _ip; }
        SIGNED_INT getPort() { return _port; }
        void setFileName(STRING fileName) { _fileName = fileName; }
        void setUserName(STRING userName) { _userName = userName; }
        void setIp(STRING ip) { _ip = ip; }
        void setPort(SIGNED_INT port) { _port = port; }

        void dump();
    };

    class Query
    {
    private:
        STRING _ipAddr;
        STRING _fileName;
        SIGNED_INT _port;
        SIGNED_INT _queryAll;
    public:
        Query()
        {
            _queryAll = false; //set 1 for all files query
        }
        Query(char* buf);
        int serialize(char *buf);
        STRING getIpAddr() { return _ipAddr; }
        STRING getFileName() { return _fileName; }
        SIGNED_INT getPort() { return _port; }
        SIGNED_INT getQueryAll() { return _queryAll ; }
        void setIpAddr(STRING ipAddr) { _ipAddr = ipAddr; }
        void setFileName(STRING fileName) { _fileName = fileName; }
        void setPort(SIGNED_INT port) { _port = port; }
        void setQueryAll(SIGNED_INT queryAll ) { _queryAll = queryAll; }
        void dump();
    };
    class FileData
    {
    private:
        SIGNED_INT _seqNo;
        STRING _fileName;
        STRING _data;
    public:
        FileData(){}
        FileData(SIGNED_INT seqNo, STRING fileName, STRING data)
        {
            _seqNo = seqNo;
            _fileName = fileName;
            _data = data;

        }
        FileData(char *buf);
        int serialize(char *buf);
        SIGNED_INT getSeqNo() { return _seqNo; }
        STRING getFileName() { return _fileName; }
        STRING getData() { return _data; }
        void setSeqNo(SIGNED_INT seqNo) { _seqNo = seqNo; }
        void setFileName(STRING fileName) {  _fileName = fileName; }
        void setData(STRING data) { _data = data; }

        void dump();


    };
    class FileInfo
    {
    private:
        STRING _fileName;
        STRING _ip;
        SIGNED_INT _port;
        SIGNED_INT _seq;
    public:
        FileInfo(){}
        FileInfo(SIGNED_INT port, SIGNED_INT seq, STRING fileName, STRING ip)
        {
            _port = port;
            _fileName = fileName;
            _ip = ip;
            _seq = seq;

        }
        FileInfo(char *buf);
        int serialize(char *buf);
        SIGNED_INT getSeq() { return _seq; }
        SIGNED_INT getPort() { return _port; }
        STRING getFileName() { return _fileName; }
        STRING getIp() { return _ip; }
        void setPort(SIGNED_INT port) { _port = port; }
        void setSeq(SIGNED_INT seq) { _seq = seq; }
        void setFileName(STRING fileName) {  _fileName = fileName; }
        void setIp(STRING ip) { _ip = ip; }

        void dump();
      };

    class ClientExit
    {
    private:
        STRING _ip;
        SIGNED_INT _port;
    public:
        ClientExit(){}
        ClientExit(SIGNED_INT port, STRING ip)
        {
            _port = port;
            _ip = ip;

        }
        ClientExit(char *buf);
        int serialize(char *buf);
        SIGNED_INT getPort() { return _port; }
        STRING getIp() { return _ip; }
        void setPort(SIGNED_INT port) { _port = port; }
        void setIp(STRING ip) { _ip = ip; }

        void dump();
      };


}
namespace RSP
{
        enum Response
        {
            QUERY_OUTPUT = 1,
            ACK_INFORM_AND_UPDATE_SERVER,
        };
        class AckFromServer
        {
            private:
                int _seqNo;
                char _rsp;
        public:
                AckFromServer(int seq, char cmd)
                {
                    _seqNo = seq;
                    _rsp = cmd;
                }
                AckFromServer(char *buf);
                int serialize(char *buf);
                int getSeq() { return _seqNo; }
                void setSeq(int seq) { _seqNo = seq; }
                char getCommand() { return _rsp; }
                void dump();
        };
        class QueryOutput
        {
            private:
                int _seqNo;
                STRING _ipAddr;
                STRING _fileName;
                SIGNED_INT _port;
                char _buff[128];
            public:
                QueryOutput() {}
                QueryOutput(char* buf);
                int serialize(char *buf);

                STRING getIpAddr() { return _ipAddr; }
                STRING getFileName() { return _fileName; }
                SIGNED_INT getPort() { return _port; }
                SIGNED_INT getSeq() { return _seqNo; }

                void setIpAddr(STRING ipAddr) { _ipAddr = ipAddr; }
                void setFileName(STRING fileName) { _fileName = fileName; }
                void setPort(SIGNED_INT port) { _port = port; }
                void setSeq(SIGNED_INT seqNo) { _seqNo = seqNo; }
                void dump();
             };


}
#endif // COMMANDS

