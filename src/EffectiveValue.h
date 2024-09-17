//
// Created by amir on 09.09.24.
//

#ifndef IEC61850_OBSERVER_EFFECTIVEVALUE_H
#define IEC61850_OBSERVER_EFFECTIVEVALUE_H

#include <cmath>


class EffectiveValue{
public:

    struct RingBuffer{

        RingBuffer(int capacity):
        capacity{capacity}{}


        void add(float val){
            buffer[increment_count()] = val;
        }


        float get_oldest_value(){
            return buffer[counter];
        }


        int increment_count(){
            int ret = counter;
            counter = (++counter) % capacity;
            return ret;
        }

        int capacity = 0;
        int counter = 0;
        float buffer[360];

    };



    EffectiveValue(int number = 80):
            point_number{number}, ringBuffer{number}
            {}


    float add_value(float val){

        float new_point = pow(val, 2);
        /// накапливаем данные
        if(count < point_number){
            count++;
            summ = summ + new_point;
            current_value = std::sqrt(summ / count);
        }
        else{
            summ = summ - ringBuffer.get_oldest_value() + new_point;
            current_value = std::sqrt(summ / point_number);
        }

        ringBuffer.add(new_point);
        return current_value;

    }

    float get_effective_value(){
        return current_value;
    }

    RingBuffer ringBuffer;
    int point_number;
    float current_value = 0;
    float multiplier = 1;

private:

    int count = 0;
    float summ = 0;
    float previous = 0;

};

#endif //IEC61850_OBSERVER_EFFECTIVEVALUE_H
