#ifndef SETTINGS_TOOLBAR_H
#define SETTINGS_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QWidget>
#include "settings_manager.h"

class SettingsToolbar : public QToolBar
{
    Q_OBJECT

public:
    explicit SettingsToolbar(SettingsManager *settingsManager, QWidget *parent = nullptr);

private slots:
    void onModeActionTriggered();
    void onModeChanged(QuizMode newMode);

private:
    SettingsManager *settings_manager;
    QActionGroup *mode_action_group;
    QAction *python_action;
    QAction *cpp_action;
    QAction *c_action;

    void setupActions();
    void updateActionStates();
};

#endif 