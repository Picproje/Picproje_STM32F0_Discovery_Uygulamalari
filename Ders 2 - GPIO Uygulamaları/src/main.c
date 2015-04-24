/*****************************************************************************

* File Name        : main.c

* Description      : GPIO_Uygulamas�

********************************************************************************

* Additional Information : Buton'a bas�ld���nda ilgili ��k��� set eden program

*******************************************************************************/
/***********************************************************
* Function Name  :
* Description    :
* Input          :
* Return         :
***********************************************************/
// K�t�phane dosyalar�n� kullanabilmek i�in eklenmi�tir.
#include "stm32f0xx_conf.h"

//Delay fonskiyonunun prototipini tan�mlad�k d�nderece�i de�er ve alaca�� parametreleri belirledik �de�er d�nd�rmeden int32 de�er alacak
void Delay(__IO uint32_t);
//Bu sayede main fonksiyonun �st�n� �i�irmedik ama program i�erisinde b�yle bir fonksiyon var dedik ��nk� kodlar yukar�dan a�a�� akaca��ndan bu parametreyi girmezsek delay fonksiyonun kulland���m�z zaman (tan�mlad�ktan �nce ) hata verecektir.

/***********************************************************
* Function Name  : main
* Description    : Ana program burada d�ner
* Input          : void (hi� bir �ey d�nd�rmez)
* Return         : int (sonsuz d�ng� kuand�ld���ndan "return 0;" komutuna gerek kalmad�)
***********************************************************/
int main(void)
{
    /*NOTE:
    Output open-drain: Pinin ��k�� baca��, P-Mos ile N-Mos mosfetleri aras�nda bulunmaktad�r.
    E�er ��k��� open-drain ayarlarsan�z, pin baca�� GND'ye ba�lan�r. VDD ile aras�nda sonsuz empedans olu�ur.
    Bu durumda pini high olarak ��k�� vermek i�in d��ar�dan pull-up diren�leri eklemeniz gerekmekte. Bu �zellikle, ��k��a ba�lanacak olan devre eleman�n�n daha fazla ak�m �ekmesi sa�lanabilmektedir.
    R�le gibi ak�m gerektiren devre elemanlar�n� s�rmek i�in kullan�l�r. Ak�m mikroi�lemciden de�il, pull-up direnci �zerinden �ekilir.
    Output push-pull:  Pin baca�� P-Mos mosfeti ile VDD pinine ba�lan�r. Ak�m gerektirmeyen i�lemlerde, ��k��� logik 1 yapmak i�in bu mod kullan�l�r.
    */
    GPIO_InitTypeDef  GPIO_InitStructure;
    /*
        GPIO_InitTypeDef yap�s�ndaki datalar� GPIO_InitStructure'a verdik.
        GPIO_InitTypeDef'e sa� t�klay�p "find declaration derseniz kayna��n� g�r�nt�leyip i�indekileri g�rebilirsiniz.
    */

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);  //Bu fonksiyon ile kulanca��m�z porta clock vererek o portun bir nevi beslemesini yapt�k ve enabled ederek aktifle�tirdik.
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);  //Ayn� i�lemi a portu i�in apt�k

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9; //yukar�da ki yap�da, alt elemanlara (hocam�n �rne�indeki manav�n meyve �e�itleri gibi :)) nokta koyup er�ebiliriz tan�mlama ve i�er ayr�nt�lar i�in yap�n�n kayna��n� g�rnt�leyebilirsiniz.(| i�areti ve anlam gibidir 2 pin i�in belirtilen i�lemi yap�yor)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; //yap�dak mode elemetini kullanarak pineri ��k�� olarak tan�mlad�k benim kitimde bu pinlerde led mevcut
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // #Bu ayar�n 2 �e�idi var a��klama uzun oldu�undan "goto note;" :) Ak�m gerektirmeyen bir i� yapt���m�zdan push pul kulland�k
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;//GPIO Speed 3 farkl� seviyede leve1 2 mhz, level2 10mhz, level3 50mhz olabildi�ince d��k ak�m �ekip enerjiden tasarruf etmk i�in en uygun h�z se�ilmedlidir.
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//PuPd elementi ile pine pull up pull down ekleyebiliyoruz yine 3 se�enek mevcut
    /*
    GPIO_PuPd_NOPULL = 0x00, direnc yok
    GPIO_PuPd_UP     = 0x01, pull up aktif  "Biz kullanmad�k led olaca��ndan"
    GPIO_PuPd_DOWN   = 0x02 pull down aktif
    */
    GPIO_Init(GPIOC,&GPIO_InitStructure); //�imdi bu fonksiyon ile C portuna yapt���m�z ayarlar�n adreslerini veriyoruz.ingilizce ve i�areti de�i�kenin �n�nde ise adres anlam�nda

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;//kitimde a0 pininde led bulunmakta bunu kulland���m i�in tan�mlad�m
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN; //in yani giri� olaca��n� se�tim
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//pus pull kullan�l�yor d��k ak�mdan dolay�
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;//h�z 1 ci seviyede (2mhz)
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; //normalde direnc kulln��rd� faket kitimdeki butonda PULL-DOWN yap�lm��

    GPIO_Init(GPIOA,&GPIO_InitStructure); //yine a portunda ayar yat���mdan dolay� GPIO_InitStructure �n adresini a portuna y�kledik

    while(1)//sonsuz d�ng�
    {
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))//pin durumunu kumak i�in bu fonksiyonu kulland�k 1 veya 0 d�nd�recek
        {//butona bas�nca pull down drenci kalkacak pine lojik 1 gelece�inden true d�necek ve if i�ine girecek (butona ba�ld�)
            GPIO_SetBits(GPIOC,GPIO_Pin_8);//�lgii portun ilgili pinini set eder (lojik 1 verir) kitte 8 ve 9 pinleri le ba�l�
            GPIO_SetBits(GPIOC,GPIO_Pin_9);
            //ayn� i�lemi tek sat�rda yapabiliriz GPIO_SetBits(GPIOC,GPIO_Pin_9 | GPIO_Pin_8);
        }
        else// bas�lmam� ise reset yani lojik 0 konumuna al�r
        {
            GPIO_ResetBits(GPIOC,GPIO_Pin_8);
            GPIO_ResetBits(GPIOC,GPIO_Pin_9);
        }
        //Buton ile ledi yakma uygulamas�ndan �nce led blinkde yapt�m tek fark while i�ine ek olarak elay kuland�k
        //Delay(10000000); ile akla�k olarak 3-4 bekleme yap�l�yor.
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
* Description    : G�nderdi�imiz say� kadar eksiltip gecikme saglar
* Input          : Counter de�eri
* Return         : void
***********************************************************/
void Delay(__IO uint32_t nCount) {
  while(nCount--) {// verdi�imiz 32 bitlik de�er s�f�r olana kadar azalt�r ve burada oyalan�r
  }
}
