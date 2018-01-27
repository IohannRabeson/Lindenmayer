//
// Created by Io on 27/01/2018.
//

#include "MidiMatrixWidget.hpp"

#include <QAbstractTableModel>
#include <QTableView>
#include <QGridLayout>
#include <QHeaderView>
#include <QPainter>

namespace
{
    class HorizontalHeader : public QHeaderView
    {
    protected:
        void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override
        {
            QStyleOptionHeader opt;

            initStyleOption(&opt);
            QStyle::State state = QStyle::State_None;

            if (isEnabled())
            {
                state |= QStyle::State_Enabled;
            }
            if (window()->isActiveWindow())
            {
                state |= QStyle::State_Active;
            }

            opt.section = logicalIndex;
            opt.state |= state;
            opt.rect = rect;

            painter->save();
            style()->drawControl(QStyle::CE_Header, &opt, painter, this);
            painter->restore();

            int const margin = style()->pixelMetric(QStyle::PM_HeaderMargin, 0, this);

            painter->save();

            QMatrix m;
            QRect rotatedRectangle(0, 0, rect.height(), rect.width());

            m.rotate(-90);
            painter->setWorldMatrix(m, true);
            rotatedRectangle.moveCenter(QPoint(-rect.center().y() + margin, rect.center().x()));

            opt.text = model()->headerData(logicalIndex, Qt::Horizontal, Qt::DisplayRole).toString();
            opt.rect = rotatedRectangle;

            style()->drawControl(QStyle::CE_HeaderLabel, &opt, painter, nullptr);
            painter->restore();
        }

        QSize sectionSizeFromContents(int logicalIndex) const override
        {
            auto size = QHeaderView::sectionSizeFromContents(logicalIndex);

            size.setHeight(200);
            return size;
        }

    public:
        explicit HorizontalHeader(QWidget* parent)
                : QHeaderView(Qt::Horizontal, parent)
        {
        }
    };
}

MidiMatrixWidget::MidiMatrixWidget(QWidget* parent)
: QWidget(parent)
, m_table(new QTableView(this))
{
    QGridLayout* const layout = new QGridLayout(this);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_table);

    m_table->setHorizontalHeader(new HorizontalHeader(this));
}

void MidiMatrixWidget::setModel(QAbstractTableModel* model)
{
    constexpr int const MatrixCellSize = 32;

    m_table->setModel(model);
    m_table->horizontalHeader()->setDefaultSectionSize(MatrixCellSize);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    m_table->verticalHeader()->setDefaultSectionSize(MatrixCellSize);
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    m_table->verticalHeader()->setSectionsClickable(false);
    connect(model, &QAbstractTableModel::modelReset, [this, model]()
    {
        for (auto i = 0; i < model->columnCount(); ++i)
        {
            m_table->setColumnWidth(i, MatrixCellSize);
        }
        for (auto i = 0; i < model->rowCount(); ++i)
        {
            m_table->setRowHeight(i, MatrixCellSize);
        }
    });
}
