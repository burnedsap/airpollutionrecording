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
    scrollSpeed+=1;
  }

  void display() {
    textSize(40);
    fill(240, 0, 0);
    yPos = -30-textLength+scrollSpeed;
    text(textToDisplay, yPos, height/2);
  }
}
