#ifndef WARNING_H
#define WARNING_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
signals:
    void warn();//=========================================
private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // WARNING_H
