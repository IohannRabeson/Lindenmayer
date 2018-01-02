//
// Created by Io on 01/01/2018.
//

#ifndef MIDIMONITOR_DEVICESCHEMEWIDGET_HPP
#define MIDIMONITOR_DEVICESCHEMEWIDGET_HPP
#include <QWidget>

class QDeviceSchemeFactory;
class QItemSelectionModel;
class QListView;

class DeviceSchemeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceSchemeWidget(QDeviceSchemeFactory* schemeFactory, QWidget* parent = nullptr);
signals:
    void schemeRequested(QModelIndex const& index);
    void schemeClicked(QModelIndex const& index);
    void schemeDoubleClicked(QModelIndex const& index);
private:
    QDeviceSchemeFactory* const m_schemeFactory;
    QListView* const m_schemeView;
};


#endif //MIDIMONITOR_DEVICESCHEMEWIDGET_HPP