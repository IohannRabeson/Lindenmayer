//
// Created by Io on 04/01/2018.
//
#include <gtest/gtest.h>
#include <QMidiManufacturerModel.hpp>
#include <QMidiMessage.hpp>
#include <initializer_list>

struct HardCodedManufacturer
{
    explicit HardCodedManufacturer(std::initializer_list<QMidiManufacturerModel::Element>&& elements) :
            m_elements(elements)
    {
    }

    QVector<QMidiManufacturerModel::Element> operator()()
    {
        return m_elements;
    }

    QVector<QMidiManufacturerModel::Element> const m_elements;
};

struct QMidiManufacturerModelTestFixture : public ::testing::Test
{
public:
    void load(std::initializer_list<QMidiManufacturerModel::Element>&& elements)
    {
        m_manufacturerModel->load(HardCodedManufacturer(std::move(elements)));
    }

    bool findCode(std::initializer_list<unsigned char>&& code) const
    {
        QVector<unsigned char> bytes;

        bytes << 0xF0 << code;

        return m_manufacturerModel->findCode(QMidiMessage(bytes.toStdVector())) != -1;
    }

    QString getName(std::initializer_list<unsigned char>&& code) const
    {
        QVector<unsigned char> bytes;

        bytes << 0xF0 << code;

        auto const index = m_manufacturerModel->findCode(QMidiMessage(bytes.toStdVector()));

        return m_manufacturerModel->getName(index);
    }
protected:
    void SetUp()
    {
        m_manufacturerModel.reset(new QMidiManufacturerModel);
    }

    void TearDown()
    {
        m_manufacturerModel.reset();
    }
private:
    std::unique_ptr<QMidiManufacturerModel> m_manufacturerModel;
};


TEST_F(QMidiManufacturerModelTestFixture, general_tests)
{
    load({{{"0_0_0"}, {0, 0, 0}}});
    load({{{"0"}, {0}}});
    load({{{"0_0_1"}, {0, 0, 1}}});
    load({{{"1"}, {1}}});

    ASSERT_TRUE( findCode({0, 0, 1}) );
    ASSERT_TRUE( findCode({1}) );
    ASSERT_TRUE( findCode({1}) );
    ASSERT_FALSE( findCode({1, 0, 0}) );
    ASSERT_FALSE( findCode({0, 1, 0}) );
}

TEST_F(QMidiManufacturerModelTestFixture, duplicate)
{
    // The duplicate should be ignored
    load({{{"0_0_0"}, {0, 0, 0}}});
    load({{{"0_0_0_duplicate"}, {0, 0, 0}}});

    ASSERT_TRUE( findCode({0, 0, 0}) );
    ASSERT_STRCASEEQ( getName({0, 0, 0}).toStdString().c_str(), "0_0_0" );
}