#include   "PS2.H"


#define CLK_L PS2_sck_clr
#define CLK_H PS2_sck_setb

#define MOSI_L PS2_mosi_clr
#define MOSI_H PS2_mosi_setb

#define MISO_InDat PS2_miso_indat
//-----SPI，一个字节的读写
/*
uchar PS2_RWByte(uchar dat)
{
SPDR=dat;  
while(!(SPSR&0x80));  //读SPSR，判断是否发送/接受完成！
return(SPDR);  
}
*/

//------GPIO版本-----
uchar PS2_RWByte(uchar id)
{
uchar i=0,j;
uchar od=0;

for(i=0;i<8;i++)
{

if( id&(1<<i) )
MOSI_H;
else
MOSI_L;	
CLK_L;


for(j=0;j<10;j++);


CLK_H; 
if(MISO_InDat) 
od|=1<<i;
else
od|=0<<i;

}
return (od);
}


void PS2_Init(void)   //--SPI端口初始化
{
uchar i=0;

PS2_sck_outmode;
PS2_mosi_outmode;
PS2_miso_inmode;

PS2_miso_setb;
PS2_mosi_setb;

PS2_CS_outmode;
PS2_CS_out_setb;

delay(30);
}



/*
至少需要20ms的间隔时间访问一次
buf至少需要9个空间
返回0,失败
1：成功，普通模式
2：成功，摇杆扩展模式
*/
uchar Get_PS2Dat(uchar *buf)
{
uchar i;

	PS2_CS_out_clr;
	
	for(i=0;i<100;i++);	
	buf[0]=PS2_RWByte(0x01);for(i=0;i<100;i++); //延时再读取
	buf[1]=PS2_RWByte(0x42);for(i=0;i<100;i++);
	buf[2]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	buf[3]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	buf[4]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	buf[5]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	buf[6]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	buf[7]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	buf[8]=PS2_RWByte(0x00);for(i=0;i<100;i++);
	
	PS2_CS_out_setb;


if((buf[0]==0xff)&&(buf[1]==0x41)&&(buf[2]==0x5a))  //成功，普通模式
return 1;

if((buf[0]==0xff)&&(buf[1]==0x73)&&(buf[2]==0x5a))  //成功，摇杆扩展模式
return 2;

return 0;
}


/*
返回数据中某一位置1为按下
按下的掩膜值为_PS2_Key_Dir_U 等...

*/
uint PS2_ChangeKey(uchar *inBuf)
{

return( ((uint)(~inBuf[4])<<8) + (~inBuf[3]) ) ;

}
