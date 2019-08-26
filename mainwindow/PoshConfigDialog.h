#ifndef POSHCONFIGDIALOG_H
#define POSHCONFIGDIALOG_H

#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;
class QDialogButtonBox;

class PoshConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PoshConfigDialog(QWidget *parent = nullptr);
    ~PoshConfigDialog();

    QString& getScriptName(){return strScriptFile;}
    QString& getScriptEngine(){return strScriptEngine;}

public slots:
    void onClickedScriptFileButton();
    void accept();
    void reject();

private:
    PoshConfigDialog *ui;
    QHBoxLayout* hBox;
    QVBoxLayout* vBox;
    QLineEdit* scriptEngine;
    QLineEdit* scriptFile;
    QPushButton* scriptFileButton;

    QDialogButtonBox* buttonBox;
    QPushButton* ok;
    QPushButton* cancel;

    QString strScriptFile;
    QString strScriptEngine;

    enum {MIN_WIDTH = 600, MAX_HEIGHT=180};
};

#endif // POSHCONFIGDIALOG_H
