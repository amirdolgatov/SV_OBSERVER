//
// Created by amir on 09.09.24.
//

#ifndef IEC61850_OBSERVER_SVFLOW_H
#define IEC61850_OBSERVER_SVFLOW_H

#include "SvParser.h"

struct СalculatedValues{
    float Ia = 0;
    float Ib = 0;
    float Ic = 0;
    float In = 0;

    float Ua = 0;
    float Ub = 0;
    float Uc = 0;
    float Un = 0;
};

class SvFlow{
public:

    std::string dst_mac;
    std::string sv_ID;

    Seq_of_Data latest_values;
    СalculatedValues effective_values;



    std::string get_flow_id(){
        return this->dst_mac + this->sv_ID;
    }

    void update_values(Seq_of_Data values){
        latest_values = values;
    }

};

#endif //IEC61850_OBSERVER_SVFLOW_H
