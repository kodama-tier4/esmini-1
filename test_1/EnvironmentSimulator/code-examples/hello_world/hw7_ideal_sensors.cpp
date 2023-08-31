#include "stdio.h"
#include "esminiLib.hpp"

#define MAX_HITS 10

int main(int, char**)
{
    SE_Init("../resources/xosc/cut-in.xosc", 0, 1, 0, 0);

    SE_AddObjectSensor(0, 2.0f, 1.0f, 0.5f, 1.57f, 1.0f, 50.0f, 1.57f, MAX_HITS);
    SE_AddObjectSensor(0, -1.0f, 0.0f, 0.5f, 3.14f, 0.5f, 20.0f, 1.57f, MAX_HITS);

    // Turn on visualization of object sensors, toggle key 'r'
    SE_ViewerShowFeature(1, true);

    for (int i = 0; i < 2000 && !(SE_GetQuitFlag() == 1); i++)
    {
        SE_Step();

        int objList[MAX_HITS];
        for (int j = 0; j < 2; j++)  // iterate over added sensors
        {
            int nHits = SE_FetchSensorObjectList(j, objList);
            for (int k = 0; k < nHits; k++)
            {
                printf("Sensor[%d] detected obj: %d\n", j, objList[k]);

                // Get some info of that detected object
                SE_ScenarioObjectState state;

                SE_GetObjectState(objList[k], &state);
                printf("object[%d] pos: (%.2f, %.2f) heading: %.2f\n",
                       objList[k],
                       static_cast<double>(state.x),
                       static_cast<double>(state.y),
                       static_cast<double>(state.h));
            }
        }
    }

    SE_Close();

    return 0;
}
