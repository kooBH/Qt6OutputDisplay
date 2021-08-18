#include <QWidget>
#include <QPainter>
#include <QMainWindow>
#include <QPushbutton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>

#include <QTextStream>
#include <QFile>
#include <QDir>

#include <chrono>
#include <filesystem>
#include <thread>
#include <iostream>

/*
https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c/37494654#37494654
How can I get the list of files in a directory using C or C++ ? 
```
  #include <string>
  #include <iostream>
  #include <filesystem>
  namespace fs = std::filesystem;

  int main()
  {
      std::string path = "/path/to/directory";
      for (const auto & entry : fs::directory_iterator(path))
          std::cout << entry.path() << std::endl;
  }
```
*/
namespace fs = std::filesystem;


class Display : public QWidget{
    Q_OBJECT

private :
  QWidget widget_main;
  QVBoxLayout layout_main;
  QHBoxLayout layout_first;
  QHBoxLayout layout_multi;
  QHBoxLayout layout_text;
  QHBoxLayout layout_audio;


  QLabel label_text;

  QLabel label_emotion_multi;
  QLabel label_face_multi;

  QLabel label_emotion_text;
  QLabel label_face_text;

  QLabel label_emotion_audio;
  QLabel label_face_audio;
  std::string path_detect="../output/";
  std::string path_multi="../output/";
  std::string path_text="../output/";
  std::string path_audio="../output/";

  /* params */
  const int size_text     = 64;
  const int size_emotion  = 48; 
  const int width_face = 150;
  const int height_face = 150;


  /* Detect related */
  void Detect();
  std::thread* thread_detect = nullptr;
  bool flag_detect = false;

  std::string path_positive = "../res/positive.png";
  std::string path_neutral = "../res/neutral.png";
  std::string path_negative = "../res/negative.png";

  QImage image_positive;
  QImage image_neutral;
  QImage image_negative;
  QImage image_bkgnd;

  QPixmap pixmap_bkgnd;
  QPixmap pixmap_positive;
  QPixmap pixmap_neutral;
  QPixmap pixmap_negative;


  enum emotion {
    EMO_positive,
    EMO_NEUTRAL,
    EMO_negative
  };


public :
  Display();
  ~Display();

signals:
  void SignalSetText(QString text);
  void SignalSetEmotionMulti(QString text);
  void SignalSetFaceMulti(QString text);
  void SignalSetEmotionText(QString text);
  void SignalSetFaceText(QString text);
  void SignalSetEmotionAudio(QString text);
  void SignalSetFaceAudio(QString text);
public slots:
  void SlotGetText(QString text);
  void SlotGetEmotionMulti(QString text);
  void SlotGetEmotionText(QString text);
  void SlotGetEmotionAudio(QString text);
  void SlotGetFaceMulti(QString text);
  void SlotGetFaceText(QString text);
  void SlotGetFaceAudio(QString text);

protected :
  void paintEvent(QPaintEvent* p);


};
