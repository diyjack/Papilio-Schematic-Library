/*
 Gadget Factory
 YM2149 Audio Player Example
 To learn more about the Papilio Schematic Library please visit http://learn.gadgetfactory.net
 
 To use this sketch do the following steps:
 1)
 Load the bit file that defines the circuit required by this sketch by clicking on the bit file link below that corresponds to the type of Papilio board you have. 
 You should see the Papilio Loader pop up and program the circuit for this example to your Papilio Board. 
 IMPORTANT NOTE: Please be sure that you are programming the bit file to SPI Flash. ZPUino will not work if written to FPGA only.
 
 2)
 Make sure you have selected the correct ZPUino board type and com port, the Papilio always uses channel B of the FT2232 so it is usually the second com port you see.
 The board type should be ZPUino Vanilla or Hyperion depending on what the schematic shows, or look at the Board Type section below.
 Be sure to select the board type for the type of Papilio you have.
 
 3)
 Make sure you have the required hardware attached to your Papilio, look at the schematic to see what is required. Or look at the Hardware section below.
 You can open a PDF showing the circuit defined in the bit file by selecting the appropriate link to view the scheamtic for the type of Papilio board you have.

 4)
 Run the sketch, it should communicate with the ZPUino included in the bit file you loaded earlier. 
 
 If you want to modify the circuit then make sure you have Xilinx ISE Webpack installed and click on the appropriate link below to modify the schematic.
 If you do not have Xilinx ISE Webpack installed visit this tutorial to learn how. http://gadgetfactory.net/learn/2013/10/23/install-xilinxs-free-ise-webpack/
 
 If the links below do not work then then open the sketch directory from Sketch/Show Sketch Folder (Ctl-K) and manually access the files defined in the links below.

 Papilio Pro
   Click to program bit file: sketchdir://LX9/papilio_pro.bit
   Click to view schematic:   sketchdir://schematic_papilio_pro.pdf
   Click to modify schematic: sketchdir://PSL_Papilio_Pro_LX9.xise
   
 Papilio One 500K
   Click to program bit file: sketchdir://500K/papilio_one_500k.bit
   Click to view schematic:   sketchdir://schematic_papilio_one_500k.pdf
   Click to modify schematic: sketchdir://PSL_Papilio_One_500K.xise

 Tutorials:
   http://gadgetfactory.net/learn/2013/12/09/7336/
  
 Related library documentation:
   http://www.papilio.cc/index.php?n=Papilio.YM2149
   http://www.papilio.cc/index.php?n=Papilio.YMVoice
 
 Additional Software:
   Use the ym_converter script to convert ym files to ymd files
   sketchdir://ym_converter
   
 
 Hardware:
   This sketch will work with any hardware with an audio jack: Audio Wing, LogicStart MegaWing, Arcade MegaWing, RetroCade MegaWing.
   This example circuit is wired for an Audio Wing connected to CH, to use with any of the other hardware click the link to modify the schematic to adjust for your hardware.
   This sketch will work with the Papilio Pro and Papilio One 500K.
 
 Board Type:
   ZPUino Vanilla Variant for your board type.

 created 2013
 by Jack Gassett
 http://www.gadgetfactory.net
 
 This example code is in the public domain.
 */

 #define FREQ 17000          //Freq for ymplayer
 
#include <SD.h>
#include "YM2149.h"
#include "SmallFS.h"
#include "ymplayer.h"
#include "ramFS.h"
#include "cbuffer.h"

YMPLAYER ymplayer;
YM2149 ym2149;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
   //Start SmallFS
  if (SmallFS.begin()<0) {
	//Serial.println("No SmalLFS found.");
  }
  else{
     //Serial.println("SmallFS Started.");
  }  

  //Set what wishbone slot the ym2149 device is connected to.  
  ymplayer.setup(&ym2149,5); 
  
  ym2149.V1.setVolume(11);
  ym2149.V2.setVolume(11);
  ym2149.V3.setVolume(11);   

  ymplayer.loadFile("music.ymd");
  ymplayer.play(true);
  
 //Setup timer for YM and mod players, this generates an interrupt at 17Khz
  TMR0CTL = 0;
  TMR0CNT = 0;
  TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
  TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
  	_BV(TCTLCP0) | _BV(TCTLIEN);
  INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
  INTRCTL=1;    

}

void _zpu_interrupt()
{
  //Interrupt runs at 17KHz
  ymplayer.zpu_interrupt(); 
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (ymplayer.getPlaying() == 1)
    ymplayer.audiofill();
}
