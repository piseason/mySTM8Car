#include "uart.h"
#include "stdarg.h"
#define NULL 0
/* 
由于不同的编译器 putcha 和 getchar 形参和返回值略有不同。 
因此此处采用宏定义的方式区别。 

_RAISONANCE_ 和 _COSMIC_ 这2个宏是由编译器自动添加的预编译宏 
*/ 
#ifdef _RAISONANCE_ 
#define PUTCHAR_PROTOTYPE int putchar (char c) 
#define GETCHAR_PROTOTYPE int getchar (void) 
#elif defined (_COSMIC_) 
#define PUTCHAR_PROTOTYPE char putchar (char c) 
#define GETCHAR_PROTOTYPE char getchar (void) 
#else /* _IAR_ */ 
#define PUTCHAR_PROTOTYPE int putchar (int c) 
#define GETCHAR_PROTOTYPE int getchar (void) 
#endif /* _RAISONANCE_ */ 

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {   
    nCount--;
  }
}

void Uart1_Init(void)
{
    UART1_DeInit();		/* 将寄存器的值复位 */

	/*
	 * 将UART1配置为：
	 * 波特率 = 115200
	 * 数据位 = 8
	 * 1位停止位
	 * 无校验位
	 * 使能接收和发送
	 * 使能接收中断
	 */

    UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO , UART1_SYNCMODE_CLOCK_DISABLE , UART1_MODE_TXRX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART1_Cmd(ENABLE);
}

void UART1_SendByte(u8 data)
{
	UART1_SendData8((unsigned char)data);

	/* 等待传输结束 */
	while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
}

void UART1_Sendstr(const unsigned char *p_str)
{
//  u8 i = 0;
  while(*p_str != '\0')
  {
    UART1_SendByte(*p_str);
 //   Delay(1000);
    p_str++;
  }
}

/*
  功能：将int型数据转为2，8，10，16进制字符串
  参数：value --- 输入的int整型数
        str --- 存储转换的字符串
        radix --- 进制类型选择
  注意：8位单片机int字节只占2个字节
*/
char *sky_itoa(int value, char *str, unsigned int radix)
{
  char list[] = "0123456789ABCDEF";
  unsigned int tmp_value;
  int i = 0, j, k = 0;
  if (NULL == str) {
    return NULL;
  }
  if (2 != radix && 8 != radix && 10 != radix && 16 != radix) {
    return NULL;
  }
  if (radix == 10 && value < 0) {
    //十进制且为负数
    tmp_value = (unsigned int)(0 - value);
    str[i++] = '-';
    k = 1;
  } else {
    tmp_value = (unsigned int)value;
  }
  //数据转换为字符串，逆序存储
  do {
    str[i ++] = list[tmp_value%radix];
    tmp_value /= radix;
  } while(tmp_value);
  str[i] = '\0';
  //将逆序字符串转换为正序
  char tmp;
  for (j = k; j < (i+k)/2; j++) {
    tmp = str[j];
    str[j] = str[i-j-1+k];
    str[i-j-1+k] = tmp;
  }
  return str;
}

/*
  功能：将double型数据转为字符串
  参数：value --- 输入的double浮点数
        str --- 存储转换的字符串
        eps --- 保留小数位选择，至少保留一个小数位,至多保留4个小数位
  注意：8位单片机int字节只占2个字节
*/
void sky_ftoa(double value, char *str, unsigned int eps)
{
  unsigned int integer;
  double decimal;
  char list[] = "0123456789";
  int i = 0, j, k = 0;
  //将整数及小数部分提取出来
  if (value < 0) {
    decimal = (double)(((int)value) - value);
    integer = (unsigned int)(0 - value);
    str[i ++] = '-';
    k = 1;
  } else {
    integer = (unsigned int)(value);
    decimal = (double)(value - integer);
  }
  //整数部分数据转换为字符串，逆序存储
  do {
    str[i ++] = list[integer%10];
    integer /= 10;
  } while(integer);
  str[i] = '\0';
  //将逆序字符串转换为正序
  char tmp;
  for (j = k; j < (i+k)/2; j++) {
    tmp = str[j];
    str[j] = str[i-j-1+k];
    str[i-j-1+k] = tmp;
  }
  //处理小数部分
  if (eps < 1 || eps > 4) {
    eps = 4;
  }
  
  //精度问题，防止输入1.2输出1.19等情况
  double pp = 0.1;
  for (j = 0; j <= eps; j++) {
    pp *= 0.1;
  }
  decimal += pp;
  while (eps) {
    decimal *= 10;
    eps --;
  }
  int tmp_decimal = (int)decimal;
  str[i ++] = '.';
  k = i;
  //整数部分数据转换为字符串，逆序存储
  do {
    str[i ++] = list[tmp_decimal%10];
    tmp_decimal /= 10;
  } while(tmp_decimal);
  str[i] = '\0';
  //将逆序字符串转换为正序
  for (j = k; j < (i+k)/2; j++) {
    tmp = str[j];
    str[j] = str[i-j-1+k];
    str[i-j-1+k] = tmp;
  }
  str[i] = '\0';
}


void mprintf(char * Data, ...)
{
  const char *s;
  int d;   
  char buf[16];
  uint8_t txdata;
  va_list ap;
  va_start(ap, Data);
  while ( * Data != 0 ) {				                          
    if ( * Data == 0x5c )  {									  
      switch ( *++Data ) {
        case 'r':							          
          txdata = 0x0d;
          UART1_SendByte(txdata);
          Data ++;
          break;
        case 'n':							          
          txdata = 0x0a;
          UART1_SendByte(txdata);
          Data ++;
          break;
        default:
          Data ++;
          break;
      }			 
    } else if ( * Data == '%') {									  
      switch ( *++Data ) {				
      case 's':										 
        s = va_arg(ap, const char *);
        for ( ; *s; s++) {
          UART1_SendByte(*((uint8_t *)s));
        }				
        Data++;				
        break;
      case 'd':			
        d = va_arg(ap, int);					
        sky_itoa(d, buf, 10);					
        for (s = buf; *s; s++) {
          UART1_SendByte(*((uint8_t *)s));
        }					
        Data++;				
        break;
//      case 'x': {
//        d = va_arg(ap, int);					
//        sky_itoa(d, buf, 16);					
//        for (s = buf; *s; s++) {
//          UART1_SendByte(*((uint8_t *)s));
//        }					
//        Data++;			
//        break;
//      }
//      case 'f': {
//        double num = va_arg(ap, double);					
//        sky_ftoa(num, buf, 4);
//        for (s = buf; *s; s++) {
//          UART1_SendByte(*((uint8_t *)s));
//        }					
//        Data++;			
//        break;
//      }
      default:
        Data++;				
        break;				
      }		 
    } else {
        UART1_SendByte(*((uint8_t *)Data));
        Data++;
    }
  }
}

