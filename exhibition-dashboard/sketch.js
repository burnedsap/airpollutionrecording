var t = 500;
let fr = 1;
let txtcolor = 0;
let img;
let opacity = 150;
let data;
let pos;
let rangemin = 0;
let rangemax = 255;

// Variables for ellipse/text

// let kitchenX = 170;
// let kitchenY = 450;
// let livingX = 420;
// let livingY = 450;
// let bed2X = 420;
// let bed2Y = 250;
// let bed1X = 70;
// let bed1Y = 170;
// let bed1pX = 130 ;
// let bed1pY = 250;
// let outX = 250;
// let outY = 70;
// let radius = 200;

let kitchenX = 235;
let kitchenY = 530;
let livingX = 485;
let livingY = 540;
let bed2X = 485;
let bed2Y = 330;
let bed1X = 135;
let bed1Y = 250;
let bed1pX = 195 ;
let bed1pY = 330;
let outX = 315;
let outY = 150;

function preload() {
  img = loadImage("https://cdn.glitch.global/6a3f9270-e448-4d54-b3e4-c85347222a9e/floorplan_sample.png?v=1651011772763");
  myFont = loadFont("https://cdn.glitch.global/6a3f9270-e448-4d54-b3e4-c85347222a9e/SpaceGrotesk-Bold.ttf?v=1651035358982");
  // data = loadTable("pmdata-p5-2.csv", "csv", "header");
  // pos = loadTable("positions.csv", "csv", "header");
}

function setup() {
  createCanvas(windowHeight, windowHeight);
  frameRate(fr);
  textFont(myFont);
  textSize(10);
  noStroke();
  pixelDensity(5);
  getData();
  setInterval(getData, 30000); //gets data every 30 seconds
}

function draw() {
  background(0);
  console.log(latestValue); //data stored in latestValue

  drawingContext.filter = "blur(40px)";

  //Living Room Ms
  if (latestValue[3] > 0) {
  fill(map(latestValue[3], rangemin, rangemax, 0, 255), 0, map(latestValue[3], rangemin, rangemax, 255, 0));
  ellipse(livingX, livingY, map(latestValue[3], 0, 300, 100, 300));
    }

  //Kitchen s1
  if (latestValue[5] > 0) {
  fill(map(latestValue[5], rangemin, rangemax, 0, 255), 0, map(latestValue[5], rangemin, rangemax, 255, 0));
  ellipse(kitchenX, kitchenY, map(latestValue[5], 0, 300, 100, 300));
  }

  //Bedroom 2 s2
  if (latestValue[7] > 0) {
  fill(map(latestValue[7], rangemin, rangemax, 0, 255), 0, map(latestValue[7], rangemin, rangemax, 255, 0));
  ellipse(bed2X, bed2Y, map(latestValue[7], 0, 300, 100, 300));
  }

  //Bedroom 1 s3
  if (latestValue[9] > 0) {
  fill(map(latestValue[9], rangemin, rangemax, 0, 255), 0, map(latestValue[9], rangemin, rangemax, 255, 0));
  ellipse(bed1X, bed1Y, map(latestValue[9], 0, 300, 100, 300));
  }

  //Bedroom 1 Purifier s4
  if (latestValue[11] > 0) {
  fill(map(latestValue[11], rangemin, rangemax, 0, 255), 0, map(latestValue[11], rangemin, rangemax, 255, 0));
  ellipse(bed1pX, bed1pY, map(latestValue[11], 0, 300, 100, 300));
  }

  //Outdoor s5
  if (latestValue[13] > 0) {
  fill(map(latestValue[13], rangemin, rangemax, 0, 255), 0, map(latestValue[13], rangemin, rangemax, 255, 0));
  ellipse(outX, outY, map(latestValue[13], 0, 300, 100, 300));
  }

  drawingContext.filter = "blur(0px)";

    //Living
  if((latestValue[2]||latestValue[3]) > 0) {
    fill(255);
    textAlign(RIGHT, CENTER);
    text("PM 2.5", livingX - 7, livingY-7);
    text("PM 10", livingX - 7, livingY+7);
    textAlign(LEFT, CENTER);
    text(latestValue[2], livingX+7, livingY-7);
    text(latestValue[3], livingX+7, livingY+7);
  }

  // Kitchen Text
  if((latestValue[4]||latestValue[5]) > 0) {
    fill(255);
    textAlign(RIGHT, CENTER);
    text("PM 2.5", kitchenX - 7, kitchenY-7);
    text("PM 10", kitchenX - 7, kitchenY+7);
    textAlign(LEFT, CENTER);
    text(latestValue[4], kitchenX+7, kitchenY-7);
    text(latestValue[5], kitchenX+7, kitchenY+7);
  }

   // Bedroom2 Text
  if((latestValue[6]||latestValue[7]) > 0) {
    fill(255);
    textAlign(RIGHT, CENTER);
    text("PM 2.5", bed2X - 7, bed2Y-7);
    text("PM 10", bed2X - 7, bed2Y+7);
    textAlign(LEFT, CENTER);
    text(latestValue[6], bed2X+7, bed2Y-7);
    text(latestValue[7], bed2X+7, bed2Y+7);
  }

  //Bedroom 1 text
    if((latestValue[8]||latestValue[9]) > 0) {
      fill(255);
      textAlign(RIGHT, CENTER);
      text("PM 2.5", bed1X - 7, bed1Y-7);
      text("PM 10", bed1X - 7, bed1Y+7);
      textAlign(LEFT, CENTER);
      text(latestValue[8], bed1X+7, bed1Y-7);
      text(latestValue[9], bed1X+7, bed1Y+7);
  }

  //Bedroom 1 Purifier text
    if((latestValue[10]||latestValue[11]) > 0) {
      fill(255);
      textAlign(RIGHT, CENTER);
      text("PM 2.5", bed1pX - 7, bed1pY-7);
      text("PM 10", bed1pX - 7, bed1pY+7);
      textAlign(LEFT, CENTER);
      text(latestValue[10], bed1pX+7, bed1pY-7);
      text(latestValue[11], bed1pX+7, bed1pY+7);
  }



   //Outdoor text
    if((latestValue[12]||latestValue[13]) > 0) {
      fill(255);
      textAlign(RIGHT, CENTER);
      text("PM 2.5", outX - 7, outY-7);
      text("PM 10", outX - 7, outY+7);
      textAlign(LEFT, CENTER);
      text(latestValue[12], outX+7, outY-7);
      text(latestValue[13], outX+7, outY+7);
  }


var timeString = latestValue[1];
  var dateString = latestValue[0].substring(6,10)+"-"+latestValue[0].substring(3,5)+"-"+latestValue[0].substring(0,2)+"T";
var datetime = dateString + timeString;

  var dateUTC = new Date(datetime);
var dateUTC = dateUTC.getTime()
var dateIST = new Date(dateUTC);
  dateIST.setHours(dateIST.getHours() + 9);
dateIST.setMinutes(dateIST.getMinutes() + 30);

    let indTime = dateIST.getHours()+":"+dateIST.getMinutes()+":"+dateIST.getSeconds();

  fill(255);
  textAlign(RIGHT);
  text("Last updated at: "+indTime, 662, 665);

  //flooplan();
}

function flooplan() {
  // floorplan over everything else
  imageMode(CORNER);
  // image(img, 0, 0, 560, 550);
  let scl = 5;
  image(img, 100, 75, 2983/scl, 2952/scl);
  //2983 x 2952
}



/* CODE SNIPPET ARCHIVE

//   if (t<data.getRowCount()) {
//   for (let j = 0; j < data.getColumnCount(); j++) {
//     let x = pos.getNum(j, 1);
//     let y = pos.getNum(j, 2);
//     let ten = data.getNum(t, j);
//     let r = map(ten, rangemin, rangemax, 0, 255);
//     let b = map(ten, rangemin, rangemax, 255, 0);
//     let radius = map(ten, 0, 200, 0, 200);

//     // consolelog
//     // console.log(t, j, ten);

//     // draw ellipses
//     drawingContext.filter = "blur(50px)";
//     fill(r, 0, b, opacity);
//     ellipse(x, y, radius);
//     drawingContext.filter = "blur(0px)";

//     // dataprint
//     fill(txtcolor);
//     textAlign(LEFT, CENTER);
//     text("PM10", x - 35, y);
//     textAlign(LEFT, CENTER);
//     text(ten, x, y);

//   }
//   t++;
//       } else {
//         t = 0;
//       }
*/
