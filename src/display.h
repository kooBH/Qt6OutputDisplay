#include <QWidget>
#include <QPushbutton>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QTextBrowser>

#include <QTextStream>
#include <QFile>

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
  QLabel label_text;
  std::string path_detect="../output/";

  /* Detect related */
  void Detect();
  std::thread* thread_detect = nullptr;
  bool flag_detect = false;

public :
  Display();
  ~Display();

signals:
  void SignalSetText(QString text);
public slots:
  void SlotGetText(QString text);
};
