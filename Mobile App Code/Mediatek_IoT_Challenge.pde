
import mqtt.*;
import ketai.ui.*;


KetaiVibrate vibe;
PImage bg_frame, bg_frame_tmp,photo;
String msg = " ";
String msg1 = " ";
int imagenum=0;
int door=10,glass=20,pir=30,sound=40,buzzer=0,click=0,arm=80,glassf=0,pirf=0,soundf=0;
MQTTClient client;
color red = color(235,50,50);
color green = color(50,235,50);

//udp_log = createWriter("//sdcard//utlp_log//utlp_log_"+year()+"_"+month()+"_"+day()+"//utlp_log_"+hour()+"_"+minute()+"_"+second()+".csv");

void setup()
{
  size(displayWidth, displayHeight);
  orientation(PORTRAIT);

  client = new MQTTClient(this);
  client.connect("tcp://broker.whileone.co.in");
  client.publish("test","connected");
  client.subscribe("imagesub");
  client.subscribe("sensorsub");
  
  vibe = new KetaiVibrate(this);
  bg_frame_tmp = loadImage("background.png");
  bg_frame = new PImage(width, height);
  bg_frame.copy(bg_frame_tmp, 0, 0, bg_frame_tmp.width, bg_frame_tmp.height, 0, 0, width, height);
  
}
void messageReceived(String topic, byte[] payload) {
  if(topic.equals("imagesub")==true)
  {
    imagenum++;
    saveBytes("subimg"+str(imagenum)+".jpg",payload);
    
    
    saveBytes("//sdcard//Mediatek_IoT_Challenge//IMG_"+year()+"_"+month()+"_"+day()+"//"+"IMG_"+"_"+hour()+"_"+minute()+"_"+second()+".jpg",payload);
  }
  else if(topic.equals("sensorsub")==true)
  {
    if(payload[0]=='1' && payload[1]=='0')
      door=10;
    else if(payload[0]=='1' && payload[1]=='1')
    {
      door=11;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "Alert!", "Door Opened....!!");
    }
  
    else if(payload[0]=='2' && payload[1]=='0')
      glass=20;
    else if(payload[0]=='2' && payload[1]=='1')
    {
      glass=21;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "Alert!", "Glass Break Detected....!!");
    } 
  
    else if(payload[0]=='3' && payload[1]=='0')
      pir=30;
    else if(payload[0]=='3' && payload[1]=='1')
    {
      pir=31;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "Alert!", "Motion Detected....!!");
    }
    else if(payload[0]=='4' && payload[1]=='0')
      sound=40;
    else if(payload[0]=='4' && payload[1]=='1')
    {
      sound=41;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "Alert!", "Light Triggered....!!");
    } 
    else if(payload[0]=='6' && payload[1]=='0')
    {
      click=0;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "Clicked!", "Photo Captured....!!");
    }
    else if(payload[0]=='8' && payload[1]=='0')
    {
      arm=80;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "Alert!", "System Disarmed....!!");
    }
    else if(payload[0]=='8' && payload[1]=='1')
    {
      arm=81;
      vibe.vibrate(500);
      KetaiAlertDialog.popup(this, "ALert!", "System Armed....!!");
    }
  }
  msg = new String(payload);
  println("new message: " + topic + " - " + new String(payload));
  
}
void draw()
{
  fill(color(0,0,0));
  rect(0,2.10*height/4,width-1,0.48*height);
  photo = loadImage("subimg"+str(imagenum)+".jpg");
  photo.resize((98*width/100),(44*height/100));
   background(bg_frame);
   //line(width/4, 0, width/4, height);
   //line(2*width/4, 0, 2*width/4, height);
   //line(3*width/4, 0, 3*width/4, height);
   //line(width, 0, width, height);
   //line(0, height/4, width, height/4);
   //line(0, 2*height/4, width, 2*height/4);
   //line(0, 3*height/4, width, 3*height/4);
   //line(0, height, width, height);
   textAlign(CENTER,CENTER);
   if(door==10)
     fill(green);
   else if(door==11)
     fill(red);
   rect(0.5*width/4, 1*height/4 - height/20, width/4, height/20);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("DOOR",width/4, height/4 + height/40 - height/20);
   
   if(glass==20)
     fill(green);
   else if(glass==21)
     fill(red);
   rect(2.5*width/4, 1*height/4 - height/20, width/4, height/20);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("GLASS",3*width/4, height/4 + height/40 - height/20);
  
  if(pir==30)
     fill(green);
   else if(pir==31)
     fill(red);
   rect(0.5*width/4, 1.4*height/4 - height/20, width/4, height/20);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("MOTION",width/4, height/4 +height/10+ height/40 - height/20);
   
   if(sound==40)
     fill(green);
   else if(sound==41)
     fill(red);
   rect(2.5*width/4, 1.4*height/4 - height/20, width/4, height/20);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("LIGHT",3*width/4, height/4 +height/10+ height/40 - height/20);
   
   if(buzzer==0)
     fill(green);
    else if(buzzer==1)
      fill(red);
   rect(0.5*width/4, 1.8*height/4 - height/20, width/4, height/20);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("BUZZER",width/4, height/4 + 4*height/20 + height/40 - height/20);
    
   if(click==0) 
     fill(green);
   else if(click==1)
     fill(red);
   rect(2.5*width/4, 1.8*height/4 - height/20, width/4, height/20);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("CLICK",3*width/4,  height/4 + 4*height/20 + height/40 - height/20);
   
   
   fill(color(244,151,23));
   rect(2.5*width/4, height - height/20, width/4, height/20 - 5);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("EXIT",3*width/4,  height - height/40);
   
   if(arm==80)
   {
     fill(color(244,151,23));
     rect(0.5*width/4, height - height/20, width/4, height/20 - 5 );
   fill(color(0, 0, 0));
   textSize(height/27);
   text("ARM",width/4,  height - height/40);
   }
   else if(arm==81)
   {
     fill(color(244,151,23));
     rect(0.5*width/4, height - height/20, width/4, height/20 -5);
   fill(color(0, 0, 0));
   textSize(height/27);
   text("DISARM",width/4,  height - height/40);
   }
   
   
   image(photo, 0.05*width/4,2.12*height/4 - height/20);
}

void mousePressed()
{
  if(mouseY<(2*height/4 - height/20) && mouseY>(2*height/4 - height/20 - height/20))
  {
    if(mouseX>width/8 && mouseX<3*width/8 && buzzer==0)
    {
      client.publish("sensorpub","51");
      buzzer=1;
    }
    else if(mouseX>width/8 && mouseX<3*width/8 && buzzer==1)
    {
      client.publish("sensorpub","50");
      buzzer=0;
    }
    else if(mouseX>5*width/8 && mouseX<7*width/8 && click==0)
    {
      client.publish("sensorpub","61");
      click=1;
    }  
  }
  
  if(mouseY<(2*height/4 - 3*height/20 ) && mouseY>(height/4 + height/20))
  {
    println("here");
    if(mouseX>width/8 && mouseX<3*width/8 && pir==31)
    {
      pir=30;
      println("motion");
    }
    else if(mouseX>5*width/8 && mouseX<7*width/8 && sound==41)
    {
      sound=40;
      println("sound");
    }
    
  }
  if(mouseY>(height/4 - height/20) && mouseY<(height/4 +  height/20 - height/20))
  {
    if(mouseX>width/8 && mouseX<3*width/8 && door==11)
    {
      door=10;
    }
    else if(mouseX>5*width/8 && mouseX<7*width/8 && glass==21)
    {
      glass=20;
    }
    
  }
  if(mouseY>(height - height/20 - 5))
  {
    if(mouseX>width/8 && mouseX<3*width/8 && arm==81)
    {
      client.publish("sensorpub","80");
      arm=80;
    }
    else if(mouseX>width/8 && mouseX<3*width/8 && arm==80)
    {
      client.publish("sensorpub","81");
      arm=81;
    }
    else if(mouseX>5*width/8 && mouseX<7*width/8)
    {
      exit();
    }
  }
  
}