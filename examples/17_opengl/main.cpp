#include <QtWidgets>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QSurfaceFormat>
#include <memory>

class Triangle final : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
public:
    explicit Triangle(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
    ~Triangle() override { QObject::disconnect(cleanupConnection_); cleanup(); }
    bool ready() const { return ready_; }
    QString error() const { return error_; }
protected:
    void initializeGL() override {
        functionsReady_ = initializeOpenGLFunctions();
        if (!functionsReady_) { error_ = "OpenGL 3.3 Core unavailable"; return; }
        cleanupConnection_ = connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, [this] { cleanup(); }, Qt::DirectConnection);
        program_ = std::make_unique<QOpenGLShaderProgram>();
        const char *vertex = R"(#version 330 core
layout(location=0) in vec2 position;
layout(location=1) in vec3 color;
out vec3 vertexColor;
void main() { gl_Position=vec4(position,0.0,1.0); vertexColor=color; }
)";
        const char *fragment = R"(#version 330 core
in vec3 vertexColor;
out vec4 fragmentColor;
void main() { fragmentColor=vec4(vertexColor,1.0); }
)";
        if (!program_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex)
            || !program_->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment)
            || !program_->link()) { error_ = program_->log(); return; }
        const float vertices[] = {
            -0.7f,-0.6f, 1.f,0.f,0.f,
             0.7f,-0.6f, 0.f,1.f,0.f,
             0.0f, 0.7f, 0.f,0.f,1.f
        };
        if (!vao_.create() || !vbo_.create()) { error_ = "VAO/VBO creation failed"; return; }
        QOpenGLVertexArrayObject::Binder binding(&vao_);
        if (!vbo_.bind() || !program_->bind()) { error_ = "GPU resource bind failed"; return; }
        vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
        vbo_.allocate(vertices, sizeof(vertices));
        program_->enableAttributeArray(0);
        program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, 5 * sizeof(float));
        program_->enableAttributeArray(1);
        program_->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 3, 5 * sizeof(float));
        vbo_.release(); program_->release();
        ready_ = true;
    }
    void resizeGL(int, int) override {
        // QOpenGLWidget 已维护 framebuffer 和 viewport；矩阵扩展在教程中讲解。
    }
    void paintGL() override {
        if (!isValid() || !functionsReady_) return;
        glClearColor(0.06f, 0.10f, 0.16f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (!ready_) return;
        program_->bind();
        QOpenGLVertexArrayObject::Binder binding(&vao_);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        program_->release();
    }
private:
    void cleanup() {
        if (!context() || !context()->isValid()) return;
        makeCurrent();
        ready_ = false;
        vao_.destroy(); vbo_.destroy(); program_.reset();
        doneCurrent();
    }
    QOpenGLBuffer vbo_{QOpenGLBuffer::VertexBuffer};
    QOpenGLVertexArrayObject vao_;
    std::unique_ptr<QOpenGLShaderProgram> program_;
    bool ready_ = false;
    bool functionsReady_ = false;
    QMetaObject::Connection cleanupConnection_;
    QString error_;
};

int main(int argc, char **argv) {
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);
    Triangle window;
    window.setWindowTitle(QStringLiteral("示例 17 · OpenGL 彩色三角形"));
    window.resize(640, 480); window.show();
    if (app.arguments().contains("--self-test")) {
        QTimer::singleShot(1500, &window, [&] {
            if (!window.ready()) { qCritical() << window.error(); app.exit(1); return; }
            const QImage image = window.grabFramebuffer();
            if (image.isNull()) { app.exit(2); return; }
            const auto center = image.pixelColor(image.width()/2, image.height()/2);
            const auto corner = image.pixelColor(4,4);
            if (center == corner) { qCritical() << "Triangle pixels missing"; app.exit(3); return; }
            qInfo() << "PASS OpenGL context, shaders, buffer, draw and pixel readback";
            app.exit(0);
        });
    }
    return app.exec();
}
