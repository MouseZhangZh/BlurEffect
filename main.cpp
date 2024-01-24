#include <QApplication>
#include <QMainWindow>

#include "blur_effect.h"
#include "drag_move_widget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow w;
    w.setMinimumSize(800, 640);

    QLabel *backgroundLabel = new QLabel(&w);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setPixmap(QPixmap(":/res/DSC02890.jpg"));
    backgroundLabel->setGeometry(10, 10, w.width(), w.height());

    DragMoveWidget *transparentWidget = new DragMoveWidget(backgroundLabel);
    QPalette pal(transparentWidget->palette());
    pal.setColor(QPalette::Window, Qt::transparent);
    transparentWidget->setPalette(pal);
    transparentWidget->setAutoFillBackground(true);
    transparentWidget->setText("菜就多练！");
    transparentWidget->setGeometry(300, 200, 100, 100);

    BlurEffect *effect = new BlurEffect();
    transparentWidget->setGraphicsEffect(effect);

    w.show();
    return a.exec();
}