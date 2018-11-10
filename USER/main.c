#include "sys.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "lora_app.h"
#include "usart3.h"

/************************************************
�߼�ժҪ
    1.��ʼ��loraģ��
    2.loraģ������
    3.����lora���ݣ�����ʾ�����Զ�

************************************************/

int main(void)
{
    u8 temp,i;
    u8 hour=1,min=2,locx=5,locy=6,crc=9;
            //      0,  1,   2,   3,       4,       5,   6,  7,   8,   9,  10
    u8 sendData[]={0x80,0x07,0x11,MY_ADDRH,MY_ADDRL,hour,min,locx,locy,crc,0x81};

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

    printf("start while(1)\n");
    while(1)
    {
		/*printf("USART3_RX_STA=%x\n",USART3_RX_STA);
		for(i=0;i<(USART3_RX_STA&0x7fff);i++)
			printf("%x ",USART3_RX_BUF[i]);
		printf("\n");
		USART3_RX_STA=0;*/
        if(USART3_RX_STA&0X8000)	
		{
            for(i=0;i<(USART3_RX_STA&0x7fff);i++)
                data_rev[i]=USART3_RX_BUF[i];
            if(data_rev[0]==0x80 && data_rev[1]==0x07 && data_rev[2]==0x11 && data_rev[10]==0x81){
                for(i=1,temp=0; i<9; i++)
                    temp+=data_rev[i];
                if(data_rev[9]==temp){
                    //code here:

                }
            }

			/*printf("USART3_RX_STA=%x\n",(USART3_RX_STA));
			for(i=0;i<(USART3_RX_STA&0x7fff);i++)
                printf("%x ",USART3_RX_BUF[i]);
            printf("\n");
			USART3_RX_STA=0;*/
		}
        // delay_ms(1000);
    }
}
