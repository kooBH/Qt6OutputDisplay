#include "display.h"

Display::Display() :QWidget() {
   /* init widgets */{
    layout_main.addStretch();
    
    layout_first.addWidget(&label_text,Qt::AlignCenter);

    layout_main.addLayout(&layout_first);

    layout_multi.addStretch();
    layout_multi.addWidget(&label_emotion_multi);
    layout_multi.addWidget(&label_face_multi);
    layout_multi.addStretch();

    layout_multi.addStretch();
    layout_multi.addWidget(&label_emotion_multi);
    layout_multi.addWidget(&label_face_multi);
    layout_multi.addStretch();

    layout_multi.addStretch();
    layout_multi.addWidget(&label_emotion_multi);
    layout_multi.addWidget(&label_face_multi);
    layout_multi.addStretch();

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
     label_text.setText("label_text");

     QFont font_emotion;

     font_emotion = label_emotion_multi.font();
     font_emotion.setPointSize(size_emotion);
     label_emotion_multi.setFont(font_emotion);
     label_emotion_multi.setText("label_emotion");

     font_emotion = label_emotion_text.font();
     font_emotion.setPointSize(size_emotion);
     label_emotion_text.setFont(font_emotion);
     label_emotion_text.setText("label_emotion");

     font_emotion = label_emotion_audio.font();
     font_emotion.setPointSize(size_emotion);
     label_emotion_audio.setFont(font_emotion);
     label_emotion_audio.setText("label_emotion");

     // full screen
     setWindowState(Qt::WindowMaximized);
     
     //QApplication::desktop();

    // label_text.setFixedSize(QSize(this->width(),400));

    // TODO - 
     label_face_multi.setPixmap(QPixmap::fromImage(image_neutral).scaled(width_face,height_face,Qt::KeepAspectRatio));
     label_face_text.setPixmap(QPixmap::fromImage(image_neutral).scaled(width_face,height_face,Qt::KeepAspectRatio));
     label_face_audio.setPixmap(QPixmap::fromImage(image_neutral).scaled(width_face,height_face,Qt::KeepAspectRatio));

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
    QObject::connect(this, &Display::SignalSetEmotionMulti, this, &Display::SlotGetEmotionMulti);
    QObject::connect(this, &Display::SignalSetFaceMulti, this, &Display::SlotGetFaceMulti);
    QObject::connect(this, &Display::SignalSetEmotionText, this, &Display::SlotGetEmotionText);
    QObject::connect(this, &Display::SignalSetFaceText, this, &Display::SlotGetFaceText);
    QObject::connect(this, &Display::SignalSetEmotionAudio, this, &Display::SlotGetEmotionAudio);
    QObject::connect(this, &Display::SignalSetFaceAudio, this, &Display::SlotGetFaceAudio);
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
   //std::cout << entry.path() << " | " << entry.last_write_time().time_since_epoch() << std::endl;
    for (const auto& entry : fs::directory_iterator(path_detect)) {
      //std::cout << entry.path() << " | " << entry.last_write_time().time_since_epoch() << std::endl;
      std::cout << entry.path() << " | " << std::endl;
      auto tmp = entry.last_write_time().time_since_epoch().count();

      if (tmp > now) {

        QFile file_in(entry.path());
        now = tmp;

        //multi
        QFile file_in_multi(QString::fromStdString(path_multi));
        QTextStream text_in_multi(&file_in_multi);
        QString qstr_in_multi = text_in_multi.readLine();
        emit SignalSetText(qstr_in_multi);

        // text
        QFile file_in_text(QString::fromStdString(path_text));
        QTextStream text_in_text(&file_in_text);
        QString qstr_in_text = text_in_text.readLine();

        //audio
        QFile file_in_audio(QString::fromStdString(path_audio));
        QTextStream text_in_audio(&file_in_audio);
        QString qstr_in_audio = text_in_audio.readLine();

        emit SignalSetEmotionMulti(qstr_in_multi);
        emit SignalSetFaceMulti(qstr_in_multi);
        emit SignalSetEmotionText(qstr_in_text);
        emit SignalSetFaceText(qstr_in_text);
        emit SignalSetEmotionAudio(qstr_in_audio);
        emit SignalSetFaceAudio(qstr_in_audio);

        file_in.remove();
      }
    }

   /*
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

        emit SignalSetEmotionMulti(qstr_in);
        emit SignalSetFaceMulti(qstr_in);
        file_in.remove();
      }
    }
    */
    std::cout << "===== loop ====="<<std::endl;

    //emit SignalSetText(text);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }


}

void Display::SlotGetText(QString text) {
  label_text.setText(text);
}

void Display::SlotGetEmotionMulti(QString text) {
  label_emotion_multi.setText(text);
}

void Display::SlotGetFaceMulti(QString text) {
  if (!QString::compare("positive", text)) {
    label_face_multi.setPixmap(pixmap_positive);
  }
  else if (!QString::compare("neutral", text)) {
    label_face_multi.setPixmap(pixmap_neutral);
  }
  else if (!QString::compare("negative", text)) {
    label_face_multi.setPixmap(pixmap_negative);
  }
}
void Display::SlotGetEmotionText(QString text) {
  label_emotion_text.setText(text);
}

void Display::SlotGetFaceText(QString text) {
  if (!QString::compare("positive", text)) {
    label_face_text.setPixmap(pixmap_positive);
  }
  else if (!QString::compare("neutral", text)) {
    label_face_text.setPixmap(pixmap_neutral);
  }
  else if (!QString::compare("negative", text)) {
    label_face_text.setPixmap(pixmap_negative);
  }
}

void Display::SlotGetEmotionAudio(QString text) {
  label_emotion_audio.setText(text);
}

void Display::SlotGetFaceAudio(QString text) {
  if (!QString::compare("positive", text)) {
    label_face_audio.setPixmap(pixmap_positive);
  }
  else if (!QString::compare("neutral", text)) {
    label_face_audio.setPixmap(pixmap_neutral);
  }
  else if (!QString::compare("negative", text)) {
    label_face_audio.setPixmap(pixmap_negative);
  }
}


