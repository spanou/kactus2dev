//-----------------------------------------------------------------------------
// File: NewDesignDialog.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti M��tt�
// Date: 2.8.2012
//
// Description:
// Dialog used in creation of new designs to existing components.
//-----------------------------------------------------------------------------

#include "NewDesignDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDialog>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCoreApplication>

#include <models/Component.h>

#include <common/widgets/LibraryPathSelector/librarypathselector.h>
#include <LibraryManager/libraryinterface.h>

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::NewDesignDialog()
//-----------------------------------------------------------------------------
NewDesignDialog::NewDesignDialog(LibraryInterface* libInterface,
                                 QSharedPointer<Component> component,
                                 KactusAttribute::Implementation designType,
                                 QWidget *parent)
    : QDialog(parent), 
      lh_(libInterface),
      component_(component),
      usedViewNames_(),
      viewNameLabel_(new QLabel(tr("View name:"), this)),
      viewNameEdit_(new QLineEdit(this)),
      vlnvEditor_(new VLNVEditor(VLNV::DESIGN, libInterface, this, this, true)),
      directoryEdit_(new LibraryPathSelector(this)), 
      okButton_(new QPushButton(tr("&OK"))),
      designExt_(),
      designConfExt_()
{
    switch (designType)
    {
    case KactusAttribute::KTS_HW:
        {
            viewNameLabel_->setText(tr("Name for a new view to HW component:"));
            vlnvEditor_->setTitle("VLNV for new HW design and design configuration");
            designExt_ = ".design";
            designConfExt_ = ".designcfg";
            usedViewNames_ = component->getHierViews();
            break;
        }

    case KactusAttribute::KTS_SW:
        {
            viewNameLabel_->setText(tr("Name for a new SW view to component:"));
            vlnvEditor_->setTitle("VLNV for new SW design and design configuration");
            designExt_ = ".swdesign";
            designConfExt_ = ".swdesigncfg";
            usedViewNames_ = component->getSWViewNames();
            break;
        }

    case KactusAttribute::KTS_SYS:
        {
            viewNameLabel_->setText(tr("Name for a new system view to HW component:"));
            vlnvEditor_->setTitle("VLNV for new system design and design configuration");
            designExt_ = ".sysdesign";
            designConfExt_ = ".sysdesigncfg";
            usedViewNames_ = component->getSystemViewNames();
            break;
        }
    }

    connect(viewNameEdit_, SIGNAL(textChanged(QString const&)), this, SLOT(updateVlnvName()));
    connect(viewNameEdit_, SIGNAL(textChanged(QString const&)), this, SLOT(onContentChanged()));

    vlnvEditor_->setNameExtension(designExt_ + "/" + designConfExt_);
    vlnvEditor_->addContentType(VLNV::DESIGNCONFIGURATION);
    vlnvEditor_->updateFiltering();

    connect(vlnvEditor_, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
    connect(vlnvEditor_, SIGNAL(contentChanged()), this, SLOT(updateDirectory()));

    QLabel* directoryLabel = new QLabel(tr("Directory:"), this);
	connect(directoryEdit_, SIGNAL(editTextChanged(QString const&)), this, SLOT(onContentChanged()));

    okButton_->setEnabled(false);
    connect(okButton_, SIGNAL(released()), this, SLOT(accept()));

    QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
    connect(cancelButton, SIGNAL(released()), this, SLOT(reject()));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(okButton_, QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(viewNameLabel_);
    mainLayout->addWidget(viewNameEdit_);
    mainLayout->addWidget(vlnvEditor_);
    mainLayout->addWidget(directoryLabel);
    mainLayout->addWidget(directoryEdit_);
    mainLayout->addWidget(buttonBox);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    setWindowTitle("New Design");
    setFixedHeight(sizeHint().height());
    resize(400, sizeHint().height());
    okButton_->setDefault(true);
}

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::setViewName()
//-----------------------------------------------------------------------------
void NewDesignDialog::setViewName(QString const& viewName)
{
    viewNameEdit_->setText(viewName);
}

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::getViewName()
//-----------------------------------------------------------------------------
QString NewDesignDialog::getViewName() const
{
    return viewNameEdit_->text();
}

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::getDesignVLNV()
//-----------------------------------------------------------------------------
VLNV NewDesignDialog::getDesignVLNV() const
{
    VLNV vlnv = vlnvEditor_->getVLNV();
    vlnv.setType(VLNV::DESIGN);
    vlnv.setName(vlnv.getName().remove(".comp") + designExt_);
    return vlnv;
}

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::getDesignConfVLNV()
//-----------------------------------------------------------------------------
VLNV NewDesignDialog::getDesignConfVLNV() const
{
    VLNV vlnv = vlnvEditor_->getVLNV();
    vlnv.setType(VLNV::DESIGNCONFIGURATION);
    vlnv.setName(vlnv.getName().remove(".comp") + designConfExt_);
    return vlnv;
}

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::getPath()
//-----------------------------------------------------------------------------
QString NewDesignDialog::getPath() const
{
    return directoryEdit_->currentText();
}

//-----------------------------------------------------------------------------
// Function: onContentChanged()
//-----------------------------------------------------------------------------
void NewDesignDialog::onContentChanged()
{
    // Enable/disable the ok button if the contents are valid/invalid.
    okButton_->setEnabled(!viewNameEdit_->text().isEmpty() &&
                          !directoryEdit_->currentText().isEmpty() &&
                          vlnvEditor_->isValid());
}

//-----------------------------------------------------------------------------
// Function: accept()
//-----------------------------------------------------------------------------
void NewDesignDialog::accept()
{
    // Check that the view name is unused.
    if (usedViewNames_.contains(getViewName()))
    {
        QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
                           tr("The component already contains a view with name '%1'.").arg(getViewName()),
                           QMessageBox::Ok, this);

        msgBox.exec();
        return;
    }

    // Make sure that the VLNVs are not already in use.
    if (lh_->contains(getDesignVLNV()))
    {
        QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
                           tr("VLNV %1 already exists in the library.").arg(getDesignVLNV().toString()),
                           QMessageBox::Ok, this);
        
        msgBox.exec();
        return;
    }

    if (lh_->contains(getDesignConfVLNV()))
    {
        QMessageBox msgBox(QMessageBox::Warning, QCoreApplication::applicationName(),
                           tr("VLNV %1 already exists in the library.").arg(getDesignConfVLNV().toString()),
                           QMessageBox::Ok, this);

        msgBox.exec();
        return;
    }

    QDialog::accept();
}

void NewDesignDialog::setVLNV( const VLNV& vlnv ) {
	vlnvEditor_->setVLNV(vlnv);
    updateDirectory();
}

//-----------------------------------------------------------------------------
// Function: updateDirectory()
//-----------------------------------------------------------------------------
void NewDesignDialog::updateDirectory()
{
    QString dir = directoryEdit_->currentLocation();

    VLNV vlnv = vlnvEditor_->getVLNV();

    if (!vlnv.getVendor().isEmpty())
    {
        dir += "/" + vlnv.getVendor();

        if (!vlnv.getLibrary().isEmpty())
        {
            dir += "/" + vlnv.getLibrary();

            if (!vlnv.getName().isEmpty())
            {
                dir += "/" + vlnv.getName();

                if (!vlnv.getVersion().isEmpty())
                {
                    dir += "/" + vlnv.getVersion();
                }
            }
        }
    }

    directoryEdit_->setEditText(dir);
}

//-----------------------------------------------------------------------------
// Function: NewDesignDialog::updateVlnvName()
//-----------------------------------------------------------------------------
void NewDesignDialog::updateVlnvName()
{
    vlnvEditor_->setName(component_->getVlnv()->getName().remove(".comp") + "." + getViewName());
}