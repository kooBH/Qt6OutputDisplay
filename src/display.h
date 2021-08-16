#include <QWidget>
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


class Display : public QWidget {
    Q_OBJECT

private :
  QVBoxLayout layout_main;

  QHBoxLayout layout_first;
  QHBoxLayout layout_second;


  QLabel label_text;
  QLabel label_emotion;
  QLabel label_face;
  std::string path_detect="../output/";

  /* params */
  const int size_text     = 64;
  const int size_emotion  = 48; 
  const int width_face = 150;
  const int height_face = 150;


  /* Detect related */
  void Detect();
  std::thread* thread_detect = nullptr;
  bool flag_detect = false;

  std::string path_happy = "../res/happy.png";
  std::string path_neutral = "../res/neutral.png";
  std::string path_angry = "../res/angry.png";

  QImage image_happy;
  QImage image_neutral;
  QImage image_angry;

  QImage image_bkgnd;
  QPixmap pixmap_bkgnd;

  enum emotion {
    EMO_HAPPY,
    EMO_NEUTRAL,
    EMO_ANGRY
  };


public :
  Display();
  ~Display();

signals:
  void SignalSetText(QString text);
  void SignalSetEmotion(QString text);
  void SignalSetFace(QString text);
public slots:
  void SlotGetText(QString text);
  void SlotGetEmotion(QString text);
  void SlotGetFace(QString text);


};
