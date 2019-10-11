#include "PoshConfigDialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QSettings>
#include <QLabel>



PoshConfigDialog::PoshConfigDialog( QSettings& settings, QWidget *parent) :
    QDialog(parent),
    poshConfigSettings(settings){

    scriptEngine = new QLineEdit;
    scriptFile = new QLineEdit;
    scriptArgs = new QLineEdit;
    scriptFileButton = new QPushButton;

    scriptFileButton->setText(tr(".."));

    QString strScriptEngineSetting = poshConfigSettings.value(
      "poshSettings/scriptEngine", QVariant("/bin/bash -c")).toString();

    if(strScriptEngineSetting.length() == 0){
      strScriptEngineSetting = "/bin/bash -c";
    }
    scriptEngine->setPlaceholderText(strScriptEngineSetting);
    scriptEngine->setText(strScriptEngineSetting);

    QString scritFileSetting = poshConfigSettings.value(
      "poshSettings/scriptFile", QVariant("pysimgen")).toString();

    if(scritFileSetting.length() == 0){
      scritFileSetting = "pysimgen";
    }

    scriptFile->setPlaceholderText(
      tr("Click on the button on the right to select a script to run.."));
    scriptFile->setText(scritFileSetting);

    QString scriptArgsSetting = poshConfigSettings.value(
      "poshSettings/scriptArgs", QVariant("-o /tmp/outdir --build --run")).toString();

    if(scriptArgsSetting.length() == 0){
      scriptArgsSetting = "-o /tmp/outdir --build --run";
    }
    scriptArgs->setPlaceholderText(tr("Place the script arguments here.."));
    scriptArgs->setText(scriptArgsSetting);

    scriptFile->setCursorPosition(0);
    scriptArgs->setCursorPosition(0);
    scriptEngine->setCursorPosition(0);

    QLabel* lblScriptFile = new QLabel(this);
    lblScriptFile->setText("Script File: ");
    lblScriptFile->setFixedWidth(LABEL_WIDTH);

    QLabel* lblScriptArgs = new QLabel(this);
    lblScriptArgs->setText("Script Arguments: ");
    lblScriptArgs->setFixedWidth(LABEL_WIDTH);

    QLabel* lblScriptEngine = new QLabel(this);
    lblScriptEngine->setText("Script Engine: ");
    lblScriptEngine->setFixedWidth(LABEL_WIDTH);

    vBox = new QVBoxLayout;
    hBox1 = new QHBoxLayout;
    hBox2 = new QHBoxLayout;
    hBox3 = new QHBoxLayout;

    hBox1->addWidget(lblScriptFile);
    hBox1->addWidget(scriptFile);
    hBox1->addWidget(scriptFileButton);

    hBox2->addWidget(lblScriptArgs);
    hBox2->addWidget(scriptArgs);

    hBox3->addWidget(lblScriptEngine);
    hBox3->addWidget(scriptEngine);

    vBox->addLayout(hBox1);
    vBox->addLayout(hBox2);
    vBox->addLayout(hBox3);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    ok = buttonBox->button(QDialogButtonBox::Ok);
    ok->setDefault(true);
    ok->setAutoDefault(true);

    cancel = buttonBox->button(QDialogButtonBox::Cancel);
    cancel->setDefault(false);
    cancel->setAutoDefault(false);

    vBox->addWidget(buttonBox);

    QObject::connect(scriptFileButton, SIGNAL(clicked()), this, SLOT(onClickedScriptFileButton()));
    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));

    setLayout(vBox);
    setWindowTitle(tr("POSH Configuration"));
    this->setModal(true);
    this->setMinimumWidth(MIN_WIDTH);
    this->setMaximumHeight(MAX_HEIGHT);
}

PoshConfigDialog::~PoshConfigDialog(){
    delete hBox1;
    delete hBox2;
    delete hBox3;
    delete vBox;
}

void PoshConfigDialog::accept(){
    poshConfigSettings.setValue("poshSettings/scriptEngine",scriptEngine->text());
    poshConfigSettings.setValue("poshSettings/scriptFile",scriptFile->text());
    poshConfigSettings.setValue("poshSettings/scriptArgs",scriptArgs->text());

    QDialog::accept();
}

void PoshConfigDialog::reject(){
    QDialog::reject();

}

void PoshConfigDialog::onClickedScriptFileButton(){
    QString homeDirectory = QDir::homePath();

    QString strScriptFile = QFileDialog::getOpenFileName( this,
        tr("POSH Script File"),
        homeDirectory,
        tr("Python Script (*.py)"));

    if(strScriptFile.length() != 0){
        this->scriptFile->setText(strScriptFile);
    }

}
