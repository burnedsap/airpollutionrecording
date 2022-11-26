import processing.javafx.*;
import hypermedia.net.*;

float minVal = 0; //write down the minimal value
float maxVal = 15; //write down the max value
float refreshTime = 4000; //time between messages

PFont pixelFont, monoFont;
float pm2, pm10;
float twoFive, ten = 0.0;
UDP udp;  // define the UDP object

FloatList twoFiveHistory = new FloatList();
FloatList tenHistory = new FloatList();

float angle = 0;
int h, m, d, mo, y;
float eyeonex, eyetwox, eyey;
float FL;
int index = 0;
int state = 0;
String status;
String[] messages;


//String[] messages = {
//  "HELLO, I AM T-71. IT'S NICE TO MEET YOU!",
//  "LET’S LOOK AT THE AIR QUALITY ON THIS STREET.",
//  "IT SEEMS TO BE GETTING QUITE DUSTY AROUND HERE.",
//  "WOAH, ACCORDING TO MY SENSOR, THE PM10 LEVEL IS " + pm10 + ".",
//  "THAT'S QUITE " + status + "."
//};

float w; // Width of entire wave, in pixels
float theta = 0; // Start angle at 0
float amplitude = 1.2; // How high is the wave
float period = 6; // How many pixels before the wave repeats
float dx; // Value for incrementing x
float[] yvalues; // Using an array to store height values for the wave
float xspacing = 2; // How smooth the curve should be. Small value=more smooth

float timer = 0;


void setup() {
  //size(1280, 720, P2D); //swap 'P2D' for 'FX2D' for faster+sharper rendering
  fullScreen(FX2D);
  udp = new UDP( this, 4210 );
  udp.listen( true );
  updateStatus();

  // calculations for drawing the sin wave
  w = width / 1.5;
  dx = (TWO_PI / period) * xspacing;
  yvalues = new float[floor(w / xspacing)];

  //  // placement of eyes
  eyeonex = width / 2 - 120;
  eyetwox = width / 2 + 120;
  eyey = height / 2 - 100;
  
  //pixelFont = createFont("PPNeueBit-Regular.otf", 128);
  pixelFont = createFont("PPMondwest-Bold.otf", 128);
  
  //monoFont = createFont("PPNeueMachina-Light.otf", 128);
  monoFont = createFont("PPNeueMachina-Regular.otf", 128);
  //monoFont = createFont("PPNeueMachina-Ultrabold.otf", 128);
  //monoFont = createFont("PPNeueMachina-Ultralight.otf", 128);
}

void draw() {

  String[] messages1 = {
    "HELLO, I AM T-71. IT'S NICE TO MEET YOU!",
    "LET’S SEE HOW WE ARE DOING TODAY",
    "I FEEL LIKE THE AIR TODAY IS " + status +"",
    "DO YOU AGREE?",
    "THE WORLD HEALTH ORGANISATION SURE DOESN'T THINK SO.",
    "THE PM2.5 IS " + int(pm2/5) + " TIMES HIGHER THAN WHAT W.H.O RECOMMENDS",
    "WHO DECIDES WHAT IS HEALTHY AIR AND WHAT IS NOT ANYWAY?",
    "WHAT DO YOU THINK?",
    "DOES THE BLUE SKY LIE TO US?",
    "I THINK IT SOMETIMES DOES",

    "IF YOUR SPACES COULD SPEAK, WHAT WOULD THEY SAY?",
    "HELLO, I AM T-71. YOU ARE STILL HERE!",
    "MY EMOTION REFLECTS THE STATE OF AIR IN THIS ROOM",
    "IT'S BEEN RAINING THIS PAST WEEK SO TBH I HAVE BEEN FEELING GOOD",
    "I HAVE ALWAYS LIKED RAIN",
    "DON'T CARE MUCH FOR FLOODS",
    "I FEEL LIKE THE AIR TODAY IS " + status +"",
    "ACCORDING TO MY READINGS, THE PM10 LEVEL IS " + pm10 + ".",
    "THE PM2.5 LEVEL IS " + pm2 + ".",
    "BTW, I AM CURIOUS..",
    "IS THE AIR CLEANER INDOORS THAN WHAT IT IS OUTDOORS?",
    "HOW IS IT IN THE GALLERY?",
    "TRY MOVING THE AIR QUALITY SENSOR AROUND TO SEE FOR YOURSELF"
  };
  messages = messages1;

  if (millis()-timer > refreshTime) {
    changeWord();
    println("change");
    timer = millis();
  }
  background(0);

  h = hour();
  m = minute();
  d = day();
  mo = month();
  y = year();


  // calculating float
  FL = sin(angle) * 10;
  angle += 0.03;
  pushMatrix();
  translate(0, FL);

  // draw eyes
  float r = map(sin(angle), -1, 1, 160, 150);
  noStroke();
  //fill(26, 207, 237);
  fill(255);
  ellipse(eyeonex, eyey, r, r);
  ellipse(eyetwox, eyey, r, r);

  // expression based on PM levels
  fill(0);
  if (pm10 >= 10 && pm10 < 50) {
    // sad ellipses
    float sad = map(pm10, 10, 50, 140, 350);
    ellipse(eyeonex, eyey - 140, 300, sad);
    ellipse(eyetwox, eyey - 140, 300, sad);
  } else if (pm10 < 10) {
    // happy ellipses
    float happy = map(pm10, 0, 10, 360, 200);
    ellipse(eyeonex, eyey + 140, 300, happy);
    ellipse(eyetwox, eyey + 140, 300, happy);
  } else if (pm10 >= 50) {
    // static expression
    ellipse(eyeonex, eyey - 140, 300, 350);
    ellipse(eyetwox, eyey - 140, 300, 350);
  }

  translate(0, -FL);

  // digital wave effect
  stroke(0);
  noFill();
  strokeWeight(1.5);
  calcWave(); //calculate the points on the wave
  renderWave(); //draw the wave

  // messages
  fill(255);
  textAlign(CENTER, CENTER);
  //  textFont(andalemono);
  textFont(pixelFont);
  textSize(34);
  text(messages[index], width/2, height - 300, width - 300);

  // PM readings display
  textFont(monoFont);
  textSize(18);
  textAlign(LEFT, BOTTOM);
  fill(#54FFAC);
  text("PM2.5: " + twoFive, 40, height - 20);
  textAlign(LEFT, BOTTOM);
  fill(#E84B35);
  text("PM10: " + ten, width-120, height - 20);
  //text(
  //  "" + d + "/" + mo + "/" + y + " | " + h + ":" + m + "",
  //  width - 20,
  //  height - 20
  //  );

  strokeWeight(2);
  popMatrix();
  noFill();
  //stroke(0, 50, 200, 150);
  stroke(#54FFAC, 150);
  fill(#54FFAC, 50);
  beginShape();
  for (int i = 0; i < twoFiveHistory.size(); i++) {
    float y = map(twoFiveHistory.get(i), minVal, maxVal, height-60, height-100);
    float x = map(i, 0, twoFiveHistory.size(), 10, width-10);
    curveVertex(x, y);
  }
  endShape();
  //stroke(200, 50, 0, 150);
  stroke(#E84B35, 150);
  fill(#E84B35, 50);
  beginShape();
  for (int i = 0; i < tenHistory.size(); i++) {
    float y = map(tenHistory.get(i), minVal, maxVal, height-60, height-100);
    float x = map(i, 0, tenHistory.size(), 10, width-10);
    curveVertex(x, y);
  }
  endShape();

  //pm10++;
  //print(index, pm10, reading2, state);
}


void changeWord() {
  index++;
  if (index >= messages.length) {
    index = 0;
  }
  updateStatus();
}

void updateStatus() {
  // update readings and status
  pm2 = twoFive;
  pm10 = ten;
  if (pm10 <= 25) {
    state = 1;
    status = "PRETTY GOOD";
  } else if (pm10 <= 50) {
    state = 2;
    status = "OKAY ENOUGH";
  } else if (pm10 <= 100) {
    state = 3;
    status = "A BIT BAD, COUGH!";
  } else if (pm10 <= 200) {
    state = 4;
    status = "QUITE UNHEALTHY, I DON'T KNOW HOW YOU ARE DOING IT";
  } else if (pm10 > 200) {
    state = 5;
    status = "SEVERE, I DON'T THINK WE SHOULD BE DOING THIS";
  }
}
//functions to calculate and draw the wave
void calcWave() {
  // Increment theta to animate the wave (try different values for different speeds)
  theta += 5; // animation speed

  // For every x value, calculate a y value with sine function
  float x = theta;
  for (int i = 0; i < yvalues.length; i++) {
    yvalues[i] = sin(x) * amplitude;
    x += dx;
  }
}

void renderWave() {
  for (int i = 1; i < 30; i++) {
    beginShape();
    // A simple way to draw the wave with an ellipse at each location
    for (int x = 0; x < yvalues.length; x++) {
      vertex(width / 6 + x * xspacing, height / 2 - i * 7 + yvalues[x]);
    }
    endShape();
  }
}



void receive(byte[] data) {

  String m = new String( data ); //convert byte[] to String

  //Format incoming data and allocate to variables
  String[] tempA = match(m, "M-PM2.5: ");
  if (tempA != null) {
    String[] c = split(m, "M-PM2.5: ");
    twoFive = float(c[1]);
    twoFiveHistory.append(twoFive);
  }

  String[] tempC = match(m, "M-PM10: ");
  if (tempC != null) {
    String[] c = split(m, "M-PM10: ");
    ten = float(c[1]);
    tenHistory.append(ten);
  }
  //println(m);

  //println( "receive: \""+a+"\" from "+ip+" on port "+port ); //debug
}
