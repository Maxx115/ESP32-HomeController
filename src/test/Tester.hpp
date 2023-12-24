#ifndef TEST_TESTER
#define TEST_TESTER

#include <iostream>
#include <typeinfo>

#include "self_arduino.hpp"

enum status
{
    NOT_INIT = -1,
    INIT = 0,
    STARTED,
    ENDED
};

class Tester
{
    private:
    String test_name;
    String test_description;
    status test_status = NOT_INIT;
    unsigned long startTime;
    unsigned long stopTime;
    unsigned int assert_calls = 0;

    void printTime(void)
    {
        Serial.print("Elapsed time: ");
        Serial.print((stopTime-startTime));
        Serial.print(" ms | time per assert: ");
        if(assert_calls > 0)
        {
            Serial.print((stopTime-startTime)/assert_calls);
            Serial.println(" ms");
        }
    }

    public:
    Tester(String name, String description)
    {
        test_name = name;
        test_description = description;
        test_status = INIT;
    }

    void begin()
    {
        Serial.println("\nStarting " + test_name);
        Serial.println("######################");
        test_status = STARTED;
        startTime = millis();
    }
    void end()
    {
        stopTime = millis();
        Serial.println("######################");
        Serial.println("Ending " + test_name);
        printTime();
        test_status = ENDED;
    }

    template <typename T, typename U>
    bool tester_assert(T statement_1, U statement_2)
    {
        assert_calls += 1;
        String result = "";
        bool ret = false;

        if(statement_1 == statement_2)
        {
            result = "SUCCESS ✔";
            ret = true;
        }
        else
        {
            result = "FAILURE ❌";
            result += "\nstatement_1 = " + String(statement_1);
            result += "\nstatement_2 = " + String(statement_2);
            ret = false;
        }

        Serial.println(test_name + " finished with: " + result);
        return ret;
    }

    String getTestName()
    {
        return test_name;
    }
    String getTestDescription()
    {
        return test_description;
    }

};

#endif /* TEST_TESTER */