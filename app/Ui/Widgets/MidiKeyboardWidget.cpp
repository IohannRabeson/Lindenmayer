//
// Created by Io on 07/01/2018.
//

#include "MidiKeyboardWidget.hpp"
#include "Ui/CommonUi.hpp"
#include <Format.hpp>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>
#include <QFormLayout>
#include <QGraphicsSceneMouseEvent>
#include <QSpinBox>
#include <QSettings>
#include <QStateMachine>
#include <QPushButton>
#include <QGroupBox>

#include <set>

#include <QMidiMessageBuilder.hpp>

class MidiMessageSender : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    void sendNoteMessage(unsigned char const note, bool const onOrOff);
    void setVelocity(unsigned char const value);
    void setChannel(unsigned char const value);
signals:
    void sendMessage(QMidiMessage const& message);
private:
    unsigned char m_velocity = 100u;
    unsigned char m_channel = 1u;
};

class KeyboardGraphicsScene;

class KeyGraphicsItem : public QGraphicsRectItem
{
public:
    explicit KeyGraphicsItem(QColor const& color, int const width, int const height, QFont const& font = QFont(), QGraphicsItem* parent = nullptr);

    void setPressed(bool const pressed);
    void setHovered(bool const hovered);
    void setHighlighted(bool const selected);
    void setMidiNote(unsigned char const note);
    unsigned char midiNote() const;
private:
    void updateColor()
    {
        if (m_clicked)
        {
            setBrush(m_clickedBrush);
        }
        else if (m_selected)
        {
            setBrush(m_selectedBrush);
        }
        else if (m_hovered)
        {
            setBrush(m_hoveredBrush);
        }
    }
private:
    QBrush m_normalBrush;
    QBrush m_hoveredBrush;
    QBrush m_clickedBrush;
    QBrush m_selectedBrush;
    QGraphicsRectItem* const m_rectangle;
    QGraphicsSimpleTextItem* const m_noteText;
    unsigned char m_midiNote = std::numeric_limits<unsigned char>::max();
    bool m_hovered;
    bool m_clicked;
    bool m_selected;
};

class KeyboardGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit KeyboardGraphicsScene(QObject* parent = nullptr)
    : QGraphicsScene(parent)
    , m_sender(new MidiMessageSender(this))
    {
        createKeyboard();
    }

    ~KeyboardGraphicsScene()
    {
        releaseAllPressedKeys();
    }

    void addItem(KeyGraphicsItem* item)
    {
        QGraphicsScene::addItem(item);

        m_keyItems.insert(item->midiNote(), item);
    }

    MidiMessageSender* messageSender() const
    {
        return m_sender;
    }

    /*!
     * \brief Returns the items of the keys which should be triggered.
     */
    QSet<KeyGraphicsItem*> keysFromCurrentChord(QPointF const& scenePos) const
    {
        auto* const item = dynamic_cast<KeyGraphicsItem*>(itemAt(scenePos, QTransform()));
        QSet<KeyGraphicsItem*> results;

        if (item)
        {
            if (!m_chordEnabled)
            {
                results.insert(item);
            }
            else if (m_chordEditingEnabled)
            {
                results.insert(item);
            }
            else
            {
                auto const midiNote = item->midiNote();
                auto const lowerChordNote = m_chordNotes.empty() ? 0u : *m_chordNotes.begin();

                results.insert(item);

                for (auto const offset : m_chordNotes)
                {
                    auto const item = m_keyItems.value(midiNote + offset - lowerChordNote, nullptr);

                    if (item)
                    {
                        results.insert(item);
                    }
                }
            }
        }
        return results;
    }

    void setKeysPressed(QSet<KeyGraphicsItem*> const& keys)
    {
        QSet<KeyGraphicsItem*> const toAdd = keys - m_keyItemsPressed;
        QSet<KeyGraphicsItem*> const toRemove = m_keyItemsPressed - keys;

        for (auto* item : toRemove)
        {
            item->setPressed(false);
            m_keyItemsPressed.remove(item);
            m_sender->sendNoteMessage(item->midiNote(), false);
        }

        for (auto* item : toAdd)
        {
            item->setPressed(true);
            m_keyItemsPressed.insert(item);
            m_sender->sendNoteMessage(item->midiNote(), true);
        }
    }

    void setKeysHovered(QSet<KeyGraphicsItem*> const& keys)
    {
        QSet<KeyGraphicsItem*> const toAdd = keys - m_keyItemsHovered;
        QSet<KeyGraphicsItem*> const toRemove = m_keyItemsHovered - keys;

        for (auto* item : toRemove)
        {
            item->setHovered(false);
            m_keyItemsHovered.remove(item);
        }

        for (auto* item : toAdd)
        {
            item->setHovered(true);
            m_keyItemsHovered.insert(item);
        }
    }

    void clearChord()
    {
        m_chordNotes.clear();
        updateSelectedKeyColors();
    }

    void setChord(std::set<unsigned char> const& chord)
    {
        m_chordNotes = chord;
        updateSelectedKeyColors();
    }

    std::set<unsigned char> const& chord() const
    {
        return m_chordNotes;
    }

    void setChordEditionEnabled(bool const enabled)
    {
        if (m_chordEditingEnabled != enabled)
        {
            m_chordEditingEnabled = enabled;
            updateSelectedKeyColors();
        }
    }

    bool isChordEditionEnabled() const
    {
        return m_chordEditingEnabled;
    }

    void setChordEnabled(bool const enabled)
    {
        m_chordEnabled = enabled;
    }
protected:
    KeyGraphicsItem* keyItemAt(QPointF const& pos) const
    {
        return dynamic_cast<KeyGraphicsItem*>(itemAt(pos, QTransform()));
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (m_chordEditingEnabled)
        {
            auto* item = keyItemAt(event->scenePos());

            if (item)
            {
                unsigned char const note = item->midiNote();
                auto noteIt = m_chordNotes.find(note);

                if (noteIt != m_chordNotes.end())
                {
                    m_chordNotes.erase(noteIt);
                }
                else
                {
                    m_chordNotes.insert(note);
                }
                updateSelectedKeyColors();
            }
            return;
        }
        setKeysHovered(QSet<KeyGraphicsItem*>());
        setKeysPressed(keysFromCurrentChord(event->scenePos()));
        m_mousePressed = true;
        event->accept();
        QGraphicsScene::mousePressEvent(event);
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (m_chordEditingEnabled)
        {
            return;
        }

        auto const newKeys = keysFromCurrentChord(event->scenePos());

        if (m_mousePressed)
        {
            setKeysPressed(newKeys);
        }
        else
        {
            setKeysHovered(newKeys);
        }
        event->accept();
        QGraphicsScene::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override
    {
        if (m_chordEditingEnabled)
        {
            return;
        }
        auto const newKeys = keysFromCurrentChord(event->scenePos());

        setKeysPressed(QSet<KeyGraphicsItem*>());
        setKeysHovered(newKeys);
        m_mousePressed = false;
        event->accept();
        QGraphicsScene::mouseReleaseEvent(event);
    }
private:
    void updateSelectedKeyColors()
    {
        for (auto it = m_keyItems.begin(); it != m_keyItems.end(); ++it)
        {
            KeyGraphicsItem* const item = it.value();
            auto const highlight = m_chordEditingEnabled && (m_chordNotes.find(item->midiNote()) != m_chordNotes.end());

            item->setHighlighted(highlight);
        }
    }

    void releaseAllPressedKeys()
    {
        for (auto* const item : m_keyItemsPressed)
        {
            m_sender->sendNoteMessage(item->midiNote(), false);
        }
        m_keyItemsPressed.clear();
    }

    void createKeyboard()
    {
        constexpr static qreal const WhiteKeyWidth = 70;
        constexpr static qreal const WhiteKeyHeight = 250;
        constexpr static qreal const BlackKeyWidth = WhiteKeyWidth / 2.2;
        constexpr static qreal const BlackKeyHeight = 180;
        constexpr static qreal const OctaveWidth = WhiteKeyWidth * 7;
        constexpr static qreal const WhiteZValue = -1;
        constexpr static qreal const BlackZValue = 0;
        constexpr static qreal const TextPadding = 8;

        QFont textFont;

        textFont.setPointSize(textFont.pointSize() + 15);

        auto createWhite = [this, &textFont](unsigned const octave, unsigned const offset, unsigned char const midiNote)
        {
            auto* item = new KeyGraphicsItem(QColor(Qt::white), WhiteKeyWidth, WhiteKeyHeight, textFont);

            item->setMidiNote(midiNote);
            item->setX(offset * WhiteKeyWidth + octave * OctaveWidth);
            item->setZValue(WhiteZValue);
            addItem(item);
        };

        auto createBlack = [this](unsigned const octave, unsigned const offset, unsigned char const midiNote)
        {
            auto* item = new KeyGraphicsItem(QColor(Qt::black), BlackKeyWidth, BlackKeyHeight);

            item->setMidiNote(midiNote);
            item->setX((offset + 1) * WhiteKeyWidth - BlackKeyWidth / 2 + octave * OctaveWidth);
            item->setZValue(BlackZValue);
            addItem(item);
        };

        unsigned char currentMidiNote = 0u;

        for (auto octave = 0; octave < 10; ++octave)
        {
            createWhite(octave, 0, currentMidiNote++);
            createBlack(octave, 0, currentMidiNote++);
            createWhite(octave, 1, currentMidiNote++);
            createBlack(octave, 1, currentMidiNote++);
            createWhite(octave, 2, currentMidiNote++);
            createWhite(octave, 3, currentMidiNote++);
            createBlack(octave, 3, currentMidiNote++);
            createWhite(octave, 4, currentMidiNote++);
            createBlack(octave, 4, currentMidiNote++);
            createWhite(octave, 5, currentMidiNote++);
            createBlack(octave, 5, currentMidiNote++);
            createWhite(octave, 6, currentMidiNote++);
        }
        setSceneRect(itemsBoundingRect());
    }
private:
    QMap<unsigned char, KeyGraphicsItem*> m_keyItems;
    QSet<KeyGraphicsItem*> m_keyItemsPressed;
    QSet<KeyGraphicsItem*> m_keyItemsHovered;
    // Notice we use std::set instead of QSet because
    // QSet is hash based so the elements order is not guaranted.
    std::set<unsigned char> m_chordNotes;
    MidiMessageSender* const m_sender;
    bool m_mousePressed = false;
    bool m_chordEditingEnabled = false;
    bool m_chordEnabled = true;
};

class KeyboardGraphicsView : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;
protected:
    void leaveEvent(QEvent* event) override
    {
        KeyboardGraphicsScene* const scene = qobject_cast<KeyboardGraphicsScene*>(this->scene());

        if (scene)
        {
            scene->setKeysHovered({});
            scene->setKeysPressed({});
        }
        QWidget::leaveEvent(event);
    }
};

MidiKeyboardWidget::MidiKeyboardWidget(QWidget* parent)
: QWidget(parent)
, m_scene(new KeyboardGraphicsScene(this))
, m_view(new KeyboardGraphicsView(m_scene, this))
, m_velocity(new QSpinBox(this))
, m_channel(new QSpinBox(this))
, m_editChord(new QPushButton(tr("Edit chord"), this))
, m_clearChord(new QPushButton(tr("Clear chord"), this))
, m_chordGroup(new QGroupBox(tr("Chord"), this))
{
    auto* const layout = new QHBoxLayout(this);
    auto* const controlLayout = new QFormLayout;
    auto* const chordLayout = new QGridLayout(m_chordGroup);

    CommonUi::standardLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addLayout(controlLayout);
    layout->addWidget(m_view);

    controlLayout->setSpacing(3);
    controlLayout->setMargin(6);
    controlLayout->addRow(tr("Channel:"), m_channel);
    controlLayout->addRow(tr("Velocity:"), m_velocity);
    controlLayout->addRow(m_chordGroup);

    m_chordGroup->setCheckable(true);
    m_chordGroup->setTitle(tr("Chord"));
    chordLayout->setMargin(3);

    chordLayout->addWidget(m_editChord);
    chordLayout->addWidget(m_clearChord);

    connect(m_editChord, &QPushButton::clicked, this, &MidiKeyboardWidget::setChordEditionEnabled);
    connect(m_clearChord, &QPushButton::clicked, this, &MidiKeyboardWidget::clearChord);
    connect(m_chordGroup, &QGroupBox::clicked, this, &MidiKeyboardWidget::setChordEnabled);
    connect(m_velocity, qOverload<int>(&QSpinBox::valueChanged), m_scene->messageSender(), &MidiMessageSender::setVelocity);
    connect(m_channel, qOverload<int>(&QSpinBox::valueChanged), m_scene->messageSender(), &MidiMessageSender::setChannel);

    m_editChord->setCheckable(true);
    m_velocity->setRange(1, 127);
    m_velocity->setValue(100);
    m_channel->setRange(1, 16);

    m_view->setMouseTracking(true);
    connect(m_scene->messageSender(), &MidiMessageSender::sendMessage, this, &MidiKeyboardWidget::sendMessage);

    m_chordEditor = false;
}

MidiKeyboardWidget::~MidiKeyboardWidget()
{
}

void MidiKeyboardWidget::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MidiKeyboardWidget::showEvent(QShowEvent* e)
{
    QWidget::showEvent(e);

    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MidiKeyboardWidget::saveSettings(QSettings& settings) const
{
    settings.beginGroup("midi_keyboard_widget");
    settings.setValue("velocity", m_velocity->value());
    settings.setValue("channel", m_channel->value());
    settings.setValue("chord_edit", m_chordGroup->isChecked());

    auto const& chord = m_scene->chord();
    auto chordIt = chord.begin();

    settings.beginWriteArray("chord", chord.size());
    for (auto i = 0; i < chord.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("note", *chordIt);
        ++chordIt;
    }
    settings.endArray();
    settings.endGroup();
}

void MidiKeyboardWidget::loadSettings(QSettings& settings)
{
    settings.beginGroup("midi_keyboard_widget");
    m_velocity->setValue(settings.value("velocity", 100).toInt());
    m_channel->setValue(settings.value("channel", 1).toInt());
    m_chordGroup->setChecked(settings.value("chord_edit").toBool());

    std::set<unsigned char> chord;
    auto const size = settings.beginReadArray("chord");

    for (auto i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        chord.insert(settings.value("note").toUInt());
    }
    m_scene->setChord(chord);
    settings.endArray();
    settings.endGroup();
}

void MidiKeyboardWidget::setChordEditionEnabled(bool const enabled)
{
    m_scene->setChordEditionEnabled(enabled);
    m_editChord->setChecked(enabled);
}

void MidiKeyboardWidget::clearChord()
{
    m_scene->clearChord();
}

void MidiKeyboardWidget::setChordEnabled(bool const enabled)
{
    m_chordGroup->setChecked(enabled);
    m_scene->setChordEnabled(enabled);
    if (!enabled)
    {
        setChordEditionEnabled(false);
    }
}

void MidiMessageSender::sendNoteMessage(unsigned char const note, bool const onOrOff)
{
    emit sendMessage(QMidiMessageBuilder::note(note, m_velocity, m_channel - 1u, onOrOff));
}

void MidiMessageSender::setVelocity(unsigned char const value)
{
    Q_ASSERT(m_velocity < 128u);

    m_velocity = value;
}

void MidiMessageSender::setChannel(unsigned char const value)
{
    Q_ASSERT(m_channel > 0u);
    Q_ASSERT(m_channel < 17u);

    m_channel = value;
}

KeyGraphicsItem::KeyGraphicsItem(QColor const& color, int const width, int const height, QFont const& font, QGraphicsItem* parent)
: QGraphicsRectItem(parent)
, m_rectangle(new QGraphicsRectItem)
, m_noteText(new QGraphicsSimpleTextItem(this))
, m_hovered(false)
, m_clicked(false)
{
    setBrush(color);

    auto rect = this->rect();

    rect.setWidth(width);
    rect.setHeight(height);

    setRect(rect);
    m_noteText->setFont(font);
    m_noteText->setZValue(1);

    m_normalBrush = color;
    m_hoveredBrush = Qt::lightGray;
    m_clickedBrush = Qt::darkGray;
    m_selectedBrush = Qt::red;

    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setHandlesChildEvents(true);
}

void KeyGraphicsItem::setMidiNote(unsigned char const note)
{
    setToolTip(Format::formatMidiNote(note));
    m_midiNote = note;

    if (m_midiNote % 12u == 0u)
    {
        constexpr static qreal const TextPadding = 8;

        m_noteText->setText(Format::formatMidiNote(m_midiNote));
        m_noteText->setPos(TextPadding, boundingRect().bottom() - m_noteText->boundingRect().height() - TextPadding);
    }
}

unsigned char KeyGraphicsItem::midiNote() const
{
    return m_midiNote;
}

void KeyGraphicsItem::setPressed(bool const pressed)
{
    if (pressed)
    {
        setBrush(m_clickedBrush);
    }
    else
    {
        setBrush(m_normalBrush);
    }
}

void KeyGraphicsItem::setHovered(bool const hovered)
{
    if (hovered)
    {
        setBrush(m_hoveredBrush);
    }
    else
    {
        setBrush(m_normalBrush);
    }
}

void KeyGraphicsItem::setHighlighted(bool const selected)
{
    if (selected)
    {
        setBrush(m_selectedBrush);
    }
    else
    {
        setBrush(m_normalBrush);
    }
}

#include "MidiKeyboardWidget.moc"