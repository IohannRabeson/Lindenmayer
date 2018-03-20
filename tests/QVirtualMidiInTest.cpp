#include <gtest/gtest.h>
#include <QMidiManager.hpp>
#include <QMidiPortModel.hpp>
#include <QVirtualMidiIn.hpp>

TEST(QVirtualMidiInTest, basic)
{
    QMidiManager manager;

    manager.getInputDeviceModel()->add(std::make_shared<QVirtualMidiIn>());
}
