#ifndef ACC_H
#define ACC_H

#include <QWidget>

namespace Ui {
class acc;
}

class acc : public QWidget
{
    Q_OBJECT

public:
    explicit acc(QWidget *parent = 0);
    ~acc();

private slots:
    void on_pushButton_clicked();

private:
    Ui::acc *ui;
};

#endif // ACC_H
