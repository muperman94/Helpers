#pragma once

template<typename Type>
class Hysteresis{
public:
    /**
     * @brief Construct a new Hysteresis object
     * 
     * @param sameDirectionLimit Limit for changes in the same direction as the last accepted change
     * @param otherDirectionLimit Limit for changes in the opposite direction as the lsat accapted change
     * @param startValue Value to start with
     */
    Hysteresis(const Type& sameDirectionLimit, const Type& otherDirectionLimit, const Type& startValue = 0);

    /**
     * @brief Construct a new Hysteresis object
     * 
     * @param bothDirectionsLimit Changes must be greater than this limit to acutally change the value (same for both directions)
     * @param startValue Value to start with
     */
    Hysteresis(const Type& bothDirectionsLimit, const Type& startValue = 0);
    Hysteresis() = delete;

    virtual ~Hysteresis();

    /**
     * @brief Calculates the new value based on the latest value and the hysterese limits
     * 
     * @param newValue Compared to the current value:
     * @return Type if(newValue breaks hysterese limits)
     *                  return newValue
     *              else
     *                  return oldValue
     */
    Type hysteresis(const Type& newValue);

    void setSameDirectionLimit(const Type& newSameDirectionLimit);
    void setOtherDirectionLimit(const Type& newOtherDirectionLimit);
    Type getSameDirectionLimit();
    Type getOtherDirectionLimit();

protected:
    Type m_sameDirectionLimit;
    Type m_otherDirectionLimit;
private:
    Type m_upperLimit;
    Type m_lowerLimit;
    Type m_lastValidValue;
};

template<typename Type>
Hysteresis<Type>::Hysteresis(const Type& sameDirectionLimit, const Type& otherDirectionLimit, const Type& startValue)
 : m_lastValidValue(startValue)
 , m_sameDirectionLimit(sameDirectionLimit)
 , m_otherDirectionLimit(otherDirectionLimit)
 , m_upperLimit(startValue + m_sameDirectionLimit)
 , m_lowerLimit(startValue - m_otherDirectionLimit){

}

template<typename Type>
Hysteresis<Type>::Hysteresis(const Type& bothDirectionsLimit, const Type& startValue)
 : m_lastValidValue(startValue)
 , m_sameDirectionLimit(bothDirectionsLimit)
 , m_otherDirectionLimit(bothDirectionsLimit)
 , m_upperLimit(startValue + m_sameDirectionLimit)
 , m_lowerLimit(startValue - m_otherDirectionLimit){
    
}

template<typename Type>
Hysteresis<Type>::~Hysteresis(){
    
}

template<typename Type>
void Hysteresis<Type>::setOtherDirectionLimit(const Type& newOtherDirectionLimit){
    m_otherDirectionLimit = newOtherDirectionLimit;
}

template<typename Type>
Type Hysteresis<Type>::getOtherDirectionLimit(void){
    return m_otherDirectionLimit;
}

template<typename Type>
void Hysteresis<Type>::setSameDirectionLimit(const Type& newSameDirectionLimit){
    m_sameDirectionLimit = newSameDirectionLimit;
}

template<typename Type>
Type Hysteresis<Type>::getSameDirectionLimit(void){
    return m_sameDirectionLimit;
}

template<typename Type>
Type Hysteresis<Type>::hysteresis(const Type& newValue){
    if(newValue > m_upperLimit){
        m_lastValidValue = newValue;
        m_upperLimit = newValue + m_sameDirectionLimit;
        m_lowerLimit = newValue - m_otherDirectionLimit;
    } else if (newValue < m_lowerLimit){
        m_lastValidValue = newValue;
        m_upperLimit = newValue + m_otherDirectionLimit;
        m_lowerLimit = newValue - m_sameDirectionLimit;
    }
    return m_lastValidValue;
}

