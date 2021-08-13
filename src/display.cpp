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
      \
      ");
  }
  /* Connect */ {
    QObject::connect(this, &Display::SignalSetText, this, &Display::SlotGetText);
  }

  // Run Detect Thread
  flag_detect = true;
  thread_detect = new std::thread(&Display::Detect,this);

}

Display::~Display() {
  flag_detect=false;
  thread_detect->join();

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
        QString qstr_in = text_in.readAll();
        std::cout << qstr_in.toStdString() << std::endl;
        now = tmp;
        emit SignalSetText(qstr_in);
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
