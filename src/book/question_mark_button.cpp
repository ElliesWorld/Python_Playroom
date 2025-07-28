#include "question_mark_button.h"
#include <QRadialGradient>
#include <QLinearGradient>
#include <QFont>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <qmath.h>

QuestionMarkButton::QuestionMarkButton(QWidget *parent)
    : QPushButton(parent), m_hoverScale(1.0), m_glowOpacity(0.0),
      m_isPressed(false), m_glowPhase(0)
{
    setFixedSize(50, 50);
    setToolTip("📚 Open Learning Book");
    setCursor(Qt::PointingHandCursor);

    // Set up animations
    m_scaleAnimation = new QPropertyAnimation(this, "hoverScale");
    m_scaleAnimation->setDuration(200);
    m_scaleAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_glowAnimation = new QPropertyAnimation(this, "glowOpacity");
    m_glowAnimation->setDuration(150);
    m_glowAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // Set up glow timer for subtle pulsing effect
    m_glowTimer = new QTimer(this);
    connect(m_glowTimer, &QTimer::timeout, this, &QuestionMarkButton::updateGlow);
    m_glowTimer->start(3000); // Pulse every 3 seconds

    // Add subtle shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(8);
    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setOffset(2, 2);
    setGraphicsEffect(shadow);
}

void QuestionMarkButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QRect drawRect = rect();

    // Apply hover scaling
    if (m_hoverScale != 1.0)
    {
        painter.save();
        QPointF center = drawRect.center();
        painter.translate(center);
        painter.scale(m_hoverScale, m_hoverScale);
        painter.translate(-center);
    }

    // Draw glow effect first (behind everything)
    if (m_glowOpacity > 0.0)
    {
        drawGlow(painter, drawRect);
    }

    // Draw the main button
    drawBase(painter, drawRect);

    // Draw the question mark
    drawQuestionMark(painter, drawRect);

    if (m_hoverScale != 1.0)
    {
        painter.restore();
    }
}

void QuestionMarkButton::drawBase(QPainter &painter, const QRect &rect)
{
    // Create 3D gradient effect
    QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());

    if (m_isPressed)
    {
        // Pressed state - darker
        gradient.setColorAt(0.0, QColor(70, 130, 200)); // Darker blue
        gradient.setColorAt(0.4, QColor(50, 110, 180));
        gradient.setColorAt(0.6, QColor(40, 100, 170));
        gradient.setColorAt(1.0, QColor(30, 90, 160));
    }
    else
    {
        // Normal state - bright and 3D
        gradient.setColorAt(0.0, QColor(100, 160, 230)); // Light blue top
        gradient.setColorAt(0.4, QColor(80, 140, 210));  // Medium blue
        gradient.setColorAt(0.6, QColor(60, 120, 190));  // Darker blue
        gradient.setColorAt(1.0, QColor(40, 100, 170));  // Darkest blue bottom
    }

    painter.setBrush(QBrush(gradient));
    painter.setPen(QPen(QColor(30, 80, 150), 2));

    // Draw rounded rectangle with 3D effect
    QRect buttonRect = rect.adjusted(3, 3, -3, -3);
    painter.drawRoundedRect(buttonRect, 8, 8);

    // Add highlight on top edge for 3D effect
    if (!m_isPressed)
    {
        QLinearGradient highlight(buttonRect.topLeft(),
                                  QPoint(buttonRect.left(), buttonRect.top() + 8));
        highlight.setColorAt(0.0, QColor(255, 255, 255, 80));
        highlight.setColorAt(1.0, QColor(255, 255, 255, 0));

        painter.setBrush(QBrush(highlight));
        painter.setPen(Qt::NoPen);

        QPainterPath highlightPath;
        highlightPath.addRoundedRect(buttonRect.adjusted(1, 1, -1, -buttonRect.height() / 2), 7, 7);
        painter.drawPath(highlightPath);
    }
}

void QuestionMarkButton::drawQuestionMark(QPainter &painter, const QRect &rect)
{
    painter.setPen(Qt::NoPen);

    // Create gradient for the question mark
    QRadialGradient textGradient(rect.center(), rect.width() / 3);
    textGradient.setColorAt(0.0, QColor(255, 255, 255, 255));
    textGradient.setColorAt(0.7, QColor(240, 240, 240, 200));
    textGradient.setColorAt(1.0, QColor(200, 200, 200, 150));

    painter.setBrush(QBrush(textGradient));

    // Set up font
    QFont font = painter.font();
    font.setPointSize(20);
    font.setBold(true);
    font.setFamily("Arial");
    painter.setFont(font);

    // Draw shadow for question mark
    painter.setPen(QColor(0, 0, 0, 100));
    QFontMetrics fm(font);
    QString text = "?";
    QRect textRect = fm.boundingRect(text);
    QPoint textPos = rect.center() - textRect.center();

    // Draw shadow slightly offset
    painter.drawText(textPos + QPoint(1, 1), text);

    // Draw main question mark
    painter.setPen(QColor(255, 255, 255, 230));
    painter.drawText(textPos, text);
}

void QuestionMarkButton::drawGlow(QPainter &painter, const QRect &rect)
{
    painter.save();

    // Create glow effect
    QRadialGradient glowGradient(rect.center(), rect.width() * 0.8);
    glowGradient.setColorAt(0.0, QColor(100, 200, 255, (int)(m_glowOpacity * 80)));
    glowGradient.setColorAt(0.5, QColor(150, 220, 255, (int)(m_glowOpacity * 40)));
    glowGradient.setColorAt(1.0, QColor(200, 240, 255, 0));

    painter.setBrush(QBrush(glowGradient));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(rect.adjusted(-5, -5, 5, 5));

    painter.restore();
}

void QuestionMarkButton::enterEvent(QEnterEvent *event)
{
    Q_UNUSED(event)

    // Start hover animation
    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_hoverScale);
    m_scaleAnimation->setEndValue(1.1);
    m_scaleAnimation->start();

    // Start glow animation
    m_glowAnimation->stop();
    m_glowAnimation->setStartValue(m_glowOpacity);
    m_glowAnimation->setEndValue(1.0);
    m_glowAnimation->start();

    QPushButton::enterEvent(event);
}

void QuestionMarkButton::leaveEvent(QEvent *event)
{
    // Return to normal size
    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_hoverScale);
    m_scaleAnimation->setEndValue(1.0);
    m_scaleAnimation->start();

    // Fade out glow
    m_glowAnimation->stop();
    m_glowAnimation->setStartValue(m_glowOpacity);
    m_glowAnimation->setEndValue(0.0);
    m_glowAnimation->start();

    QPushButton::leaveEvent(event);
}

void QuestionMarkButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        update();
    }
    QPushButton::mousePressEvent(event);
}

void QuestionMarkButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    update();
    QPushButton::mouseReleaseEvent(event);
}

void QuestionMarkButton::updateGlow()
{
    // Subtle pulsing effect when idle
    if (!underMouse())
    {
        m_glowPhase = (m_glowPhase + 1) % 60;
        qreal pulseOpacity = 0.3 + 0.2 * qSin(m_glowPhase * M_PI / 30.0);
        setGlowOpacity(pulseOpacity);
    }
}