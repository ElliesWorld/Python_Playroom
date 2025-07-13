#include "settings_toolbar.h"
#include <QDebug>

SettingsToolbar::SettingsToolbar(SettingsManager *settingsManager, QWidget *parent)
    : QToolBar(parent), settings_manager(settingsManager)
{
    setObjectName("settings_toolbar");
    setWindowTitle("Quiz Settings");
    setFloatable(false);
    setMovable(false);

    setupActions();
    updateActionStates();

    connect(settings_manager, &SettingsManager::modeChanged,
            this, &SettingsToolbar::onModeChanged);
}

void SettingsToolbar::setupActions()
{
    mode_action_group = new QActionGroup(this);

    python_action = new QAction("🐍 Python", this);
    python_action->setCheckable(true);
    python_action->setToolTip("Switch to Python Quiz");
    python_action->setData(static_cast<int>(QuizMode::Python));
    mode_action_group->addAction(python_action);
    addAction(python_action);

    cpp_action = new QAction("⚡ C++", this);
    cpp_action->setCheckable(true);
    cpp_action->setToolTip("Switch to C++ Quiz");
    cpp_action->setData(static_cast<int>(QuizMode::Cpp));
    mode_action_group->addAction(cpp_action);
    addAction(cpp_action);

    c_action = new QAction("🔧 C", this);
    c_action->setCheckable(true);
    c_action->setToolTip("Switch to C Quiz");
    c_action->setData(static_cast<int>(QuizMode::C));
    mode_action_group->addAction(c_action);
    addAction(c_action);

    connect(python_action, &QAction::triggered, this, &SettingsToolbar::onModeActionTriggered);
    connect(cpp_action, &QAction::triggered, this, &SettingsToolbar::onModeActionTriggered);
    connect(c_action, &QAction::triggered, this, &SettingsToolbar::onModeActionTriggered);
}

void SettingsToolbar::onModeActionTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QuizMode mode = static_cast<QuizMode>(action->data().toInt());
        settings_manager->setCurrentMode(mode);
    }
}

void SettingsToolbar::onModeChanged(QuizMode newMode)
{
    updateActionStates();
}

void SettingsToolbar::updateActionStates()
{
    QuizMode currentMode = settings_manager->currentMode();

    python_action->setChecked(currentMode == QuizMode::Python);
    cpp_action->setChecked(currentMode == QuizMode::Cpp);
    c_action->setChecked(currentMode == QuizMode::C);
}