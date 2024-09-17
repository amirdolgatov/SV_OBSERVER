//
// Created by amir on 31.08.24.
//

#ifndef IEC61850_OBSERVER_SVPARSER_H
#define IEC61850_OBSERVER_SVPARSER_H

#include <stdint.h>
#include <byteswap.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <string>
#include <vector>
#include <iostream>


#define COMPOSITE 0x20

struct TLV{
    uint8_t tag;
    uint8_t length;
    uint8_t value;
};

enum struct PDU{
    savPDU = 0x60,
    seqASDU = 0xA2,
    ASDU = 0x30,
    noASDU = 0x80
};

enum struct ASDU_TAG{
    svID = 0x80,
    datSet = 0x81,
    smpCnt = 0x82,
    confRev = 0x83,
    refrTm = 0x84,
    smpSynch = 0x85,
    smpRate = 0x86,
    seq_of_Data = 0x87
};

#pragma pack(push)
#pragma pack(0)
struct Seq_of_Data{
    int32_t Ia = 0;
    int32_t Ia_q = 0;
    int32_t Ib = 0;
    int32_t Ib_q = 0;
    int32_t Ic = 0;
    int32_t Ic_q = 0;
    int32_t In = 0;
    int32_t In_q = 0;

    int32_t Ua = 0;
    int32_t Ua_q = 0;
    int32_t Ub = 0;
    int32_t Ub_q = 0;
    int32_t Uc = 0;
    int32_t Uc_q = 0;
    int32_t Un = 0;
    int32_t Un_q = 0;
};
#pragma pack(pop)


struct SV_HDR{
    uint16_t app_id;
    uint16_t length;
    uint16_t reserved1;
    uint16_t reserved2;
};

struct ASDU{
    std::string svID;
    std::string datSet;
    uint16_t smpCnt{0};
    uint32_t confRev{0};
    uint64_t refrTm{0};
    uint8_t smpSynch{0};
    uint16_t smpRate{0};
    Seq_of_Data seq_of_Data;

    int add_attribute(uint8_t tag, uint8_t length, uint8_t *value){
        switch((ASDU_TAG)tag){
            case ASDU_TAG::svID:
            {
                for (int i = 0; i < length; ++i)
                    svID.push_back(value[i]);
                return 0;
            };
            
            case ASDU_TAG::datSet:
            {
                for (int i = 0; i < length; ++i)
                    datSet.push_back(value[i]);
                return 0;
            };
            
            case ASDU_TAG::smpCnt:
            {

                uint16_t *data = (uint16_t *)value;
                smpCnt = bswap_16(*data);
                return 0;
            };
            
            case ASDU_TAG::confRev:
            {
                uint32_t *data = (uint32_t *)value;
                confRev = bswap_32(*data);
                return 0;
            };
            
            case ASDU_TAG::refrTm:
            {
                uint64_t *data = (uint64_t *)value;
                refrTm = bswap_64(*data);
                return 0;
            };

            case ASDU_TAG::smpSynch:
            {
                uint8_t *data = (uint8_t *)value;
                smpSynch = *data;
                return 0;
            };

            case ASDU_TAG::smpRate:
            {
                uint16_t *data = (uint16_t *)value;
                smpRate = bswap_16(*data);
                return 0;
            };

            case ASDU_TAG::seq_of_Data:
            {
                Seq_of_Data *data = (Seq_of_Data *)value;
                seq_of_Data = *data;
                return 0;
            };

            default:
                return -1;
        }
    }
};



struct SV_PACKET{
    ether_header eth_hdr;
    SV_HDR sv_hdr;
    uint32_t noASDU;
    std::vector<ASDU> seqASDU;

    void print(){

        printf("DST MAC: ");
        for (int i = 0; i < ETH_ALEN; ++i) {
            printf("%.2x ", eth_hdr.ether_dhost[i]);
        }
        printf("\n");

        printf("SRC MAC: ");
        for (int i = 0; i < ETH_ALEN; ++i) {
            printf("%.2x ", eth_hdr.ether_shost[i]);
        }
        printf("\n");

        std::cout << "APPID \t" << sv_hdr.app_id << std::endl;
        std::cout << "LENGTH \t" << sv_hdr.length << std::endl;
        std::cout << "noASDU \t" << noASDU << std::endl;

        for(auto& asdu: seqASDU){
            std::cout << "svID\t\t\t\t" << asdu.svID << std::endl;
            std::cout << "datSet\t\t\t\t" << asdu.datSet << std::endl;
            std::cout << "smpCnt\t\t\t\t" << asdu.smpCnt << std::endl;
            std::cout << "confRev\t\t\t\t" << asdu.confRev << std::endl;
            std::cout << "refrTm\t\t\t\t" << asdu.refrTm << std::endl;
            std::cout << "smpSynch\t\t\t" << (int)asdu.smpSynch << std::endl;
            std::cout << "smpRate\t\t\t\t" << asdu.smpRate << std::endl;

            std::cout << "Ia \t\t" << (int)bswap_32(asdu.seq_of_Data.Ia) << std::endl;
            std::cout << "Ib \t\t" << (int)bswap_32(asdu.seq_of_Data.Ib) << std::endl;
            std::cout << "Ic \t\t" << (int)bswap_32(asdu.seq_of_Data.Ic) << std::endl;
            std::cout << "Ua \t\t" << (int)bswap_32(asdu.seq_of_Data.Ua) << std::endl;
            std::cout << "Ub \t\t" << (int)bswap_32(asdu.seq_of_Data.Ub) << std::endl;
            std::cout << "Uc \t\t" << (int)bswap_32(asdu.seq_of_Data.Uc) << std::endl;
        }
    }
};

#pragma pack(push)
#pragma pack(0)
struct vlan_ethhdr {
    u_int8_t  ether_dhost[ETH_ALEN];  /* destination eth addr */
    u_int8_t  ether_shost[ETH_ALEN];  /* source ether addr    */
    u_int16_t          h_vlan_proto;
    u_int16_t          h_vlan_TCI;
    u_int16_t ether_type;
};
#pragma pack(pop)



class SvParser{

public:

    uint32_t get_length_BER(uint8_t *begin, uint32_t bytes){
        uint32_t length = 0;
        uint8_t *ptr = (uint8_t*) &length;
        if(bytes > 4)
            bytes = 4;

        for (int i = 0; i < bytes; ++i) {
            ptr[bytes - i - 1] = begin[i];
        }

        return length;
    }



    int parse(uint8_t *frame, SV_PACKET& packet){

        struct ether_header *eh = (struct ether_header *) frame;
        uint16_t type = bswap_16(eh->ether_type);

        uint32_t offset = 14;

        if( type != 0x88ba){            // is it Sampled Valued frame&
            if( type != 0x8100)         // may be it is VLAN tag?
                return -1;
            else {
                struct vlan_ethhdr *vlan_eth_hdr = (struct vlan_ethhdr *) frame;
                uint16_t ether_type = bswap_16(vlan_eth_hdr->ether_type);
                offset = 18;
                if(ether_type != 0x88ba)
                    return -1;
            }
        }

        packet.eth_hdr = *eh;
        return parse_savPdu(&frame[offset], packet);

    }

    int parse_savPdu(const uint8_t *pdu, SV_PACKET& packet){

        SV_HDR *hdr = (SV_HDR *) pdu;

        packet.sv_hdr.app_id = bswap_16(hdr->app_id);
        packet.sv_hdr.length = bswap_16(hdr->length);

        uint32_t offset = sizeof(SV_HDR);
        // start to parse PDU
        PDU tag = PDU::savPDU;
        TLV *attr_ptr = (TLV*) &pdu[offset];

        // fill staruct PDU
        while((PDU)attr_ptr->tag != PDU::ASDU){

            if(attr_ptr->length <= 0x80){
                if( COMPOSITE & attr_ptr->tag)
                    offset = offset + 2;
                else
                    offset = offset + 2 + attr_ptr->length;
            }
            else{
                /// добавить получение длины через новую функцию
                if( COMPOSITE & attr_ptr->tag){
                    offset = offset + attr_ptr->length - 0x80 + 2;
                }

                else{
                    auto len = get_length_BER(&(attr_ptr->value), attr_ptr->length - 0x80);
                    offset = offset + attr_ptr->length - 0x80 + 2 + len;
                }
            }

            if((PDU)attr_ptr->tag == PDU::noASDU)
                packet.noASDU = attr_ptr->value;

            attr_ptr = (TLV*) &pdu[offset];
        }

        if( parse_asdu(&pdu[offset], packet.noASDU, packet) + offset == packet.sv_hdr.length)
            return 0;

        return -1;

    }



    int parse_asdu(const uint8_t *ptr, uint32_t noASDU, SV_PACKET& packet){
        TLV *attr_ptr;
        uint32_t asduLen = 0;
        uint32_t full_asdu_length = 0;

        for (int i = 0; i < noASDU; ++i) {
            ASDU asdu;
            uint32_t offset = 0;
            attr_ptr = (TLV*) &ptr[offset];

            if(attr_ptr->tag != (uint8_t)PDU::ASDU)
                return -1;

            if(attr_ptr->length <= 0x80)
                offset = offset + 2;
            else
                offset = offset + attr_ptr->length - 0x80 + 1;

            asduLen = attr_ptr->length + 2;

            // уменьшаем asduLen пока не станет равным нулю
            while(asduLen > offset){
                attr_ptr = (TLV*) &ptr[offset];
                asdu.add_attribute(attr_ptr->tag, attr_ptr->length, &attr_ptr->value);
                offset = offset + attr_ptr->length + 2;
            }

            full_asdu_length += offset;
            packet.seqASDU.emplace_back(asdu);

        }

        return full_asdu_length;
    }

    /// map {tag, value},

};

#endif //IEC61850_OBSERVER_SVPARSER_H
