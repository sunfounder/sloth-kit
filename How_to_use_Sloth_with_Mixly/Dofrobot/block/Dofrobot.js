'use strict';

goog.provide('Blockly.Blocks.Dofrobot');

goog.require('Blockly.Blocks');


Blockly.Blocks.Dofrobot.HUE = "#698B69"; 
Blockly.Blocks.Dofrobot.Sensor = "#698B69";

var BUZZER_PORT =[["10", "10"],["20", "20"],["30", "30"],["40", "40"],["50", "50"]];


//舵机

Blockly.Blocks.Dofrobot_dj = {
  init: function() {
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
  this.appendValueInput("PIN", Number)
        .appendField(Blockly.MIXLY_Dofrobot_SERVO_TEST)
        .appendField(Blockly.MIXLY_Dofrobot_PIN)
    this.appendValueInput("DEGREE", Number)
        .setCheck(Number)
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(Blockly.MIXLY_Dofrobot_JIAODU);
    this.appendValueInput("DELAY_TIME", Number)
        .setCheck(Number)
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(Blockly.MIXLY_DELAY+'('+Blockly.MIXLY_DELAY_MS+')');
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    //this.setTooltip(Blockly.MIXLY_TOOLTIP_BLOCKGROUP_SERVO_MOVE);
  }
};

/*Blockly.Blocks.Dofrobot_forward={
init: function() {
    this.setColour(Blockly.Blocks.Dofrobot.HUE );
    this.appendDummyInput("")
        .appendTitle(Blockly.MIXLY_DOFROBOT_MOVEFOR)
       
  this.appendValueInput("X", 'Number')
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendTitle(Blockly.MIXLY_DOFROBOT_VEL);
    this.setPreviousStatement(true);
    this.setNextStatement(true);
  }
};
Blockly.Blocks.Dofrobot_back = {
      init: function() {
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
        .appendTitle(Blockly.MIXLY_DOFROBOT_BACK)
       
    this.appendValueInput("X", 'Number')
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendTitle(Blockly.MIXLY_DOFROBOT_BACKVEL); 
   this .setPreviousStatement(true);
   this .setNextStatement(true);
      }
      };   


Blockly.Blocks.Dofrobot_chao = {
    init: function () {
        this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendValueInput("DEGREE", Number)
        .setCheck(Number)
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(Blockly.MIXLY_Dofrobot_JIAODU);
    this.appendValueInput("DELAY_TIME", Number)
        .setCheck(Number)
        .setAlign(Blockly.ALIGN_RIGHT)
        .appendField(Blockly.MIXLY_DELAY+'('+Blockly.MIXLY_DELAY_MS+')');
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    //this.setTooltip(Blockly.MIXLY_TOOLTIP_BLOCKGROUP_SERVO_MOVE);
  }
};    */

      
   
  //aviodance

  Blockly.Blocks.Dofrobot_forward={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_DOFROBOT_MOVEFOR)     
        .appendTitle(Blockly.MIXLY_DOFROBOT_VEL)
         .appendTitle(new Blockly.FieldDropdown(BUZZER_PORT), "PIN");
         
      this.setPreviousStatement(true);
       this.setNextStatement(true);
     }
     };

    Blockly.Blocks.Dofrobot_back={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_DOFROBOT_BACKVEL)     
        .appendTitle(Blockly.MIXLY_DOFROBOT_VEL)
         .appendTitle(new Blockly.FieldDropdown(BUZZER_PORT), "PIN");
         
      this.setPreviousStatement(true);
       this.setNextStatement(true);
     }
     };  

Blockly.Blocks.Dofrobot_turnright={
    init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
    .appendTitle(Blockly.MIXLY_Dofrobot_TURNRIGHT)
    this.setPreviousStatement(true);
    this.setNextStatement(true);
     }
     };

     Blockly.Blocks.Dofrobot_avoid={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_Dofrobot_AVOID)     
        .appendTitle(Blockly.MIXLY_DOFROBOT_VEL)
         .appendTitle(new Blockly.FieldDropdown(BUZZER_PORT), "PIN");
         this.appendValueInput("X")
            .setCheck(Number)
            .setAlign(Blockly.ALIGN_RIGHT)
            .appendTitle(Blockly.MIXLY_Dofrobot_BI);

      this.setPreviousStatement(true);
       this.setNextStatement(true);
     }
     };
  

     Blockly.Blocks.Dofrobot_kong={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_Dofrobot_kong)
      //.appendField(new Blockly.FieldImage("../../media/Dofrobot/Dofrobot_csb.png", 43, 32));
  this.appendDummyInput("")
      this.setPreviousStatement(true, null);
       this.setNextStatement(true, null);
     }
     };



//超声波
Blockly.Blocks.Dofrobot_csb = {
    init: function () {
        this.setColour(Blockly.Blocks.Dofrobot.HUE);
        this.appendDummyInput("")
            .appendField(Blockly.MIXLY_Dofrobot_CHAOSHENGBO);
        this.appendValueInput("PIN1", Number)
            .appendField('TRIG#')
            .setCheck(Number);
        this.appendValueInput("PIN2", Number)
            .appendField('ECHO#')
            .setCheck(Number);
        this.setInputsInline(true);
        this.setOutput(true, Number);
        this.setTooltip(Blockly.MIXLY_TOOLTIP_BLOCKGROUP_CHAOSHENGBO);
    }
};





//dancing-circle
Blockly.Blocks.Dofrobot_circle={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_Dofrobot_CIRCLE)
     
 // this.appendDummyInput("")
      this.setPreviousStatement(true);
       this.setNextStatement(true);
     }
     };
     // dancing-leg lift
     Blockly.Blocks.Dofrobot_leglift={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_Dofrobot_LEGLIFT)
     
  //this.appendDummyInput("")
      this.setPreviousStatement(true);
       this.setNextStatement(true);
     }
     };
     // dancing-two
     Blockly.Blocks.Dofrobot_dancingtwo={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_Dofrobot_DANCINGTWO)
      
 // this.appendDummyInput("")
      this.setPreviousStatement(true );
       this.setNextStatement(true);
     }
     };

// dancing 
Blockly.Blocks.Dofrobot_left={
    init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
    .appendTitle(Blockly.MIXLY_Dofrobot_DANCINGONE)
  //  this.appendDummyInput("")
    this.setPreviousStatement(true);
    this.setNextStatement(true);
     }
     };


 Blockly.Blocks.Dofrobot_dancingthree={
init:function(){
    this.setColour(Blockly.Blocks.Dofrobot.HUE);
    this.appendDummyInput("")
      .appendTitle(Blockly.MIXLY_Dofrobot_DANCINGTHREE)
      
 // this.appendDummyInput("")
      this.setPreviousStatement(true);
       this.setNextStatement(true);
     }
     };







