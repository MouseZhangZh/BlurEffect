//
// Created by Mouse Zhang on 2024/1/24.
//

#pragma once

#include <QGraphicsEffect>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

class BlurEffect : public QGraphicsEffect {
public:
    ~BlurEffect() {
        delete_pointer(graphics_blur_effect_);
        delete_pointer(graphics_pixmap_item_);
        delete_pointer(graphics_scene_);
        delete_pointer(painter_);
        delete_pointer(res_pixmap_);
    }
    void draw(QPainter* painter) override {
        QPixmap pixmap;
        QPoint offset;
        double scale = painter->device()->devicePixelRatio();
        QRectF rect = this->sourceBoundingRect(Qt::DeviceCoordinates);
        // rect.adjust(-blur_radius, -blur_radius, blur_radius * 2, blur_radius * 2);
        rect_ = {
            rect.x() * scale - blur_radius_,
            rect.y() * scale - blur_radius_,
            rect.width() * scale + blur_radius_ * 2,
            rect.height() * scale + blur_radius_ * 2
        };
        switch (painter->device()->devType()) {
        case QInternal::Image:
            {
                QImage img = static_cast<QImage*>(painter->device())->copy(rect_.toRect());
#ifdef QT_DEBUG
                img.save("capture.png");
#endif
                pixmap = this->blur_image(QPixmap::fromImage(img), blur_radius_);
                break;
            }
        default:
            {
                qWarning() << "device type is: " << painter->device()->devType() << "not implement now.";
                drawSource(painter);
                return;
            }
        }
        pixmap.setDevicePixelRatio(scale);
#ifdef QT_DEBUG
        pixmap.save("draw.png");
#endif
        painter->drawPixmap(offset, pixmap);
        painter->drawPixmap(offset, sourcePixmap(Qt::LogicalCoordinates, &offset));
    }
    void set_blur_radius(const double r) {
        this->blur_radius_ = r;
    }
private:
     QPixmap blur_image(const QPixmap& source_pixmap, double radius) {
         if (radius == 0) {
             return source_pixmap;
         }
         if (first_time_) {
             if (!graphics_blur_effect_) graphics_blur_effect_ = new QGraphicsBlurEffect();
             if (!graphics_scene_) graphics_scene_ = new QGraphicsScene();
             if (!graphics_pixmap_item_) graphics_pixmap_item_ = new QGraphicsPixmapItem();
             if (!res_pixmap_) res_pixmap_ = new QPixmap(source_pixmap.size());
             if (!painter_) painter_ = new QPainter(res_pixmap_);
             graphics_pixmap_item_->setGraphicsEffect(graphics_blur_effect_);
             graphics_scene_->addItem(graphics_pixmap_item_);
             painter_->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
             painter_->setCompositionMode(QPainter::CompositionMode_Source);
             first_time_ = false;
         }
         graphics_blur_effect_->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
         graphics_blur_effect_->setBlurRadius(radius);
         graphics_pixmap_item_->setPixmap(source_pixmap);
         // res_pixmap->fill(Qt::transparent);
         graphics_scene_->render(painter_, QRectF(0, 0, source_pixmap.width(), source_pixmap.height()));
#ifdef QT_DEBUG
         if (!res_pixmap_->save("blur_capture.png")) {
             qWarning() << "pixmap saved error!";
         }
         res_pixmap_->copy(QRectF(blur_radius_, blur_radius_, rect_.width() - blur_radius_ * 2, rect_.height() - blur_radius_ * 2).toRect()).save("after_copy.png");
#endif
         return res_pixmap_->copy(QRectF(blur_radius_, blur_radius_, rect_.width() - blur_radius_ * 2, rect_.height() - blur_radius_ * 2).toRect());
    }

    template<typename T>
    void delete_pointer(T p) {
         static_assert(std::is_pointer<T>(), "Here should use pointer");
         if constexpr (std::is_pointer<T>())
             if (p) {
                 delete p;
                 p = nullptr;
             }
     }

    bool first_time_ = true;
    double blur_radius_ = 32;
    QGraphicsBlurEffect* graphics_blur_effect_ = nullptr;
    QGraphicsScene* graphics_scene_ = nullptr;
    QGraphicsPixmapItem* graphics_pixmap_item_ = nullptr;
    QPixmap* res_pixmap_ = nullptr;
    QPainter* painter_ = nullptr;
    QRectF rect_;
};

