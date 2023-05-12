#pragma once

#include <map>
#include <functional>

/**
 * @brief This class provides a generic approach to setup a state mashine.
 * 
 * You can pass functions that are called while a certain state is active.
 * You can pass functions that are called once a certain state transition happens.
 * 
 * @tparam StateEnum The underlying enum-type, that defines the different states
 * @tparam Data The type of data you want to access in any passed function
 */
template<typename StateEnum, typename Data>
class StateMashine{
public:
    /**
     * @brief Construct a new State Mashine object
     * 
     * @param firstState The state in which the stateMashine will start
     * @param userData Address of data you want to access in your passed functions
     */
    StateMashine(StateEnum firstState, Data* userData);

    StateMashine() = delete;

    /**
     * @brief The handler will call the given function while it is in the given state
     * 
     * @param state State the function belongs to
     * @param func Function to be called, when stateMashine is in upper state
     */
    void AddCyclicFunction(StateEnum state, std::function<void(StateMashine<StateEnum, Data>*)> func);

    /**
     * @brief The handler will call the given function when the state changes in the given way
     * 
     * @param oldState Transition from this state
     * @param newState Transition to this state
     * @param func If state changes from oldState to newState this function will be called once
     */
    void AddTransitionFunction(StateEnum oldState, StateEnum newState, std::function<void(StateMashine<StateEnum, Data>*)> func);

    /**
     * @brief Set the Next State object
     * 
     * @param newState In the next call to HandleStates() we change to this state.
     */
    void SetNextState(StateEnum newState);

    /**
     * @brief Handles stateChanges and calls the set functions.
     * 
     * If a state change is pending
     *   Calls the transition function for this transition once.
     * 
     * Calls the cyclic function for the active state
     *   If a state change was pending, only the new state function is called.     * 
     */
    void HandleStates(void);

    /**
     * @brief Get the Current State object
     * 
     * @return StateEnum 
     */
    StateEnum GetCurrentState(void);

    /**
     * @brief Get the User Data object
     * 
     * @return Data* The address to the data given in the constructor
     */
    Data* getUserData(void);

private:
    StateEnum m_currentState;
    StateEnum m_nextState;
    Data* m_userData;
    std::map<StateEnum, std::function<void(StateMashine<StateEnum, Data>*)>> m_cyclicStateFunctions;
    std::map<StateEnum, std::map<StateEnum, std::function<void(StateMashine<StateEnum, Data>*)>>> m_transitionFunctions;
};

template<typename StateEnum, typename Data>
StateMashine<StateEnum, Data>::StateMashine(StateEnum firstState, Data* userData)
 : m_currentState(firstState)
 , m_nextState(firstState)
 , m_userData(userData){

}

template<typename StateEnum, typename Data>
void StateMashine<StateEnum, Data>::AddCyclicFunction(StateEnum state, std::function<void(StateMashine<StateEnum, Data>*)> func){
    m_cyclicStateFunctions[state] = func;
}

template<typename StateEnum, typename Data>
void StateMashine<StateEnum, Data>::AddTransitionFunction(StateEnum oldState, StateEnum newState, std::function<void(StateMashine<StateEnum, Data>*)> func){
    m_transitionFunctions[oldState][newState] = func;
}

template<typename StateEnum, typename Data>
void StateMashine<StateEnum, Data>::SetNextState(StateEnum newState){
    m_nextState = newState;
}

template<typename StateEnum, typename Data>
void StateMashine<StateEnum, Data>::HandleStates(void){
    //Check if a state change happened
    if(m_currentState != m_nextState){
        auto it1 = m_transitionFunctions.find(m_currentState);
        if (it1 != m_transitionFunctions.end()){
            auto it2 = it1->second.find(m_nextState);
            if (it2 != it1->second.end()){
                //call the registered function for that transition
                it2->second(this);
            }
        }
    }
    m_currentState = m_nextState;
    auto it3 = m_cyclicStateFunctions.find(m_nextState);
    if (it3 != m_cyclicStateFunctions.end()){
        //call the registered function for that state
        it3->second(this);
    }
}

template<typename StateEnum, typename Data>
StateEnum StateMashine<StateEnum, Data>::GetCurrentState(void){
    return m_currentState;
}

template<typename StateEnum, typename Data>
Data* StateMashine<StateEnum, Data>::getUserData(void){
    return m_userData;
}




