#include <QtWidgets>
#include <QtMath>
class Gauge : public QWidget {
public:
    Gauge() {
        auto *timer=new QTimer(this);
        connect(timer,&QTimer::timeout,this,[this] { value_=(value_+1)%101; update(); });
        timer->start(50); resize(420,320);
    }
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this); p.setRenderHint(QPainter::Antialiasing);
        const int side=qMin(width(),height());
        p.translate(width()/2.0,height()/2.0); p.scale(side/240.0,side/240.0);
        p.setPen(QPen(QColor("#334155"),12)); p.drawArc(QRectF(-90,-90,180,180),30*16,300*16);
        p.setPen(QPen(QColor("#16a085"),12)); p.drawArc(QRectF(-90,-90,180,180),210*16,-value_*3*16);
        p.setPen(palette().text().color()); QFont font=p.font(); font.setPixelSize(32); p.setFont(font);
        p.drawText(QRectF(-70,-30,140,60),Qt::AlignCenter,QString::number(value_));
    }
private: int value_=0;
};
int main(int argc,char **argv) {
    QApplication app(argc,argv); Gauge w; w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
