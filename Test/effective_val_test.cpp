//
// Created by amir on 15.09.24.
//

#include<gtest/gtest.h>
#include"../src/EffectiveValue.h"
#include <cmath>

class SineGenerator{
public:

    SineGenerator(double freq = 50.0, double fase = 0.0, int points = 80):
            freq{freq}, fase{fase}, number_of_points{points}
    {
        period = 1/freq;
        dt = 1.0/number_of_points;
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


TEST(EffectiveValue, first){

    SineGenerator sineWave{50, 0, 80};
    EffectiveValue value{80};
    float A = 750000.0;

    for(int i = 0; i < 8000; ++i){
        value.add_value(A * sineWave.get_sine_wave());
    }

    EXPECT_NEAR(value.get_effective_value(), A * M_SQRT1_2, 0.01);

}

int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
