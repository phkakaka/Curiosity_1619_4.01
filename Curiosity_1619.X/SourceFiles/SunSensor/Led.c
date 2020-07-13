/** @file Led.c
 *  @ingroup SunSensor
 *
 * @author Samuel Wang
 *
 * 
 */

/*-- Includes --*/
#include "../../mcc_generated_files/mcc.h"
#include "Led.h"


#define BreathLedPhease1 100//(500/SLOT_TIME)  
#define BreathLedPhease2 200//(1000/SLOT_TIME)  
#define BreathLedPhease3 100//(500/SLOT_TIME) 

#define HighLimitRatio 80 //Percent
#define HighLimitDutyValue 400//(HighLimitRatio * 4 * (PR2 + 1) / 100) 
#define TimeSlotPerRiseRatio 4//(HighLimitDutyValue/BreathLedPhease1)
#define TimeSlotPerDownRatio 2//(HighLimitDutyValue/BreathLedPhease2)

static uint16_t counter;
static uint8_t BreathState = 1;
static uint8_t TimeSlotCounter;
static uint16_t dutyValue;
static uint8_t Timer2Flag;

void Led2_PWM(void)
{
    switch (BreathState)
    {
        case 1:
            counter++;
            if(counter > BreathLedPhease1)
            {
                BreathState = 2;
            }
            else
            {
                dutyValue += TimeSlotPerRiseRatio;
            }
            break;
        case 2:
            counter++;
            if (counter >= (BreathLedPhease1 + BreathLedPhease2))
            {
                BreathState = 3;

                dutyValue = 0;
                //counter = 0;
            }
            else
            {
                if (dutyValue > 0)
                {
                    if (dutyValue > TimeSlotPerDownRatio)
                    {
                        dutyValue-=TimeSlotPerDownRatio;
                    }
                    else
                    {
                        dutyValue = 0;
                    }

                }
            }
            break;
        case 3:
            counter++;
            if (counter > (BreathLedPhease1 + BreathLedPhease2 + BreathLedPhease3))
            {
                counter = 0;
                BreathState = 1;
            }
            break;
        default:
            break;
    }

    PWM3_LoadDutyValue(dutyValue);               
    
}

void LedBreathControl(void)
{
    IO_RA2_Toggle();
}


