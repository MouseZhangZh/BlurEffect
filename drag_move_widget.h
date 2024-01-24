//
// Created by Mouse Zhang on 2024/1/24.
//

#pragma once

#include <QLabel>
#include <QKeyEvent>

class DragMoveWidget : public  QLabel {
public:
    explicit DragMoveWidget(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags()) : QLabel(parent, f) {
        this->setFocusPolicy(Qt::StrongFocus);
    }
    explicit DragMoveWidget(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags()) : QLabel(text, parent, f) {}
protected:
    void keyPressEvent(QKeyEvent* event) override {
        QPoint newPos(this->x(),this->y());
        if (event->key() == Qt::Key_Up) newPos.setY(newPos.y() - 10);
        if (event->key() == Qt::Key_Down) newPos.setY(newPos.y() + 10);
        if (event->key() == Qt::Key_Left) newPos.setX(newPos.x() - 10);
        if (event->key() == Qt::Key_Right) newPos.setX(newPos.x() + 10);
        this->move(newPos);
        this->setFixedSize(this->width() + 1, this->height() + 1);
    }
private:
    bool is_pressed = false;
    QPoint start_pos;
};
