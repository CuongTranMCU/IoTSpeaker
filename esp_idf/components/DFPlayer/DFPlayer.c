/******************************************************************************************************************
@File:  	DFPlayer Mini Module
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao UART Baud 9600
- Khoi tao bien DFPlayer : DFPLAYER_Name MP3;
- Khoi tao DFPlayer do:
	DFPLAYER_Init(&MP3, &huart1);
- Su dung cac ham phai truyen dia chi cua DFPlayer do: 
	DFPLAYER_Play(&MP3);
******************************************************************************************************************/
#include "DFPLAYER.h"
static const char *TAG = "DFPLAYER";

uint8_t SendFrame[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};

//******************************** LOW Level Functions ****************************//
static void DFPLAYER_SendUART(DFPLAYER_Name* MP3)
{
	uart_write_bytes(MP3->DFP_UART, (uint8_t *)&MP3->SendBuff, sizeof(MP3->SendBuff));
	ESP_LOGI("ESP32","Send data");
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	for (int i = 0; i < sizeof(MP3->SendBuff); i++) {
    printf("%02X ", MP3->SendBuff[i]);
}
	printf("\n");
}
static void DFPLAYER_Delay(uint16_t Time)
{
	vTaskDelay(Time);
}

static uint16_t DFPLAYER_GetChecksum (uint8_t *thebuf)
{
	uint16_t sum = 0;
	for (int i=1; i<7; i++) 
	{
		sum += thebuf[i];
	}
	return -sum;
}

static void DFPLAYER_FillBigend (uint8_t *thebuf, uint16_t data) {
	*thebuf =	(uint8_t)(data>>8);
	*(thebuf+1) =	(uint8_t)data;
}

static void DFPLAYER_FillChecksum (DFPLAYER_Name* MP3){
	uint16_t checksum = DFPLAYER_GetChecksum (MP3->SendBuff);
	DFPLAYER_FillBigend (MP3->SendBuff+7, checksum);
}

static void DFPLAYER_SendCmd(DFPLAYER_Name* MP3, uint8_t cmd, uint16_t high_arg, uint16_t low_arg)
{
	DFPLAYER_Delay(100);
	MP3->SendBuff[3] = cmd;
	MP3->SendBuff[5] = high_arg;
	MP3->SendBuff[6] = low_arg;
	DFPLAYER_FillChecksum(MP3);
	DFPLAYER_SendUART(MP3);
}

//******************************** High Level Functions ****************************//

void DFPLAYER_Init(DFPLAYER_Name* MP3, uart_port_t * UART)
{
	DFControl.finished= false;
	DFControl.play = false;
	DFControl.songId = 0;
	DFControl.volume = 0;
	for(int i = 0; i<10; i++)
	{
		MP3->SendBuff[i] = SendFrame[i];
	}
	MP3->DFP_UART = UART;

    
}

void DFPLAYER_PlayTrack(DFPLAYER_Name* MP3, uint16_t num)
{
	uint8_t num1 = num>>8;
	uint8_t num2 = num;
	DFPLAYER_SendCmd(MP3, DFP_PLAYTRACK, num1, num2);
}

void DFPLAYER_Next(DFPLAYER_Name* MP3) 
{
	DFPLAYER_SendCmd(MP3, DFP_NEXT,00,00);
}

void DFPLAYER_Prev(DFPLAYER_Name* MP3)
{
	DFPLAYER_SendCmd(MP3, DFP_PREV,00,00);
}

void DFPLAYER_SetVolume(DFPLAYER_Name* MP3, uint16_t volume)
{
	uint8_t volume1 = volume>>8;
	uint8_t volume2 = volume;
	DFPLAYER_SendCmd(MP3, DFP_SETVOLUME, volume1, volume2);
}

void DFPLAYER_Play(DFPLAYER_Name* MP3) 
{
	DFPLAYER_SendCmd(MP3, DFP_PLAY, 00, 00);
} 

void DFPLAYER_Pause(DFPLAYER_Name* MP3)
{
	DFPLAYER_SendCmd(MP3, DFP_PAUSE, 00, 00);
}

void DFPLAYER_Stop(DFPLAYER_Name* MP3)
{
	DFPLAYER_SendCmd (MP3, DFP_STOP, 00, 00);
}

void DFPLAYER_RandomPlay(DFPLAYER_Name* MP3)
{
	DFPLAYER_SendCmd (MP3, DFP_RANDOM,0,0);
}

void DFPLAYER_PlayFileInFolder(DFPLAYER_Name* MP3, uint8_t folder, uint32_t num) 
{
	DFPLAYER_SendCmd (MP3, DFP_PLAYFILEINFOLDER, folder, num);
}
void mqtt_get_data_callback(char *data, uint16_t length)
{
	uint16_t songIDRev = DFControl.songId;
	uint16_t volumeRev = DFControl.volume;
	bool finishedRev = DFControl.finished;
	bool playRev = DFControl.play;
	
	// Parse JSON Data
        cJSON *root = cJSON_Parse(data);
        if (root) {
            // Extract Integer Values
            finishedRev = cJSON_GetObjectItem(root, "finished")->valueint;
            playRev = cJSON_GetObjectItem(root, "play")->valueint;
            songIDRev = cJSON_GetObjectItem(root, "songID")->valueint;
            volumeRev = cJSON_GetObjectItem(root, "volume")->valueint;
            // Print Extracted Receive Value
			printf("CURRENT VALUE");
            printf("finished: %d\n", DFControl.finished);
            printf("play: %d\n", DFControl.play);
            printf("songID: %d\n", DFControl.songId);
            printf("volume: %d\n", DFControl.volume);
			//
			printf("RECEIVED VALUE");
			printf("finished: %d\n", finishedRev);
            printf("play: %d\n", playRev);
            printf("songID: %d\n", songIDRev);
            printf("volume: %d\n", volumeRev);
            // Free the cJSON object
            cJSON_Delete(root);
        } else {
            ESP_LOGE(TAG, "Error parsing JSON data");
        }
		DFPLAYER_PlayTrack(&MP3,songIDRev);		
}