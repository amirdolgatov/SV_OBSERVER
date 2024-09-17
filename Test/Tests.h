//
// Created by amir on 11.09.24.
//

#ifndef IEC61850_OBSERVER_TESTS_H
#define IEC61850_OBSERVER_TESTS_H

#include <iostream>
#include <cmath>


class SineGenerator{

    SineGenerator(double freq = 50.0, double fase = 0.0, int points = 80):
    freq{freq}, fase{fase}, number_of_points{points}
    {
        period = 1/freq;
        dt = period/number_of_points;


    }

    float get_sine_wave(){
        return sin( PI_2 * get_count() * dt + fase );
    }

    int get_count(){
        int ret = count;
        count = (++count) % number_of_points;
        return ret;
    }


    double freq = 1.0;
    double fase = 0.0;
    int number_of_points = 1;
    double period = 1.0;
    double dt = 0;
    uint32_t count = 0;
    double PI_2 = 2 * M_PI;

};

#endif //IEC61850_OBSERVER_TESTS_H
