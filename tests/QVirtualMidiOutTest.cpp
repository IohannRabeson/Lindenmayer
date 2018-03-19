#include <gtest/gtest.h>
#include <QMidiManager.hpp>
#include <QMidiPortModel.hpp>
#include <QVirtualMidiOut.hpp>

TEST(QVirtualMidiOutTest, basic)
{
    QMidiManager manager;

    manager.getOutputDeviceModel()->add(std::make_shared<QVirtualMidiOut>());
}
