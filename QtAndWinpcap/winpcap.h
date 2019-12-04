#ifndef WINPCAP_H
#define WINPCAP_H

#pragma once

#pragma warning(disable : 4996)
#include <winsock2.h>
#include "pcap.h"
#ifdef inline
#undef inline
#endif

#include<iostream>
#include<vector>
#include<set>
#include <QMainWindow>

#include <QObject>
using namespace std;

typedef vector<QString> strVec;


struct row{
    QString sid;
    QString did;
    QString url;
};

struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
};

struct ip_header{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service)
    u_short tlen;           // 总长(Total length)
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
};

struct udp_header{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
};


class Winpcap : public QWidget{
    Q_OBJECT
    set<QString> rows_set;
    strVec deviceList;
    QString Usingdevice;

public:
    bool isRunning;
    int packCount = 0;  //total package find by winpcap
    strVec rows_vec;  //container of packages message

private:    //tools functions
     bool getDevNameList();

public:
    Winpcap();
    strVec getDeviceList();
    bool init();
    void Start(QString);
    void Stop();

signals:
    void sendMsg(QString sig);
};

#endif // WINPCAP_H
