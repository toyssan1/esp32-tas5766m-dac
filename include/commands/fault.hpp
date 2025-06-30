#pragma once

#include <string.h>
#include <tas5766m.hpp>

#include "command.hpp"
#include "argtable3/argtable3.h"

extern TAS5766m TAS5766m;

class FaultCommand : public Command
{
    static inline const char *TAG = "CMD.FAULT";

private:

    static int fault_command_handler(int argc, char **argv)
    {
        int nerrors = arg_parse(argc, argv, (void **)&fault_args);
        if (nerrors > 0)
        {
            arg_print_errors(stderr, fault_args.end, "fault");
            ESP_LOGE("CMD", "Invalid command usage");
            return 1;
        }

        if (fault_args.action->count == 0)
        {
            checkFaults();
            // TAS5766M_FAULT fault;
            // TAS5766m.getFaultState(&fault);
            // TAS5766m.decodeFaults(fault);
            return 0;
        }

        const char *action = fault_args.action->sval[0];

        if (strcmp(action, "on") == 0)
        {
            if (taskHandle == NULL)
            {
                ESP_LOGD(TAG, "Starting fault monitoring task");
                xTaskCreatePinnedToCore(
                    &taskLoop,   // Task function
                    "FaultTask",  // Task name
                    4096,        // Stack size
                    NULL,        // Task parameters (pass the instance)
                    1,           // Priority (1 is normal)
                    &taskHandle, // Task handle
                    1            // Run on Core 1
                );
            }
            else
            {
                ESP_LOGI(TAG, "Fault monitoring already enabled");
            }
        }
        else if (strcmp(action, "off") == 0)
        {
            if (taskHandle != NULL)
            {
                ESP_LOGI(TAG, "Disabling fault monitoring");
                vTaskDelete(taskHandle);
                taskHandle = NULL;
            }
            else
            {
                ESP_LOGI(TAG, "Fault monitoring already disabled");
            }
        }
        else
        {
            ESP_LOGI(TAG, "Invalid action! Use: on, off");
            return 1;
        }

        return 0;
    }

public:
    static inline TaskHandle_t taskHandle = nullptr;

    struct FaultArgs
    {
        struct arg_str *action;
        struct arg_end *end;
    };

    static inline FaultArgs fault_args = {
        arg_str0(NULL, NULL, "[action]", "Monitor fault state: on, off"),
        arg_end(1)};

    String getName()
    {
        return "fault";
    };

    esp_console_cmd_t getCommand()
    {
        return {
            .command = "fault",
            .help = "Get fault states",
            .hint = NULL,
            .func = &fault_command_handler,
            .argtable = &fault_args};
    }

    static void taskLoop(void *pvParameters)
    {
        while (1)
        {
            FaultCommand::checkFaults();
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }

    static void checkFaults()
    {
        TAS5766M_FS_FREQ freq;
        uint8_t ratio;
        TAS5766m.getFsFreq(&freq);
        TAS5766m.getBckRatio(&ratio);
        
        TAS5766M_CTRL_STATE state;
        TAS5766m.getPowerState(&state);
        
        bool is_r_muted, is_l_muted;
        TAS5766m.getAutomuteState(&is_r_muted, &is_l_muted);
        
        ESP_LOGI(TAG, "FS Frequency: %s, BCK ratio: %d; Power state: %s; Automute: R: %d, L: %d", 
            TAS5766m_map_fs_freq(freq), ratio, 
            TAS5766m_map_amp_state(state), 
            is_r_muted, is_l_muted
        );
        
        TAS5766M_FAULT fault;
        TAS5766m.getFaultState(&fault);
        TAS5766m.decodeFaults(fault);

        if (fault.err0 || fault.err1 || fault.err2 || fault.ot_warn)
        {
            ESP_LOGI(TAG, "Clearing fault states");
            TAS5766m.clearFaultState();
        }
    }
};