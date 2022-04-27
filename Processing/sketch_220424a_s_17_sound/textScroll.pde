class textScroll {
  String textToDisplay;
  float scrollSpeed;
  float textLength;
  float yPos;

  textScroll(String text) {
    textToDisplay = text;
    textLength = textWidth(textToDisplay);
  }

  void run() {
    update();
    display();
  }

  void update() {
    scrollSpeed-=2;
  }

  void display() {
    textSize(70);
    fill(240, 0, 0);
    noStroke();
    yPos = width+10+scrollSpeed;
    text(textToDisplay, yPos, 50);
    if (debugMode) {
      stroke(255);
      line(yPos, 0, yPos, height);
      line(yPos+textLength, 0, yPos+textLength, height);
    }
  }
}
