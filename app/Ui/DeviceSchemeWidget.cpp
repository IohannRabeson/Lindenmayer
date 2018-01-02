//
// Created by Io on 01/01/2018.
//

#include "DeviceSchemeWidget.hpp"
#include <QDeviceSchemeFactory.hpp>

#include <QListView>
#include <QVBoxLayout>

DeviceSchemeWidget::DeviceSchemeWidget(QDeviceSchemeFactory* schemeFactory, QWidget* parent)
: QWidget(parent)
, m_schemeFactory(schemeFactory)
, m_schemeView(new QListView(this))
{
    auto* const layout = new QVBoxLayout(this);

    m_schemeView->setModel(m_schemeFactory);
    layout->addWidget(m_schemeView);
    layout->setMargin(0);

    connect(m_schemeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &DeviceSchemeWidget::schemeRequested);
    connect(m_schemeView, &QListView::clicked, this, &DeviceSchemeWidget::schemeClicked);
    connect(m_schemeView, &QListView::doubleClicked, this, &DeviceSchemeWidget::schemeDoubleClicked);
}
