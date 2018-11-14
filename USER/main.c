#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "lora_app.h"
#include "usart2.h"

/************************************************
�߼�ժҪ
    1.��ʼ��loraģ��
    2.loraģ������
    3.����lora���ݣ�����ʾ�����Զ�

************************************************/

int main(void)
{
    u8 temp,i;
	u8 data_rev[1024];
            //      0,  1,   2,   3,   4
    u8 sendData[]={0x80,0x01,0x50,0x51,0x81};
    u8 test[]={1,2,3,4,5};
    u8 len= sizeof(sendData)/sizeof(sendData[0]);

//	SystemInit();
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart1_init(115200);
    printf("uart1_init success!\n");
    LED_Init();

    while(LoRa_Init())  //��ʼ��ATK-LORA-01ģ��,����ʼ��ʧ����300ms�����ԣ�ֱ���ɹ�
    {
        printf("LoRa undetected...\n");
        delay_ms(300);
    }
    printf("LoRa detected!\n");
    
    LoRa_Set();     //LoRa����(�������������ô��ڲ�����Ϊ115200)
    LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,test,5);

    printf("start while(1)\n");
    while(1)
    {
		/*printf("USART2_RX_STA=%x\n",USART2_RX_STA);
		for(i=0;i<(USART2_RX_STA&0x7fff);i++)
			printf("%x ",USART2_RX_BUF[i]);
		printf("\n");
		USART2_RX_STA=0;*/
        if(USART2_RX_STA&0X8000){
            printf("data receive= ");
            for(i=0;i<(USART2_RX_STA&0x7fff);i++){
                printf("%x ",USART2_RX_BUF[i]);
                data_rev[i]=USART2_RX_BUF[i];
            }
            printf("\n");
            if(data_rev[0]==0x80 && data_rev[1]==0x07 && data_rev[2]==0x11 && data_rev[10]==0x81){
                for(i=1,temp=0; i<9; i++)
                    temp+=data_rev[i];
                if(data_rev[9]==temp){
                    //code here:
                    //store key data

                    delay_ms(100);
                    LoRa_SendData(OBJ_ADDRH,OBJ_ADDRL,OBJ_CHN,sendData,len);
                    printf("ack= %x %x %x %x %x\n", sendData[0], sendData[1], sendData[2], sendData[2], sendData[4]);
                }
                memset(data_rev,0,1024);
            }
			/*printf("USART2_RX_STA=%x\n",(USART2_RX_STA));
			for(i=0;i<(USART2_RX_STA&0x7fff);i++)
                printf("%x ",USART2_RX_BUF[i]);
            printf("\n");*/
			USART2_RX_STA=0;
		}
        delay_ms(500);
        printf("500ms passed\n");
    }
}
