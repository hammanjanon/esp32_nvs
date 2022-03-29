
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "esp_log.h"

#include "nvs_flash.h"

static const char *TAG = "NVS_LOG";

nvs_handle my_handle;   

char *str_key="nvs_str";
char str_value[100]="hello nvs";
uint32_t len=100;

int8_t nvs_i8;
int16_t nvs_i16;
uint32_t nvs_u32;
uint64_t nvs_u64;

//�û�������ڣ��൱��main����
void app_main()
{
    //��ʼ��NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        //�����°汾
        //����
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    //��
    err = nvs_open("hx_list", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "opening NVS Error (%s)!\n", esp_err_to_name(err));
    } else {
		ESP_LOGI(TAG, "NVS open OK");

        //��ȡ���ݣ���ͨ��LOG���
        err = nvs_get_i8(my_handle, "nvs_i8", &nvs_i8);
        if(err==ESP_OK) ESP_LOGI(TAG, "nvs_i8 = %d\n", nvs_i8);
        err = nvs_get_i16(my_handle, "nvs_i16", &nvs_i16);
        if(err==ESP_OK) ESP_LOGI(TAG, "nvs_i16 = %d\n", nvs_i16);
        err = nvs_get_u32(my_handle, "nvs_u32", &nvs_u32);
        if(err==ESP_OK) ESP_LOGI(TAG, "nvs_u32 = %d\n", nvs_u32);
        err = nvs_get_u64(my_handle, "nvs_u64", &nvs_u64);
        if(err==ESP_OK) ESP_LOGI(TAG, "nvs_u64 = %llu\n", nvs_u64);
        //�ַ���
        err = nvs_get_str (my_handle, str_key, str_value, &len);//100�Ƕ�ȡ��󳤶�
        if(err==ESP_OK) ESP_LOGI(TAG, "nvs_str = %s\n", str_value);

        //���ݵ���1���޸��ֶε�ֵ
        nvs_i8+=1;
        nvs_i16+=1;
        nvs_u32+=1;
        nvs_u64+=1;
        err = nvs_set_i8(my_handle, "nvs_i8", nvs_i8);
        if(err!=ESP_OK) ESP_LOGE(TAG, "nvs_i8 Error");
        err = nvs_set_i16(my_handle, "nvs_i16", nvs_i16);
        if(err!=ESP_OK) ESP_LOGE(TAG, "nvs_i16 Error");
        err = nvs_set_u32(my_handle, "nvs_u32", nvs_u32);
        if(err!=ESP_OK) ESP_LOGE(TAG, "nvs_u32 Error");
        err = nvs_set_u64(my_handle, "nvs_u64", nvs_u64);
        if(err!=ESP_OK) ESP_LOGE(TAG, "nvs_u64 Error");
        //д�ַ���
        sprintf(str_value,"hello nvs %llu",nvs_u64);
        err = nvs_set_str (my_handle, str_key, str_value);

        //�ύд��NVS
        err = nvs_commit(my_handle);
        if(err!=ESP_OK) ESP_LOGE(TAG, "nvs_commit Error");

        //�ر�
        nvs_close(my_handle);
	} 
    //10�븴λ����ʱ
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    esp_restart();   //ϵͳ����
}