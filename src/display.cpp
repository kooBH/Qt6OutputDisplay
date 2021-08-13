#include "display.h"

Display::Display() :QWidget() {
  /* init widgets */{
    layout_main.addWidget(&label_text);

    setLayout(&layout_main);
  }

  /* Configure Widgets */ {
    //setFixedSize(QSize(600, 400));

    setStyleSheet("\
			QWidget{background:rgb(153, 189, 138);}\
      QLabel{background:white;border: 1px solid black;}\
      QLabel:disabled{color:gray;}\
      \
      ");
  }
  /* Connect */ {
    QObject::connect(this, &Display::SignalSetText, this, &Display::SlotGetText);
  }

  // Run Detect Thread

}

Display::~Display() {

}

/* Detect Newly Created Text File and Display */
void Display::Detect() {
  QString text;



  emit SignalSetText(text);
}

void Display::SlotGetText(QString text) {
  label_text.setText(text);
}
