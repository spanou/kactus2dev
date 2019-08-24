#include "PoshConfigDialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>



PoshConfigDialog::PoshConfigDialog(QWidget *parent) :
    QDialog(parent){

    strScriptEngine = "/usr/bin/python3";

    scriptEngine = new QLineEdit;
    scriptFile = new QLineEdit;
    scriptFileButton = new QPushButton;

    scriptFileButton->setText(tr(".."));
    scriptEngine->setPlaceholderText(strScriptEngine);
    scriptEngine->setText(strScriptEngine);
    scriptFile->setPlaceholderText(tr("Click on the button on the right to select a script to run.."));


    vBox = new QVBoxLayout;
    hBox = new QHBoxLayout;

    hBox->addWidget(scriptFile);
    hBox->addWidget(scriptFileButton);

    vBox->addLayout(hBox);
    vBox->addWidget(scriptEngine);

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
    delete hBox;
    delete vBox;
}

void PoshConfigDialog::accept(){
    strScriptFile = scriptFile->text();
    strScriptEngine = scriptEngine->text();
    QDialog::accept();
}

void PoshConfigDialog::reject(){
    QDialog::reject();

}

void PoshConfigDialog::onClickedScriptFileButton(){
    QString homeDirectory = QDir::homePath();

    strScriptFile = QFileDialog::getOpenFileName( this,
        tr("POSH Script File"),
        homeDirectory,
        tr("Python Script (*.py)"));

    if(strScriptFile.length() != 0){
        this->scriptFile->setText(strScriptFile);
    }

}
