// import UDP library
import hypermedia.net.*;

float s0a, s0b, s1a, s1b, s2a, s2b, s3a, s3b, s4a, s4b, s5a, s5b, s6a, s6b = 0.0; //store emf readings
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
 s6a: Sensor 6 PM2.5
 s6b: Sensor 6 PM10
 */
UDP udp;  // define the UDP object

void setup() {
  size(1400, 500);
  udp = new UDP( this, 4210 );
  udp.listen( true );
}

int timer=0;
void draw() {
  background(50);
  textSize(20);
  text("Main PM2.5: "+s0a, width*1/3, 100);
  text("Main PM10: "+s0b, width*2/3, 100);
  text("S1 PM2.5: "+s1a, width*1/3, 150);
  text("S1 PM10: "+s1b, width*2/3, 150);
  text("S2 PM2.5: "+s2a, width*1/3, 200);
  text("S2 PM10: "+s2b, width*2/3, 200);
  text("S3 PM2.5: "+s3a, width*1/3, 250);
  text("S3 PM10: "+s3b, width*2/3, 250);
  text("S4 PM2.5: "+s4a, width*1/3, 300);
  text("S4 PM10: "+s4b, width*2/3, 300);
  text("S5 PM2.5: "+s5a, width*1/3, 355);
  text("S5 PM10: "+s5b, width*2/3, 355);
  text("S6 PM2.5: "+s6a, width*1/3, 400);
  text("S6 PM10: "+s6b, width*2/3, 400);
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
  String[] a12 = match(m, "S6-PM2.5");
  if (a12 != null) {
    String[] c = split(m, "S6-PM2.5: ");
    s6a = float(c[1]);
  }
  String[] a13 = match(m, "S6-PM10");
  if (a13 != null) {
    String[] c = split(m, "S6-PM10: ");
    s6b = float(c[1]);
  }


  //println( "receive: \""+m+"\" from "+ip+" on port "+port );
}
