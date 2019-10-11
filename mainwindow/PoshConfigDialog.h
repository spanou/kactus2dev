#ifndef POSHCONFIGDIALOG_H
#define POSHCONFIGDIALOG_H

#include <QDialog>

class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;
class QDialogButtonBox;
class QSettings;

class PoshConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PoshConfigDialog(QSettings& settings, QWidget *parent = nullptr);
    ~PoshConfigDialog();
    QSettings& getSettings(){return poshConfigSettings;}

public slots:
    void onClickedScriptFileButton();
    void accept();
    void reject();

private:
    PoshConfigDialog *ui;
    QHBoxLayout* hBox1;
    QHBoxLayout* hBox2;
    QHBoxLayout* hBox3;
    QVBoxLayout* vBox;
    QLineEdit* scriptEngine;
    QLineEdit* scriptArgs;
    QLineEdit* scriptFile;
    QPushButton* scriptFileButton;

    QDialogButtonBox* buttonBox;
    QPushButton* ok;
    QPushButton* cancel;

    QSettings& poshConfigSettings;

    enum {MIN_WIDTH = 800, MAX_HEIGHT=180, LABEL_WIDTH=120};
};

#endif // POSHCONFIGDIALOG_H
