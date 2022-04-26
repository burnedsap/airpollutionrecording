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
    yPos = width+10+scrollSpeed;
    text(textToDisplay, yPos, height/2);
  }
}
