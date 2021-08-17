#include "display.h"

Display::Display() :QWidget() {
   /* init widgets */{
    layout_main.addStretch();
    
    layout_first.addWidget(&label_text,Qt::AlignCenter);

    layout_main.addLayout(&layout_first);

    layout_second.addStretch();
    layout_second.addWidget(&label_emotion);
    layout_second.addWidget(&label_face);
    layout_second.addStretch();
    layout_main.addLayout(&layout_second);


    layout_main.addStretch();

    //widget_main.setLayout(&layout_main);
    //this->setCentralWidget(&widget_main);

    setLayout(&layout_main);




  }

   /* load resoruces*/
   {
     image_positive.load("../res/positive.png");
     image_neutral.load("../res/neutral.png");
     image_negative.load("../res/negative.png");

     pixmap_positive = QPixmap::fromImage(image_positive).scaled(width_face, height_face, Qt::KeepAspectRatio, Qt::SmoothTransformation);
     pixmap_neutral = QPixmap::fromImage(image_neutral).scaled(width_face, height_face, Qt::KeepAspectRatio, Qt::SmoothTransformation);
     pixmap_negative = QPixmap::fromImage(image_negative).scaled(width_face, height_face, Qt::KeepAspectRatio, Qt::SmoothTransformation);
     
     // NOTE :: jpg file didn't loaded well.
     pixmap_bkgnd.load("../res/background.png");
   }

  /* Configure Widgets */ {
    //setFixedSize(QSize(600, 400));

     QFont font_text = label_text.font();
     font_text.setPointSize(size_text);
     label_text.setFont(font_text);
     label_text.setAlignment(Qt::AlignCenter);

     label_text.setWordWrap(true);

     QFont font_emotion = label_emotion.font();
     font_emotion.setPointSize(size_emotion);
     label_emotion.setFont(font_emotion);

     label_text.setText("label_text");
     label_emotion.setText("label_emotion");

     // full screen
     setWindowState(Qt::WindowMaximized);
     
     //QApplication::desktop();

    // label_text.setFixedSize(QSize(this->width(),400));

    // TODO - 
    label_face.setPixmap(QPixmap::fromImage(image_neutral).scaled(width_face,height_face,Qt::KeepAspectRatio));

     /*
			QWidget{background:rgb(153, 189, 138);}\
      QWidget{background-image:url(../../res/background.jpg)}\
      QLabel{background:white;border: 1px solid black;}\
     */
     // QApp{background-image:url(../../res/background.jpg)}\
     //  background-image:url(../res/background.jpg);background-postion : center;

    //this->centralWidget()->setStyleSheet("

    //palette.setBrush(this->backgroundRole(), QBrush(pixmap_bkgnd));
    //setPalette(palette);

   
  }
  /* Connect */ {
    QObject::connect(this, &Display::SignalSetText, this, &Display::SlotGetText);
    QObject::connect(this, &Display::SignalSetEmotion, this, &Display::SlotGetEmotion);
    QObject::connect(this, &Display::SignalSetFace, this, &Display::SlotGetFace);
  }

  // Run Detect Thread
  flag_detect = true;
  thread_detect = new std::thread(&Display::Detect,this);

}

void Display::paintEvent(QPaintEvent* p) {
    QPixmap tmp = pixmap_bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPainter painter(this);
    painter.drawPixmap(0, 0, tmp);

    QWidget::paintEvent(p);


}

Display::~Display() {
  flag_detect=false;
  if (thread_detect) {
    thread_detect->join();
    delete thread_detect;
  }

}

/* Detect Newly Created Text File and Display */
void Display::Detect() {
  QString text;

 auto now = std::chrono::system_clock::now().time_since_epoch().count();

  while (flag_detect) {
    for (const auto& entry : fs::directory_iterator(path_detect)) {
      //std::cout << entry.path() << " | " << entry.last_write_time().time_since_epoch() << std::endl;
      std::cout << entry.path() << " | " << std::endl;
      auto tmp = entry.last_write_time().time_since_epoch().count();
      if (tmp > now) {
        QFile file_in(entry.path());
        if (!file_in.open(QIODevice::ReadOnly)) {
          std::cout<<"ERROR::"<< file_in.errorString().toStdString()<<std::endl;
        }

        QTextStream text_in(&file_in);
        QString qstr_in = text_in.readLine();
        std::cout << qstr_in.toStdString() << std::endl;
        now = tmp;
        emit SignalSetText(qstr_in);

        // emotion
        qstr_in = text_in.readLine();
        std::cout << qstr_in.toStdString() << std::endl;
        emit SignalSetEmotion(qstr_in);
        emit SignalSetFace(qstr_in);

        file_in.remove();
      }
    }

    std::cout << "===== loop ====="<<std::endl;

    //emit SignalSetText(text);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }


}

void Display::SlotGetText(QString text) {
  label_text.setText(text);
}

void Display::SlotGetEmotion(QString text) {
  label_emotion.setText(text);
}

void Display::SlotGetFace(QString text) {
  if (!QString::compare("positive", text)) {
    label_face.setPixmap(pixmap_positive);
  }
  else if (!QString::compare("neutral", text)) {
    label_face.setPixmap(pixmap_neutral);
  }
  else if (!QString::compare("negative", text)) {
    label_face.setPixmap(pixmap_negative);
  }
}
