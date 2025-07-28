#ifndef QUESTION_MARK_BUTTON_H
#define QUESTION_MARK_BUTTON_H

#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QTimer>

class QuestionMarkButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverScale READ hoverScale WRITE setHoverScale)
    Q_PROPERTY(qreal glowOpacity READ glowOpacity WRITE setGlowOpacity)

public:
    explicit QuestionMarkButton(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void updateGlow();

private:
    void drawBase(QPainter &painter, const QRect &rect);
    void drawQuestionMark(QPainter &painter, const QRect &rect);
    void drawGlow(QPainter &painter, const QRect &rect);

    // Animation properties
    qreal hoverScale() const { return m_hoverScale; }
    void setHoverScale(qreal scale)
    {
        m_hoverScale = scale;
        update();
    }

    qreal glowOpacity() const { return m_glowOpacity; }
    void setGlowOpacity(qreal opacity)
    {
        m_glowOpacity = opacity;
        update();
    }

    QPropertyAnimation *m_scaleAnimation;
    QPropertyAnimation *m_glowAnimation;
    QTimer *m_glowTimer;

    qreal m_hoverScale;
    qreal m_glowOpacity;
    bool m_isPressed;
    int m_glowPhase;
};

#endif // QUESTION_MARK_BUTTON_H