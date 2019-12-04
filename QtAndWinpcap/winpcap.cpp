
#pragma warning(disable : 4996)
#include <winsock2.h>
#include "pcap.h"
#include "winpcap.h"
#ifdef inline
#undef inline
#endif
#include<QDebug>
#include<stdio.h>
#include<iostream>
#include<vector>
#include<string.h>
#include<string>
#define MAXLEN 65536
#define MAXNUM 100000

using namespace std;



Winpcap::Winpcap(){
    this->isRunning = false;
}

strVec Winpcap::getDeviceList(){
    qDebug()<<deviceList.size();
    return this->deviceList;
}

void Winpcap::Start(QString devName){
    pcap_t *adhandle;
    char errbuf[PCAP_ERRBUF_SIZE];
    if ((adhandle = pcap_open(devName.toStdString().c_str(), MAXLEN, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf )) == NULL){
        emit sendMsg("error@Unable to open the adapter: "+ devName);
        return;
    }else{
        emit sendMsg("success@Open device success!");
    }

   struct pcap_pkthdr *header;
   const u_char *pkt_data;
   u_int ip_len;
   int res;
   while(( res = pcap_next_ex( adhandle, &header, &pkt_data)) >= 0){
      if(res == 0) {
          qDebug()<<"time over!";
          continue;
      }
      ip_header *ih = (ip_header *)(pkt_data + 14);
      ip_len = (ih->ver_ihl & 0xf) * 4;
      udp_header *uh = (udp_header *)((u_char*)ih + ip_len);

      char sid[50];
      sprintf(sid, "%d.%d.%d.%d:%d", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4, ntohs(uh->sport));
      char did[50];
      sprintf(did, "%d.%d.%d.%d:%d",  ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4, ntohs(uh->dport));

      QString idnti = sid;
      idnti += did;
      if(rows_set.find(idnti)==rows_set.end()) continue;
      rows_set.insert(idnti);

      idnti += "@tmpUrl";
      rows_vec.push_back(idnti);

      qDebug()<<idnti;

      packCount++;
      if (packCount > MAXNUM) break;
      if ( !isRunning ) break;
   }
    return;
}

bool Winpcap::init(){
    if(!getDevNameList()){
        emit sendMsg("error@Can't not get device list!");
        return false;
    }
    emit sendMsg("success@Get Devices list success!");
    return true;
}

void Winpcap::Stop(){
    isRunning = false;
}


//get all device in localhost and write all device name into vec
bool Winpcap::getDevNameList(){
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
    //get all device
    if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1){
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return false;
    }
    int count = 0;
    //write all device name into vec
    for (d = alldevs; d; d = d->next){
        count++;
        deviceList.push_back(d->name);
    }
    pcap_freealldevs(alldevs);
    return count!=0;
}


