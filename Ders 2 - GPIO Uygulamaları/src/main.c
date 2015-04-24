/*****************************************************************************

* File Name        : main.c

* Description      : GPIO_Uygulamasý

********************************************************************************

* Additional Information : Buton'a basýldýðýnda ilgili çýkýþý set eden program

*******************************************************************************/
/***********************************************************
* Function Name  :
* Description    :
* Input          :
* Return         :
***********************************************************/
// Kütüphane dosyalarýný kullanabilmek için eklenmiþtir.
#include "stm32f0xx_conf.h"

//Delay fonskiyonunun prototipini tanýmladýk döndereceði deðer ve alacaðý parametreleri belirledik çdeðer döndürmeden int32 deðer alacak
void Delay(__IO uint32_t);
//Bu sayede main fonksiyonun üstünü þiþirmedik ama program içerisinde böyle bir fonksiyon var dedik çünkü kodlar yukarýdan aþaðý akacaðýndan bu parametreyi girmezsek delay fonksiyonun kullandýðýmýz zaman (tanýmladýktan önce ) hata verecektir.

/***********************************************************
* Function Name  : main
* Description    : Ana program burada döner
* Input          : void (hiç bir þey döndürmez)
* Return         : int (sonsuz döngü kuandýldýðýndan "return 0;" komutuna gerek kalmadý)
***********************************************************/
int main(void)
{
    /*NOTE:
    Output open-drain: Pinin çýkýþ bacaðý, P-Mos ile N-Mos mosfetleri arasýnda bulunmaktadýr.
    Eðer çýkýþý open-drain ayarlarsanýz, pin bacaðý GND'ye baðlanýr. VDD ile arasýnda sonsuz empedans oluþur.
    Bu durumda pini high olarak çýkýþ vermek için dýþarýdan pull-up dirençleri eklemeniz gerekmekte. Bu özellikle, çýkýþa baðlanacak olan devre elemanýnýn daha fazla akým çekmesi saðlanabilmektedir.
    Röle gibi akým gerektiren devre elemanlarýný sürmek için kullanýlýr. Akým mikroiþlemciden deðil, pull-up direnci üzerinden çekilir.
    Output push-pull:  Pin bacaðý P-Mos mosfeti ile VDD pinine baðlanýr. Akým gerektirmeyen iþlemlerde, çýkýþý logik 1 yapmak için bu mod kullanýlýr.
    */
    GPIO_InitTypeDef  GPIO_InitStructure;
    /*
        GPIO_InitTypeDef yapýsýndaki datalarý GPIO_InitStructure'a verdik.
        GPIO_InitTypeDef'e sað týklayýp "find declaration derseniz kaynaðýný görüntüleyip içindekileri görebilirsiniz.
    */

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);  //Bu fonksiyon ile kulancaðýmýz porta clock vererek o portun bir nevi beslemesini yaptýk ve enabled ederek aktifleþtirdik.
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);  //Ayný iþlemi a portu için aptýk

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9; //yukarýda ki yapýda, alt elemanlara (hocamýn örneðindeki manavýn meyve çeþitleri gibi :)) nokta koyup erþebiliriz tanýmlama ve iðer ayrýntýlar için yapýnýn kaynaðýný görntüleyebilirsiniz.(| iþareti ve anlam gibidir 2 pin için belirtilen iþlemi yapýyor)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; //yapýdak mode elemetini kullanarak pineri çýkýþ olarak tanýmladýk benim kitimde bu pinlerde led mevcut
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // #Bu ayarýn 2 çeþidi var açýklama uzun olduðundan "goto note;" :) Akým gerektirmeyen bir iþ yaptýðýmýzdan push pul kullandýk
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;//GPIO Speed 3 farklý seviyede leve1 2 mhz, level2 10mhz, level3 50mhz olabildiðince düþk akým çekip enerjiden tasarruf etmk için en uygun hýz seçilmedlidir.
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//PuPd elementi ile pine pull up pull down ekleyebiliyoruz yine 3 seçenek mevcut
    /*
    GPIO_PuPd_NOPULL = 0x00, direnc yok
    GPIO_PuPd_UP     = 0x01, pull up aktif  "Biz kullanmadýk led olacaðýndan"
    GPIO_PuPd_DOWN   = 0x02 pull down aktif
    */
    GPIO_Init(GPIOC,&GPIO_InitStructure); //þimdi bu fonksiyon ile C portuna yaptýðýmýz ayarlarýn adreslerini veriyoruz.ingilizce ve iþareti deðiþkenin önünde ise adres anlamýnda

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;//kitimde a0 pininde led bulunmakta bunu kullandýðým için tanýmladým
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN; //in yani giriþ olacaðýný seçtim
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//pus pull kullanýlýyor düþk akýmdan dolayý
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;//hýz 1 ci seviyede (2mhz)
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; //normalde direnc kullnýýrdý faket kitimdeki butonda PULL-DOWN yapýlmýþ

    GPIO_Init(GPIOA,&GPIO_InitStructure); //yine a portunda ayar yatýðýmdan dolayý GPIO_InitStructure ýn adresini a portuna yükledik

    while(1)//sonsuz döngü
    {
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))//pin durumunu kumak için bu fonksiyonu kullandýk 1 veya 0 döndürecek
        {//butona basýnca pull down drenci kalkacak pine lojik 1 geleceðinden true dönecek ve if içine girecek (butona baýldý)
            GPIO_SetBits(GPIOC,GPIO_Pin_8);//Ýlgii portun ilgili pinini set eder (lojik 1 verir) kitte 8 ve 9 pinleri le baðlý
            GPIO_SetBits(GPIOC,GPIO_Pin_9);
            //ayný iþlemi tek satýrda yapabiliriz GPIO_SetBits(GPIOC,GPIO_Pin_9 | GPIO_Pin_8);
        }
        else// basýlmamþ ise reset yani lojik 0 konumuna alýr
        {
            GPIO_ResetBits(GPIOC,GPIO_Pin_8);
            GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        }
        //Buton ile ledi yakma uygulamasýndan önce led blinkde yaptým tek fark while içine ek olarak elay kulandýk
        //Delay(10000000); ile aklaýk olarak 3-4 bekleme yapýlýyor.
        /*
            GPIO_SetBits(GPIOC,GPIO_Pin_8);
            GPIO_SetBits(GPIOC,GPIO_Pin_9);

            Delay(10000000);

            GPIO_ResetBits(GPIOC,GPIO_Pin_8);
            GPIO_ResetBits(GPIOC,GPIO_Pin_9);

            Delay(10000000);*/
    }
}

/***********************************************************
* Function Name  : Delay
* Description    : Gönderdiðimiz sayý kadar eksiltip gecikme saglar
* Input          : Counter deðeri
* Return         : void
***********************************************************/
void Delay(__IO uint32_t nCount) {
  while(nCount--) {// verdiðimiz 32 bitlik deðer sýfýr olana kadar azaltýr ve burada oyalanýr
  }
}
