#include "commands.h"
RSP::AckFromServer::AckFromServer(char* buf)
{
    int offset = 0;
    SIGNED_INT tmp1 = 0, tmp2 = 0;
    DESERIALIZE_32(tmp1, tmp2, setSeq(tmp2), buf, offset);
}
int RSP::AckFromServer::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;

    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(_rsp), buf, bytes);
    SERIALIZE_32(tmpInt, getSeq(), buf, bytes);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void RSP::AckFromServer::dump()
{
    std::cout<<"---dump of AckFromServer------"<<endl;
    std::cout<<"getCommand(): "<<(int)getCommand()<<endl;
    std::cout<<"getSeq(): "<<getSeq()<<endl;
}



RSP::QueryOutput::QueryOutput(char* buf)
{
    int offset = 0;
    SIGNED_INT tmp1 = 0, tmp2 = 0;
    STRING tmpS1,tmpS2;
    SIGNED_INT stringSize;
    DESERIALIZE_STRING(tmpS1, setIpAddr(tmpS1), buf, offset, stringSize);
    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);
    DESERIALIZE_32(tmp1, tmp2, setPort(tmp2), buf, offset);
    DESERIALIZE_32(tmp1, tmp2, setSeq(tmp2), buf, offset);

}
int RSP::QueryOutput::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(QUERY_OUTPUT), buf, bytes);
    SERIALIZE_STRING(tmpString,getIpAddr(),buf,bytes,stringSize);
    SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);
    SERIALIZE_32(tmpInt, getPort(), buf, bytes);
    SERIALIZE_32(tmpInt, getSeq(), buf, bytes);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void RSP::QueryOutput::dump()
{
    std::cout<<"---dump of QueryOutput------"<<endl;
    std::cout<<"getIpAddr(): "<<getIpAddr()<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
    std::cout<<"getPort(): "<<getPort()<<endl;
    std::cout<<"getSeq(): "<<getSeq()<<endl;
}
CMD::Query::Query(char* buf)
{
    int offset = 0;
    SIGNED_INT tmp1 = 0, tmp2 = 0;
    STRING tmpS1,tmpS2;
    SIGNED_INT stringSize;

    DESERIALIZE_STRING(tmpS1, setIpAddr(tmpS1), buf, offset, stringSize);
    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);
    DESERIALIZE_32(tmp1, tmp2, setPort(tmp2), buf, offset);
    DESERIALIZE_32(tmp1, tmp2, setQueryAll(tmp2), buf, offset);


}
int CMD::Query::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(QUERY_FOR_CONTENT), buf, bytes);
    SERIALIZE_STRING(tmpString,getIpAddr(),buf,bytes,stringSize);
    SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);
    SERIALIZE_32(tmpInt, getPort(), buf, bytes);
    SERIALIZE_32(tmpInt, getQueryAll(), buf, bytes);


    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void CMD::Query::dump()
{
    std::cout<<"---dump of Query------"<<endl;
    std::cout<<"getIpAddr(): "<<getIpAddr()<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
    std::cout<<"getPort(): "<<getPort()<<endl;
    std::cout<<"getQueryAll(): "<<getQueryAll()<<endl;
}

CMD::ClientConfirmation::ClientConfirmation(char* buf)
{
    int offset = 0;

    SIGNED_INT tmp1 = 0, tmp2 = 0;
    STRING tmpS1,tmpS2;
    SIGNED_INT stringSize;

    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);
    DESERIALIZE_32(tmp1, tmp2, setTotalNoOfPackets(tmp2), buf, offset);
}
int CMD::ClientConfirmation::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(CLIENT_CONFIMATION_FOR_UPLOADING), buf, bytes);
    SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);
    SERIALIZE_32(tmpInt, getTotalNoOfPackets(), buf, bytes);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void CMD::ClientConfirmation::dump()
{
    std::cout<<"---dump of ClientConfirmation------"<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
    std::cout<<"getTotalNoOfPackets(): "<<getTotalNoOfPackets()<<endl;
}

CMD::ClientFileNotFound::ClientFileNotFound(char* buf)
{
    int offset = 0;
    STRING tmpS1;
    SIGNED_INT stringSize;

    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);
}
int CMD::ClientFileNotFound::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(CLIENT_FILE_NOT_FOUND), buf, bytes);
    SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}

void CMD::ClientFileNotFound::dump()
{
    std::cout<<"---dump of ClientConfirmation------"<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
}

CMD::ClientRequest::ClientRequest(char* buf)
{
    int offset = 0;


    STRING tmpS1;

    SIGNED_INT stringSize,tmp1,tmp2;

    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);
    DESERIALIZE_STRING(tmpS1, setUserName(tmpS1), buf, offset, stringSize);
    DESERIALIZE_STRING(tmpS1, setIp(tmpS1), buf, offset, stringSize);
    DESERIALIZE_32(tmp1, tmp2,setPort(tmp1), buf, offset);

}
int CMD::ClientRequest::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(CLIENT_REQUEST), buf, bytes);
    tmpString = getFileName(); \
    stringSize = tmpString.size(); \
    memcpy(buf+bytes, &stringSize, sizeof(stringSize));\
    bytes += sizeof(stringSize);    \
    memcpy(buf+bytes, tmpString.c_str(), tmpString.size()); \
    bytes += tmpString.size();

    // SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);
    SERIALIZE_STRING(tmpString,getUserName(),buf,bytes,stringSize);
    SERIALIZE_STRING(tmpString,getIp(),buf,bytes,stringSize);
    SERIALIZE_32(tmpInt,getPort(),buf,bytes);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void CMD::ClientRequest::dump()
{
    std::cout<<"---dump of ClientRequest------"<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
    std::cout<<"getUserName(): "<<getUserName()<<endl;
    std::cout<<"getIp(): "<<getIp()<<endl;
    std::cout<<"getPort(): "<<getPort()<<endl;

}

CMD::FileData::FileData(char* buf)
{
    int offset = 0;

    SIGNED_INT tmp1 = 0, tmp2 = 0;
    STRING tmpS1,tmpS2;
    SIGNED_INT stringSize;

    DESERIALIZE_32(tmp1, tmp2, setSeqNo(tmp2), buf, offset);
    DESERIALIZE_STRING(tmpS1, setData(tmpS1), buf, offset, stringSize);
    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);

}
int CMD::FileData::
serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(FRAGEMENT_FILE_DATA), buf, bytes);
    SERIALIZE_32(tmpInt, getSeqNo(), buf, bytes);
    SERIALIZE_STRING(tmpString,getData(),buf,bytes,stringSize);
    SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void CMD::FileData::dump()
{
    std::cout<<"---dump of FileData------"<<endl;
    std::cout<<"getSeqNo(): "<<getSeqNo()<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
    std::cout<<"getData(): "<<getData()<<endl;
}
CMD::FileInfo::FileInfo(char* buf)
{
    int offset = 0;

    SIGNED_INT tmp1 = 0, tmp2 = 0;
    STRING tmpS1,tmpS2;
    SIGNED_INT stringSize;

    DESERIALIZE_32(tmp1, tmp2, setPort(tmp2), buf, offset);
    DESERIALIZE_32(tmp1, tmp2, setSeq(tmp2), buf, offset);
    DESERIALIZE_STRING(tmpS1, setIp(tmpS1), buf, offset, stringSize);
    DESERIALIZE_STRING(tmpS1, setFileName(tmpS1), buf, offset, stringSize);

}
int CMD::FileInfo::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(INFORM_AND_UPDATE), buf, bytes);
    SERIALIZE_32(tmpInt, getPort(), buf, bytes);
    SERIALIZE_32(tmpInt, getSeq(), buf, bytes);
    SERIALIZE_STRING(tmpString,getIp(),buf,bytes,stringSize);
    SERIALIZE_STRING(tmpString,getFileName(),buf,bytes,stringSize);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void CMD::FileInfo::dump()
{
    std::cout<<"---dump of FileInfo------"<<endl;
    std::cout<<"getPort(): "<<getPort()<<endl;
    std::cout<<"getFileName(): "<<getFileName()<<endl;
    std::cout<<"getIp(): "<<getIp()<<endl;
}
CMD::ClientExit::ClientExit(char* buf)
{
    int offset = 0;

    SIGNED_INT tmp1 = 0, tmp2 = 0;
    STRING tmpS1,tmpS2;
    SIGNED_INT stringSize;

    DESERIALIZE_32(tmp1, tmp2, setPort(tmp2), buf, offset);
    DESERIALIZE_STRING(tmpS1, setIp(tmpS1), buf, offset, stringSize);
}
int CMD::ClientExit::serialize(char *buf)
{
    int bytes = 0;
    UNSIGNED_CHARACTER tmpChar = 0;
    SIGNED_INT tmpInt = 0;
    STRING tmpString;
    SIGNED_INT stringSize;
    bytes = sizeof(SIGNED_INT);

    SERIALIZE_8(tmpChar, (UNSIGNED_CHARACTER)(EXIT_BY_CLIENT), buf, bytes);
    SERIALIZE_32(tmpInt, getPort(), buf, bytes);
    SERIALIZE_STRING(tmpString,getIp(),buf,bytes,stringSize);

    SIGNED_INT dummyBytes = 0;
    SERIALIZE_32(tmpInt, (SIGNED_INT)(bytes-sizeof(SIGNED_INT)), buf, dummyBytes);

    return bytes;
}
void CMD::ClientExit::dump()
{
    std::cout<<"---dump of ClientExit------"<<endl;
    std::cout<<"getPort(): "<<getPort()<<endl;
    std::cout<<"getIp(): "<<getIp()<<endl;
}
