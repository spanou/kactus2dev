
#include "SingleIndirectInterfaceitem.h"

#include <library/LibraryInterface.h>

#include <editors/ComponentEditor/IndirectInterfaces/SingleIndirectInterfaceEditor.h>
#include <editors/ComponentEditor/common/ExpressionParser.h>

#include <IPXACTmodels/Component/IndirectInterface.h>
#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/PortMap.h>
//#include <IPXACTmodels/Component/validators/IndirectInterfaceValidator.h>

//-----------------------------------------------------------------------------
// Function: SingleIndirectInterfaceItem::SingleIndirectInterfaceItem()
//-----------------------------------------------------------------------------
SingleIndirectInterfaceItem::SingleIndirectInterfaceItem(QSharedPointer<IndirectInterface> busif,
    ComponentEditorTreeModel* model, LibraryInterface* libHandler, QSharedPointer<Component> component,
    QSharedPointer<ReferenceCounter> referenceCounter,
    QSharedPointer<ParameterFinder> parameterFinder,
    QSharedPointer<ExpressionFormatter> expressionFormatter,
    QSharedPointer<ExpressionParser> expressionParser,
  //  QSharedPointer<IndirectInterfaceValidator> validator,
    ComponentEditorItem* parent, QWidget* parentWnd):
ComponentEditorItem(model, libHandler, component, parent),
    busif_(busif),
    parentWnd_(parentWnd),
    expressionParser_(expressionParser)
//    validator_(validator)
{
    setParameterFinder(parameterFinder);
    setExpressionFormatter(expressionFormatter);
    setReferenceCounter(referenceCounter);
}

//-----------------------------------------------------------------------------
// Function: SingleIndirectInterfaceItem::~SingleIndirectInterfaceItem()
//-----------------------------------------------------------------------------
SingleIndirectInterfaceItem::~SingleIndirectInterfaceItem()
{
}

//-----------------------------------------------------------------------------
// Function: SingleIndirectInterfaceItem::text()
//-----------------------------------------------------------------------------
QString SingleIndirectInterfaceItem::text() const
{
	return busif_->name();
}

//-----------------------------------------------------------------------------
// Function: SingleIndirectInterfaceItem::isValid()
//-----------------------------------------------------------------------------
bool SingleIndirectInterfaceItem::isValid() const
{
 
	return true;
}

//-----------------------------------------------------------------------------
// Function: SingleIndirectInterfaceItem::editor()
//-----------------------------------------------------------------------------
ItemEditor* SingleIndirectInterfaceItem::editor()
{
	if (!editor_)
    {
		editor_ = new SingleIndirectInterfaceEditor(busif_, component_, libHandler_, parentWnd_);
		editor_->setProtection(locked_);

		connect(editor_, SIGNAL(contentChanged()), this, SLOT(onEditorChanged()), Qt::UniqueConnection);
        connect(editor_, SIGNAL(errorMessage(const QString&)),
            this, SIGNAL(errorMessage(const QString&)),Qt::UniqueConnection);
		connect(editor_, SIGNAL(helpUrlRequested(QString const&)), this, SIGNAL(helpUrlRequested(QString const&)));

        connectItemEditorToReferenceCounter();

        connectReferenceTree();
	}

	return editor_;
}

//-----------------------------------------------------------------------------
// Function: SingleIndirectInterfaceItem::getTooltip()
//-----------------------------------------------------------------------------
QString SingleIndirectInterfaceItem::getTooltip() const
{
	return tr("Defines properties of a specific indirect interface in a component");
}
