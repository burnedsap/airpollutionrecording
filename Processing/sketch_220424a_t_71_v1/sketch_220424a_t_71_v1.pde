/*
 s0a: Main sensor PM2.5
 s0b: Main sensor PM10
 s1a: Sensor 1 PM2.5
 sab: Sensor 1 PM10
 s2a: Sensor 2 PM2.5
 s2b: Sensor 2 PM10
 s3a: Sensor 3 PM2.5
 s3b: Sensor 3 PM10
 s4a: Sensor 4 PM2.5
 s4b: Sensor 4 PM10
 s5a: Sensor 5 PM2.5
 s5b: Sensor 5 PM10
 
 Living Room: Main Sensor
 Kitchen: Sensor 1
 Bedroom 2: Sensor 2
 Bedroom 1: Sensor 3
 Bedroom 1 Purifier: Sensor 4
 Outdoor: Sensor 5
 */

import java.io.*;
import processing.javafx.*;
import hypermedia.net.*;
UDP udp;
float s0a, s0b, s1a, s1b, s2a, s2b, s3a, s3b, s4a, s4b, s5a, s5b = 0.0; //store emf readings

ArrayList<textScroll> textList;
FloatList indoorHistory;
FloatList outdoorHistory;
PFont myFont;
float meanIndoorHistory;
float meanOutdoorHistory;
int logs = 0;
String displayMessage = "";
String[] greetingMessages = {"Hey, I am S-17!", "How are you doing today?"};
float indoorHistoryTotal = 0;
float outdoorHistoryTotal = 0;
int timer=0;
int hourlyTimer = 0;
float timeBetweenLog = 4;
void setup() {
  udp = new UDP( this, 4210 );
  udp.listen( true );
  size(1464, 120);

  myFont = createFont("Space Grotesk", 120);
  textFont(myFont);
  textAlign(LEFT, CENTER);

  textList = new ArrayList<textScroll>();
  indoorHistory = new FloatList();
  outdoorHistory = new FloatList();

  selectMessage();
  if (displayMessage == "") {
    selectMessage();
  }
  textList.add(new textScroll(displayMessage.toUpperCase()));
  println(textList.size());
}

void draw() {
  background(0);
  selectMessage();
  if (millis()-timer>timeBetweenLog*1000) {
    timer=millis();
    indoorHistory.append(indoorMean());
    outdoorHistory.append(s5b);
    logs++;
  }

  if (millis()-hourlyTimer>60*60*1000) {
    indoorHistory.remove(0);
    indoorHistoryTotal = 0;
    outdoorHistory.remove(0);
    outdoorHistoryTotal = 0;
  }

  if (textList.get(textList.size()-1).yPos<20) {
    if (displayMessage!="") {
      println(displayMessage.toUpperCase());
      textList.add(new textScroll(displayMessage.toUpperCase()));
    }
  }

  for (int i = indoorHistory.size()-1; i >= 0; i--) {
    float p = indoorHistory.get(i);
    indoorHistoryTotal += p;
  }
  for (int i = outdoorHistory.size()-1; i >= 0; i--) {
    float p = outdoorHistory.get(i);
    outdoorHistoryTotal += p;
  }

  meanIndoorHistory = floor(indoorHistoryTotal/logs);
  meanOutdoorHistory = floor(outdoorHistoryTotal/logs);

  for (int i = textList.size()-1; i >= 0; i--) {
    textScroll p = textList.get(i);
    p.run();
  }
  if (textList.size()>2) {
    for (int i = textList.size()-1; i >= 0; i--) {
      textScroll p = textList.get(i);
      if (p.yPos<-500) {
        textList.remove(i);
        println("remove");
      }
    }
  }
}


void selectMessage() {
  float dice = random(6);
  if ((dice>0)&&(dice<1)) {
    displayMessage = greetingMessages[int(random(greetingMessages.length))];
  } else if ((dice>1)&&(dice<2)) {
    displayMessage = dataReporting();
  } else if ((dice>2)&&(dice<3)) {
    displayMessage = outdoorReporting();
  } else if ((dice>3)&&(dice<4)) {
    displayMessage = indoorReporting();
  } else if ((dice>4)&&(dice<5)) {
    displayMessage = recommendationReporting();
  } else if ((dice>5)&&(dice<6)) {
    if (millis()>60*60*1000) {
      displayMessage = comparisonReporting();
    }
  } else {
    displayMessage = "I am ill.";
  }
}

String outdoorReporting() {
  String outdoorVal = "";
  if ((s5b>1)&&(s5b<=50)) {
    outdoorVal = "very good. Perfect time to go for a run.";
  } else if ((s5b>50)&&(s5b<=150)) {
    outdoorVal = "good. Let's go meet some friends.";
  } else if ((s5b>150)&&(s5b<=300)) {
    outdoorVal = "quite bad. Let's stay in?";
  } else if (s5b>300) {
    outdoorVal = "very poor. Time to fire up that purifier!";
  } else {
    outdoorVal = "okay";
  }
  String outdoorValMessage = "Outdoor air looks "+outdoorVal+".";
  return outdoorValMessage;
}


String indoorReporting() {
  String indoorVal = "";
  String indoorValMessage = "";
  float indoorMeanVal = indoorMean();
  if (indoorMean()>1) {
    if ((indoorMeanVal>1)&&(indoorMeanVal<=50)) {
      indoorVal = "very good";
    } else if ((indoorMeanVal>50)&&(indoorMeanVal<=150)) {
      indoorVal = "good";
    } else if ((indoorMeanVal>150)&&(indoorMeanVal<=300)) {
      indoorVal = "bad";
    } else if (indoorMeanVal>300) {
      indoorVal = "very poor";
    } else {
      indoorVal = "okay";
    }
    indoorValMessage = indoorMeanVal+"—The air quality in the house is "+indoorVal+".";
  }
  return indoorValMessage;
}

String recommendationReporting() {
  String recommendationMessage = "";
  String safestSpace = "";
  Float[] allpm25 = {s0b, s1b, s2b, s3b};
  int minAt = 0;

  for (int i = 0; i < allpm25.length; i++) {
    minAt = allpm25[i] < allpm25[minAt] ? i : minAt;
  }

  if (s0b+s1b+s2b+s3b>4) {
    if (minAt==0) {
      safestSpace = "living room";
    } else if (minAt==1) {
      safestSpace = "Kitchen";
    } else if (minAt==2) {
      safestSpace = "bedroom 2";
    } else if (minAt==3) {
      safestSpace = "bedroom 1";
    }
    recommendationMessage = "Currently, the "+safestSpace+" seems like the safest place to be.";
  }
  return recommendationMessage;
}

String comparisonReporting() {
  String comparisonReportingMessage = "";
  float comparisonDice = random(1);

  if (comparisonDice<0.5) {
    if (indoorMean()<meanIndoorHistory) {
      comparisonReportingMessage = "In the last hour, the air quality in the house has worsened by "+int((indoorMean()/meanIndoorHistory)*100)+"%.";
    } else {
      comparisonReportingMessage = "In the last hour, the air quality in the house has improved by "+int((indoorMean()/meanIndoorHistory)*100)+"%.";
    }
  } else {
    if (s5b<meanOutdoorHistory) {
      comparisonReportingMessage = "In the last hour, the air quality outside has worsened by "+int((s5b/meanOutdoorHistory)*100)+"%.";
    } else {
      comparisonReportingMessage = "In the last hour, the air quality outside has improved by "+int((s5b/meanOutdoorHistory)*100)+"%.";
    }
  }
  return comparisonReportingMessage;
}

String dataReporting() {
  float dataDice = random(4);
  String dataReportingMessage = "";

  if (s0a+s1b+s2b+s3b+s5b>5) {
    dataReportingMessage = "PM10 LEVELS → LIVING ROOM "+s0b+" — KITCHEN "+s1b+" — BEDROOM-1 "+s2b+" — BEDROOM-2 "+s3b+" — OUTDOOR "+s5b;
  }
  return dataReportingMessage;
}

Float indoorMean() {
  float mean = floor((s0b+s1b+s2b+s3b)/4);
  return mean;
}

void receive( byte[] data, String ip, int port ) {  // <-- extended handler

  String m = new String( data ); //convert byte[] to String
  //println(m);
  String[] a = match(m, "M-PM2.5");
  if (a != null) {
    String[] c = split(m, "M-PM2.5: ");
    s0a = float(c[1]);
  }
  String[] a1 = match(m, "M-PM10");
  if (a1 != null) {
    String[] c = split(m, "M-PM10: ");
    s0b = float(c[1]);
  }
  String[] a2 = match(m, "S1-PM2.5");
  if (a2 != null) {
    String[] c = split(m, "S1-PM2.5: ");
    s1a = float(c[1]);
  }
  String[] a3 = match(m, "S1-PM10");
  if (a3 != null) {
    String[] c = split(m, "S1-PM10: ");
    s1b = float(c[1]);
  }
  String[] a4 = match(m, "S2-PM2.5");
  if (a4 != null) {
    String[] c = split(m, "S2-PM2.5: ");
    s2a = float(c[1]);
  }
  String[] a5 = match(m, "S2-PM10");
  if (a5 != null) {
    String[] c = split(m, "S2-PM10: ");
    s2b = float(c[1]);
  }
  String[] a6 = match(m, "S3-PM2.5");
  if (a6 != null) {
    String[] c = split(m, "S3-PM2.5: ");
    s3a = float(c[1]);
  }
  String[] a7 = match(m, "S3-PM10");
  if (a7 != null) {
    String[] c = split(m, "S3-PM10: ");
    s3b = float(c[1]);
  }
  String[] a8 = match(m, "S4-PM2.5");
  if (a8 != null) {
    String[] c = split(m, "S4-PM2.5: ");
    s4a = float(c[1]);
  }
  String[] a9 = match(m, "S4-PM10");
  if (a9 != null) {
    String[] c = split(m, "S4-PM10: ");
    s4b = float(c[1]);
  }
  String[] a10 = match(m, "S5-PM2.5");
  if (a10 != null) {
    String[] c = split(m, "S5-PM2.5: ");
    s5a = float(c[1]);
  }
  String[] a11 = match(m, "S5-PM10");
  if (a11 != null) {
    String[] c = split(m, "S5-PM10: ");
    s5b = float(c[1]);
  }
  //println( "receive: \""+m+"\" from "+ip+" on port "+port );
}
