var t = 500;
let fr = 1;
let txtcolor = 0;
let img;
let opacity = 150;
let data;
let pos;
let rangemin = 100;
let rangemax = 200;

function preload() {
  img = loadImage("floorplan_sample.png");
  myFont = loadFont("SpaceGrotesk-Regular.otf");
  data = loadTable("pmdata-p5-2.csv", "csv", "header");
  pos = loadTable("positions.csv", "csv", "header");
}

function setup() {
  createCanvas(windowHeight, windowHeight);
  frameRate(fr);
  textFont(myFont);
  textSize(12);
  noStroke();
  
  getData();
  setInterval(getData, 30*1000); //gets data every 30 seconds
}

function draw() {
  background(255);
  console.log(latestValue); //data stored in latestValue
  if (t<data.getRowCount()) {
  for (let j = 0; j < data.getColumnCount(); j++) {
    let x = pos.getNum(j, 1);
    let y = pos.getNum(j, 2);
    let ten = data.getNum(t, j);
    let r = map(ten, rangemin, rangemax, 0, 255);
    let b = map(ten, rangemin, rangemax, 255, 0);
    let radius = map(ten, 0, 200, 0, 200);

    // consolelog
    // console.log(t, j, ten);

    // draw ellipses
    drawingContext.filter = "blur(50px)";
    fill(r, 0, b, opacity);
    ellipse(x, y, radius);
    drawingContext.filter = "blur(0px)";
    
    // dataprint
    fill(txtcolor);
    textAlign(LEFT, CENTER);
    text("PM10", x - 35, y);
    textAlign(LEFT, CENTER);
    text(ten, x, y);
    
  }
  t++;
      } else {
        t = 0;
      }
  
  flooplan();
}

function flooplan() {
  // floorplan over everything else
  imageMode(CORNER);
  image(img, 0, 0, width, (img.height * width) / img.width); // to fit width
}
