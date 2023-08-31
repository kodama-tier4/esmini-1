/*
  This example shows how to add and control traffic via API starting
  from an empty scenario just defining the OpenDRIVE (road network)
*/

#include "esminiLib.hpp"
#include "stdio.h"
#include <string>
#include <vector>

typedef struct
{
    int    id;
    double x;
} Car;

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    SE_AddPath("../resources");

    if (SE_Init("../EnvironmentSimulator/code-examples/ad_hoc_traffic/empty_scenario.xosc", 1, 1, 0, 0) != 0)
    {
        SE_LogMessage("Failed to initialize the scenario, quit\n");
        return -1;
    }

    std::vector<Car> cars;
    const double     speed    = 100.0 / 3.6;
    const double     distance = 40.0;

    int    counter       = 0;
    double timestamp_now = SE_GetSimulationTime();
    double timestamp_old = timestamp_now;

    while (timestamp_now < 30.0 && !(SE_GetQuitFlag() == 1))
    {
        if (timestamp_now > (distance * static_cast<double>(counter)) / speed)
        {
            // Add a vehicle at regular distance
            std::string name = "object_" + std::to_string(counter);
            Car         car  = {0, 0.0f};

            car.id = SE_AddObject(name.c_str(), 1, 0, 0, counter % 11);
            if (car.id >= 0)
            {
                cars.push_back(car);
            }
            else
            {
                printf("Failed to add car #%d\n", counter);
            }
            counter++;
        }

        for (unsigned int i = 0; i < cars.size(); i++)
        {
            cars[i].x += speed * (timestamp_now - timestamp_old);
            if (cars[i].x > 500)
            {
                printf("Removing car with id %d\n", cars[i].id);
                SE_DeleteObject(cars[i].id);
                cars.erase(cars.begin() + i);
                i--;
            }
            else
            {
                SE_ReportObjectPos(cars[i].id, 0.0f, static_cast<float>(cars[i].x), -1.5f, 0.0f, 0.0f, 0.0f, 0.0f);
                SE_ReportObjectSpeed(cars[i].id, static_cast<float>(speed));
            }
        }

        SE_Step();
        timestamp_old = timestamp_now;
        timestamp_now = SE_GetSimulationTime();
        if (counter == 4)
        {
            SE_SetCameraObjectFocus(3);
        }
    }

    SE_Close();

    return 0;
}