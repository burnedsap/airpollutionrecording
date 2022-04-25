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

// import UDP library
import hypermedia.net.*;
float timeBetweenLog = 5; //seconds between each log
float s0a, s0b, s1a, s1b, s2a, s2b, s3a, s3b, s4a, s4b, s5a, s5b = 0.0; //store emf readings
PFont myFont;
PImage img;
UDP udp;  // define the UDP object
int fr = 5;
int opacity = 100;
int rangemin = 50;
int rangemax = 300;
float scaling = 2;
int hTextSeparation = 30; //Horizontal Text Separation
void setup() {
  size(1492, 1476, P2D);
  udp = new UDP( this, 4210 );
  udp.listen( true );
  img = loadImage("floorplan_sample.png");
  myFont = createFont("Space Grotesk", 62);
  textFont(myFont);
  frameRate(fr);
}

//2983 × 2952

int timer=0;
void draw() {
  background(255);
  blobs();
  image(img, 0, 0, width, height);
  textSize(12);
}



void blobs() {

  //Living Room
  noStroke();
  fill(map(s0a, rangemin, rangemax, 0, 255), 0, map(s0b, rangemin, rangemax, 0, 255), opacity);
  ellipse(width-755/scaling, height-725/scaling, map(s0a, 0, 100, 100, 250), map(s0a, 0, 100, 100, 250));
  fill(0);
  textAlign(LEFT, CENTER);
  text("PM2.5:", width-755/scaling-hTextSeparation, height-725/scaling-8);
  text("PM10:", width-755/scaling-hTextSeparation, height-725/scaling+8);
  textAlign(RIGHT, CENTER);
  text(int(s0a), width-755/scaling+hTextSeparation, height-725/scaling-8);
  text(int(s0b), width-755/scaling+hTextSeparation, height-725/scaling+8);

  //Kitchen
  noStroke();
  fill(map(s1a, rangemin, rangemax, 0, 255), 0, map(s1b, rangemin, rangemax, 0, 255), opacity);
  ellipse(width-2224/scaling, height-725/scaling, map(s1a, 0, 100, 100, 250), map(s1a, 0, 100, 100, 250));
  fill(0);
  textAlign(LEFT, CENTER);
  text("PM2.5:", width-2224/scaling-hTextSeparation, height-725/scaling-8);
  text("PM10:", width-2224/scaling-hTextSeparation, height-725/scaling+8);
  textAlign(RIGHT, CENTER);
  text(int(s1a), width-2224/scaling+hTextSeparation, height-725/scaling-8);
  text(int(s2b), width-2224/scaling+hTextSeparation, height-725/scaling+8);

  //Bedroom 2
  noStroke();
  fill(map(s2a, rangemin, rangemax, 0, 255), 0, map(s2b, rangemin, rangemax, 0, 255), opacity);
  ellipse(width-835/scaling, height-1617/scaling, map(s2a, 0, 100, 100, 250), map(s2a, 0, 100, 100, 250));
  fill(0);
  textAlign(LEFT, CENTER);
  text("PM2.5:", width-835/scaling-hTextSeparation, height-1617/scaling-8);
  text("PM10:", width-835/scaling-hTextSeparation, height-1617/scaling+8);
  textAlign(RIGHT, CENTER);
  text(int(s2a), width-835/scaling+hTextSeparation, height-1617/scaling-8);
  text(int(s2b), width-835/scaling+hTextSeparation, height-1617/scaling+8);

  //Bedroom 1
  noStroke();
  fill(map(s3a, rangemin, rangemax, 0, 255), 0, map(s3b, rangemin, rangemax, 0, 255), opacity);
  ellipse(width-2488/scaling, height-1993/scaling, map(s3a, 0, 100, 100, 250), map(s3a, 0, 100, 100, 250));
  fill(0);
  textAlign(LEFT, CENTER);
  text("PM2.5:", width-2488/scaling-hTextSeparation, height-1993/scaling-8);
  text("PM10:", width-2488/scaling-hTextSeparation, height-1993/scaling+8);
  textAlign(RIGHT, CENTER);
  text(int(s3a), width-2488/scaling+hTextSeparation, height-1993/scaling-8);
  text(int(s3b), width-2488/scaling+hTextSeparation, height-1993/scaling+8);

  //Bedroom 1 AP
  noStroke();
  fill(map(s4a, rangemin, rangemax, 0, 255), 0, map(s4b, rangemin, rangemax, 0, 255), opacity);
  ellipse(width-2313/scaling, height-1714/scaling, map(s4a, 0, 100, 100, 250), map(s4a, 0, 100, 100, 250));
  fill(0);
  textAlign(LEFT, CENTER);
  text("PM2.5:", width-2313/scaling-hTextSeparation, height-1714/scaling-8);
  text("PM10:", width-2313/scaling-hTextSeparation, height-1714/scaling+8);
  textAlign(RIGHT, CENTER);
  text(int(s4a), width-2313/scaling+hTextSeparation, height-1714/scaling-8);
  text(int(s4b), width-2313/scaling+hTextSeparation, height-1714/scaling+8);

  //Outdoor
  noStroke();
  fill(map(s5a, rangemin, rangemax, 0, 255), 0, map(s5b, rangemin, rangemax, 0, 255), opacity);
  ellipse(width-1704/scaling, height-2803/scaling, map(s5a, 0, 100, 100, 250), map(s5a, 0, 100, 100, 250));
  fill(0);
  textAlign(LEFT, CENTER);
  text("PM2.5:", width-1704/scaling-hTextSeparation, height-2803/scaling-8);
  text("PM10:", width-1704/scaling-hTextSeparation, height-2803/scaling+8);
  textAlign(RIGHT, CENTER);
  text(int(s5a), width-1704/scaling+hTextSeparation, height-2803/scaling-8);
  text(int(s5b), width-1704/scaling+hTextSeparation, height-2803/scaling+8);
  //1704 2803
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
