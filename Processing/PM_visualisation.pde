// import UDP library
import hypermedia.net.*;
float twoFive1, ten1 = 0.0; //store pm readings
float twoFive2, ten2 = 0.0; //store pm readings
UDP udp;  // define the UDP object
float mult = 1; //multiplier to artifically bump values
FloatList twoFiveHistory1 = new FloatList();
FloatList tenHistory1 = new FloatList();
FloatList twoFiveHistory2 = new FloatList();
FloatList tenHistory2 = new FloatList();


void setup() {
  size(1440, 600);
  udp = new UDP( this, 4210 );
  udp.listen( true );

  textAlign(CENTER);
}

void draw() {
  background(240);
  
  //Remove this block if it gets in the way
  fill(51);
  text("Main PM2.5", width/2-200, height/2-120);
  text("Main PM10", width/2+200, height/2-120);
  text("Support PM2.5", width/2-200, height/2+120);
  text("Support PM10", width/2+200, height/2+120);


  noFill();
  stroke(20);

  beginShape();
  for (int i = 0; i < twoFiveHistory1.size(); i++) {
    float y = map(twoFiveHistory1.get(i), 0, 100, height/2-10, height/2-110);
    float x = map(i, 0, twoFiveHistory1.size(), width/2-400, width/2-100);
    vertex(x, y);
  }
  endShape();

  beginShape();
  for (int i = 0; i < tenHistory1.size(); i++) {
    float y = map(tenHistory1.get(i), 0, 100, height/2-10, height/2-110);
    float x = map(i, 0, tenHistory1.size(), width/2+100, width/2+400);
    vertex(x, y);
  }
  endShape();


  beginShape();
  for (int i = 0; i < twoFiveHistory2.size(); i++) {
    float y = map(twoFiveHistory2.get(i), 0, 100, height/2+10, height/2+110);
    float x = map(i, 0, twoFiveHistory2.size(), width/2-400, width/2-100);
    vertex(x, y);
  }
  endShape();

  beginShape();
  for (int i = 0; i < tenHistory2.size(); i++) {
    float y = map(tenHistory2.get(i), 0, 100, height/2+10, height/2+110);
    float x = map(i, 0, tenHistory2.size(), width/2+100, width/2+400);
    vertex(x, y);
  }
  endShape();
}

void receive(byte[] data) {

  String m = new String( data ); //convert byte[] to String

  //Format incoming data and allocate to variables
  String[] tempA = match(m, "M-PM2.5: ");
  if (tempA != null) {
    String[] c = split(m, "M-PM2.5: ");
    twoFive1 = float(c[1])*mult;
    twoFiveHistory1.append(twoFive1/mult);
  }

  String[] tempC = match(m, "M-PM10: ");
  if (tempC != null) {
    String[] c = split(m, "M-PM10: ");
    ten1 = float(c[1])*mult;
    tenHistory1.append(ten1/mult);
  }
  String[] tempA1 = match(m, "S1-PM2.5: ");
  if (tempA1 != null) {
    String[] c = split(m, "S1-PM2.5: ");
    twoFive2 = float(c[1])*mult;
    twoFiveHistory2.append(twoFive2/mult);
  }

  String[] tempC1 = match(m, "S1-PM10: ");
  if (tempC1 != null) {
    String[] c = split(m, "S1-PM10: ");
    ten2 = float(c[1])*mult;
    tenHistory2.append(ten2/mult);
  }
  println(m);

  //println( "receive: \""+a+"\" from "+ip+" on port "+port ); //debug
}
