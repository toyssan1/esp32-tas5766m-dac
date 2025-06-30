#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <Wire.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include "esp_log.h"

#include "tas5766m.h"
    class TAS5766m
    {
    private:
        TwoWire *wire;

    public:
        TAS5766m(TwoWire *wire) : 
            wire(wire) {};

        esp_err_t deinit() {
            return TAS5766m_deinit();
        }

        esp_err_t init() {
            return TAS5766m_init();
        };

        esp_err_t getVolume(uint8_t *vol) {
            return TAS5766m_get_volume(vol);
        };

        esp_err_t setVolume(uint8_t vol) {
            return TAS5766m_set_volume(vol);
        };

        esp_err_t getVolume100(uint8_t *vol) {
            return TAS5766m_get_volume_pct(vol);
        };

        esp_err_t setVolume100(uint8_t vol) {
            return TAS5766m_set_volume_pct(vol);
        };

        esp_err_t getState(TAS5766M_CTRL_STATE *state) {
            return TAS5766m_get_state(state);
        };

        esp_err_t setState(TAS5766M_CTRL_STATE state) {
            return TAS5766m_set_state(state);
        };

        esp_err_t getDacMode(TAS5766M_DAC_MODE *mode) {
            return TAS5766m_get_dac_mode(mode);
        };

        esp_err_t setDacMode(TAS5766M_DAC_MODE mode) {
            return TAS5766m_set_dac_mode(mode);
        };

        esp_err_t getEqEnabled(bool *enabled) {
            return TAS5766m_get_eq(enabled);
        };

        esp_err_t setEqEnabled(bool enabled) {
            return TAS5766m_set_eq(enabled);
        };

        esp_err_t getEqGain(int band, int *gain) {
            return TAS5766m_get_eq_gain(band, gain);
        };

        esp_err_t setEqGain(int band, int gain) {
            return TAS5766m_set_eq_gain(band, gain);
        };

        esp_err_t getModulationMode(TAS5766M_MOD_MODE *mode, TAS5766M_SW_FREQ *freq, TAS5766M_BD_FREQ *bd_freq) {
            return TAS5766m_get_modulation_mode(mode, freq, bd_freq);
        };

        esp_err_t setModulationMode(TAS5766M_MOD_MODE mode, TAS5766M_SW_FREQ freq, TAS5766M_BD_FREQ bd_freq) {
            return TAS5766m_set_modulation_mode(mode, freq, bd_freq);
        };

        esp_err_t getAnalogGain(uint8_t *gain) {
            return TAS5766m_get_again(gain);
        };

        esp_err_t setAnalogGain(uint8_t gain) {
            return TAS5766m_set_again(gain);
        };

        esp_err_t getMixerMode(TAS5766M_MIXER_MODE *mode) {
            return TAS5766m_get_mixer_mode(mode);
        };

        esp_err_t setMixerMode(TAS5766M_MIXER_MODE mode) {
            return TAS5766m_set_mixer_mode(mode);
        };

        esp_err_t getFsFreq(TAS5766M_FS_FREQ *freq) {
            return TAS5766m_get_fs_freq(freq);
        };

        esp_err_t getBckRatio(uint8_t *ratio) {
            return TAS5766m_get_bck_ratio(ratio);
        };

        esp_err_t getPowerState(TAS5766M_CTRL_STATE *state) {
            return TAS5766m_get_power_state(state);
        };

        esp_err_t getAutomuteState(bool *is_r_muted, bool *is_l_muted) {
            return TAS5766m_get_automute_state(is_r_muted, is_l_muted);
        };

        esp_err_t getFaultState(TAS5766M_FAULT* fault) {
            return TAS5766m_get_faults(fault);
        };

        esp_err_t clearFaultState() {
            return TAS5766m_clear_faults();
        };

        void decodeFaults(TAS5766M_FAULT fault){
            TAS5766m_decode_faults(fault);
        }
    };