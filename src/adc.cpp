/**
 * @file adc.cpp
 * @brief ADC采集与中值滤波实现
 */

#include "adc.h"

void ADCReader::begin() {
    // 配置ADC衰减为11dB，支持0-3.6V输入
    analogSetAttenuation(ADC_11db);
    
    // 设置引脚为输入模式
    for (int i = 0; i < 5; i++) {
        pinMode(pins[i], INPUT);
    }
}

int16_t ADCReader::read(int ch) {
    if (ch < 0 || ch >= 5) return 0;
    
    int samples[ADC_SAMPLE_COUNT];
    for (int i = 0; i < ADC_SAMPLE_COUNT; i++) {
        samples[i] = analogRead(pins[ch]);
    }
    
    // 冒泡排序取中值
    for (int i = 0; i < ADC_SAMPLE_COUNT - 1; i++) {
        for (int j = i + 1; j < ADC_SAMPLE_COUNT; j++) {
            if (samples[i] > samples[j]) {
                int tmp = samples[i];
                samples[i] = samples[j];
                samples[j] = tmp;
            }
        }
    }
    
    // 返回中间两个值的平均
    return (int16_t)((samples[ADC_SAMPLE_COUNT/2 - 1] + samples[ADC_SAMPLE_COUNT/2]) / 2);
}

void ADCReader::read_all(int16_t out[5]) {
    for (int i = 0; i < 5; i++) {
        out[i] = read(i);
    }
}
