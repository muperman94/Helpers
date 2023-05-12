#include <gtest/gtest.h>
#include "StateMashine.hpp"

#define GTEST_COUT std::cerr << "[   INFO   ] "

enum class INIT{
    NOT_INITITIALIZED,
    GOTO_INIT_SWITCH,
    GOTO_END_SWITCH,
    INITIALIZED,
};

typedef struct SomeData{
    double test;
}SomeData;

void AddOneToData(StateMashine<INIT, SomeData>* stateMashine){
    stateMashine->getUserData()->test++;
}

void SubtractOneFromData(StateMashine<INIT, SomeData>* stateMashine){
    stateMashine->getUserData()->test--;
}

void CheckForInitSwitch(StateMashine<INIT, SomeData>* stateMashine){
    static unsigned int delay = 0;
    if(10 < delay++){
        stateMashine->SetNextState(INIT::GOTO_END_SWITCH);
        delay = 0;
    }    
}

void CheckForEndSwitch(StateMashine<INIT, SomeData>* stateMashine){
    static unsigned int delay = 0;
    if(10 < delay++){
        stateMashine->SetNextState(INIT::INITIALIZED);
        delay = 0;
    }
}

TEST(StateMashine, Transition){
    SomeData userData;
    StateMashine<INIT, SomeData> stateMashine(INIT::NOT_INITITIALIZED, &userData);

    stateMashine.AddCyclicFunction(INIT::GOTO_INIT_SWITCH, CheckForInitSwitch);
    stateMashine.AddCyclicFunction(INIT::GOTO_END_SWITCH, CheckForEndSwitch);

    stateMashine.AddTransitionFunction(INIT::GOTO_INIT_SWITCH, INIT::GOTO_END_SWITCH, AddOneToData);
    stateMashine.AddTransitionFunction(INIT::GOTO_END_SWITCH, INIT::INITIALIZED, SubtractOneFromData);

    stateMashine.SetNextState(INIT::GOTO_INIT_SWITCH);
    while(stateMashine.GetCurrentState() != INIT::GOTO_END_SWITCH){
        stateMashine.HandleStates();
    }

    ASSERT_EQ(1, userData.test);

    while(stateMashine.GetCurrentState() != INIT::INITIALIZED){
        stateMashine.HandleStates();
    }

    ASSERT_EQ(0, userData.test);
}

//Runs all tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    //testing::GTEST_FLAG(filter) = "*Orito**";
    return RUN_ALL_TESTS();
}